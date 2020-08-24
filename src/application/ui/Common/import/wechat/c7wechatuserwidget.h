#ifndef C7WECHATUSERWIDGET_H
#define C7WECHATUSERWIDGET_H

#include "c7basewidget.h"

#include <QTime>

class QScrollArea;
class TLBaseControl;
class QPropertyAnimation;
class C7WechatUserItem;

/**
  * @brief 微信连接用户显示，可进行滑动
  * @author xinde song
  * @date 2019-09-11
  */
class C7WechatUserWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7WechatUserWidget(int w, int h, QWidget *parent = nullptr);
    C7WechatUserItem * addNewUser(QString headimg);
    void removeUser(C7WechatUserItem * item);
    void addQrcodeItem(C7WechatUserItem *item);
    void setItemTop(C7WechatUserItem *item);//控件移到第2个

protected:
    void initUi();
    void paintEvent(QPaintEvent *event);
    //是否移动到头
    bool isSliperOverHead() const;
    //是否移动到尾
    bool isSliperOverTrail() const;
    int sliperAlignment() const{return m_sliperAlignment;}

private slots:
    void onTrayPress();
    void onTrayRelease();
    void onTrayMoveByAsk(const QPoint &pos);

private:
    //重新布局
    void relayoutItem();
private:
    QPoint m_currentPos;
    QTime m_currentTime;
    int m_sliperAlignment = 0x1;
    QScrollArea* m_pScrollArea = nullptr;
    TLBaseControl* m_pItemTray = nullptr;
    QPropertyAnimation *m_pMoveBackAnimation = nullptr;
    C7WechatUserItem *m_p2dCodeItem = nullptr;
    QList<C7WechatUserItem*> m_mapItem;
signals:
    void sigItemClick(C7WechatUserItem*);
};

#endif // C7WECHATUSERWIDGET_H
