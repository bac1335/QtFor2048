#include "c7settingaboutwidget.h"
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
#include "tlutility.h"
#include "c7oem.h"

C7SettingAboutWidget::C7SettingAboutWidget(int width,int height,C7BaseWidget *parent)
    :C7BaseWidget(width,height,parent), m_version(cOem->version().branch)
{
    initUi();
}

C7SettingAboutWidget::~C7SettingAboutWidget()
{

}

void C7SettingAboutWidget::setVerson(qreal verson)
{
    QFontMetrics metrics(m_font);
    m_versionStr = "V" + QString::number(verson,'f',1);
    update();
}

void C7SettingAboutWidget::changeScreenSize(int size)
{
    m_sSize = size;
    retranslateUi();
    update();
}

void C7SettingAboutWidget::initUi()
{    
    m_logoBg.load(m_strAppPath + "/skin/settings/logo.png");

    if(m_version == C7Version::Branch_Normal){
        m_pLinkLabel = new QLabel(tr("Learn more please visit: <a style=color:#32bc84; href = http://www.tyingyun.com>www.tyingyun.com</a>"), this);
        m_pLinkLabel->setAlignment(Qt::AlignCenter);
        m_pLinkLabel->setFont(m_font);
        m_pLinkLabel->setStyleSheet("color:#32bc84;");
        connect(m_pLinkLabel, &QLabel::linkActivated, this, [=](const QString& link){
            QDesktopServices::openUrl(QUrl(link));
        });
    }
}

void C7SettingAboutWidget::resizeEvent(QResizeEvent *event)
{
    if(m_version == C7Version::Branch_Normal){
        m_pLinkLabel->resize(this->width(), 30*m_rScaleFactor);
        m_pLinkLabel->move(this->width()/2 - m_pLinkLabel->width()/2, 380*m_rScaleFactor);
    }
}

void C7SettingAboutWidget::retranslateUi()
{
    if(m_version == C7Version::Branch_Normal){
        m_pLinkLabel->setText(tr("Learn more please visit: <a style=color:#32bc84; href = http://www.tyingyun.com>www.tyingyun.com</a>"));
    }
}

void C7SettingAboutWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::NoPen);
    QRect pixmapRect = QRect(this->width()/2 - 64*m_rScaleFactor, 70*m_rScaleFactor, 128*m_rScaleFactor, 128*m_rScaleFactor);
    painter.drawPixmap(pixmapRect, m_logoBg);
    QRect nameRect = QRect(0, pixmapRect.y() + pixmapRect.height() + 10*m_rScaleFactor, this->width(), 25*m_rScaleFactor);
    painter.setFont(m_font);
    painter.setPen(Qt::black);
    painter.drawText(nameRect, Qt::AlignCenter, tr("CloudBoard 7"));
    QRect versionRect = QRect(0, nameRect.y() + nameRect.height() + 5*m_rScaleFactor, this->width(), 20*m_rScaleFactor);
    painter.setFont(Uti->getFont(16*m_rScaleFactor));
    painter.setPen(QColor(180, 180, 180));
    painter.drawText(versionRect, Qt::AlignCenter, tr("Version : V %1").arg(cOem->version().toString()));
    painter.setPen(QColor(200, 200, 200));
    painter.drawText(QRect(0, versionRect.y() + versionRect.height() + 185*m_rScaleFactor, this->width(), 20*m_rScaleFactor),
                     Qt::AlignCenter, tr("Screen Size : %1Inch").arg(m_sSize));
    C7BaseWidget::paintEvent(event);
}

