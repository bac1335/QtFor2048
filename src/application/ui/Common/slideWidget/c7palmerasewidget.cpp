#include "c7palmerasewidget.h"

C7PalmEraseWidget::C7PalmEraseWidget(int w, int h, QWidget *parent):
    C7DoubleSelectWidget(w, h, parent)
{
    this->setBgText(tr("NoPalm"), tr("PalmErase"));

    connect(this, &C7PalmEraseWidget::sigChecked, this, [=](bool checked){
        PalmType type;
        if(checked)
        {
            type = Type_PalmErase;
        }
        else
        {
            type = Type_NoPalm;
        }
        emit sigPalmSet(type);
    });
}
