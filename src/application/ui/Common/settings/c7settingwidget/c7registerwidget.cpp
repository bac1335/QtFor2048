#include "c7registerwidget.h"
#include "c7commondef.h"
#include "tlutility.h"
#include "lineedit/tllineedit.h"
#include "XSignature.h"
#include "messagebox/tlmessagebox.h"
#include "tlhttpdef.h"
#include "c7application.h"
#include "tlinternet.h"
#include <QStackedWidget>
#include <QRegExpValidator>

C7RegisterWidget::C7RegisterWidget(int width, int height, C7BaseWidget *parent)
    :C7BaseWidget(width,height,parent), m_version(cOem->version().branch)
{    
    initUI();
    retranslateUi();
}

C7RegisterWidget::~C7RegisterWidget()
{

}

void C7RegisterWidget::initUI()
{  
    m_pStackWidget = new QStackedWidget(this);
    m_pRegisterWidget = new QWidget(this);
    m_pStackWidget->addWidget(m_pRegisterWidget);
    m_registerText = new QLabel(m_pRegisterWidget);
    m_serialNumberText = new QLabel(m_pRegisterWidget);
    m_okButton = new TLPushButton(m_pRegisterWidget);
    m_clearButton = new TLPushButton(m_pRegisterWidget);
    m_pTipsLabel = new  QLabel(m_pRegisterWidget);
    m_lineEdit = new C7CommonLineEdit(m_pRegisterWidget);

    QValidator *validator = new QRegExpValidator(QRegExp("^[^\u4e00-\u9fa5]{0,}$"),this);
    m_lineEdit->setValidator(validator);

    m_registerText->setFont(Uti->getFont(20 * m_rScaleFactor));
    m_registerText->setStyleSheet("color:#32bb85;");
    m_registerText->setAlignment(Qt::AlignCenter);

    m_lineEdit->setFixedSize(540 * m_rScaleFactor, 45 * m_rScaleFactor);
    m_lineEdit->setColor(QColor(80, 82, 91));
    m_lineEdit->setFont(m_font);
    m_lineEdit->setMaxLength(29);
    m_lineEdit->setRadius(4 * m_rScaleFactor,4 * m_rScaleFactor);
    connect(m_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChange(QString)));
    connect(m_lineEdit,&C7CommonLineEdit::returnPressed,this,&C7RegisterWidget::onUpdate);

    m_pTipsLabel->setGeometry(m_lineEdit->x(), m_lineEdit->y() + m_lineEdit->height() + 10 * m_rScaleFactor,
                                 m_lineEdit->width() , 30 * m_rScaleFactor);
    m_pTipsLabel->setFont(m_font);
    m_pTipsLabel->setStyleSheet("color:red");

    m_okButton->setDisabled(true);
    m_clearButton->setDisabled(true);

    m_clearButton->setFont(m_font);
    m_clearButton->setNormalBackColor(Color_CancelButton_Normal);
    m_clearButton->setHoverBackColor(Color_CancelButton_Normal);
    m_clearButton->setPressBackColor(Color_CancelButton_Press);
    m_clearButton->setNormalTextColor(Color_Cancel_Text_Normal);
    m_clearButton->setBorderLine(true);
    m_clearButton->setRadius(6 * m_rScaleFactor);

    m_okButton->setNormalBackColor(Color_MenuButton_Normal);
    m_okButton->setHoverBackColor(Color_MenuButton_Hover);
    m_okButton->setPressBackColor(Color_MenuButton_Press);
    m_okButton->setDisableBackColor(Color_Button_Disable);
    m_okButton->setDisableTextColor(Color_MenuButton_Text);
    m_okButton->setTextColor(Color_MenuButton_Text);
    m_okButton->setRadius(6 * m_rScaleFactor);

    QFont fontTMP = Uti->getFont(16 * m_rScaleFactor);
    m_serialNumberText->setFont(fontTMP);
    m_okButton->setFont(fontTMP);
    m_clearButton->setFont(fontTMP);

    if(m_version == C7Version::Branch_Normal){
        m_pQRCodeImg = new QLabel(m_pRegisterWidget);
        m_pTipsScan = new QLabel(m_pRegisterWidget);
        m_pQRCodeImg->setScaledContents(m_pRegisterWidget);

        //扫码处的等待按钮
        m_pLoadingBtn = new TLLoadingBtn(48*m_rScaleFactor,48*m_rScaleFactor,m_pRegisterWidget);
        m_pLoadingBtn->setTimeOut(20000);
        connect(m_pLoadingBtn,&TLLoadingBtn::sigClicked,this,[=](){
            if(cInternet->isConnected()){
                if(!cApp->isTcpConnected()){ //如果tcp未连接，去重连tcp
                    cApp->reConnectTcp();
                    m_pCheckNetWorkTimer->start();
                }
                else { //如果tcp已连接，直接显示二维码
                    _showRegisterCode();
                }
            }
            else {//如果网络未连接，直接提示
                m_pLoadingBtn->setLoadingStatus(false);
                m_pLoadingBtn->show();
                setScanTips(tr("The network is abnormal,please check"));
            }
        });
        //超时后文字提示
        connect(m_pLoadingBtn,&TLLoadingBtn::sigTimeOut,this,[=](){
            qDebug() << "--->Lynn<---" << __FUNCTION__;
            setScanTips(tr("Timed out,Click the refresh button to update"));
        });
        m_pLoadingBtn->setLoadingIcon(QPixmap(Uti->path(Apppath) +
                                              "/skin/refresh/icon_refresh_normal.png"));

        //检查tcp连接
        m_pCheckNetWorkTimer = new QTimer(this);
        m_pCheckNetWorkTimer->setInterval(1500);
        connect(m_pCheckNetWorkTimer,&QTimer::timeout,[=](){
            if(cApp->isTcpConnected()){
                _showRegisterCode();
            }
        });
        m_pTipsScan->setFont(fontTMP);
    }

    connect(m_okButton, SIGNAL(sigClicked()), this, SLOT(onUpdate()));
    connect(m_clearButton, SIGNAL(sigClicked()), this, SLOT(onClear()));
    updateUI();


    //点击激活按钮后居中显示的等待控件
    m_pLoading = new TLLoading(false,this);
    m_pLoading->setTimeoutTime(30000);
    connect(m_pLoading,&TLLoading::sigWaitTimeout,this,[=](){
        m_pTipsLabel->setText(tr("Timed out,please check the network settings"));
    });

    m_pRegisterResultWidget = new QWidget(this);
    m_pStackWidget->addWidget(m_pRegisterResultWidget);

    m_pRegisterResultLabel = new QLabel(m_pRegisterResultWidget);
    m_font.setPixelSize(22*m_rScaleFactor);
    m_pRegisterResultLabel->setFont(m_font);
    m_pRegisterResultLabel->setAlignment(Qt::AlignCenter);

    if(cOem->oemType() < C7OEM::OEM_Professional) {
        m_pStackWidget->setCurrentIndex(0);
    }
    else {
        m_pStackWidget->setCurrentIndex(1);
    }
}

