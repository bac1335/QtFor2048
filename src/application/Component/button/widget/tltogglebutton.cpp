#include "tltogglebutton.h"
#include <QPainter>

TLToggleButton::TLToggleButton(QWidget* parent): TLBaseControl(parent)
{
    connect(this,&TLToggleButton::sigClicked,
            this,&TLToggleButton::onButtonClicked);
}

void TLToggleButton::setText(const QString &text)
{
    TLButtonInterface::setText(text);
    update();
}

void TLToggleButton::onButtonClicked()
{
    m_bChecked = !m_bChecked;
    emit sigChecked(isChecked());
    update();
}

void TLToggleButton::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::NoPen);

    int ratio = getDevicePixelRatio();
    QPixmap currentImage;
    QString currentText;
    QColor currentTextColor;
    QColor currentBackCOlor;
    if (m_bChecked) {
        currentImage = m_pressImage;
        currentText = m_pressText;
        currentTextColor = m_pressTextColor;
        currentBackCOlor = m_pressBackColor;
    } else {
        currentImage = m_normalImage;
        currentText = m_normalText;
        currentTextColor = m_normalTextColor;
        currentBackCOlor = m_normalBackColor;
    }

    if (!currentText.isEmpty())
    {
        painter.setFont(m_font);
        painter.setPen(currentTextColor);
    }

    if (!currentImage.isNull())
    {
        currentImage.setDevicePixelRatio(ratio);
    }

    painter.drawPixmap(this->rect(),currentImage,currentImage.rect());
    if(!currentText.isEmpty()){
        QRect rect = this->rect();

        rect.setHeight(this->height() - m_nTextIndent);
        painter.drawText(rect,Qt::AlignBottom | Qt::AlignHCenter,currentText);

    }

    QWidget::paintEvent(e);
}
