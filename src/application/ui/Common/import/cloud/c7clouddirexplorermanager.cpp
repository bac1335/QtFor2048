#include "c7clouddirexplorermanager.h"
#include <QDir>
#include <QDebug>
#include <QPropertyAnimation>
#include <QPushButton>
#include "c7clouddirexplorer.h"
#include "tlutility.h"
#include "tlpushbutton.h"
#include "networkdisk/c7networkdiskmanager.h"
#include "c7clouditem.h"
#include "tlloading.h"
#include <QJsonArray>
#include <QLabel>

#define PAGECHOICE 2
#define DURINGTIME 300

C7CloudDirExplorerManager::C7CloudDirExplorerManager(SelectMode mode, QSize size, QWidget *parent)
    : C7BaseWidget(size.width(), size.height(), parent), m_selectMode(mode)
{
    initUi();
}

void C7CloudDirExplorerManager::addFileFilter(const QString &suffix)
{
    m_lstFileFilter.append(suffix);
}

void C7CloudDirExplorerManager::addFileFilter(const QStringList &lstSuffix)
{
    m_lstFileFilter += lstSuffix;
}

void C7CloudDirExplorerManager::addFileType(FileType fileType)
{
    m_nFileType |= fileType;

    QString strSuffix = QString("");
    switch (fileType) {
    case ImageFile:
        strSuffix = IMAGE_SUFFIX;
        break;
    case VideoFile:
        strSuffix = VIDEO_SUFFIX;
        break;
    case AudioFile:
        strSuffix = AUDIO_SUFFIX;
        break;
    case TextFile:
        strSuffix = TEXT_SUFFIX;
        break;
    case WordFile:
        strSuffix = WORD_SUFFIX;
        break;
    case ExcelFile:
        strSuffix = EXCEL_SUFFIX;
        break;
    case PowerpointFile:
        strSuffix = POWERPOINT_SUFFIX;
        break;
    case OfficeFile:
        strSuffix = WORD_SUFFIX + EXCEL_SUFFIX + POWERPOINT_SUFFIX;
        break;
    default:
        break;
    }

    m_lstFileFilter += strSuffix.split(",");

}

void C7CloudDirExplorerManager::setFileType(FileType fileType)
{
    m_nFileType = 0;
    m_lstFileFilter.clear();
    addFileType(fileType);
}

//这个是处理多种类型组合的情况
void C7CloudDirExplorerManager::setFileType(int fileType)
{
    m_nFileType = 0;
    m_lstFileFilter.clear();
    addFileType(fileType, ImageFile);
//    addFileType(fileType, VideoFile);
//    addFileType(fileType, AudioFile);
    addFileType(fileType, TextFile);
    addFileType(fileType, WordFile);
    addFileType(fileType, ExcelFile);
    addFileType(fileType, PowerpointFile);
}

void C7CloudDirExplorerManager::changeFirstDir(const QString &dirPath)
{

    if (m_pCurrentDirExplorer != nullptr)
    {
        m_pCurrentDirExplorer->deleteLater();
        m_pCurrentDirExplorer = nullptr;
    }

    m_lstHistoryDir.clear();
    m_pBackBtn->setBtnVisible(false);
    this->setDirPath(dirPath);
}

void C7CloudDirExplorerManager::setHasClicked()
{
    m_pCurrentDirExplorer->setHasClicked();
}

void C7CloudDirExplorerManager::initState()
{ 
    if (m_pCurrentDirExplorer != nullptr)
    {
        m_pCurrentDirExplorer->deleteLater();
        m_pCurrentDirExplorer = nullptr;
    }

    m_lstHistoryDir.clear();
    m_pBackBtn->setBtnVisible(false);
    this->setDirPath(YUN_CHOICE);
}

void C7CloudDirExplorerManager::searchSetLast(SearchType type)
{
    searchSet(type,m_oriYear,m_orimonth,m_endYear,m_endMonth);
}

