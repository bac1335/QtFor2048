#include "tlutility.h"
#include "header/tlpaths.h"
#include <QDesktopWidget>
#include <QApplication>
#include "tlsettings.h"
#include "../Component/maskwidget/tlmaskwidget.h"
#include <QDebug>
#include <QFileInfoList>
#include <QDir>
#include <QCryptographicHash>
#include <QProcess>
#ifdef Q_OS_WIN
#include <windows.h>
#include <Softpub.h>
#include <wincrypt.h>
#include <wintrust.h>
#include <mscat.h>
#include <wchar.h>
#pragma comment(lib, "wintrust")
#include <Windows.h>
#include <atlbase.h>
#include <atlconv.h>
#include "iphlpapi.h"
#pragma comment(lib,"Iphlpapi.lib")
#endif

#include <QNetworkInterface>
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include <QPainter>
#include <QUuid>
#include <QBitmap>
#include "hardwareinfo.h"


//注册表位置..为设置开机自启动 需要修改注册表
#define REG_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"

TLUtility * TLUtility::m_instance = nullptr;

TLUtility *TLUtility::GetInstance()
{
    if(m_instance == nullptr){
        m_instance = new TLUtility;
        atexit(Release);
    }
    return m_instance;
}

void TLUtility::Release()
{
    if(m_instance){
        delete m_instance;
        m_instance = nullptr;
    }
}

QString TLUtility::path(PathType type)
{
    return TLPaths::path(type);
}

QString TLUtility::path()
{
    return m_pSettings->path();
}

TLSettings *TLUtility::setting()
{
    return m_pSettings;
}

#define SettingError qWarning() <<__FUNCTION__ << "setting not init!";
void TLUtility::initSettings(const QString &fileName)
{
//    qDebug() << "-->Lynn<--"<< __FUNCTION__ << "setting filename = " << fileName;
    if(!m_pSettings){
        m_pSettings = new TLSettings(fileName);
    }
}

void TLUtility::setValue(const QString &group, const QString &key, const QVariant &value)
{
    if(m_pSettings){
        m_pSettings->setValue(group,key,value);
    }
    else{
        SettingError
    }
}

QVariant TLUtility::value(const QString &group, const QString &key, const QVariant &defaultValue)
{
    if(m_pSettings){
        return m_pSettings->value(group,key,defaultValue);
    }
    else{
        SettingError
        return QVariant();
    }
}

bool TLUtility::containts(const QString &group, const QString &key)
{
    if(m_pSettings){
        return m_pSettings->containts(group,key);
    }
    else{
        SettingError
        return false;
    }
}

void TLUtility::remove(const QString &group, const QString &key)
{
    if(m_pSettings){
        m_pSettings->remove(group,key);
    }
    else{
        SettingError
    }
}

void TLUtility::sync()
{
    if(m_pSettings){
        m_pSettings->sync();
    }
    else{
        SettingError
    }
}

QSize TLUtility::getDesktopSize()
{
    int screen = getScreenIndex();//调试用，多屏确定哪个是液晶屏
    return qApp->desktop()->screenGeometry(screen).size();
}

QRect TLUtility::getDesktopRect()
{
    int screen = getScreenIndex();
    return qApp->desktop()->screenGeometry(screen);
}

qreal TLUtility::getScaleFactor()
{
    return m_rScaleFactor;
}

QFont TLUtility::getFont(int pixSize, bool bBold, const QString &fontFamily)
{
    QFont font;
    font.setFamily(fontFamily);
    font.setPixelSize(pixSize+2);
    font.setBold(bBold);
    return font;
}

int TLUtility::calStringWidth(const QFont &font, const QString &str)
{
    if(str.isEmpty())
    {
        return 0;
    }
    QFontMetrics fm(font);
    return fm.width(str);
}

QString TLUtility::getElideText(const QString &srcText, QFont font, int displayWidth, Qt::TextElideMode elideMode)
{
    QFontMetrics fontMetrics(font);
    return fontMetrics.elidedText(srcText, elideMode, displayWidth);
}

