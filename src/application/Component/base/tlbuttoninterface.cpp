#include "tlbuttoninterface.h"

TLButtonInterface::TLButtonInterface()
{
    initParams();
}

void TLButtonInterface::setButtonLayout(TLButtonInterface::ButtonLayout layout)
{
    m_btnLayout = layout;
}

void TLButtonInterface::setText(const QString &text)
{
    setNormalText(text);
    setHoverText(text);
    setPressText(text);
    setDisableText(text);
}

void TLButtonInterface::setImage(const QString &imagePath)
{
    QPixmap image(imagePath);
    setNormalImage(image);
    setHoverImage(image);
    setPressImage(image);
    setDisableImage(image);
}

void TLButtonInterface::setImage(const QPixmap &image)
{
    setNormalImage(image);
    setHoverImage(image);
    setPressImage(image);
}

void TLButtonInterface::setBackColor(const QColor &color)
{
    setNormalBackColor(color);
    setPressBackColor(color);
    setHoverBackColor(color);
}

void TLButtonInterface::setTextColor(const QColor &color)
{
    setNormalTextColor(color);
    setHoverTextColor(color);
    setPressTextColor(color);
}

void TLButtonInterface::setTextFont(const QFont &font)
{
    m_font = font;
}

void TLButtonInterface::setNormalText(const QString &text)
{
    m_normalText = text;
}

void TLButtonInterface::setNormalImage(const QString &imagePath)
{
    setNormalImage(QPixmap(imagePath));
}

void TLButtonInterface::setNormalImage(const QPixmap &image)
{
    m_normalImage = image;
}

void TLButtonInterface::setNormalBackColor(const QColor &color)
{
    m_normalBackColor = color;
}

void TLButtonInterface::setNormalTextColor(const QColor &color)
{
    m_normalTextColor = color;
}


void TLButtonInterface::setPressText(const QString &text)
{
    m_pressText = text;
}

void TLButtonInterface::setPressImage(const QString &imagePath)
{
    setPressImage(QPixmap(imagePath));
}

void TLButtonInterface::setPressImage(const QPixmap &image)
{
    m_pressImage = image;
}

void TLButtonInterface::setPressBackColor(const QColor &color)
{
    m_pressBackColor = color;
}

void TLButtonInterface::setPressTextColor(const QColor &color)
{
    m_pressTextColor = color;
}

void TLButtonInterface::setHoverText(const QString &text)
{
    m_hoverText = text;
}

void TLButtonInterface::setHoverImage(const QString &imagePath)
{
    setHoverImage(QPixmap(imagePath));
}

void TLButtonInterface::setHoverImage(const QPixmap &image)
{
    m_hoverImage = image;
}

void TLButtonInterface::setHoverBackColor(const QColor &color)
{
    m_hoverBackColor = color;
}

void TLButtonInterface::setHoverTextColor(const QColor &color)
{
    m_hoverTextColor = color;
}

void TLButtonInterface::setDisableText(const QString &text)
{
    m_disableText = text;
}

void TLButtonInterface::setDisableImage(const QString &imagePath)
{
    setDisableImage(QPixmap(imagePath));
}

void TLButtonInterface::setDisableImage(const QPixmap &image)
{
    m_disableImage = image;
}

void TLButtonInterface::setDisableBackColor(const QColor &color)
{
    m_disableBackColor = color;
}

void TLButtonInterface::setDisableTextColor(const QColor &color)
{
    m_disableTextColor = color;
}

void TLButtonInterface::setTextIndent(int indent)
{
    m_nTextIndent = indent;
}

void TLButtonInterface::setTextHeight(int height)
{
    m_nTextHeight = height;
}

void TLButtonInterface::initParams()
{
    m_normalText = QString("");
    m_normalImage.fill(Qt::white);
    m_normalBackColor = QColor(Qt::white);
    m_normalTextColor = QColor(Qt::black);

    m_pressText = QString("");
    m_pressImage.fill(Qt::white);
    m_pressBackColor = QColor(Qt::white);
    m_pressTextColor = QColor(Qt::black);

    m_disableText = QString("");
    m_disableImage.fill(Qt::white);
    m_disableBackColor = QColor(Qt::white);
    m_disableTextColor = QColor(Qt::black);
}
