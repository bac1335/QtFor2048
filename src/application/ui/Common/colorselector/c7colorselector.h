#ifndef C7COLORSELECTOR_H
#define C7COLORSELECTOR_H

#include "c7basewidget.h"
#include "c7commondef.h"

/**********************************************
 * @author:caisi.hong
 * @date:2018.11.07
 * @effect:画笔颜色选择
 * @brief:用来选择画笔颜色
***********************************************/

class QLabel;
class C7ColorWheel;
class TLPushButton;

class C7ColorSelector : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7ColorSelector(int width, int height, QWidget* parent = nullptr);

    void setColor(const QColor& color);
    void setPostion(ColorWheelPos pos){
        m_colorWheelPos = pos;
    }
    ColorWheelPos getPostion(){
        return m_colorWheelPos;
    }

signals:
    //type 用来标识是从那支笔里面点击打开的色盘
    void sigColorChange(const QColor& color,int type);
    void sigCancel(int type);
protected:
    void initUi();
    void retranslateUi();
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);
private slots:
    void onSetColor(const QColor &color);
    void onOkClick();
    void onCancelClick();
private:
    QPixmap m_bg;
    QLabel* m_pLblTitle;
    QLabel* m_pLblColor;
    QLabel* m_pLblColorValue;
    QColor m_currentColor = Qt::green;
    QColor m_lastColor;
    TLPushButton *m_pOkBtn = nullptr;
    TLPushButton *m_pCancelBtn = nullptr;
    C7ColorWheel *m_pColorWheel = nullptr;
    ColorWheelPos m_colorWheelPos;
};

#endif // C7COLORSELECTOR_H
