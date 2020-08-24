#ifndef C7FINGERWRITWIDGET_H
#define C7FINGERWRITWIDGET_H

#include "c7doubleselectwidget.h"
#include "c7commondef.h"

class C7FingerWritWidget : public C7DoubleSelectWidget
{
    Q_OBJECT
public:
    explicit C7FingerWritWidget(int w, int h, QWidget *parent = nullptr);


signals:
    void sigFingerSet(FingerType type);


};

#endif // C7FINGERWRITWIDGET_H
