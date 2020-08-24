#include "tlthressstatebutton.h"

#include <QPainter>

TLThressStateButton::TLThressStateButton(QWidget *parent) : TLBaseControl(parent)
{
    connect(this, &TLThressStateButton::sigClicked, this, &TLThressStateButton::onClick);
}

void TLThressStateButton::setSelectText(const QString &text)
{
    m_selectText = text;
}

void TLThressStateButton::setSelectImage(const QString &imagePath)
{
    setSelectImage(QPixmap(imagePath));
}

void TLThressStateButton::setSelectImage(const QPixmap &image)
{
    m_selectImage = image;
}

void TLThressStateButton::setSelectBackColor(const QColor &color)
{
    m_selectBackColor = color;
}

void TLThressStateButton::setSelectTextColor(const QColor &color)
{
    m_selectTextColor = color;
}

void TLThressStateButton::setCurrentState(TLThressStateButton::StateType state)
{
    m_currentState = state;
    emit sigStateChange(m_currentState);
    update();
}

void TLThressStateButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
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
    } else if (m_currentState == Select_State) {
        currentText = m_selectText;
        currentImage = m_selectImage;
        currentTextColor = m_selectTextColor;
        currentBackColor = m_selectBackColor;
    } else if (m_currentState == Press_State) {
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
            painter.setBrush(currentBackColor);
            painter.setPen(Qt::NoPen);
            painter.drawRect(this->rect());
        }

        if (!currentText.isEmpty()) {
            painter.drawText(this->rect(),Qt::AlignCenter, currentText);
        }

        break;
    case Layout_LeftToRight:
        if (!currentImage.isNull())
        {
            currentImage = currentImage.scaled(this->height() * ratio, this->height() * ratio,
                                               Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            painter.drawPixmap(QRectF(0, 0, this->height(), this->height()), currentImage, currentImage.rect());
        }
        else{
            painter.setBrush(currentBackColor);
            painter.setPen(Qt::NoPen);
            painter.drawRect(this->rect());
        }

        if (!currentText.isEmpty()) {
            QRect rect;
            /*if (!currentImage.isNull()) {
                rect = QRect(this->height() * 1.6, 0, this->width() - this->height() * 1.2)
            } else */{
                rect = QRect(0, 0, this->width(), this->height());
            }

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
            painter.drawRect(this->rect());
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
}

void TLThressStateButton::onClick()
{
    if (m_currentState == Normal_State)
    {
        this->setCurrentState(Select_State);
    } else if (m_currentState == Select_State)
    {
        this->setCurrentState(Press_State);
    } else if (m_currentState == Press_State)
    {
        this->setCurrentState(Select_State);
    }
}