void C7RegisterWidget::showEvent(QShowEvent *ev)
{
//    cInternet->check();
    QByteArray hardwareInfo = cSign->getHardwareInfoParam();
    qDebug() << __FUNCTION__ << "song hardwareInfo=" << hardwareInfo;
    QString uuid = Uti->getUuid();
    m_strUrl = DEF_HTTP_VERIFY_ADDR + QString("?uuid=%1&").arg(uuid) +QString(hardwareInfo);
    if(m_version == C7Version::Branch_Normal){
        if(cApp->isTcpConnected() && cInternet->isConnected()){
            _showRegisterCode();
        }
        else {
            m_pLoadingBtn->show();
            setScanTips(tr("The network is abnormal,please check"));
        }
    }

    C7BaseWidget::showEvent(ev);
}

void C7RegisterWidget::hideEvent(QHideEvent *ev)
{
    m_pLoading->stopWaiting();
    if(m_pLoadingBtn && m_pLoadingBtn->isActive()){
        m_pLoadingBtn->hide();
        m_pLoadingBtn->setLoadingStatus(false);
    }
    if(m_pCheckNetWorkTimer && m_pCheckNetWorkTimer->isActive()){
        m_pCheckNetWorkTimer->stop();
    }
    C7BaseWidget::hideEvent(ev);
}