TLMaskWidget *TLUtility::addMask(QWidget *widget)
{
    if (widget == nullptr)
    {
        return nullptr;
    }

    QWidget* parent = widget->parentWidget();

    TLMaskWidget* maskWidget = qobject_cast<TLMaskWidget *> (parent);
    //说明已经有遮罩
    if (maskWidget != nullptr) {
        Q_ASSERT(parent->metaObject()->className() == "TLMaskWidget");
        widget->show();
        return maskWidget;
    }

    maskWidget = new TLMaskWidget;
    maskWidget->setChild(widget);
    maskWidget->hide();
    return maskWidget;
}

TLMaskWidget *TLUtility::showMask(QWidget *widget)
{
    TLMaskWidget* parent = qobject_cast<TLMaskWidget *>(widget->parentWidget());

    if(parent != nullptr) {
        widget->show();
        parent->show();
        parent->raise();
    } else {
        widget->show();
        widget->raise();
    }

    return parent;
}

bool TLUtility::copyDirectory(QString fromDir, QString toDir, bool bCoverIfFileExists)
{
    QDir formDir_(fromDir);
    QDir toDir_(toDir);

    if(!toDir_.exists()){
        if(!toDir_.mkdir(toDir)){
            return false;
        }
    }

    QFileInfoList fileInfoList = formDir_.entryInfoList(QDir::Dirs
                                                        |QDir::Files|
                                                        QDir::Hidden|
                                                        QDir::NoDotAndDotDot);
    QString fileName;
    foreach(QFileInfo fileInfo, fileInfoList){
        fileName = fileInfo.fileName();
        if(fileInfo.isDir()){
            //递归调用拷贝
            if(!copyDirectory(fileInfo.filePath(),
                              toDir+ QString("/") + (fileName),
                              bCoverIfFileExists)){
                Q_ASSERT_X(1 == 0, "QFile::copy, can't copy dir", qPrintable(fileInfo.filePath()));
                continue;
            }
        }else{
            //拷贝子文件
            if(toDir_.exists(fileName)){
                if(bCoverIfFileExists){
                    toDir_.remove(fileName);
                }else{
                    continue;
                }
            }
            QString fromfile = fileInfo.filePath();
            QString tofile = toDir_.filePath(fileName);
            if(!QFile::copy(fromfile, tofile)){
                Q_ASSERT_X(1 == 0, "QFile::copy, can't copy file", qPrintable(fromfile));
                continue;
            }
        }
    }
    return true;
}

void TLUtility::cleanDirectory(const QString dirName)
{
    QDir dir(dirName);
    if(!dir.exists()){
        return;
    }
    QStringList dirNames;
    QFileInfoList filst;
    QFileInfoList::iterator curFi;
    dirNames << dirName;
    for(int i = 0; i < dirNames.size(); ++i ){
        //遍历各级文件夹，并将这些文件夹中的文件删除
        dir.setPath(dirNames.at(i));
        filst = dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::Readable|QDir::Writable|
                                  QDir::Hidden|QDir::NoDotAndDotDot,QDir::Name);
        curFi=filst.begin();
        while(curFi!=filst.end()){
            if(curFi->isDir()){
                //遇到文件夹,则添加至文件夹列表dirs尾部
                dirNames << (curFi->filePath());
            }else if(curFi->isFile()){
                bool ok = dir.remove(curFi->filePath());	//删除文件
//                qDebug() << "--->Lynn<---" << __FUNCTION__ << ok;
            }
            curFi++;
        }
    }
    //不删除最外层文件夹 i不能等于0
    for(int i = dirNames.size() - 1; i > 0; i-- ){
        //删除文件夹
        dir.setPath(dirNames.at(i));
        if(!dir.rmdir(".")){//删除失败
            dirNames.removeAt(i);
        }
    }
}

void TLUtility::removeDirectory(const QString dirName)
{
    QDir dir(dirName);
    if(!dir.exists()){
        return;
    }
    QStringList dirNames;
    QFileInfoList filst;
    QFileInfoList::iterator curFi;
    dirNames << dirName;
    for(int i = 0; i < dirNames.size(); ++i ){
        //遍历各级文件夹，并将这些文件夹中的文件删除
        dir.setPath(dirNames.at(i));
        filst = dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::Readable|QDir::Writable|
                                  QDir::Hidden|QDir::NoDotAndDotDot,QDir::Name);
        curFi=filst.begin();
        while(curFi!=filst.end()){
            if(curFi->isDir()){
                //遇到文件夹,则添加至文件夹列表dirs尾部
                dirNames << (curFi->filePath());
            }else if(curFi->isFile()){
                dir.remove(curFi->filePath());	//删除文件
            }
            curFi++;
        }
    }
    for(int i = dirNames.size() - 1; i >= 0; i-- ){
        //删除文件夹
        dir.setPath(dirNames.at(i));
        dir.rmdir(".");
    }
}

