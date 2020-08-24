#ifndef C7WECHATUSERITEM_H
#define C7WECHATUSERITEM_H

#include "c7basewidget.h"

class QMouseEvent;

/**
  * @brief 微信用户子控件
  * @author xinde song
  * @date 2019-09-11
  */
class C7WechatUserItem : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7WechatUserItem(int w, int h, QWidget *parent = nullptr);

    void setAvatarImage(const QPixmap &pixmap);
    void setChecked(bool checked);
    void setStatusTip(bool status);  //当新消息来临时，在头像右上角会出现一个红点
    bool isReaded() const;

protected:
    void initUi();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QPixmap m_bg;
    bool m_bIsChecked;
    bool m_bIsReaded;

    bool m_bChecked = false;
    bool m_bMove = false;
    QPoint m_pressPoint;
    bool m_bPress = false;
    QPixmap m_bgNormal;
    QPixmap m_bgPress;

signals:
    void sigItemClick();

};

#endif // C7WECHATUSERITEM_H
