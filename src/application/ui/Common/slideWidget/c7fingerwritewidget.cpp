#include "c7fingerwritewidget.h"
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>

C7FingerWriteWidget::C7FingerWriteWidget(int w, int h, QWidget *parent):
    C7DoubleSelectWidget(w, h, parent)
{
    this->setBgText(tr("Write"), tr("Eraser"));
    connect(this, &C7FingerWriteWidget::sigChecked, this, [=](bool checked){
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

void C7FingerWriteWidget::retranslateUi()
{
    this->setBgText(tr("Write"), tr("Eraser"));
}