QString TLUtility::getFileMd5(const QString &filePath)
{
    QFile theFile(filePath);
    if(!theFile.open(QIODevice::ReadOnly))
    {
        qDebug() << __FUNCTION__ << "open fail";
        return "";
    }
    QByteArray ba = QCryptographicHash::hash(theFile.readAll(),QCryptographicHash::Md5);
    theFile.close();
    return QString(ba);

}

QString TLUtility::getCpuID()
{
    QString id;
#ifdef Q_OS_WIN
    id = getCpuIdThroughCode();
#endif
    return id;
}

bool TLUtility::CheckAppRunningStatus(const QString &appName)
{
#ifdef Q_OS_WIN
    QProcess* process = new QProcess;
    process->start("tasklist" ,QStringList()<<"/FI"<<"imagename eq " +appName);
    process->waitForFinished();
    QString outputStr = QString::fromLocal8Bit(process->readAllStandardOutput());
    process->deleteLater();
    if(outputStr.contains(appName)){
        return true;
    }
    else{
        return false;
    }
#endif
}

void TLUtility::KillAppProcess(const QString &appName)
{
#ifdef Q_OS_WIN
    QProcess p;
    QString c = "taskkill /im " + appName + " /f";
    p.execute(c);
    p.close();
#endif
}

void TLUtility::setAutoStart(bool isAutoStart)
{
#ifdef Q_OS_WIN
    QString applicationName = QApplication::applicationName();
    QSettings *settings = new QSettings(REG_RUN, QSettings::NativeFormat);
    if(isAutoStart)
    {
        QString applicationPath = QApplication::applicationFilePath();
        settings->setValue(applicationName, applicationPath.replace("/", "\\"));
    }
    else
    {
        settings->remove(applicationName);
    }
    delete settings;
#elif defined Q_OS_OSX
#elif defined Q_OS_ANDROID
#endif
}

QString TLUtility::getLocalIp()
{
    return m_ipAddr;
}

QString TLUtility::getLocalMask()
{
    return m_ipMask;
}