void C7RegisterWidget::resizeEvent(QResizeEvent *event)
{
    m_pStackWidget->resize(this->size());
    m_pRegisterWidget->resize(m_pStackWidget->size());
    m_pRegisterResultWidget->resize(m_pStackWidget->size());
    m_pRegisterResultLabel->resize(m_pRegisterResultWidget->width(), 35*m_rScaleFactor);
    m_pRegisterResultLabel->move(m_pRegisterResultWidget->width()/2 - m_pRegisterResultLabel->width()/2,
                                 m_pRegisterResultWidget->height()/2 - m_pRegisterResultLabel->height()/2 - 8*m_rScaleFactor);

     m_registerText->resize(this->width(),40* m_rScaleFactor);
     m_registerText->move((this->width()-m_registerText->width())/2.,15* m_rScaleFactor);

     m_serialNumberText->resize(this->width(), 30 * m_rScaleFactor);
     m_serialNumberText->move(60 * m_rScaleFactor,50 * m_rScaleFactor);

     m_lineEdit->move(m_serialNumberText->x(),
                      m_serialNumberText->y()+m_serialNumberText->height()+20 * m_rScaleFactor);

     m_pTipsLabel->setGeometry(m_lineEdit->x(), m_lineEdit->y() + m_lineEdit->height() + 7 * m_rScaleFactor,
                                  m_lineEdit->width() , 30 * m_rScaleFactor);

     m_okButton->resize(120 * m_rScaleFactor, 48 * m_rScaleFactor);
     m_okButton->move(m_lineEdit->x() + m_lineEdit->width()-m_okButton->width(),
                      m_lineEdit->y() + m_lineEdit->height() +40 * m_rScaleFactor);

     m_clearButton->resize(120 * m_rScaleFactor, 48 * m_rScaleFactor);
     m_clearButton->move(m_okButton->x()-m_clearButton->width()-20* m_rScaleFactor,
                      m_okButton->y());
     if(m_version == C7Version::Branch_Normal){
         m_pQRCodeImg->resize(160*m_rScaleFactor,160*m_rScaleFactor);
         m_pQRCodeImg->move(this->width()/2-m_pQRCodeImg->width()/2.,
                            m_okButton->y() + m_okButton->height() + 32*m_rScaleFactor);
         m_pTipsScan->resize(this->width(),30*m_rScaleFactor);
         m_pTipsScan->move(this->width()/2.-m_pTipsScan->sizeHint().width()/2.,
                           m_pQRCodeImg->height() + m_pQRCodeImg->y() + 10*m_rScaleFactor);
         m_pLoadingBtn->move(m_pQRCodeImg->x() + m_pQRCodeImg->width()/2.-m_pLoadingBtn->width()/2.,
                             m_pQRCodeImg->y() + m_pQRCodeImg->height()/2. -m_pLoadingBtn->height()/2.);
     }
}

bool C7RegisterWidget::checkIsOemPrivateStr(QString licence)
{
    return false;
}

void C7RegisterWidget::retranslateUi()
{
    m_registerText->setText(tr("Verson:Basic"));
    m_lineEdit->setPlaceholderText(tr("please input serial number"));
    m_serialNumberText->setText(tr("Serial Number"));
    m_okButton->setText(tr("Activate"));
    m_clearButton->setText(tr("Clear"));
    if(m_pTipsScan)
        m_pTipsScan->setText(tr("Scan code activation"));
    m_pRegisterResultLabel->setText(tr("Currently a professional version"));//当前已是专业版
}

void C7RegisterWidget::updateUI()
{
    connect(cSign, SIGNAL(sigVerifyResult(int)), this,SLOT(onVerifyResult(int)));
}

void C7RegisterWidget::updateOemTypeByLicens()
{
//    Q_ASSERT(QFile::exists(cSign->GetVerifyPath()));
    C7OEM::OEMType type = C7OEM::decideOemTypeByLicense(cSign->version().toInt());
    C7OEM::OEMType oldtype = cOem->oemType();
    if(type != oldtype){
        cOem->setOemType(type);
        m_pStackWidget->setCurrentWidget(m_pRegisterResultWidget);
        emit sigVersionChange(type);
    }
}

bool C7RegisterWidget::updateOemTypeByDevice()
{
    return false;
}


QString C7RegisterWidget::fixInputText(const QString &text)
{
    QString temp = text;
    if(temp.isEmpty()){
        return QString();
    }
    if(temp.contains("-")){
        temp = temp.remove("-");
    }
    if(temp.contains(" ")){
        temp = temp.remove(" ");
    }
    return temp.toUpper();
}

QString C7RegisterWidget::getVerType(int oemType)
{
    QString version;
    switch (oemType) {
    case C7OEM::OEM_Probation:
    case C7OEM::OEM_Unknow:
        version = tr("Probation");
        break;
    case C7OEM::OEM_Normal:
        version = tr("normal");
        break;
    case C7OEM::OEM_Professional:
        version = tr("Professional");
        break;
    case C7OEM::OEM_Senior:
        version = tr("Senior");
        break;
    default:
        break;
    }
    return version;
}

void C7RegisterWidget::setDoubleScreenEnable(bool bEnable)
{

}

void C7RegisterWidget::setScanTips(QString tips)
{
    m_pTipsScan->setText(tips);
    m_pTipsScan->move(this->width()/2.-m_pTipsScan->sizeHint().width()/2.,
                      m_pQRCodeImg->height() + m_pQRCodeImg->y() + 10*m_rScaleFactor);
}

