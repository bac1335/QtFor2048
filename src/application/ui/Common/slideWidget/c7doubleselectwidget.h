#ifndef C7DOUBLESELECTWIDGET_H
#define C7DOUBLESELECTWIDGET_H

#include "c7basewidget.h"

class QTimer;
class QMouseEvent;

class C7DoubleSelectWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    enum Position{
        Pos_Left,
        Pos_Right,
    };
    explicit C7DoubleSelectWidget(int w, int h, QWidget *parent = nullptr,int x = 20,int r = 20,int radius = 8);
    void setBgText(const QString &leftText, const QString &rightText);
    //true表示右边,false表示左边
    void setChceked(bool check);
    void setBg(QString str);
    void setSelectBg(const QString &left, const QString &right);
    void setLeftBg(const QString &normal, const QString &select);
    void setRightBg(const QString &normal, const QString &select);

protected:
    void initUi();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void sigChecked(bool checked);

protected:
    QImage m_bg;
    QTimer *m_pTimer = nullptr;
    int m_x;
    int m_width;
    int m_height;
    int m_widthOffset;
    int m_heightOffset;
    int m_radius;
    bool m_bChecked;
    Position m_position;

    QString m_leftText;
    QString m_rightText;
    QPixmap m_leftNormalBg;
    QPixmap m_leftSelectBg;
    QPixmap m_rightNormalBg;
    QPixmap m_rightSelectBg;

};

#endif // C7DOUBLESELECTWIDGET_H
