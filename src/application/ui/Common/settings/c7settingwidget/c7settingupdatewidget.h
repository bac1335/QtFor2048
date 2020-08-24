#ifndef C7SETTINGUPDATEWIDGET_H
#define C7SETTINGUPDATEWIDGET_H

#include "c7basewidget.h"

class QLabel;
class TLPushButton;

class C7SettingUpdateWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7SettingUpdateWidget(QWidget *parent = nullptr);
    void setUpdateState(bool state);

protected:
    void initUi();
    void retranslateUi();
    void resizeEvent(QResizeEvent *event);
signals:
    void sigUpdate();
private:
    QLabel *m_pTextLabel = nullptr;
    TLPushButton *m_pUpdateBtn = nullptr;

};

#endif // C7SETTINGUPDATEWIDGET_H
