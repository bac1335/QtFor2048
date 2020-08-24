#include "tllabelbutton.h"
#include <QMouseEvent>

TLLabelButton::TLLabelButton(QWidget *parent):
    QLabel(parent)
{

}

TLLabelButton::TLLabelButton(const QString &text, QWidget *parent):
    QLabel(text,parent)
{

}

void TLLabelButton::setColor(QColor color)
{
    this->setStyleSheet(QString("color:%1;").arg(color.name()));
}

void TLLabelButton::mousePressEvent(QMouseEvent *e)
{
    m_bPressed = true;
    m_pressPoint = e->pos();
    m_bMoved = false;
    QLabel::mousePressEvent(e);
}

void TLLabelButton::mouseMoveEvent(QMouseEvent *e)
{
    if((qAbs(e->x() - m_pressPoint.x()) >5) || (qAbs(e->y() - m_pressPoint.y()) >5))
    {
       m_bMoved = true;
    }
    QLabel::mouseMoveEvent(e);
}

void TLLabelButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(!m_bMoved && m_bPressed)
    {
        m_bChecked = !m_bChecked;
        emit sigClicked(m_bChecked);
    }
    QLabel::mouseReleaseEvent(e);
}
