#ifndef TLCHECKBTN_H
#define TLCHECKBTN_H

#include "tlbasecontrol.h"
#include "tlbuttoninterface.h"

class TLCheckBtn : public TLBaseControl, public TLButtonInterface
{
    Q_OBJECT
public:
    TLCheckBtn(QWidget* parent = nullptr);

    enum CheckBtn_Type {
        Type_CheckStyle,    //点击选中，再次点击取消选中
        Type_RadioStyle     //点击选中，再次点击无响应，只能通过其他逻辑设置取消选中
    };
    void setCheckBtnType(CheckBtn_Type type);
    void setCheckedAble(bool f){m_ableCheck = f;}
    virtual void setChecked(bool bChecked);
    bool isChecked();
    //如果设置的是radio模式，当前已经选中了按钮，再次点击是是否要发送sigChecked信号，这是为了解决有些特殊情境
    void setAlreadyCheckedRepeatEmitSignal(bool flag){
        m_bAlreadyCheckedFlag = flag;
    }
signals:
    void sigChecked(bool bChecked);
protected:
    bool m_bChecked = false;
    bool m_ableCheck = true;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    CheckBtn_Type m_checkBtnType = Type_CheckStyle;
    QTimer* m_pDoubleClickProtectTimer = nullptr;
    bool m_bAlreadyCheckedFlag = true;
};

#endif // TLCHECKBTN_H
