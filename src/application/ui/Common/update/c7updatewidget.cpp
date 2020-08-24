#include "c7updatewidget.h"
#include <QPainter>
#include <QLabel>
#include <QTextEdit>
#include "tlpushbutton.h"
#include "tlutility.h"
#include "c7commondef.h"

C7UpdateWidget::C7UpdateWidget(int w, int h, QWidget *parent):
    C7BaseWidget(w,h,parent), m_curVersion("1.0.0"), m_newVersion("1.0.1")
{
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    initUi();
    retranslateUi();
}

void C7UpdateWidget::setUpdateInfo(const UpdateVersionInfo &info)
{
    m_packagePath = info.packagePath;
    m_pDescribEdit->setText(info.descridText);
    m_curVersion = info.curVersion;
    m_newVersion = info.newVersion;
}

void C7UpdateWidget::initUi()
{
    m_bg.load(m_strAppPath + "/skin/bg/bg_update.png");

    m_pTitleLabel = new QLabel(this);
    m_pTitleLabel->setText("5555");
    QFont ft = Uti->getFont(20*m_rScaleFactor);
    m_pTitleLabel->setFont(ft);
    m_pTitleLabel->setAlignment(Qt::AlignCenter);

    m_pDescribEdit = new QTextEdit(this);
    m_pDescribEdit->setReadOnly(true);
    m_pDescribEdit->setFont(Uti->getFont(16*m_rScaleFactor));
    m_pDescribEdit->setStyleSheet(QString("QTextEdit{border:%1px solid black;border-radius:%2px;padding:%3px;}"
                                          "QTextEdit:enabled{background-color:white;}").
                                     arg(1).arg(6*m_rScaleFactor).arg(4*m_rScaleFactor));

    //取消
    m_pCancelBtn = new TLPushButton(this);
    m_pCancelBtn->setFont(m_font);
    m_pCancelBtn->setNormalBackColor(Color_CancelButton_Normal);
    m_pCancelBtn->setHoverBackColor(Color_CancelButton_Normal);
    m_pCancelBtn->setPressBackColor(Color_CancelButton_Press);
    m_pCancelBtn->setNormalTextColor(Color_Cancel_Text_Normal);
    m_pCancelBtn->setBorderLine(true);
    m_pCancelBtn->setRadius(6 * m_rScaleFactor);
    m_pCancelBtn->resize(150*m_rScaleFactor, 48*m_rScaleFactor);
    connect(m_pCancelBtn, &TLPushButton::sigClicked, this, &C7UpdateWidget::hide);

    //确认
    m_pUpdateBtn = new TLPushButton(this);
    m_pUpdateBtn->setFont(m_font);
    m_pUpdateBtn->setNormalBackColor(Color_MenuButton_Normal);
    m_pUpdateBtn->setHoverBackColor(Color_MenuButton_Hover);
    m_pUpdateBtn->setPressBackColor(Color_MenuButton_Press);
    m_pUpdateBtn->setDisableBackColor(Color_MenuButton_Disable);
    m_pUpdateBtn->setTextColor(Color_MenuButton_Text);
    m_pUpdateBtn->setRadius(6 * m_rScaleFactor);
    m_pUpdateBtn->resize(280*m_rScaleFactor, 48*m_rScaleFactor);
    connect(m_pUpdateBtn, &TLPushButton::sigClicked, this, [=](){
        sigUpdate(m_packagePath);
    });

    m_pTitleLabel->resize(this->width(), 30*m_rScaleFactor);
    m_pTitleLabel->move(this->width()/2 - m_pTitleLabel->width()/2, 45*m_rScaleFactor);
    m_pDescribEdit->resize(447*m_rScaleFactor, 290*m_rScaleFactor);
    m_pDescribEdit->move(this->width()/2 - m_pDescribEdit->width()/2,
                         m_pTitleLabel->y() + m_pTitleLabel->height() + 20*m_rScaleFactor);
    m_pCancelBtn->move(m_pDescribEdit->x() - 3*m_rScaleFactor, m_pDescribEdit->y() + m_pDescribEdit->height() + 20*m_rScaleFactor);
    m_pUpdateBtn->move(m_pCancelBtn->x() + m_pCancelBtn->width() + 22*m_rScaleFactor, m_pCancelBtn->y());

}

void C7UpdateWidget::retranslateUi()
{
    m_pTitleLabel->setText(tr("New version"));
    m_pUpdateBtn->setText(tr("Update"));
    m_pCancelBtn->setText(tr("Cancel"));
}

void C7UpdateWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawImage(this->rect(), m_bg);

    painter.setBrush(QColor("#32bc84"));
    QRect versionRect = QRect(360*m_rScaleFactor, 48*m_rScaleFactor, 110*m_rScaleFactor, 30*m_rScaleFactor);
    painter.drawRoundedRect(versionRect, 15*m_rScaleFactor, 15*m_rScaleFactor);
    painter.setPen(Qt::white);
    painter.setFont(m_font);
    painter.drawText(versionRect, Qt::AlignCenter, QString("V%1").arg(m_newVersion));
    C7BaseWidget::paintEvent(e);
}
