#include "c7exploreritem.h"
#include <QPainter>
#include <QMouseEvent>
#include "tlutility.h"
#include <QDebug>

C7ExplorerItem::C7ExplorerItem(QSize size, QWidget *parent)
    : C7BaseWidget(size.width(), size.height(), parent)
{
    m_nTextHeight *= m_rScaleFactor;
    m_nNormalTextHeight = m_nNormalTextHeight;
}

void C7ExplorerItem::setText(const QString &strText)
{
    //因為這裡的size還沒有乘以係數，所以這裡的寬要乘
    m_displayText = Uti->getElideText(strText, Uti->getFont(14*m_rScaleFactor),
                                      this->width() * m_rScaleFactor,Qt::ElideRight);
    update();
}

void C7ExplorerItem::setImage(const QPixmap &image)
{
    m_image = image;
    update();
}

void C7ExplorerItem::setCheckedImage(const QPixmap &image)
{
    m_checkImage = image.scaled(26*m_rScaleFactor,26*m_rScaleFactor,
                                Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    update();
}

void C7ExplorerItem::setChecked(bool bChecked)
{
    m_bChecked = bChecked;
    update();
}

void C7ExplorerItem::setCheckable(bool bCheckable)
{
    m_bCheckable = bCheckable;
}

void C7ExplorerItem::setItemID(const QString &strId)
{
    m_itemId = strId;
}

void C7ExplorerItem::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    QRect imgRect = this->rect();
    imgRect.setHeight(this->height() - m_nTextHeight);
    painter.drawPixmap(imgRect, m_image);
    if (m_bCheckable && m_bChecked)
    {
       painter.drawPixmap(QRect(this->rect().width()-m_checkImage.width()-10*m_rScaleFactor,this->rect().height()-m_checkImage.height()-m_nTextHeight-10*m_rScaleFactor,
                                 m_checkImage.width(),m_checkImage.height()), m_checkImage);
    }

    painter.setPen(Qt::black);
    painter.setFont(Uti->getFont(14*m_rScaleFactor));
    QRect txtRect = this->rect();
    txtRect.setY(this->height() - m_nTextHeight);
    txtRect.setHeight(m_nTextHeight);
    painter.drawText(txtRect, Qt::AlignCenter, m_displayText);
}

void C7ExplorerItem::mouseMoveEvent(QMouseEvent *e)
{
    //点击item容易触发mouseMove，进行处理
    if((qAbs(e->x() - m_pressPoint.x()) >5) || (qAbs(e->y() - m_pressPoint.y()) >5))
    {
       m_bMove = true;
    }
    QWidget::mouseMoveEvent(e);
}

void C7ExplorerItem::mousePressEvent(QMouseEvent *e)
{
    m_bIsClick = true;
    m_pressPoint = e->pos();
    m_bMove = false;
    QWidget::mousePressEvent(e);
}

void C7ExplorerItem::mouseReleaseEvent(QMouseEvent *e)
{
    if(!m_bMove && m_bIsClick)
    {
        emit sigItemClick(m_itemId);
    }
    else //有滑动
    {
        QWidget::mouseReleaseEvent(e);
    }
    m_bIsClick = false;
}

void C7ExplorerItem::mouseDoubleClickEvent(QMouseEvent *event)
{
     emit sigItemDoubleClick(m_itemId);
}

void C7ExplorerItem::onScaleFactorChange()
{
    m_nTextHeight = m_nNormalTextHeight * m_rScaleFactor;

    C7BaseWidget::onScaleFactorChange();
}
