#ifndef C7SETTINGSWIDGET_H
#define C7SETTINGSWIDGET_H

#include "c7basewidget.h"
#include "c7settingdef.h"
#include "c7commondef.h"

/*
 * @brief   : 设置主界面
 * @author  : lls
 * @date    : 2019.9.04
 */

class QLabel;
class C7SettingsItem;
class QStackedWidget;
class C7SettingSubWidget;
class TLPushButton;

class C7SettingLanguageWidget;
class C7SettingAccountWidget;
class C7RegisterWidget;
class C7SettingScreenWidget;
class C7SettingAboutWidget;
class C7SettingPalmWidget;
class C7SettingUpdateWidget;
class C7SettingParamsWidget;

class C7SettingsWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7SettingsWidget(int width, int height, QWidget* parent = nullptr);
    void changeLoginState(bool b);
    void setScreenSize(int size);
    void setAccountPage();
    void setUpdateState(bool state);
    void updateInitParams(); //更新参数页面数据
    void initParamsConfig();

protected:
    void initUi();
    void paintEvent(QPaintEvent *event);
    void retranslateUi();
    void resizeEvent(QResizeEvent *event);
    void hideEvent(QHideEvent*);

private slots:
    void onItemClick();
private:
    void addWidget();
    void addLangSettingWidget();//语言选择界面
    void addAccountWidget(); //账号界面
    void addRegisterWidget(); //注册界面
    void addScreenWidget(); //单双屏选择
    void addAboutWidget(); //关于界面
//    void addPalmEraseWidget(); //手掌识别界面
    void addParamsWidget();  //参数设置页面
    void addUpdateWidget();

    bool getSettingStatus(const QString& key,bool de);
    void layoutItems();
    C7SettingsItem* createItem();

private:
    int m_nItemHeight;   //左侧按键高度，便于重排
    int m_nItemInterval; //左侧按键间隔，便于重排
    QLabel* m_pWidgetTitle = nullptr;
    QPalette m_palette;
    QPixmap m_bg;
    QStackedWidget * m_pStackWidget = nullptr;

    C7SettingsItem* m_pLanguageChoiceItem = nullptr;  //语言选择界面
    C7SettingLanguageWidget* m_pLanguageChoicetWidget = nullptr;

    C7SettingsItem* m_pAccountItem = nullptr;  //账号选择界面
    C7SettingAccountWidget* m_pAccountWidget = nullptr;

    C7SettingsItem* m_pRegisterItem = nullptr;  //激活界面
    C7RegisterWidget* m_pRegisterWidget = nullptr;

    C7SettingsItem* m_pScreenItem = nullptr;  //单双屏选择
    C7SettingScreenWidget* m_pScreenWidget = nullptr;

    C7SettingsItem* m_pAboutItem = nullptr;  //关于界面
    C7SettingAboutWidget* m_pAboutWidget = nullptr;

//    C7SettingsItem* m_pPalmEraseItem = nullptr;  //手掌界面
//    C7SettingPalmWidget* m_pPalmEraseWidget = nullptr;
    C7SettingsItem* m_pParamsItem = nullptr;  //手掌界面
    C7SettingParamsWidget* m_pParamsWidget = nullptr;

    C7SettingsItem* m_pUpdateItem = nullptr;  //手掌界面
    C7SettingUpdateWidget* m_pUpdateWidget = nullptr;

    C7SettingsItem* m_pCurrentItem = nullptr;
    QList<C7SettingsItem *> m_items;
    TLPushButton* m_pBtnClose = nullptr;

    int m_version = 0;

signals:
    void sigCurrentLanguage(LanguageType type);
//    void sigCurrentPalmType(PalmType sizeType);
    void sigParamsChanged(const ParamsSettingsType &type,bool check);
    void sigCurrentScreenType(ScreenType sizeType);
    void sigShowLoginWidget(bool bTransparency, bool bMainWindow); //账号登陆
    void sigChangeLoginState(bool);
    void sigUpdate();
};
#endif // C7SETTINGSWIDGET_H
