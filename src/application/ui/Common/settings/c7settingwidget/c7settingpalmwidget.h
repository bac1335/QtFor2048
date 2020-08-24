#ifndef C7SETTINGPALMWIDGET_H
#define C7SETTINGPALMWIDGET_H

#include "c7basewidget.h"
#include "../c7radiowidget.h"
#include "../c7settingdef.h"
#include "c7commondef.h"

/*
 * @brief   : 手掌擦除选择选择界面
 * @author  : lls
 * @date    : 2019.9.05
 */

class C7SettingPalmWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7SettingPalmWidget(int width,int height,C7BaseWidget* parent = nullptr);
    ~C7SettingPalmWidget();

private:
    void initUI();
    C7RadioWidget* createRadioWidget(int currentSelect);
    void retranslateUi();

private:
    C7RadioWidget* m_pPalmEraseWidget = nullptr;
    C7RadioWidget* m_pNoPalmWidget = nullptr;
    QList<C7RadioWidget*> m_lstChoice;
    int m_nNormalSettingInterval = 5;

private slots:
    void onRadioWidgetClick();

signals:
    void sigCurrentPalmType(PalmType sizeType);
};


#endif // C7SETTINGPALMWIDGET_H
