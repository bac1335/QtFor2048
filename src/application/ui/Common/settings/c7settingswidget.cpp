#include "c7settingswidget.h"

#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QStackedWidget>

#include "c7commondef.h"
#include "c7settingsitem.h"
#include "tlutility.h"
#include "c7radiowidget.h"

#include "c7settingwidget/c7settinglanguagewidget.h"
#include "c7settingwidget/c7settingaccountwidget.h"
#include "c7settingwidget/c7registerwidget.h"
#include "c7settingwidget/c7settingscreenwidget.h"
#include "c7settingwidget/c7settingaboutwidget.h"
#include "c7settingwidget/c7settingpalmwidget.h"
#include "c7settingwidget/c7settingupdatewidget.h"
#include "c7settingwidget/c7settingparamswidget.h"
#include "tlinternet.h"


C7SettingsWidget::C7SettingsWidget(int width, int height, QWidget *parent)
    : C7BaseWidget(width, height, parent), m_version(cOem->version().branch)
{
    initUi();
}

void C7SettingsWidget::changeLoginState(bool b)
{
    m_pAccountWidget->changeLoginState(b);
}

void C7SettingsWidget::setScreenSize(int size)
{
    m_pAboutWidget->changeScreenSize(size);
}

void C7SettingsWidget::setAccountPage()
{
    m_pCurrentItem->setChecked(false);
    m_pStackWidget->setCurrentIndex(m_items.indexOf(m_pAccountItem));
    m_pCurrentItem = m_pAccountItem;
    m_pCurrentItem->setChecked(true);
}

void C7SettingsWidget::setUpdateState(bool state)
{
    m_pUpdateWidget->setUpdateState(state);
}

void C7SettingsWidget::updateInitParams()
{
    m_pParamsWidget->pullInitParams();
}

void C7SettingsWidget::initParamsConfig()
{
    m_pParamsWidget->initConfig();
}

void C7SettingsWidget::initUi()
{
    //检查网络状态
//    cInternet->check();
    m_nItemHeight = 30;   //这里没有乘缩放系数，所以后面引用到这个变量的时候要乘
    m_nItemInterval = 30;   //这里没有乘缩放系数，所以后面引用到这个变量的时候要乘
    m_bg.load(m_strAppPath + "/skin/settings/bg_settings.png");
    m_palette.setColor(QPalette::Text, Qt::black);

    m_pWidgetTitle = new QLabel(this);
    m_pWidgetTitle->setPalette(m_palette);
    QFont font = Uti->getFont(20 * m_rScaleFactor);
    m_pWidgetTitle->setFont(font);
    m_pWidgetTitle->setAlignment(Qt::AlignCenter);

    m_pStackWidget = new QStackedWidget(this);

    m_pBtnClose = new TLPushButton(this);
    m_pBtnClose->setImage(m_strAppPath + "/skin/settings/btn_close.png");
    connect(m_pBtnClose,&TLPushButton::sigPressed,this,&C7SettingsWidget::hideWithAnimation);

    addWidget();
    layoutItems();
    retranslateUi();
}

void C7SettingsWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::NoBrush);
    painter.drawPixmap(this->rect(), m_bg, m_bg.rect());
    C7BaseWidget::paintEvent(event);
}

void C7SettingsWidget::retranslateUi()
{
    m_pWidgetTitle->setText(tr("Settings"));

    if (m_pLanguageChoiceItem) {
        m_pLanguageChoiceItem->setText(tr("Language"));
    }

    if(m_pAccountItem)
    {
        m_pAccountItem->setText(tr("Account"));
    }

    if(m_pRegisterItem)
    {
        m_pRegisterItem->setText(tr("Activate"));
    }

    if(m_pScreenItem)
    {
        m_pScreenItem->setText(tr("Display"));
    }

//    if(m_pPalmEraseItem != nullptr)
//    {
//        m_pPalmEraseItem->setText(tr("Palm"));
//    }

    if(m_pParamsItem)
    {
        m_pParamsItem->setText(tr("Parameters"));
    }

    if(m_pAboutItem)
    {
        m_pAboutItem->setText(tr("About"));
    }

    if(m_pUpdateItem){
        m_pUpdateItem->setText(tr("Update"));
    }
}

