#ifndef TLNOTICE_H
#define TLNOTICE_H

/**
  * 提示窗口,渐隐消失
  */

#include <QWidget>
#include <QGraphicsWidget>
class QTimer;
class QGraphicsOpacityEffect;
class QPropertyAnimation;
class QDesktopWidget;
class QFontMetrics;
class QApplication;
class QMouseEvent;
class QPainter;

class TLNoticeInterface{
public:
    virtual void notice(QString str, QPoint pos) = 0;  //显示通知,没什么需要定制就直接用这个函数啊。
    virtual void notice(QString str) = 0;              //显示通知
    virtual void notice() = 0;                         //在已经设置好了str的话就可以直接show了;
    enum BackGroupStyle{
        Style_Ellipse,
        Style_Rect,
    };
    void setBackGroupStyle(BackGroupStyle type){m_backgroupStype = type;}
protected:
    BackGroupStyle m_backgroupStype = Style_Ellipse;
};

class TLNotice: public QWidget, public TLNoticeInterface
{
    Q_OBJECT
public:
    TLNotice(QWidget* parent = nullptr);
    ~TLNotice();
    void notice(QString str, QPoint pos);//显示通知,没什么需要定制就直接用这个函数啊。
    void notice(QString str);            //显示通知
    void notice();                       //在已经设置好了str的话就可以直接show了;
    inline int pixelSize(){return font().pixelSize();}
    inline void setFontColor(QColor c){m_fontColor = c; update();}
    void setDisAppearedTime(int st);//渐隐时间
    int disAppearTime() const;
    inline void setShowTime(int t){m_showTime = t;} //显示时间（不包括渐隐的时间）
    inline void setDefaultCenterPos(QPoint p){m_defaultPos = p;}//设置显示的时候默认的[中心点]的pos，（初始化是屏幕中间）
    inline QPoint defaultCenterPos() const {return m_defaultPos;}
    int getNoticeMessageLenght(); //返回消息的长度
    void setPixelSize(int w);           //设置字体大小
    void setDefinedFont(QFont f);           //设置字体
    void setNoticetHeight(int h);       //只设置高度，长度由msg决定
    void setBackgroupColor(const QColor &color){
        m_bgColor = color;
        update();
    }
    void setDefaultPos();
    void setScaleFactor(qreal factor){
        m_nScaleFactor = factor;
    }
protected:
    void paintEvent(QPaintEvent*);      //画出提示语
    void mousePressEvent(QMouseEvent*); //点击就会隐藏
signals:
    void sigFinished();
public slots:
    void onActiveHideAnimation();//开始渐隐
    void onHideAnimotionFinish();//渐隐结束后隐藏
private:
    void init(QWidget *parent); //初始化构造函数
    void reset();                  //初始化notice
private:
    int m_height;
    int m_showTime;
    int m_disApearTime;
    QColor m_fontColor;
    QColor m_bgColor;
    QPoint m_defaultPos;
    QString m_noticeMessage;
    QPropertyAnimation* m_disappearedAnimation;     //消失动画，渐隐
    QGraphicsOpacityEffect* m_widgetOpacityEffect;  //setWindowOpcity无效，用这个
    qreal m_nScaleFactor = 1.0;
};


//可以旋转
class CbGraphicsNotice: public QGraphicsWidget, public TLNoticeInterface
{
    Q_OBJECT
public:
    CbGraphicsNotice(QGraphicsItem* parent = nullptr);
    ~CbGraphicsNotice();
    void notice(QString str, QPoint pos);//显示通知,没什么需要定制就直接用这个函数啊。
    void notice(QString str);            //显示通知
    void notice();                       //在已经设置好了str的话就可以直接show了;
    inline int pixelSize(){return font().pixelSize();}
    inline void setFontColor(QColor c){m_fontColor = c; update();}
    void setDisAppearedTime(int st);//渐隐时间
    inline void setShowTime(int t){m_showTime = t;} //显示时间（不包括渐隐的时间）
    inline void setDefaultCenterPos(QPoint p){m_defaultPos = p;}//设置显示的时候默认的[中心点]的pos，（初始化是屏幕中间）
    inline QPoint defaultCenterPos() const {return m_defaultPos;}
    int getNoticeMessageLenght(); //返回消息的长度
    void setPixelSize(int w);           //设置字体大小
    void setNoticetHeight(int h);       //只设置高度，长度由msg决定

    void setBackgroupColor(const QColor &color);
    int width();
    int height();
    void setScaleFactor(qreal factor){
        m_nScaleFactor = factor;
    }


protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *);//点击就会隐藏
signals:
    void sigFinished();
public slots:
    void onActiveHideAnimation();//开始渐隐
    void onHideAnimotionFinish();//渐隐结束后隐藏
private:
    void init(QGraphicsItem *parent); //初始化构造函数
    void reset();                  //初始化notice
private:
    int m_height;
    int m_showTime;
    QColor m_fontColor;
    QColor m_bgColor;
    QPoint m_defaultPos;
    QString m_noticeMessage;
    QPropertyAnimation* m_disappearedAnimation;     //消失动画，渐隐
    QGraphicsOpacityEffect* m_widgetOpacityEffect;  //setWindowOpcity无效，用这个
    qreal m_nScaleFactor = 1.0;
};

#endif // C5GRAPICNOTICE_H