QString TLUtility::getRandomString(int length)
{
    qsrand(QDateTime::currentMSecsSinceEpoch());//为随机值设定一个seed

    const char chrs[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int chrs_size = sizeof(chrs);

    char* ch = new char[length + 1];
    memset(ch, 0, length + 1);
    int randomx = 0;
    for (int i = 0; i < length; ++i)
    {
        randomx= qrand() % (chrs_size - 1);
        ch[i] = chrs[randomx];
    }

    QString ret(ch);
    delete[] ch;
    return ret;
}

#ifdef Q_OS_WIN

BOOL VerifyEmbeddedSignature(LPCWSTR pwszSourceFile,bool *isHaveDigitalSigned,bool *isSignedCheckOk)
{
    LONG lStatus;
    DWORD dwLastError;

    // Initialize the WINTRUST_FILE_INFO structure.

    WINTRUST_FILE_INFO FileData;
    memset(&FileData, 0, sizeof(FileData));
    FileData.cbStruct = sizeof(WINTRUST_FILE_INFO);
    FileData.pcwszFilePath = pwszSourceFile;
    FileData.hFile = NULL;
    FileData.pgKnownSubject = NULL;

    /*
    WVTPolicyGUID specifies the policy to apply on the file
    WINTRUST_ACTION_GENERIC_VERIFY_V2 policy checks:

    1) The certificate used to sign the file chains up to a root
    certificate located in the trusted root certificate store. This
    implies that the identity of the publisher has been verified by
    a certification authority.

    2) In cases where user interface is displayed (which this example
    does not do), WinVerifyTrust will check for whether the
    end entity certificate is stored in the trusted publisher store,
    implying that the user trusts content from this publisher.

    3) The end entity certificate has sufficient permission to sign
    code, as indicated by the presence of a code signing EKU or no
    EKU.
    */

    GUID WVTPolicyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    WINTRUST_DATA WinTrustData;

    // Initialize the WinVerifyTrust input data structure.

    // Default all fields to 0.
    memset(&WinTrustData, 0, sizeof(WinTrustData));

    WinTrustData.cbStruct = sizeof(WinTrustData);

    // Use default code signing EKU.
    WinTrustData.pPolicyCallbackData = NULL;

    // No data to pass to SIP.
    WinTrustData.pSIPClientData = NULL;

    // Disable WVT UI.
    WinTrustData.dwUIChoice = WTD_UI_NONE;

    // No revocation checking.
    WinTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;

    // Verify an embedded signature on a file.
    WinTrustData.dwUnionChoice = WTD_CHOICE_FILE;

    // Verify action.
    WinTrustData.dwStateAction = WTD_STATEACTION_VERIFY;

    // Verification sets this value.
    WinTrustData.hWVTStateData = NULL;

    // Not used.
    WinTrustData.pwszURLReference = NULL;

    // This is not applicable if there is no UI because it changes
    // the UI to accommodate running applications instead of
    // installing applications.
    WinTrustData.dwUIContext = 0;

    // Set pFile.
    WinTrustData.pFile = &FileData;

    // WinVerifyTrust verifies signatures as specified by the GUID
    // and Wintrust_Data.
    lStatus = WinVerifyTrust(
        NULL,
        &WVTPolicyGUID,
        &WinTrustData);

    switch (lStatus)
    {
    case ERROR_SUCCESS:
        /*
        Signed file:
        - Hash that represents the subject is trusted.

        - Trusted publisher without any verification errors.

        - UI was disabled in dwUIChoice. No publisher or
        time stamp chain errors.

        - UI was enabled in dwUIChoice and the user clicked
        "Yes" when asked to install and run the signed
        subject.
        */

        qDebug()<<"robin:WinVerifyTrust SUCCESS";
        *isHaveDigitalSigned = true;
        *isSignedCheckOk = true;

        break;

    case TRUST_E_NOSIGNATURE:
        // The file was not signed or had a signature
        // that was not valid.

        // Get the reason for no signature.
        dwLastError = GetLastError();
        if (TRUST_E_NOSIGNATURE == dwLastError ||
            TRUST_E_SUBJECT_FORM_UNKNOWN == dwLastError ||
            TRUST_E_PROVIDER_UNKNOWN == dwLastError)
        {
            // The file was not signed.
//            qDebug()<<"robin:The file was not signed";

        }
        else
        {
            // The signature was not valid or there was an error
            // opening the file.
  //          qDebug()<<"robin:The signature was not valid or there was an error opening the file";
        }

        *isHaveDigitalSigned = false;
        *isSignedCheckOk = false;

        break;

    case TRUST_E_EXPLICIT_DISTRUST:
        // The hash that represents the subject or the publisher
        // is not allowed by the admin or user.
//        qDebug()<<"robin:The signature is present, but specifically disallowed";

        *isHaveDigitalSigned = false;
        *isSignedCheckOk = false;

        break;

    case TRUST_E_SUBJECT_NOT_TRUSTED:
        // The user clicked "No" when asked to install and run.
//        qDebug()<<"robin:The signature is present, but not trusted.";

        *isHaveDigitalSigned = false;
        *isSignedCheckOk = false;

        break;

    case CRYPT_E_SECURITY_SETTINGS:
        /*
        The hash that represents the subject or the publisher
        was not explicitly trusted by the admin and the
        admin policy has disabled user trust. No signature,
        publisher or time stamp errors.
        */
//        qDebug()<<"robin:The hash that represents the subject or the publisher\
//                  was not explicitly trusted by the admin and the\
//                  admin policy has disabled user trust. No signature,\
//                  publisher or time stamp errors.";
        *isHaveDigitalSigned = false;
        *isSignedCheckOk = false;
        break;

    case TRUST_E_BAD_DIGEST:
        //这里是 程序存在数字签名，但是数字签名损坏了。通常来说是手动修改了程序的exe文件
        *isHaveDigitalSigned = true;
        *isSignedCheckOk = false;
        break;

    default:
        // The UI was disabled in dwUIChoice or the admin policy
        // has disabled user trust. lStatus contains the
        // publisher or time stamp chain error.
//        qDebug()<<"robin:Error is:"<<lStatus;
        *isHaveDigitalSigned = false;
        *isSignedCheckOk = false;
        break;
    }

    // Any hWVTStateData must be released by a call with close.
    WinTrustData.dwStateAction = WTD_STATEACTION_CLOSE;

    lStatus = WinVerifyTrust(
        NULL,
        &WVTPolicyGUID,
        &WinTrustData);
    return true;
}

#endif

void TLUtility::checkSelfWinDigitallySigned()
{
#ifdef Q_OS_WIN
    //注意这里的数字签名并没有检测证书是否是我们的，也就是说，只要存在数字签名并且数字签名正常就通过

    //这里是检测数字证书是否存在及是否正常
     bool t_bDigitalExist = false;//数字签名是否存在
     bool t_bCheckSuccess = false;//数字签名是否能验证通过

     QString t_curFilePath = qApp->applicationFilePath();
     qDebug()<<__FUNCTION__<<t_curFilePath;
     const wchar_t *wFilePath = reinterpret_cast<const wchar_t *>(t_curFilePath.utf16());

    bool t_rtn = VerifyEmbeddedSignature(wFilePath,&t_bDigitalExist,&t_bCheckSuccess);

    if(t_bDigitalExist)
    {//数字签名存在
        if(t_bCheckSuccess)
        {//如果数字签名验证通过
           //程序正常启动不做处理
        }
        else
        {//数字签名验证失败
         //定义一个定时器过一会儿让程序自己退出
            qsrand(QTime::currentTime().msecsSinceStartOfDay());
            int t_timeOut = 5000 + qrand() % 5000;//确保程序在5s~10s内退出
            QTimer::singleShot(t_timeOut,qApp,SLOT(quit()));
        }
    }
    else
    {//数字签名不存在
        qsrand(QTime::currentTime().msecsSinceStartOfDay());
        int t_timeOut = 5000 + qrand() % 5000;//确保程序在5s~10s内退出
        QTimer::singleShot(t_timeOut,qApp,SLOT(quit()));
        QMessageBox::information(NULL,tr("Digital Not Exist"),tr("digitally signed not exist"));
        QTimer::singleShot(t_timeOut,qApp,SLOT(quit()));//这里加两个定时器是因为QMessageBox出现的时候，如果不操作抵消掉一个qApp::quit事件

    }
#endif
}

QString TLUtility::getUuid()
{
    QString uuid = this->value("common","uuid","").toString();
    if(uuid.isEmpty()){
        uuid = QUuid::createUuid().toString();
        this->setValue("common","uuid",uuid);
    }
    return  uuid;
}
QPixmap TLUtility::createRadiusPixmap(QSize size, QPixmap pixmap)
{
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), size.width()/2., size.height()/2.);
    QPixmap px = pixmap.scaled(size,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    px.setMask(mask);
    return px;
}

