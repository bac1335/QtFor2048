#include "tlcheckbtn.h"

#include <QTimer>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

#define DOUBLE_CLICKED_PROTECT_INTERVAL 300  //双击保护间隔300ms

TLCheckBtn::TLCheckBtn(QWidget* parent) : TLBaseControl(parent)
{
    m_pDoubleClickProtectTimer = new QTimer(this);
    m_pDoubleClickProtectTimer->setSingleShot(true);
    m_pDoubleClickProtectTimer->setInterval(DOUBLE_CLICKED_PROTECT_INTERVAL);
}

void TLCheckBtn::setCheckBtnType(TLCheckBtn::CheckBtn_Type type)
{
    m_checkBtnType = type;
}

void TLCheckBtn::setChecked(bool bChecked)
{
    m_bChecked = bChecked;
    update();
}

bool TLCheckBtn::isChecked()
{
    return m_bChecked;
}

void TLCheckBtn::mousePressEvent(QMouseEvent *event)
{
    if(m_ableCheck)
        m_bPressed = true;

    QWidget::mousePressEvent(event);
}

void TLCheckBtn::mouseMoveEvent(QMouseEvent *event)
{
    //超出控件范围认为是弹起了
    if (!this->rect().contains(event->pos())) {
        m_bPressed = false;
    }
}

void TLCheckBtn::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_pDoubleClickProtectTimer->isActive()) {
        if (m_bPressed)
        {
            if(m_bAlreadyCheckedFlag){
                if (m_checkBtnType == Type_RadioStyle) {
                    if (!m_bChecked) {
                        m_bChecked = true;
                    }
                } else if (m_checkBtnType == Type_CheckStyle) {
                    m_bChecked = !m_bChecked;
                }
                emit sigChecked(m_bChecked);
            }
            else{
                if (m_checkBtnType == Type_RadioStyle) {
                    if (!m_bChecked) {
                        m_bChecked = true;
                        emit sigChecked(m_bChecked);
                    }
                } else if (m_checkBtnType == Type_CheckStyle) {
                    m_bChecked = !m_bChecked;
                    emit sigChecked(m_bChecked);
                }
            }

            update();
        }
        m_pDoubleClickProtectTimer->start();
    }

    QWidget::mouseReleaseEvent(event);
}

void TLCheckBtn::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

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

    switch (m_btnLayout) {
    case Layout_Tiled:
        if (!currentImage.isNull()) {
            painter.drawPixmap(this->rect(), currentImage,currentImage.rect());
        }

        if (!currentText.isEmpty()) {
            painter.drawText(this->rect(),Qt::AlignCenter, currentText);
        }

        break;
    case Layout_LeftToRight:
        if (!currentImage.isNull())
        {
            painter.drawPixmap(QRectF(0, 0, this->height(), this->height()), currentImage, currentImage.rect());
        }
        if (!currentText.isEmpty()) {
            QRect rect;
            if (!currentImage.isNull()) {
                rect = QRect(this->height() * 1.4, 0, this->width() - this->height() * 1.2, this->height());
            } else {
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

        }

        if (!currentText.isEmpty()) {

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