void C7RegisterWidget::_showRegisterCode()
{
    m_pLoadingBtn->setLoadingStatus(false);
    m_pLoadingBtn->hide();
    QPixmap qrcode = Uti->createQrCode(m_strUrl);
    m_pQRCodeImg->setPixmap(qrcode);
    setScanTips(tr("Scan code activation"));
    if(m_pCheckNetWorkTimer->isActive()){
        m_pCheckNetWorkTimer->stop();
    }
}

void C7RegisterWidget::onUpdate()
{
    QString lincenseStr = m_lineEdit->text();
#ifdef SPECIAL_SERIAL_UPDATE
    if(lincenseStr == QString("88888-22222-88888-33333-88888")){
        Uti->setValue("common",SERIAL,1);

        cOem->setOemType(C7OEM::OEM_Professional);
        m_pStackWidget->setCurrentWidget(m_pRegisterResultWidget);
        emit sigVersionChange(C7OEM::OEM_Professional);
        return;
    }
#endif
    m_pLoading->startWaiting();
    if(!cInternet->isConnected()){
        qDebug()<<__FUNCTION__<<"internal is disconnect";
        m_pLoading->stopWaiting();
        m_pTipsLabel->setText(tr("The network is abnormal,please check"));
        return;
    }

    if(cOem->decideOemTypeByPriveStr(lincenseStr) != C7OEM::OEM_Unknow){
        cOem->saveOemPriveStrToSetting(lincenseStr);
        return;
    }

    lincenseStr = fixInputText(lincenseStr);
    if(lincenseStr.isEmpty()){
        return;
    }

    qDebug()<<"C7RegisterPage::verifySerial"<<lincenseStr;

    //TODO，显示验证中
    char serial[30]; serial[0]=0;
    if(lincenseStr.size()!=25 && lincenseStr.size()!=0){
        //序列号长度不对
        m_pTipsLabel->setText(tr("Invalid serial number"));
        return;
    }

    strcpy(serial, lincenseStr.toLocal8Bit().constData());
    unsigned int licensetype = cSign->BaseDecrypt(serial);

    C7OEM::OEMType type = C7OEM::decideOemTypeByLicense(licensetype);
//    if(type <= cOem->oemType()){
//        //不支持降低版本
//        m_pTipsLabel->setText(tr("%1 can't update to %2").arg(getVerType(cOem->oemType())).arg(getVerType(type)));
//        return;
//    }

    cSign->SetParam(0, serial);
    if(!cSign->NetVerifyStart()){
        qDebug() << "--->Lynn<---" << __FUNCTION__ << "!cSign->NetVerifyStart()";
        onVerifyResult(VerifyResult_Net_Failed);
        return;
    }
}

void C7RegisterWidget::onClear()
{
    m_lineEdit->clear();
}

void C7RegisterWidget::onVerifyResult(int result)
{
    m_pLoading->stopWaiting();
    qDebug()<<__FUNCTION__<<u"序列号验证返回结果 "<<"VerifyResult:"<<result;
    switch(result){
    case VerifyResult_Net_Success:{
        Q_ASSERT(cSign->VerifySucces());
        updateOemTypeByLicens();
    }break;
    case VerifyResult_Net_Failed:
        m_pTipsLabel->setText(tr("Serial number verification failed"));
        break;
    case VerifyResult_Net_Break:
        m_pTipsLabel->setText(tr("The network is abnormal,please check"));
        break;
    }
}

void C7RegisterWidget::onTextChange(QString str)
{
    if(!m_pTipsLabel->text().isEmpty())
         m_pTipsLabel->clear();
    m_clearButton->setEnabled(!m_lineEdit->text().isEmpty());
    m_okButton->setEnabled(m_lineEdit->text().size() == 29);

    if(str.size() != 29){
        return;
    }
    QString text = m_lineEdit->text();
#ifdef SPECIAL_SERIAL_UPDATE
    if(text == QString("88888-22222-88888-33333-88888")){
        m_pTipsLabel->setText("Special serial number");
        m_pTipsLabel->setVisible(true);
        m_okButton->setEnabled(true);
        return;
    }
#endif
    text = fixInputText(text);
    char serial[30]; serial[0]=0;
    strcpy(serial, text.toLocal8Bit().constData());
    unsigned int licensetype = cSign->BaseDecrypt(serial);
    C7OEM::OEMType type = C7OEM::decideOemTypeByLicense(licensetype);
    if(type != C7OEM::OEM_Unknow){
        QString typestr = getVerType(type);
        m_pTipsLabel->setText(typestr);
        m_pTipsLabel->setVisible(true);
        m_okButton->setEnabled(true);
    }
    else {
        m_pTipsLabel->setText(tr("Serial number error"));
        m_pTipsLabel->setVisible(true);
        m_okButton->setEnabled(false);
    }

}