QString TLUtility::getDiskID()
{
    HardwareInfo hardwareInfo;
    QString str = hardwareInfo.getDiskID();
    return str;
}

QString TLUtility::getMacAddr()
{
    HardwareInfo hardwareInfo;
    QString str = hardwareInfo.getMacAdd();
    return str;
}

QString TLUtility::getCpuIdThroughCmd()
{
    QString id;
#ifdef Q_OS_WIN
    QProcess p(0);
    p.start("wmic CPU get ProcessorID");
    p.waitForStarted();
    p.waitForFinished();
    id = QString::fromLocal8Bit(p.readAllStandardOutput());
    id = id.remove("ProcessorId").trimmed();
#endif
    return id;
}

QString TLUtility::getCpuIdThroughCode()
{
    QString cpu_id = "";
#ifdef Q_OS_WIN
    unsigned int dwBuf[4]={0};
    unsigned long long ret = 0;
    getcpuid(dwBuf, 1);
    ret = dwBuf[3];
    ret = ret << 32;

    QString str0 = QString::number(dwBuf[3], 16).toUpper();
    QString str0_1 = str0.rightJustified(8,'0');//这一句的意思是前面补0，但是我遇到的情况是这里都填满了
    QString str1 = QString::number(dwBuf[0], 16).toUpper();
    QString str1_1 = str1.rightJustified(8,'0');//这里必须在前面补0，否则不会填满数据
    //cpu_id = cpu_id + QString::number(dwBuf[0], 16).toUpper();
    cpu_id = str0_1 + str1_1;
#endif
    return cpu_id;
}

