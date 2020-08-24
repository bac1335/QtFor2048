#ifndef C7SettingAccountWidget_H
#define C7SettingAccountWidget_H
#include "c7basewidget.h"
#include "../c7settingdef.h"
#include <QLabel>
#include "button/widget/tlpushbutton.h"

/*
 * @brief   : 账户登陆界面
 * @author  : lls
 * @date    : 2019.9.05
 */

class  C7SettingAccountWidget : public  C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7SettingAccountWidget(int width,int height,C7BaseWidget* parent = nullptr);
    ~C7SettingAccountWidget();
    void setSignInInfo(bool bSignIn, const QString& userName = QString(""));
    void changeLoginState(bool s);

private:
    void initUI();

protected:
    void resizeEvent(QResizeEvent *event);
    void retranslateUi();

private:

    bool m_bLogin = false;
    QString m_loginUserName;
    QLabel * m_pAccountLabel = nullptr;
    TLPushButton* m_pBtnLogin = nullptr;

public slots:
    void onLoginStatus(bool bLogin, const QString& userName);

private slots:
    void onBtnLoginClicked();

signals:
    void sigChangeLoginState(bool);

    void sigShowLoginWidget(bool bTransparency, bool bMainWindow);
};


#endif // C7SettingAccountWidget_H
