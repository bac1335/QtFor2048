#ifndef C7SETTINGSCREENWIDGET_H
#define C7SETTINGSCREENWIDGET_H

#include "c7basewidget.h"
#include "../c7radiowidget.h"
#include "../c7settingdef.h"

/*
 * @brief   : 单双屏选择界面
 * @author  : lls
 * @date    : 2019.9.05
 */


class C7SettingScreenWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7SettingScreenWidget(int width,int height,C7BaseWidget* parent = nullptr);
    ~C7SettingScreenWidget();

private:
    void initUI();
    C7RadioWidget* createRadioWidget(int currentSelect);
    void retranslateUi();

private:
    C7RadioWidget* m_pDualScreenWidget = nullptr;
    C7RadioWidget* m_pSignalScreenWidget = nullptr;
    QList<C7RadioWidget*> m_lstScreenChoice;
    int m_nNormalSettingInterval = 5;

private slots:
    void onRadioWidgetClick();

signals:
    void sigCurrentScreenType(ScreenType sizeType);
};


#endif // C7SETTINGSCREENWIDGET_H
