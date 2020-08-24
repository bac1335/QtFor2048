#ifndef C7BASEWIDGET_H
#define C7BASEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
/*
 * @brief   :窗口基类
 * @author  :lls
 * @date    :2019.8.30
 */

class QPropertyAnimation;
class QParallelAnimationGroup;

enum Screen_Type{
    Screen_Primary,
    Screen_Assistant
};

class C7BaseWidget : public QWidget
{
    Q_OBJECT

public:
    //动画展开或收起时的起点
    enum Animation_Origin{
        Animation_Center,  //中心点
        Animation_BottomCenter,  //底部中心点
        Animation_LeftTop,      //左上角
        Animation_LeftBottom,   //左下角
        Animation_RightTop,   //右上角
        Animation_RightBottom,   //右下角
    };
    explicit C7BaseWidget(QWidget *parent = nullptr);
    explicit C7BaseWidget(int width, int height, QWidget *parent = nullptr);
    virtual ~C7BaseWidget(){}

    void setScaleFactor(qreal rScaleFactor);
    void setUseAnimation(bool bUse);  //是否显示动画
    void setUseScaleAnimation(bool bUse);  //是否显示动画
    void setUseOpacityAnimation(bool bUse);  //是否显示动画
    void setAnimationOrigin(Animation_Origin origin);
    void setAnimationDuration(int duration);  //动画时间

    virtual void showWithAnimation();  //别的地方只需要调用show()就能自动显示动画
    virtual void hideWithAnimation();   //只能调用该函数才会有动画，调用hide没有动画
    virtual void reInitAnimation(QRect rect);

    void showNoAnimation(); //显示 不需要动画

    virtual void onScaleFactorChange();
    virtual void setItemCheck(bool f){}

    void setCustomType(int type){m_nType = type;}
    int getCustomType()const{return m_nType;}

signals:
    void sigHideFinished();
protected:
    virtual void initUi() {}
    virtual void retranslateUi();
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
    virtual void initAnimation();
    void changeEvent(QEvent *e);

private:
    void commonSetting();
protected:
    bool m_bUseAnimation = false;
    bool m_bUseScaleAnimation = true;
    bool m_bUseOpacityAnimation = true;
    int m_nNormalWidth;         //记住原始的宽度，动画计算缩放系数用
    qreal m_rScaleFactor = 1.0;
    QString m_strAppPath;
    QFont m_font;

    QPropertyAnimation* m_pOpacityAnimation = nullptr;
    QPropertyAnimation* m_pScaleAnimation = nullptr;
    QParallelAnimationGroup* m_pParallelAnimation = nullptr;
    Animation_Origin m_animationOrigin = Animation_BottomCenter;
    int m_nDuration = 150;
    bool m_bNoAnimatonFlag = true;
    int m_nType = -1;
};
#endif // C7BASEWIDGET_H
