#ifndef TLGRAPHICSBASECONTROL_H
#define TLGRAPHICSBASECONTROL_H

#include <QGraphicsWidget>

#include "../tlcontrolinterface.h"

class TLGraphicsBaseControl : public QGraphicsWidget, public TLControlInterface
{
    Q_OBJECT
public:
    explicit TLGraphicsBaseControl(QGraphicsObject* parent = nullptr);
    ~TLGraphicsBaseControl();
    inline qreal width() { return boundingRect().width(); }
    inline qreal height() { return boundingRect().height(); }
    inline QSizeF size() { return boundingRect().size(); }
    inline void setFixedSize(const QSizeF &size) { resize(size); }
    void move(qreal x, qreal y) { this->setPos(x, y); }
    void move(const QPointF &pos) { this->setPos(pos); }
    void setDevicePixelRatio(int ratio) { m_ndevicePixelRatio = ratio; }
    int getDevicePixelRatio() const { return m_ndevicePixelRatio; }
signals:
    void sigMoveByAsk(const QPoint &pos);
    void sigClicked();
    void sigPressed();
    void sigRelease();
    void sigDragModeChange(int);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QEvent *event);//鼠标进入事件
    void hoverLeaveEvent(QEvent *event);//鼠标离开事件
protected:
    bool m_bHover = false;
private:
    void triggerMoveEvent(const QPointF &currentPos);
    void dragModeChange(int mode);
private:
    int m_ndevicePixelRatio = 1;
    QPointF m_pressPos;
    QPointF m_lastMovePos;
};

#endif // TLGRAPHICSBASECONTROL_H
