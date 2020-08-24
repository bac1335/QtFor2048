#include "c7cloudyunmanager.h"
#include "c7clouditem.h"
#include <QCalendarWidget>
#include "Utility/tlutility.h"
#include "tlhttpdef.h"

C7CloudYunManager::C7CloudYunManager(int w, int h, C7BaseWidget *parent):
    C7BaseWidget(w,h,parent)
{
    initUi();
}

void C7CloudYunManager::setCurrentWidget(bool b)
{
    m_pStackWidget->setCurrentIndex((int)b);
}

void C7CloudYunManager::changeLoginState(bool state)
{
    setCurrentWidget(state);
    if(!state){
        m_pCloudImportWidget->initState();
        setDataChoixeVisble(state);
        m_fileChoice->setVisible(state);
    }
}

void C7CloudYunManager::setDataChoixeVisble(bool b)
{
    m_timeChoiceEnd->setVisible(b);
    m_timeChoiceOrigin->setVisible(b);
    m_dataSureBtn->setVisible(b);
    m_textLabel->setVisible(b);
}

void C7CloudYunManager::initUi()
{

    m_pStackWidget = new QStackedWidget(this);
    m_pStackWidget->setFixedSize(this->width(),this->height());

    m_accountLogin = new C7SettingAccountWidget(m_pStackWidget->width(), m_pStackWidget->height(),this);
    connect(m_accountLogin,&C7SettingAccountWidget::sigShowLoginWidget,this,&C7CloudYunManager::sigShowLoginWidget);

    m_fileChoice = new C7TimeChoiceWidget(160 * m_rScaleFactor,38* m_rScaleFactor,this);

    connect(m_fileChoice,&C7TimeChoiceWidget::sigSearchSet,this,&C7CloudYunManager::onSearchSet);
    m_fileChoice->move(this->width() - m_fileChoice->width()-30 * m_rScaleFactor,5 * m_rScaleFactor);
    m_fileChoice->setBg(m_strAppPath + "/skin/bg/bg_yunfile.png");
    m_fileChoice->hide();

    m_timeChoiceOrigin = new C7TimeChoice(140* m_rScaleFactor,40* m_rScaleFactor,this);
    connect(m_timeChoiceOrigin,&C7TimeChoice::sigTimeChoice,this,&C7CloudYunManager::onTimeChoice);
    m_timeChoiceOrigin->move(90 * m_rScaleFactor,5 * m_rScaleFactor);
    m_timeChoiceOrigin->initPos();
    m_timeChoiceEnd = new C7TimeChoice(140* m_rScaleFactor,40* m_rScaleFactor,this);
    connect(m_timeChoiceEnd,&C7TimeChoice::sigTimeChoice,this,&C7CloudYunManager::onTimeChoice);
    m_textLabel = new QLabel(this);
    m_textLabel->setFont(Uti->getFont(15*m_rScaleFactor));
    m_textLabel->setText(tr("to"));
    m_textLabel->adjustSize();
    m_textLabel->move(m_timeChoiceOrigin->x() + m_timeChoiceOrigin->width() + 10 * m_rScaleFactor,
                      m_timeChoiceOrigin->y() + m_timeChoiceOrigin->height()/2. - m_textLabel->height()/2.);
    m_timeChoiceEnd->move(m_textLabel->x() +m_textLabel->width() + 10 * m_rScaleFactor,
                          5 * m_rScaleFactor);
    m_timeChoiceEnd->initPos();

    m_dataSureBtn = new TLPushButton(this);
    m_dataSureBtn->resize(82 * m_rScaleFactor,35 * m_rScaleFactor);
    m_dataSureBtn->setFont(Uti->getFont(12*m_rScaleFactor));
    m_dataSureBtn->setText(tr("sure"));

    m_dataSureBtn->setNormalBackColor(Color_MenuButton_Normal);
    m_dataSureBtn->setHoverBackColor(Color_MenuButton_Hover);
    m_dataSureBtn->setPressBackColor(Color_MenuButton_Press);
    m_dataSureBtn->setDisableBackColor(QColor("#666666"));
    m_dataSureBtn->setDisableTextColor(Color_MenuButton_Text);
    m_dataSureBtn->setTextColor(Color_MenuButton_Text);
    m_dataSureBtn->setRadius(15 * m_rScaleFactor);
    m_dataSureBtn->setTextColor(Qt::white);
    m_dataSureBtn->move(m_timeChoiceEnd->x() + m_timeChoiceEnd->width() + 10 * m_rScaleFactor,
                        m_timeChoiceEnd->y() + m_timeChoiceEnd->height()/2.- m_timeChoiceEnd->height()/2.);

    m_dataSureBtn->hide();
    connect(m_dataSureBtn,&TLPushButton::sigRelease,this,&C7CloudYunManager::onDataSureClicked);

    setDataChoixeVisble(false);
    m_pStackWidget->addWidget(m_accountLogin);

    m_pCloudImportWidget = new C7CloudDirExplorerManager(File_Only,QSize(m_pStackWidget->width(), m_pStackWidget->height()),this);
    connect(m_pCloudImportWidget,&C7CloudDirExplorerManager::sigSelectItem,this,&C7CloudYunManager::sigSelectItem);
    connect(m_pCloudImportWidget,&C7CloudDirExplorerManager::sigBackBtnClick,this,&C7CloudYunManager::sigBackBtnClick);
    connect(m_pCloudImportWidget,&C7CloudDirExplorerManager::sigNetworkError,this,&C7CloudYunManager::onNetworkError);
    connect(m_pCloudImportWidget,&C7CloudDirExplorerManager::sigFlieClicked,this,&C7CloudYunManager::sigFlieClicked);
    connect(m_pCloudImportWidget,&C7CloudDirExplorerManager::sigShowTimeChoice,this,[=](bool b){
        m_fileChoice->setVisible(b);
        if(!b || (b&&m_searchType == Type_Search_Time))
            setDataChoixeVisble(b);
    });
    m_pCloudImportWidget->setFileType((int)(AllFile));
    m_pCloudImportWidget->changeFirstDir(YUN_CHOICE);
    m_pStackWidget->addWidget(m_pCloudImportWidget);

    m_noticeText = new TLNotice(this);
    m_noticeText->setDefinedFont(Uti->getFont(20 * m_rScaleFactor));
    m_noticeText->setNoticetHeight(40 * m_rScaleFactor);
    m_noticeText->setDefaultCenterPos(QPoint(this->width()/2.,
                                         this->height()*3/4.));
    m_noticeText->setShowTime(3000);
//    m_noticeText->notice(tr("Network exception, please wait"));
}

