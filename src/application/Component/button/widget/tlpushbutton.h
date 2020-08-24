#ifndef TLPUSHBUTTON_H
#define TLPUSHBUTTON_H

#include "tlbasecontrol.h"
#include "tlbuttoninterface.h"

class TLPushButton : public TLBaseControl, public TLButtonInterface
{
    Q_OBJECT
public:
    TLPushButton(QWidget* parent = nullptr);

    //使normal和hover的参数一样
    //因为hover总是触发，设置一样，方便一些
    //后续根据需要看要不要添加到TLButtonInterface中
    void setNormalToHover();

    QString text();

    void setChecked(bool);

    bool isChecked() const{
        return m_bChecked;
    }



public slots:
    virtual void onButtonClicked();
protected:
    void paintEvent(QPaintEvent *event);
signals:
    void sigChecked(bool checked);
private:
    bool m_bChecked = false;
};

#endif // TLPUSHBUTTON_H
