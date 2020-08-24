#ifndef C7SETTINGLANGUAGEWIDGET_H
#define C7SETTINGLANGUAGEWIDGET_H
#include "c7basewidget.h"
#include "../c7radiowidget.h"
#include "../c7settingdef.h"

/*
 * @brief   : 语言选择界面
 * @author  : lls
 * @date    : 2019.9.05
 */

class C7SettingLanguageWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7SettingLanguageWidget(int width,int height,C7BaseWidget* parent = nullptr);
    ~C7SettingLanguageWidget();

private:
    void initUI();
    C7RadioWidget* createRadioWidget(int currentSelect);

protected:
    void retranslateUi();

private:
    C7RadioWidget* m_pSimplifiedWidget = nullptr;
    C7RadioWidget* m_pComplexWidget = nullptr;
    C7RadioWidget* m_pEnglishWidget = nullptr;
    QList<C7RadioWidget*> m_lstLanguageChoice;
    int m_nNormalSettingInterval = 5;

private slots:
    void onRadioWidgetClick();

signals:
    void sigCurrentLanguage(LanguageType sizeType);
};


#endif // C7SETTINGLANGUAGEWIDGET_H
