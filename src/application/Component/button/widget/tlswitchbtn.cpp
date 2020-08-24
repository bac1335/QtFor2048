#include "tlswitchbtn.h"

#include <QDebug>
#include <QLabel>
#include <QPropertyAnimation>

TLSwitchBtn::TLSwitchBtn(QWidget *parent)
    : TLCheckBtn(parent)
{
    m_pSwitchLabel = new QLabel(this);
    m_pSwitchLabel->setScaledContents(true);

    m_pAnimation = new QPropertyAnimation(this);
    m_pAnimation->setTargetObject(m_pSwitchLabel);
    m_pAnimation->setPropertyName("pos");
    m_pAnimation->setDuration(100);
    m_pAnimation->setStartValue(QPoint(2, 1));
    setEndValue();

    connect(this, &TLSwitchBtn::sigChecked, this, &TLSwitchBtn::onChecked);
}

void TLSwitchBtn::setFgNormalImage(const QPixmap &pixmap)
{
    m_fgNormalImage = pixmap;
    m_pSwitchLabel->setPixmap(m_fgNormalImage);
    update();
}

void TLSwitchBtn::setFgChcekedImage(const QPixmap &pixmap)
{
    m_fgCheckedImage = pixmap;
}

void TLSwitchBtn::setFgNormalColor(const QColor &color)
{
    m_fgNormalColor = color;
    m_fgNormalColorString = QString("%1,%2,%3")
                            .arg(m_fgNormalColor.red())
                            .arg(m_fgNormalColor.green())
                            .arg(m_fgNormalColor.blue());
    update();
}

void TLSwitchBtn::setFgCheckedColor(const QColor &color)
{
    m_fgCheckedColor = color;
    m_fgCheckedColorString = QString("%1,%2,%3")
                            .arg(m_fgCheckedColor.red())
                            .arg(m_fgCheckedColor.green())
            .arg(m_fgCheckedColor.blue());
}

void TLSwitchBtn::setChecked(bool bChecked)
{
    m_bChecked = bChecked;
    onChecked(bChecked);
    update();
}

void TLSwitchBtn::resizeEvent(QResizeEvent *event)
{
    m_pSwitchLabel->resize(this->height()-2, this->height()-2);

    setEndValue();
    if (m_bChecked)
    {
        m_pSwitchLabel->move(QPoint(this->width() - m_pSwitchLabel->width()-2, 1));
    }
    else
    {
        m_pSwitchLabel->move(2, 1);
    }

    QWidget::resizeEvent(event);
}

void TLSwitchBtn::onChecked(bool bChecked)
{
    if (!bChecked) {
        if (!m_fgNormalImage.isNull())
        {
            m_pSwitchLabel->setPixmap(m_fgNormalImage);
        }
        else
        {
            m_pSwitchLabel->setStyleSheet(QString("QLabel{background:rgb(%1);border-radius:%2px}")
                                          .arg(m_fgNormalColorString).arg(m_pSwitchLabel->width() / 2));
        }
        m_pAnimation->setDirection(QAbstractAnimation::Backward);
    } else {

        if (!m_fgCheckedImage.isNull())
        {
            m_pSwitchLabel->setPixmap(m_fgCheckedImage);
        }
        else
        {
            m_pSwitchLabel->setStyleSheet(QString("QLabel{background:rgb(%1);border-radius:%2px}")
                                          .arg(m_fgCheckedColorString).arg(m_pSwitchLabel->width() / 2));
        }
        m_pAnimation->setDirection(QAbstractAnimation::Forward);
    }

    m_pAnimation->start();
}

void TLSwitchBtn::setEndValue()
{
    m_pAnimation->setEndValue(QPoint(this->width() - m_pSwitchLabel->width() -2, 1));
}
