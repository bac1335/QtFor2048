#include "c7settingaccountwidget.h"
#include "c7commondef.h"
#include "tlutility.h"
#include "c7oem.h"

C7SettingAccountWidget::C7SettingAccountWidget(int width,int height,C7BaseWidget *parent)
    :C7BaseWidget(width,height,parent)
{
    initUI();
    retranslateUi();
}

C7SettingAccountWidget::~C7SettingAccountWidget()
{

}

void C7SettingAccountWidget::setSignInInfo(bool bSignIn, const QString &userName)
{
    m_bLogin = bSignIn;
    m_loginUserName = userName;
    retranslateUi();
}

void C7SettingAccountWidget::changeLoginState(bool s)
{
    m_bLogin = s;
    retranslateUi();
}

void C7SettingAccountWidget::initUI()
{
    m_pAccountLabel = new QLabel(this);
    m_pAccountLabel->setFont(m_font);
    m_pAccountLabel->setAlignment(Qt::AlignCenter);
    m_pAccountLabel->resize(this->width() - 67 * m_rScaleFactor, 160 * m_rScaleFactor);
    m_pAccountLabel->move((this->width() - m_pAccountLabel->width())/2.,
                      50 * m_rScaleFactor);

    m_pBtnLogin = new TLPushButton(this);

    m_pBtnLogin->setFixedSize(118 * m_rScaleFactor, 48 * m_rScaleFactor);
    m_pBtnLogin->move((this->width() - m_pBtnLogin->width())/2.,235 * m_rScaleFactor);
    m_pBtnLogin->setFont(m_font);
    m_pBtnLogin->setTextColor(Color_MenuButton_Text);
    m_pBtnLogin->setRadius(6 * m_rScaleFactor);
    connect(m_pBtnLogin, &TLPushButton::sigClicked, this,&C7SettingAccountWidget::onBtnLoginClicked);
}

void C7SettingAccountWidget::retranslateUi()
{
    if (m_bLogin)
    {
        m_loginUserName = cOem->getLoginState().usingName;
        m_pAccountLabel->setText(tr("User: %1").arg(m_loginUserName));
        m_pBtnLogin->setText(tr("Sign out"));
        m_pBtnLogin->setNormalBackColor(QColor("#c93e33"));
        m_pBtnLogin->setHoverBackColor(QColor("#c93e33"));
        m_pBtnLogin->setPressBackColor(QColor("#b42e23"));
    }
    else
    {
        m_pAccountLabel->setText(tr("Not Login"));
        m_pBtnLogin->setText(tr("Login"));
        m_pBtnLogin->setNormalBackColor(Color_MenuButton_Normal);
        m_pBtnLogin->setHoverBackColor(Color_MenuButton_Hover);
        m_pBtnLogin->setPressBackColor(Color_MenuButton_Press);
    }
}

void C7SettingAccountWidget::resizeEvent(QResizeEvent *event)
{
    m_pAccountLabel->resize(this->width() - 67 * m_rScaleFactor, 160 * m_rScaleFactor);
    m_pAccountLabel->move((this->width() - m_pAccountLabel->width())/2.,
                      50 * m_rScaleFactor);
    m_pBtnLogin->move((this->width() - m_pBtnLogin->width())/2.,235 * m_rScaleFactor);
}

void C7SettingAccountWidget::onLoginStatus(bool bLogin, const QString &userName)
{
    setSignInInfo(bLogin, userName);
}

void C7SettingAccountWidget::onBtnLoginClicked()
{
    if (m_bLogin)
    {
        cOem->setLoginIn({false,-1,"","",""});
        emit sigChangeLoginState(false);
        setSignInInfo(false);
    }
    else
    {
        emit sigShowLoginWidget(false, true);
    }
}