void C7SettingsWidget::resizeEvent(QResizeEvent *event)
{
    qreal tempScale = m_rScaleFactor;       //保存原有缩放系数
    if (m_bUseAnimation)
    {
        m_rScaleFactor = this->width() * 1.0 / m_nNormalWidth * tempScale;        // 计算现在的缩放系数
        int pixsize = 18 * m_rScaleFactor;
        m_font.setPixelSize(pixsize <= 0 ? 1 : pixsize);
    }
    m_pBtnClose->resize(24* m_rScaleFactor,24* m_rScaleFactor);
    m_pBtnClose->move(this->width() - m_pBtnClose->width() - 29*m_rScaleFactor,
                      25*m_rScaleFactor);
    m_pWidgetTitle->resize(650 * m_rScaleFactor, 66 * m_rScaleFactor);
    m_pWidgetTitle->move(142 * m_rScaleFactor, 0 * m_rScaleFactor);
    m_pStackWidget->resize(650 * m_rScaleFactor, 500 * m_rScaleFactor);
    m_pStackWidget->move(146 * m_rScaleFactor, 76 * m_rScaleFactor);

    if (m_bUseAnimation)
    {
        layoutItems();
    }

    m_rScaleFactor = tempScale; //恢复原有缩放系数

    C7BaseWidget::resizeEvent(event);
}

void C7SettingsWidget::hideEvent(QHideEvent *)
{
    m_pCurrentItem->setChecked(false);
    m_pStackWidget->setCurrentIndex(0);
    m_pCurrentItem = m_items.at(0);
    m_pCurrentItem->setChecked(true);
}

void C7SettingsWidget::onItemClick()
{
     C7SettingsItem * item = qobject_cast<C7SettingsItem *>(sender());
     if (item != nullptr && item != m_pCurrentItem)
     {
         m_pCurrentItem->setChecked(false);
         m_pStackWidget->setCurrentIndex(m_items.indexOf(item));
         m_pCurrentItem = item;
     }
}

void C7SettingsWidget::addWidget()
{
    if(m_version == C7Version::Branch_Normal){
        addLangSettingWidget();
        addAccountWidget();
    }
    addParamsWidget();
    addRegisterWidget();
    if(m_version == C7Version::Branch_Normal){
//        addPalmEraseWidget();
        addUpdateWidget();
    }
//    addScreenWidget();
    addAboutWidget();
}


void C7SettingsWidget::addLangSettingWidget()
{
    m_pLanguageChoiceItem = createItem();
    m_pLanguageChoicetWidget = new C7SettingLanguageWidget(658 * m_rScaleFactor,510 * m_rScaleFactor,this);
    connect(m_pLanguageChoicetWidget,&C7SettingLanguageWidget::sigCurrentLanguage,this,&C7SettingsWidget::sigCurrentLanguage);
    m_pStackWidget->addWidget(m_pLanguageChoicetWidget);
}

void C7SettingsWidget::addAccountWidget()
{
    m_pAccountItem = createItem();
    m_pAccountWidget = new C7SettingAccountWidget(650 * m_rScaleFactor,510 * m_rScaleFactor,this);
    connect(m_pAccountWidget,&C7SettingAccountWidget::sigChangeLoginState,this,&C7SettingsWidget::sigChangeLoginState);
    connect(m_pAccountWidget,&C7SettingAccountWidget::sigShowLoginWidget,this,[=](){
       emit sigShowLoginWidget(false, true);
    });
    m_pStackWidget->addWidget(m_pAccountWidget);
}

void C7SettingsWidget::addRegisterWidget()
{
    m_pRegisterItem = createItem();
    m_pRegisterWidget = new C7RegisterWidget(650 * m_rScaleFactor,510 * m_rScaleFactor,this);
    m_pStackWidget->addWidget(m_pRegisterWidget);
}