void C7CloudYunManager::retranslateUi()
{
    if(m_dataSureBtn)
        m_dataSureBtn->setText(tr("sure"));
    if(m_textLabel)
        m_textLabel->setText(tr("to"));
}

void C7CloudYunManager::onSearchSet(SearchType type)
{
    setDataChoixeVisble(type);
    m_searchType = type;
    m_pCloudImportWidget->searchSetLast(m_searchType);
}

void C7CloudYunManager::onDataSureClicked()
{
     m_timeChoiceOrigin->hideChoiceData();
     m_timeChoiceEnd->hideChoiceData();

     int originYear = m_timeChoiceOrigin->getYear();
     int originMonth = m_timeChoiceOrigin->getMonth();
     int endYear = m_timeChoiceEnd->getYear();
     int endMonth = m_timeChoiceEnd->getMonth();

     m_pCloudImportWidget->searchSet(m_searchType,originYear,originMonth,endYear,endMonth);
}

void C7CloudYunManager::onTimeChoice(C7DataTime *t)
{

    if(m_currentTimeChoice == t){
        m_currentTimeChoice->setVisible(false);
        m_currentTimeChoice = nullptr;
        return;
    }

    if(m_currentTimeChoice)
        m_currentTimeChoice->setVisible(false);

    m_currentTimeChoice = t;
    m_currentTimeChoice->setVisible(true);
}

void C7CloudYunManager::onNetworkError(int errorcode)
{
    if(ERROR_CODE_NETWORKTIMEOUT == errorcode){
        m_noticeText->notice(tr("Timed out,please check the network settings"));
    }
    else if(ERROR_CODE_NETWORK == errorcode){
        m_noticeText->notice(tr("The network is abnormal,please check"));
    }
    else if(ERROR_CODE_FILEDOWNLOAD == errorcode){
        m_noticeText->notice(tr("File download failed"));
    }
    else if(ERROR_CODE_SERVER_ABNORMAL == errorcode){
        m_noticeText->notice(tr("The server returned an abnormal data"));
    }
    else{
        m_noticeText->notice(tr("Network exception"));
    }

    qDebug() << "-->lls<--" << __FUNCTION__ << errorcode;
}

C7TimeChoiceWidget::C7TimeChoiceWidget(int w, int h, QWidget *parent):
    C7DoubleSelectWidget(w, h, parent,10,5,14)
{
    this->setBgText(tr("All"), tr("Time"));
    this->setFont(Uti->getFont(14 * m_rScaleFactor));
    connect(this, &C7TimeChoiceWidget::sigChecked, this, [=](bool checked){
        SearchType type;
        if(checked)
        {
            type = Type_Search_Time;
        }
        else
        {
            type = Type_Search_All;;
        }
        emit sigSearchSet(type);
    });
}

void C7TimeChoiceWidget::retranslateUi()
{
     this->setBgText(tr("All"), tr("Time"));
}