void C7CloudDirExplorerManager::searchSet(SearchType type,int oriYear,int orimonth,int endYear,int endMonth)
{
    m_currentSearchType = type;
    if(type == Type_Search_All){
        m_pCurrentDirExplorer->setSearChearchSet(m_jsonObj);
    }
    else{
        QVariant lastObj = analysisJson(m_jsonObj,oriYear,orimonth,endYear,endMonth);
        m_pCurrentDirExplorer->setSearChearchSet(lastObj);
    }
}

void C7CloudDirExplorerManager::onCdUp()
{
    emit sigShowTimeChoice(false);

    m_pLoading->stopWaiting();

    if (!m_bAnimationFinished)
    {
        return;
    }
    m_bBack = true;
    if (m_pCurrentDirExplorer != nullptr)
    {
        m_pPreviousDirExplorer = m_pCurrentDirExplorer;
        //在动画结束时才做当前explorer的修正
        setAnimation(m_pPreviousDirExplorer,
                     m_pPreviousDirExplorer->pos(), QPoint(this->width(), m_pPreviousDirExplorer->y()));
    }

    m_pCurrentDirExplorer = m_lstHistoryDir.takeLast();

    if (m_pCurrentDirExplorer != nullptr)
    {

        m_pCurrentDirExplorer->show();
        m_pBackBtn->setBtnVisible(!m_lstHistoryDir.isEmpty());

    }
    emit sigBackBtnClick();

}

void C7CloudDirExplorerManager::onGetPath_Click(ItemType strPath,QVariant obj)
{

    qDebug() << "-->Lynn<--"<< __FUNCTION__ << m_bAnimationFinished;
    //动画结束时才响应
    if (m_bAnimationFinished)
    {
        if(strPath == PageYunChoiceItem)
        {
           QVariant var;
           m_jsonObj = obj;
           emit  sigShowTimeChoice(true);

           if(m_currentSearchType == Type_Search_All){
              var =  obj;
           }
           else{
              var = analysisJson(obj,m_oriYear,m_orimonth,m_endYear,m_endMonth);
           }

           setDirPath(YUN_FILE,var);
        }
        else if(strPath == PagePanChoiceItem){
           emit  sigShowTimeChoice(false);
           setDirPath(PAN_FILE,obj);
        }

        else if(strPath == Pan_Item){

           setDirPath(PAN_FILE,obj);
        }

        emit sigFlieClicked();
    }
    else
    {
        if (m_pCurrentDirExplorer != nullptr)
        {
            m_pCurrentDirExplorer->setLastItemCheck(false);
        }
    }

}


void C7CloudDirExplorerManager::onGetPath_DoubleClick(const QString &strPath)
{
    //动画结束时才响应
    if (m_bAnimationFinished)
    {
        QFileInfo fileInfo(strPath);
        if (!strPath.isEmpty() && fileInfo.isFile())
        {
            emit sigSelectFile(strPath);
        }
        else
        {
            setDirPath(strPath);
        }
    }
}

void C7CloudDirExplorerManager::onGetSelectFile(const QString &strPath)
{
    //只能选择文件夹的情况，不处理
    if (m_selectMode == Dir_Only)
    {
        return;
    }

    int recentFileCount = m_lstRecentFilePath.count();
    if (!m_lstRecentFilePath.contains(strPath, Qt::CaseInsensitive))
    {
        if (recentFileCount >= m_nRecentFileMax)
        {
            m_lstRecentFilePath.takeFirst();
            --recentFileCount;
        }

        m_lstRecentFilePath.append(strPath);
        ++recentFileCount;
    }
    else
    {
        //后面展示的时候是倒序的，所以最新的加到最后
        m_lstRecentFilePath.removeOne(strPath);
        m_lstRecentFilePath.append(strPath);
    }

    Uti->setValue("recentOpen", "settingIndex", recentFileCount);
    for(int i = 1; i <= m_lstRecentFilePath.size(); i++)
    {
        Uti->setValue("recentOpen", QString::number(i), m_lstRecentFilePath.at(i-1));
    }
}

