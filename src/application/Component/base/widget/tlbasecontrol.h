#ifndef TLBASECONTROL_H
#define TLBASECONTROL_H

#include <QWidget>
#include "../tlcontrolinterface.h"

class QDesktopWidget;

class TLBaseControl : public QWidget, public TLControlInterface
{
    Q_OBJECT
public:
    explicit TLBaseControl(QWidget *parent = nullptr);
    void setDoubleClickedThreshold(int threshold);  //设置双击判断的阈值，触屏用

    /*@brief        获取硬件的缩放比
     *@return       返回缩放比，1.0~2.0，一般设备都是1.0，苹果的retina显示设备返回2.0
    */
    int getDevicePixelRatio();

    //在特定场景下给按钮设置相应的类型
    void setCustomType(int type){m_nType = type;}
    int getCustomType(){return m_nType;}
signals:
    void sigClicked();
    void sigPressed();
    void sigRelease();
    void sigDoubleClicked();    //直接双击，通过mouseDoubleClickEvent中发送
    void sigDoublePosClicked(); //自己判断，两次点击之间允许有距离，适用于触控设备
    void sigMoveByAsk(const QPoint& offsetPos);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);//鼠标进入事件
    void leaveEvent(QEvent *event);//鼠标离开事件
protected:
    bool m_bActive = true;
    bool m_bDoubleClicked = false;
    bool m_bHover = false;
    bool m_bPressed = false;
    int m_nDoubleClickedThreshold = 30;   //双击判断的阈值，触屏用
    QPoint m_lastPressPos;
    QPoint m_pressPos;
    QPoint m_lastMovePos;
    QTimer* m_pDoubleClickTimer = nullptr;
    int m_nType = -1;

private:
    void triggerMoveEvent(const QPoint& currentPos);

private:
    QDesktopWidget* m_pDestopWidget = nullptr;
};

#endif // TLBASECONTROL_H