void C7SettingsWidget::addScreenWidget()
{
    m_pScreenItem = createItem();
    m_pScreenWidget = new C7SettingScreenWidget(650 * m_rScaleFactor,510 * m_rScaleFactor,this);
    connect(m_pScreenWidget,&C7SettingScreenWidget::sigCurrentScreenType,this,&C7SettingsWidget::sigCurrentScreenType);
    m_pStackWidget->addWidget(m_pScreenWidget);
}

void C7SettingsWidget::addAboutWidget()
{
    m_pAboutItem = createItem();
    m_pAboutWidget = new C7SettingAboutWidget(650 * m_rScaleFactor,510 * m_rScaleFactor,this);
    m_pStackWidget->addWidget(m_pAboutWidget);
}

void C7SettingsWidget::addParamsWidget()
{
    m_pParamsItem = createItem();
    m_pParamsWidget = new C7SettingParamsWidget(650 * m_rScaleFactor,510 * m_rScaleFactor,this);
    connect(m_pParamsWidget,&C7SettingParamsWidget::sigParamsChanged,
            this,&C7SettingsWidget::sigParamsChanged);
    if(m_version == C7Version::Branch_Oversea){
        m_pParamsWidget->setPalmEraserVisible(false);
    }
    m_pStackWidget->addWidget(m_pParamsWidget);

}

//void C7SettingsWidget::addPalmEraseWidget()
//{
//    m_pPalmEraseItem = createItem();
//    m_pPalmEraseWidget = new C7SettingPalmWidget(650 * m_rScaleFactor,510 * m_rScaleFactor,this);
//    connect(m_pPalmEraseWidget,&C7SettingPalmWidget::sigCurrentPalmType,this,&C7SettingsWidget::sigCurrentPalmType);
//    m_pStackWidget->addWidget(m_pPalmEraseWidget);
//}

void C7SettingsWidget::addUpdateWidget()
{
    m_pUpdateItem = createItem();
    m_pUpdateWidget = new C7SettingUpdateWidget(this);
    connect(m_pUpdateWidget,&C7SettingUpdateWidget::sigUpdate,this,&C7SettingsWidget::sigUpdate);
    m_pUpdateWidget->resize(650 * m_rScaleFactor,510 * m_rScaleFactor);
//    connect(m_pUpdateWidget,&C7SettingUpdateWidget::sigCurrentPalmType,this,&C7SettingsWidget::sigCurrentPalmType);
    m_pStackWidget->addWidget(m_pUpdateWidget);
}

bool C7SettingsWidget::getSettingStatus(const QString &key, bool de)
{
    return Uti->value(SETTING_GROUP, key, de).toBool();
}

void C7SettingsWidget::layoutItems()
{
    C7SettingsItem* pItem = nullptr;
    for (int i = 0, count = m_items.count(); i < count; ++i)
    {
        pItem = m_items.at(i);
        if (m_bUseAnimation)
        {
            pItem->setScaleFactor(m_rScaleFactor);                              //响应动画用
            pItem->resize(148 * m_rScaleFactor, m_nItemHeight * m_rScaleFactor);//响应动画用
            pItem->setFont(m_font);
        }
        pItem->move(7 * m_rScaleFactor, i * ((m_nItemHeight + m_nItemInterval) * m_rScaleFactor) + 115 * m_rScaleFactor);
    }
}

C7SettingsItem *C7SettingsWidget::createItem()
{
    C7SettingsItem* pItem = new C7SettingsItem(145 * m_rScaleFactor, m_nItemHeight * m_rScaleFactor, this);
    pItem->setTextColor(Color_MenuItem_Text_Normal, Color_MenuItem_Text_Checked);
    connect(pItem, &C7SettingsItem::sigClicked, this, &C7SettingsWidget::onItemClick);
    if (m_pCurrentItem == nullptr)
    {
        m_pCurrentItem = pItem;
        m_pCurrentItem->setChecked(true);
    }

    m_items << pItem;
    return pItem;
}
