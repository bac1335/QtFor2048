#ifndef C7PALMERASEWIDGET_H
#define C7PALMERASEWIDGET_H

#include "c7doubleselectwidget.h"
#include "c7commondef.h"

class C7PalmEraseWidget : public C7DoubleSelectWidget
{
    Q_OBJECT
public:
    explicit C7PalmEraseWidget(int w, int h, QWidget *parent = nullptr);

signals:
    void sigPalmSet(PalmType type);

};

#endif // C7PALMERASEWIDGET_H