void C7CloudDirExplorerManager::resizeEvent(QResizeEvent *event)
{
    if (m_pCurrentDirExplorer != nullptr)
    {
        m_pCurrentDirExplorer->setScaleFactor(m_rScaleFactor);
        m_pCurrentDirExplorer->resize(this->size());
    }

    C7BaseWidget::resizeEvent(event);
}


void C7CloudDirExplorerManager::setDirPath(const QString &dirPath,QVariant obj)
{
    m_lastPath = m_currentPath;
    m_currentPath = dirPath;
    if (m_pCurrentDirExplorer != nullptr)
    {
        if (!m_lstHistoryDir.contains(m_pCurrentDirExplorer))
        {
            m_lstHistoryDir.append(m_pCurrentDirExplorer);
        }
        m_pBackBtn->setBtnVisible(true);
        emit sigSetTitleText(dirPath, true, false);
    }

    m_pNextDirExplorer = new C7CloudDirExplorer(m_selectMode, this->size(), dirPath, this);
    connect(m_pNextDirExplorer, &C7CloudDirExplorer::sigItemClick, this, &C7CloudDirExplorerManager::onGetPath_Click);
    connect(m_pNextDirExplorer, &C7CloudDirExplorer::sigSelectItem, this, &C7CloudDirExplorerManager::sigSelectItem);
    connect(m_pNextDirExplorer, &C7CloudDirExplorer::sigNetworkError, this,[=](int error){
        m_pLoading->stopWaiting();
        emit sigNetworkError(error);
    });
    connect(m_pNextDirExplorer, &C7CloudDirExplorer::sigShowWaitBox, this,[=](bool s){
        if(s)
            m_pLoading->startWaiting();
        else {
            m_pLoading->stopWaiting();
        }
    });
    m_pBackBtn->raise();

    QPoint startPoint = QPoint(0, 0);
    if (dirPath == YUN_CHOICE)
    {
        m_pNextDirExplorer->setDisplayInfo(PAGECHOICE,PageChoice,obj);
    }
    else if (dirPath == YUN_FILE)
    {
         m_pNextDirExplorer->setDisplayInfo(m_yunPageNum,Yun_Page,obj);
         startPoint = QPoint(m_pNextDirExplorer->x() + this->width(), m_pNextDirExplorer->y());
    }
    else if (dirPath == PAN_FILE)
    {
         m_pNextDirExplorer->setDisplayInfo(m_yunPageNum,Pan_Page,obj);
         startPoint = QPoint(m_pNextDirExplorer->x() + this->width(), m_pNextDirExplorer->y());
    }

    m_pNextDirExplorer->show();
    //在动画结束时才做当前explorer的修正
    setAnimation(m_pNextDirExplorer, startPoint, m_pNextDirExplorer->pos());
}


void C7CloudDirExplorerManager::setAnimation(QObject* pObject, QVariant startValue, QVariant endValue)
{
    if (m_pAnimation == nullptr)
    {
        m_pAnimation = new QPropertyAnimation(this);
        m_pAnimation->setDuration(DURINGTIME);
        connect(m_pAnimation, &QPropertyAnimation::valueChanged,
                this, &C7CloudDirExplorerManager::onAnimationValueChange);
        connect(m_pAnimation, &QPropertyAnimation::finished,
                this, &C7CloudDirExplorerManager::onAnimationFinished);
    }
    else
    {
        if (m_pAnimation->state() == QAbstractAnimation::Running)
        {
            return;
        }
    }

    m_pAnimation->setTargetObject(pObject);
    m_pAnimation->setPropertyName("pos");
    m_pAnimation->setEasingCurve(QEasingCurve::Linear);
    m_pAnimation->setStartValue(startValue);
    m_pAnimation->setEndValue(endValue);
    m_bAnimationFinished = false;
    m_pAnimation->start();
}

void C7CloudDirExplorerManager::addFileType(int src, FileType fileType)
{
    if ((src & fileType) == fileType)
    {
        addFileType(fileType);
    }
}

