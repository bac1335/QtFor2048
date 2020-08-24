#include "tlgraphicspushbutton.h"

#include <QPainter>

TLGraphicsPushButton::TLGraphicsPushButton(QGraphicsObject *parent)
    : TLGraphicsBaseControl(parent)
{

}

void TLGraphicsPushButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    drawButtonStuff(painter, dragMode(), this->rect().toRect(), getDevicePixelRatio(), m_bHover);
//    if(!m_Image.isNull()){
//        drawFixImage(painter, this->rect().toRect(), m_Image, devicePixelRatio());
//    }
//    if(!m_strText.isEmpty()){
//        painter->setPen(pen());
//        painter->setFont(this->font());
//        painter->drawText(rect(), m_nAlign, m_strText);
//    }
}
