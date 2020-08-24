#ifndef C7COLORWHEEL_H
#define C7COLORWHEEL_H

#include <QWidget>

/**********************************************
 * @author:caisi.hong
 * @date:2018.11.07
 * @effect:色盘
 * @brief:用的是hsv颜色模型
***********************************************/

class C7ColorWheel : public QWidget
{
    Q_OBJECT
public:
    explicit C7ColorWheel(int nRadius = 121, QWidget *parent = nullptr);

    void setScaleFactor(qreal rScaleFactor);
    void setPositionFormColor(const QColor &color);
    void setRadius(int nRadius);
    void setSelectRadius(int nRadius);
    void setSelectVisible(bool bVisible);
signals:
    void sigColorChange(const QColor &color);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    float countLength(int x, int y);
    void setPosition(int nLength, int nAngle);

private:
    bool m_bSelectVisible = true; //在动画中不显示，不然效果太差
    int m_nHue;     //HSV颜色模型的h
    int m_nSaturation;  //HSV颜色模型的s
    int m_nValue;       //HSV颜色模型v
    int m_nRadius;  //色盘的半径
    int m_nWidth;   //选中圆圈的半径
    qreal m_rScaleFactor = 1.0;
    qreal m_colorX = 0;
    qreal m_colorY = 0;
};

#endif // C7COLORWHEEL_H
