#include "c7settingupdatewidget.h"
#include <QLabel>
#include "tlpushbutton.h"
#include "c7commondef.h"

C7SettingUpdateWidget::C7SettingUpdateWidget(QWidget *parent):
    C7BaseWidget(parent)
{
    initUi();
}

void C7SettingUpdateWidget::setUpdateState(bool state)
{
    m_pUpdateBtn->setVisible(state);
    m_pTextLabel->setVisible(!state);
}

void C7SettingUpdateWidget::initUi()
{

    m_pUpdateBtn = new TLPushButton(this);
    m_pUpdateBtn->setFont(m_font);
    m_pUpdateBtn->setNormalBackColor(Color_MenuButton_Normal);
    m_pUpdateBtn->setHoverBackColor(Color_MenuButton_Hover);
    m_pUpdateBtn->setPressBackColor(Color_MenuButton_Press);
    m_pUpdateBtn->setDisableBackColor(Color_MenuButton_Disable);
    m_pUpdateBtn->setTextColor(Color_MenuButton_Text);
    m_pUpdateBtn->setRadius(6 * m_rScaleFactor);
    m_pUpdateBtn->hide();
    connect(m_pUpdateBtn,&TLPushButton::sigClicked,this,&C7SettingUpdateWidget::sigUpdate);

    m_pTextLabel = new QLabel(this);
    m_font.setPixelSize(20*m_rScaleFactor);
    m_pTextLabel->setFont(m_font);
    m_pTextLabel->setAlignment(Qt::AlignCenter);

    setUpdateState(false);

}

void C7SettingUpdateWidget::retranslateUi()
{
    m_pTextLabel->setText(tr("Currently the latest version"));
    m_pUpdateBtn->setText(tr("New version"));
}

void C7SettingUpdateWidget::resizeEvent(QResizeEvent *event)
{
    m_pTextLabel->resize(this->width(), 35*m_rScaleFactor);
    m_pTextLabel->move(0, this->height()/2 - m_pTextLabel->height()/2);
    m_pUpdateBtn->resize(120*m_rScaleFactor, 48*m_rScaleFactor);
    m_pUpdateBtn->move(this->width()/2 - m_pUpdateBtn->width()/2, this->height()/2 - m_pUpdateBtn->height()/2);
}