void TLUtility::getcpuid(unsigned int CPUInfo[4], unsigned int InfoType)
{
#if defined(__GNUC__)// GCC
    __cpuid(InfoType, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#elif defined(_MSC_VER)// MSVC
#if _MSC_VER >= 1400 //VC2005才支持__cpuid
    __cpuid((int*)(void*)CPUInfo, (int)(InfoType));
#else //其他使用getcpuidex
    getcpuidex(CPUInfo, InfoType, 0);
#endif
#endif
}
void TLUtility::getcpuidex(unsigned int CPUInfo[4], unsigned int InfoType, unsigned int ECXValue)
{
#if defined(_MSC_VER) // MSVC
#if defined(_WIN64)	// 64位下不支持内联汇编. 1600: VS2010, 据说VC2008 SP1之后才支持__cpuidex.
    __cpuidex((int*)(void*)CPUInfo, (int)InfoType, (int)ECXValue);
#else
    if (NULL==CPUInfo)	return;
    _asm{
        // load. 读取参数到寄存器.
        mov edi, CPUInfo;
        mov eax, InfoType;
        mov ecx, ECXValue;
        // CPUID
        cpuid;
        // save. 将寄存器保存到CPUInfo
        mov	[edi], eax;
        mov	[edi+4], ebx;
        mov	[edi+8], ecx;
        mov	[edi+12], edx;
    }

#endif
#endif
}
#if 0
QString TLUtility::getIp()
{
    QStringList strIP;
    QStringList lstMask;
//    QList<QHostAddress> t_addrs;
    QList<QNetworkInterface> networkinterfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interfaces, networkinterfaces)
    {
        QNetworkInterface::InterfaceFlags t_interFlags =  interfaces.flags();
        if(t_interFlags & QNetworkInterface::IsUp &&
           t_interFlags & QNetworkInterface::IsRunning)
        {
            foreach (QNetworkAddressEntry entry, interfaces.addressEntries())
            {
                if (entry.ip() != QHostAddress::LocalHost
                    && entry.ip().protocol() == QAbstractSocket::IPv4Protocol
                    )
                {
                    QString ips = entry.ip().toString();
                    if (ips.indexOf("192") == 0 || ips.indexOf("172") == 0 || ips.indexOf("10") == 0)
                    {
                        /// fixme: only last address record into localDevice
//                        t_addrs.append(entry.ip());
                        strIP.append(entry.ip().toString());
                        lstMask.append(entry.netmask().toString());
                    }
                }
            }
        }
    }

    m_ipMask = lstMask.join("|");
    return strIP.join("|");
}
#endif

qreal TLUtility::scaleFactor()
{
    int width = 1920.;
    int height = 1080.;
    int deskWidth = getDesktopSize().width();
    int deskHeight = getDesktopSize().height();

    qreal normalScale = width / height;
    qreal curScale = deskWidth / deskHeight;
    qreal rRet = 1.0;
    if(normalScale <= curScale)
    {
        rRet = deskHeight / 1080.;
    }
    else
    {
        rRet = deskWidth / 1920.;
    }

    return rRet;
}
/**
 * @brief TLUtility::getScreenIndex
 * @return 获取第几块显示器是要的屏幕
 */
int TLUtility::getScreenIndex()
{
    int count = qApp->desktop()->screenCount();
    if(count<=1)//只有一块屏幕的时候
        return 0;
    int index = -1;
    int max = 0;
    //选择分辨率最大的屏幕
    for(int i = 0;i < count;++i){
        if(qApp->desktop()->screenGeometry(i).size().width() > max){
            max = qApp->desktop()->screenGeometry(i).size().width();
            index = i;
        }
    }
    return index;
}

TLUtility::TLUtility(QObject *parent) :
    QObject(parent)
{
//    m_ipAddr = getIp();
    m_rScaleFactor = scaleFactor();
}