QVariant C7CloudDirExplorerManager::analysisJson(QVariant data,int oriYear,int orimonth,int endYear,int endMonth)
{
    m_oriYear = oriYear;m_orimonth = orimonth;m_endYear = endYear;m_endMonth = endMonth;
    QJsonArray tardata;
    QJsonArray d = data.toJsonArray();
    int num2 = oriYear * 12 + orimonth;
    int num3 = endYear * 12 + endMonth;
    for(int i = 0;i<d.size();i++){
       QJsonObject source =  d.at(i).toObject();
       if(source.contains("created_at")){
           QString dat = source.value("created_at").toString();
           dat =  dat.section(' ', 0, 0);
           int year = dat.section('-',0,0).toInt();
           int month = dat.section('-',1,1).toInt();
           int num = year * 12 + month;
           if(num < num2 || num > num3)
               break;
           tardata.push_back(source);
       }
    }

    return  tardata;
}

void C7CloudDirExplorerManager::initUi()
{
    m_pBackBtn = new C7CloudExplorerBackButton(this->width() - 30*m_rScaleFactor, 45*m_rScaleFactor, this);
    m_pBackBtn->setBtnVisible(false);
    m_pLoading = new TLLoading(false,this);
    m_pLoading->setTimeoutTime(20000);
    connect(m_pBackBtn, &C7CloudExplorerBackButton::sigBack, this, &C7CloudDirExplorerManager::onCdUp);
    m_oriYear = QDateTime::currentDateTime().date().year();
    m_orimonth = QDateTime::currentDateTime().date().month();
    m_endYear = m_oriYear;
    m_endMonth = m_orimonth;
}

void C7CloudDirExplorerManager::onAnimationValueChange(const QVariant &value)
{
    if (m_pCurrentDirExplorer != nullptr)
    {
        QPoint p = value.toPoint();
        m_pCurrentDirExplorer->move(QPoint(p.x() - this->width(), m_pCurrentDirExplorer->y()));
    }
}

bool C7CloudDirExplorerManager::getExploreAnimationFlag()
{
    return m_bAnimationFinished;
}

void C7CloudDirExplorerManager::onAnimationFinished()
{
    if (m_bBack)
    {
        if (m_pPreviousDirExplorer != nullptr)
        {
            m_pPreviousDirExplorer->deleteLater();
            m_pPreviousDirExplorer = nullptr;
        }
        m_bBack = false;
    }
    else
    {
        m_pCurrentDirExplorer = m_pNextDirExplorer;
    }

    m_bAnimationFinished = true;
}


/*********************************************************************************************************************************/

C7CloudExplorerBackButton::C7CloudExplorerBackButton(int w, int h, QWidget *parent):
    C7BaseWidget(w, h, parent)
{
    initUi();
    retranslateUi();
}

void C7CloudExplorerBackButton::setBtnVisible(bool visible)
{
    m_pBackBtn->setVisible(visible);
//    m_pBackTextBtn->setVisible(visible);
}

void C7CloudExplorerBackButton::initUi()
{
    m_pBackBtn = new TLPushButton(this);
    m_pBackBtn->setNormalImage(m_strAppPath + "/skin/explorer/cloud_return_normal.png");
    m_pBackBtn->setHoverImage(m_strAppPath + "/skin/explorer/cloud_return_normal.png");
    m_pBackBtn->setPressImage(m_strAppPath + "/skin/explorer/cloud_return_press.png");
    m_pBackBtn->resize(45 * m_rScaleFactor, 45 * m_rScaleFactor);
    m_pBackBtn->move(20*m_rScaleFactor, 0);
    connect(m_pBackBtn, &TLPushButton::sigRelease, this, &C7CloudExplorerBackButton::sigBack);
}

void C7CloudExplorerBackButton::retranslateUi()
{
//    m_pBackTextBtn->setText(tr("Back"));
}

void C7CloudExplorerBackButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setBrush(QBrush(QColor(245, 245, 245)));
    painter.setPen(Qt::NoPen);
    painter.drawRect(this->rect());
    C7BaseWidget::paintEvent(event);
}

