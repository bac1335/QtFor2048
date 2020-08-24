#include "c7fingerwritwidget.h"
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>

C7FingerWritWidget::C7FingerWritWidget(int w, int h, QWidget *parent):
    C7DoubleSelectWidget(w, h, parent)
{
    this->setBgText(tr("FinWrite"), tr("FinErase"));
    connect(this, &C7FingerWritWidget::sigChecked, this, [=](bool checked){
        FingerType type;
        if(checked)
        {
            type = Type_Finger_Erase;
        }
        else
        {
            type = Type_Finger_Write;
        }
        emit sigFingerSet(type);
    });
}


