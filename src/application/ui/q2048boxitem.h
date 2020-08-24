#ifndef Q2048BOXITEM_H
#define Q2048BOXITEM_H

#include "q2048basewidget.h"

class Q2048BoxItem : public QWidget{
    Q_OBJECT
public:
    explicit Q2048BoxItem(int w,int h,QWidget* parent = nullptr);
    explicit Q2048BoxItem(QWidget* parent = nullptr);
    void setBgText(QString bg,QString text);
    void reInit();

private:
   void paintEvent(QPaintEvent *event);
   void init();


private:
    QString     m_bg = "#cbc2b2";
    QString     m_text;
    qreal       m_scaleFactory = 1.0;

};

#endif // Q2048BOXITEM_H
