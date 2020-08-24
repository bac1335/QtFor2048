#include "tlpushbutton.h"
#include <QPainter>
#include <QDebug>

TLPushButton::TLPushButton(QWidget *parent) : TLBaseControl(parent)
{
    connect(this,&TLPushButton::sigClicked,
            this,&TLPushButton::onButtonClicked);
}

void TLPushButton::setNormalToHover()
{
    m_hoverText = m_normalText;
    m_hoverImage = m_normalImage;
    m_hoverBackColor = m_normalBackColor;
    m_hoverTextColor = m_normalTextColor;
}

QString TLPushButton::text()
{
    return m_normalText;
}

void TLPushButton::setChecked(bool b)
{
    m_bChecked = b;
    update();
}

void TLPushButton::onButtonClicked()
{
    m_bChecked = !m_bChecked;
    emit sigChecked(isChecked());
    update();
}

void TLPushButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int ratio = getDevicePixelRatio();
    QString currentText;
    QPixmap currentImage;
    QColor currentTextColor;
    QColor currentBackColor;

    if (!this->isEnabled()) {
        currentText = m_disableText;
        currentImage = m_disableImage;
        currentTextColor = m_disableTextColor;
        currentBackColor = m_disableBackColor;
    } else if (m_bPressed) {
        currentText = m_pressText;
        currentImage = m_pressImage;
        currentTextColor = m_pressTextColor;
        currentBackColor = m_pressBackColor;
    } else if (m_bHover) {
        currentText = m_hoverText;
        currentImage = m_hoverImage;
        currentTextColor = m_hoverTextColor;
        currentBackColor = m_hoverBackColor;
    } else {
        currentText = m_normalText;
        currentImage = m_normalImage;
        currentTextColor = m_normalTextColor;
        currentBackColor = m_normalBackColor;
    }

    painter.setPen(currentTextColor);
    painter.setFont(m_font);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    switch (m_btnLayout) {
    case Layout_Tiled:
        if (!currentImage.isNull()) {
            painter.drawPixmap(this->rect(), currentImage,currentImage.rect());
        }
        else{
            if(!m_borderline){
                 painter.setPen(Qt::NoPen);
            }
            else {
                painter.setPen(borderLineColor);
            }
            painter.setBrush(currentBackColor);
            painter.drawRoundedRect(QRectF(2.5f,2.5f,this->rect().width()-2.5f,this->rect().height()-2.5f),
                                    m_radius, m_radius);

        }

        if (!currentText.isEmpty()) {
            painter.setPen(currentTextColor);
            painter.setFont(m_font);
            painter.drawText(this->rect(),Qt::AlignCenter, currentText);

        }

        break;
    case Layout_Tiled2:
    {
        if (!currentImage.isNull()) {
            painter.drawPixmap(this->rect(), currentImage,currentImage.rect());
        }
        else{
            if(!m_borderline){
                painter.setPen(Qt::NoPen);
            }
            else {
                painter.setPen(QColor(0,0,0,125));
            }
            painter.setBrush(currentBackColor);
            painter.drawRoundedRect(QRectF(2.5f,2.5f,this->rect().width()-2.5f,this->rect().height()-2.5f),
                                    m_radius, m_radius);

        }

        if (!currentText.isEmpty()) {
            painter.setPen(currentTextColor);
            painter.setFont(m_font);
            QRect rect = this->rect();

            rect.setHeight(this->height() - m_nTextIndent);
            painter.drawText(rect,Qt::AlignBottom | Qt::AlignHCenter,currentText);

        }

        break;
    }
    case Layout_LeftToRight:
        if (!currentImage.isNull())
        {
            currentImage = currentImage.scaled(this->height() * ratio, this->height() * ratio,
                                               Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            painter.drawPixmap(QRectF(0, 0, this->height(), this->height()), currentImage, currentImage.rect());
        }
        else{
            painter.setBrush(currentBackColor);
            painter.drawRoundedRect(this->rect(), m_radius, m_radius);
        }

        if (!currentText.isEmpty()) {
            QRect rect;
            /*if (!currentImage.isNull()) {
                rect = QRect(this->height() * 1.6, 0, this->width() - this->height() * 1.2)
            } else */{
                rect = QRect(0, 0, this->width(), this->height());
            }
            painter.setPen(currentTextColor);
            painter.drawText(rect, Qt::AlignVCenter | Qt::AlignLeft, currentText);
        }
        break;
    case Layout_RightToLeft:
        if (!currentImage.isNull()) {

        }

        if (!currentText.isEmpty()) {

        }
        break;
    case Layout_UpToDown:
        if (!currentImage.isNull()) {
            painter.drawPixmap(this->rect(), currentImage,currentImage.rect());
        }
        else{
            painter.setBrush(currentBackColor);
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(this->rect(), m_radius, m_radius);
        }

        if (!currentText.isEmpty()) {
            painter.setPen(Qt::white);
            painter.drawText(this->rect()-QMargins(0,0,0,10),
                             Qt::AlignHCenter | Qt::AlignBottom,currentText);
        }
        break;
    case Layout_DownToUp:
        if (!currentImage.isNull()) {

        }

        if (!currentText.isEmpty()) {

        }
        break;
    default:
        break;
    }
    QWidget::paintEvent(event);
}
