#include "c7wechatuseritem.h"
#include <QPainter>
#include <QMouseEvent>

C7WechatUserItem::C7WechatUserItem(int w, int h, QWidget *parent):
    C7BaseWidget(w, h, parent), m_bIsChecked(false), m_bIsReaded(false)
{
    m_bgNormal = QPixmap(m_strAppPath + "/skin/import/icon_wechat_normal.png");
    m_bgPress = QPixmap(m_strAppPath + "/skin/import/icon_wechat_press.png");
}

void C7WechatUserItem::setAvatarImage(const QPixmap &pixmap)
{
    m_bg = pixmap;
    m_bg.scaled(68*m_rScaleFactor, 68*m_rScaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void C7WechatUserItem::setChecked(bool checked)
{
    m_bIsChecked = checked;
    update();
}

void C7WechatUserItem::setStatusTip(bool status)
{
    m_bIsReaded = status;
    update();
}

bool C7WechatUserItem::isReaded() const
{
    return m_bIsReaded;
}

void C7WechatUserItem::initUi()
{

}

void C7WechatUserItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPixmap(QRect(this->width()/2 - 34*m_rScaleFactor,
                       this->height()/2 - 34*m_rScaleFactor,
                       68*m_rScaleFactor, 68*m_rScaleFactor),
                       m_bg, m_bg.rect());

    if(m_bIsChecked)
    {        
//        painter.setBrush(QBrush(QColor(242, 245, 245)));
        painter.drawPixmap(this->rect(),m_bgPress,m_bgPress.rect());
    }
    else
    {
        painter.drawPixmap(this->rect(),m_bgNormal,m_bgNormal.rect());
//        painter.setBrush(QBrush(QColor(221, 224, 226)));
    }
    painter.drawRect(this->rect());
    if(m_bIsChecked)
    {
        painter.setBrush(QBrush(QColor("#32bc84")));
        painter.drawRect(0, 0, 4*m_rScaleFactor, this->height());
    }

    if(m_bIsReaded)
    {
        painter.setBrush(QBrush(Qt::red));
        painter.drawEllipse(85*m_rScaleFactor, 10*m_rScaleFactor, 15*m_rScaleFactor, 15*m_rScaleFactor);
    }
    C7BaseWidget::paintEvent(event);
}

void C7WechatUserItem::mousePressEvent(QMouseEvent *event)
{
    m_bPress = true;
    m_pressPoint = event->pos();
    m_bMove = false;
    C7BaseWidget::mousePressEvent(event);
}

void C7WechatUserItem::mouseMoveEvent(QMouseEvent *event)
{
    //点击item容易触发mouseMove，进行处理
    if((qAbs(event->x() - m_pressPoint.x()) >5) || (qAbs(event->y() - m_pressPoint.y()) >5))
    {
       m_bMove = true;
    }
    C7BaseWidget::mouseMoveEvent(event);
}

void C7WechatUserItem::mouseReleaseEvent(QMouseEvent *event)
{
    if(!m_bMove && m_bPress)
    {
        if(!m_bIsChecked)
        {
            m_bIsChecked = true;
        }
        if(m_bIsReaded)
        {
            m_bIsReaded = false;
        }
        emit sigItemClick();
        update();
    }
    else //有滑动
    {
        C7BaseWidget::mouseReleaseEvent(event);
    }
    m_bPress = false;
}
