#include "tlgraphicsbasecontrol.h"

#include <QGraphicsSceneMouseEvent>

TLGraphicsBaseControl::TLGraphicsBaseControl(QGraphicsObject *parent)
    : QGraphicsWidget(parent)
{

}

TLGraphicsBaseControl::~TLGraphicsBaseControl()
{

}

void TLGraphicsBaseControl::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_pressPos = event->scenePos();
    m_lastMovePos = event->scenePos();
    setDragMode(Mode_Click);
    emit sigPressed();
}

void TLGraphicsBaseControl::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(dragMode() == Mode_Click || dragMode() == Mode_NoDrag )
    {
        if(qAbs(m_pressPos.y() - event->scenePos().y()) > modeChangeThreshold() ){
            if(m_pressPos.y() > event->scenePos().y()){
                setDragMode(Mode_DragY_ToBottom);
            }else{
                setDragMode(Mode_DragY_ToTop);
            }
        }else if(qAbs(m_pressPos.x() - event->scenePos().x()) > modeChangeThreshold()){
            if(m_pressPos.x() > event->scenePos().x()){
                setDragMode(Mode_DragX_ToLeft);
            }else{
                setDragMode(Mode_DragX_ToRight);
            }
        }
    }else if(dragMode() >= Mode_DragX_ToLeft){
        triggerMoveEvent(event->scenePos());
    }
}

void TLGraphicsBaseControl::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    switch (dragMode()) {
    case Mode_DragY_ToBottom:
    case Mode_DragY_ToTop:
    case Mode_DragX_ToLeft:
    case Mode_DragX_ToRight:{
        triggerMoveEvent(event->scenePos());
    }
        break;
    case Mode_Click:
        emit sigClicked();
        break;
    default:
        break;
    }
    emit sigRelease();
    setDragMode(Mode_NoDrag);
    update();
}

void TLGraphicsBaseControl::hoverEnterEvent(QEvent *event)
{
    m_bHover = true;
    update();
}

void TLGraphicsBaseControl::hoverLeaveEvent(QEvent *event)
{
    m_bHover = false;
    update();
}

void TLGraphicsBaseControl::triggerMoveEvent(const QPointF &currentPos)
{
    QPoint p;
    if(moveLimit() & MoveLimit_X){
        p.setX((currentPos - m_lastMovePos).x());
    }
    if(moveLimit() & MoveLimit_Y){
        p.setY((currentPos - m_lastMovePos).y());
    }
    m_lastMovePos = currentPos;
    if(moveFlag()  == Flag_MoveSelf){
        this->setPos(this->x() + p.x(), this->y() + p.y());
    }else if(moveFlag() == Flag_MoveByOutSide){
        emit sigMoveByAsk(p);
    }
}

void TLGraphicsBaseControl::dragModeChange(int mode)
{
    emit sigDragModeChange(mode);
}
