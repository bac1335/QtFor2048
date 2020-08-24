#include "tlcontrolinterface.h"

TLControlInterface::TLControlInterface()
{

}

void TLControlInterface::setMoveFlag(const TLControlInterface::MoveFlag &flag)
{
    m_moveFlag = flag;
}

TLControlInterface::MoveFlag TLControlInterface::moveFlag() const
{
    return m_moveFlag;
}

void TLControlInterface::setDragMode(const TLControlInterface::DragMode &mode)
{
    m_dragMode = mode;
}

TLControlInterface::DragMode TLControlInterface::dragMode() const
{
    return m_dragMode;
}

void TLControlInterface::setMoveLimit(TLControlInterface::MoveLimit direction)
{
    m_moveDirection = direction;
}

TLControlInterface::MoveLimit TLControlInterface::moveLimit()
{
    return m_moveDirection;
}

void TLControlInterface::setModeChangeThreshold(int threshold)
{
    m_nModeChangeThreshold = threshold;
}

int TLControlInterface::modeChangeThreshold()
{
    return m_nModeChangeThreshold;
}
