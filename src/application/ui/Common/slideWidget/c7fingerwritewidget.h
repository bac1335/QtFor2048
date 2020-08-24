#ifndef C7FINGERWRITWIDGET_H
#define C7FINGERWRITWIDGET_H

#include "c7doubleselectwidget.h"
#include "c7commondef.h"

class C7FingerWriteWidget : public C7DoubleSelectWidget
{
    Q_OBJECT
public:
    explicit C7FingerWriteWidget(int w, int h, QWidget *parent = nullptr);

protected:
    void retranslateUi();
signals:
    void sigFingerSet(FingerType type);


};

#endif // C7FINGERWRITWIDGET_H
