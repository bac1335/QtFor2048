#include "tlbasecontrol.h"

#include <QTimer>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>

#define DEBUG_OUTPUT

#ifdef DEBUG_OUTPUT
#include <QDebug>
#endif

TLBaseControl::TLBaseControl(QWidget *parent) : QWidget(parent)
{
    m_pDoubleClickTimer = new QTimer(this);
    m_pDoubleClickTimer->setSingleShot(true);
    m_pDoubleClickTimer->setInterval(100);
    connect(m_pDoubleClickTimer, &QTimer::timeout, [=]{
        m_bDoubleClicked = false;
    });
}

void TLBaseControl::setDoubleClickedThreshold(int threshold)
{
    m_nDoubleClickedThreshold = threshold;
}

int TLBaseControl::getDevicePixelRatio()
{
    if (m_pDestopWidget == nullptr) {
        m_pDestopWidget = QApplication::desktop();
    }

    return m_pDestopWidget->screen()->devicePixelRatio();
}

void TLBaseControl::mousePressEvent(QMouseEvent *event)
{
    if (!m_bActive) {
        return;
    }

    m_bPressed = true;
    m_pressPos = event->globalPos();
    m_lastPressPos = event->globalPos();
    m_lastMovePos = event->globalPos();
    setDragMode(Mode_Click);
    emit sigPressed();
    update();
}

void TLBaseControl::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_bActive) {
        return;
    }

    if (dragMode() == Mode_Click) {
        if (qAbs(m_pressPos.y() - event->globalPos().y()) > modeChangeThreshold()) {
            if (m_pressPos.y() > event->globalPos().y()) {
                setDragMode(Mode_DragY_ToBottom);
            } else {
                setDragMode(Mode_DragY_ToTop);
            }
        } else if (qAbs(m_pressPos.x() - event->globalPos().x()) > modeChangeThreshold()) {
            if (m_pressPos.x() > event->globalPos().x()) {
                setDragMode(Mode_DragX_ToLeft);
            } else {
                setDragMode(Mode_DragX_ToRight);
            }
        }
    } else if (dragMode() >= Mode_DragX_ToLeft) {
        triggerMoveEvent(event->globalPos());
    }

    update();
}

void TLBaseControl::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_bActive) {
        return;
    }

    switch (dragMode()) {
    case Mode_DragX_ToLeft:
    case Mode_DragX_ToRight:
    case Mode_DragY_ToTop:
    case Mode_DragY_ToBottom:
        triggerMoveEvent(event->globalPos());
        break;
    case Mode_Click:
        emit sigClicked();   //直接发送单击信号
        if (m_bDoubleClicked
            && (qAbs(m_pressPos.x() - m_lastPressPos.x()) < m_nDoubleClickedThreshold
            && (qAbs(m_pressPos.y() - m_lastPressPos.y()) < m_nDoubleClickedThreshold)))
        {
            m_pDoubleClickTimer->stop();
            emit sigDoublePosClicked();
            m_bDoubleClicked = false;
        } else {
            m_bDoubleClicked = true;
            if (!m_pDoubleClickTimer->isActive()) {
                m_pDoubleClickTimer->start();
            }
        }

        break;
    default:
        break;
    }
    setDragMode(Mode_NoDrag);
    update();
    m_lastPressPos = m_pressPos;
    m_pressPos = QPoint();
    if(m_bPressed){
        m_bPressed = false;
        emit sigRelease();
    }
}

void TLBaseControl::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit sigDoubleClicked();
}

void TLBaseControl::enterEvent(QEvent *event)
{
    m_bHover = true;
    update();
    QWidget::enterEvent(event);
}

void TLBaseControl::leaveEvent(QEvent *event)
{
    m_bHover = false;
    update();
    QWidget::leaveEvent(event);
}

void TLBaseControl::triggerMoveEvent(const QPoint &currentPos)
{
    QPoint p;

    if (moveLimit() & MoveLimit_X) {
        p.setX(currentPos.x() - m_lastMovePos.x());
    }

    if (moveLimit() & MoveLimit_Y) {
        p.setY(currentPos.y() - m_lastMovePos.y());
    }

    if (moveFlag() == Flag_MoveSelf) {
        this->move(this->x() + p.x(), this->y() + p.y());
    } else if (Flag_MoveByOutSide) {
        emit sigMoveByAsk(p);
    };

    m_lastMovePos = currentPos;
}
