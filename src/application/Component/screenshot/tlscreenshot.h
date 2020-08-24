#ifndef TLSCREENSHOT_H
#define TLSCREENSHOT_H

#include <QGraphicsObject>

class QGraphicsView;
class TLScreenshotPixmap;
class TLGraphicsPushButton;
class QTouchEvent;

class TLScreenshot : public QGraphicsObject
{
    Q_OBJECT
public:
    enum BtnType{
        BtnType_Save = 1,
        BtnType_Exit,
        BtnType_Confirm
    };

    explicit TLScreenshot(QSize size, QGraphicsObject* parent = nullptr, QGraphicsView* parentView = 0);
    ~TLScreenshot();

    void cut(bool bSelectArea = true);
    void setShadowColor(const QColor& color);
    void setSelectColor(const QColor& color);
signals:
    void sigBtnPress(BtnType btnType);
    void sigScreenshotFinished(const QImage& image, const QString& name);

    void sigExitScreenshot();
    void sigSaveRequest();
    void sigSaveImageFinished();
public slots:
    void onGetSavePath(const QString& savePath);
protected:
    void initUi();
    QRectF boundingRect() const;
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool sceneEvent(QEvent *event);
    bool touchEvent(QTouchEvent *event);
private:
    TLGraphicsPushButton* createGraphicsPushButton(const QString& normal, const QString& press);
    void setBtnVisible(bool bVisible);
    void drawBorderRect(QPainter* painter, QRectF rect, QColor color);
    void dotPress(int id, QPointF pos);
    void dotMove(int id, QPointF pos);
    void dotRelease(int id, QPointF pos);
    int getTouchBtn(QPointF pos);
    void adjustBtnPos();
    void initMoveOrRelease(int id, QPointF pos);
    QRectF getRectF(QPointF pointO, QPointF pointT);
    void adjustRectMoveAndScale(QRectF& rect);
    QString screenShot(QRectF rect, bool bSave, QString dirPath = QString(""));
    QImage cutImage(const QRectF &rect, const QImage &image);
    void rescreenShot();
    void exitScreenshot();
    void execScreenShot(bool bSave, const QString& savePath = QString("")); //用于截图时隐藏框和按键
private slots:
    void onBtnClick();

private:
    bool m_bGestureMode;   //移动或缩放模式
    bool m_bTouchMode = false;
    bool m_bTouchToMousePressMode = false;
    bool m_bSelectArea = true;
    bool m_bShoting = false;
    qreal m_rScaleFactor = 1.0;
    QSize m_size;
    QRectF m_selectRect;
    QRectF m_realtimeRect;
    QColor m_shadowColor = QColor(0, 0, 0, 100);
    QColor m_selectColor = QColor(0, 0, 0,  10);
    QPointF m_pressPos;
    QPointF m_endPos_mouse;
    QPointF m_previousPos_mouse;
    QPointF m_currentPos;
    QGraphicsView* m_pParentView = nullptr;
    TLScreenshotPixmap* m_pScreenshotPixmap = nullptr;
    TLGraphicsPushButton* m_pSaveButton = nullptr;
    TLGraphicsPushButton* m_pExitButton = nullptr;
    TLGraphicsPushButton* m_pConfirmButton = nullptr;
    QMap<int, QPointF> m_touchMap;
};

/****************************************************************************
 * @author  :Hongcansi
 * @date    :2019.02.12
 * @brief   :这个是模仿云6的截图功能加的，好像没有用到，后面视情况可以删掉它
 * @other   :
****************************************************************************/

class TLScreenshotPixmap : public QGraphicsObject
{
    Q_OBJECT
public:
    enum DragMode{
        NoDrag_Mode = 1,
        Press_Mode,
        Drag_Mode
    };
    explicit TLScreenshotPixmap(const QPixmap& pixmap, QGraphicsObject* parent = nullptr);
    inline void setMoveable(bool bMove) { m_bMoveable = bMove; }
    inline void setPixmap(const QPixmap& pixmap) { m_pixmap = pixmap; }\

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    DragMode m_dragMode = NoDrag_Mode;
    bool m_bMoveable = false;
    bool m_bRightButtonPress = true;
    QPixmap m_pixmap;
};

#endif // TLSCREENSHOT_H
