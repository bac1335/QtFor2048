#ifndef TIMECHOICE_H
#define TIMECHOICE_H
#include "c7basewidget.h"
#include "tlpushbutton.h"
#include <QLabel>

/****************************************************************************
 * @author  : lls
 * @date    : 2019.11.6
 * @brief   : 时间选择器
 * @other   :C7TimeChoice为选择时间，C7DataTime时间列表，C7DataItem时间item
****************************************************************************/
class C7DataTime;
class C7DataItem;

class C7TimeChoice : public C7BaseWidget{
    Q_OBJECT
public:
    explicit C7TimeChoice(int w,int h,QWidget* parent = nullptr);
    void initPos();
    int getYear();
    int getMonth();
    void hideChoiceData();

protected:
    void paintEvent(QPaintEvent *event);
    void hideEvent(QHideEvent*);

private:
    void initUi();

private:
    C7DataTime* m_dataTime = nullptr;
    QPixmap m_pix;
    TLPushButton* m_timeChoiceBtn = nullptr;
    QLabel* m_dataLabel = nullptr;

private slots:
    void onData(int,int);

signals:
    void sigTimeChoice(C7DataTime*);
};

class C7DataTime : public C7BaseWidget{
    Q_OBJECT
public:
    explicit C7DataTime(QWidget* parent = nullptr);
    int getYear(){return m_yearData;}
    int getMonth(){return m_monthData;}

protected:
    void paintEvent(QPaintEvent *event);
    void initUi();
    C7DataItem* createBtn(int num);

private:
    TLPushButton* m_pLeftBtn = nullptr;
    TLPushButton* m_pRightBtn = nullptr;
    QLabel* m_yealLabel = nullptr;
    QSize m_size;
    C7DataItem* m_staticItem = nullptr;
    QList<C7DataItem*> m_itemList;
    int m_radius = 8;
    int m_yearData;
    int m_monthData;

signals:
    void sigData(int,int);
};

class C7DataItem : public C7BaseWidget{
    Q_OBJECT
public:
    explicit C7DataItem(int w,int h,QWidget* parent = nullptr);
    void setChecked(bool checked);
    void setText(QString);
    QString getText(){return m_text;}

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent*);

private:
    bool m_bMove = false;
    bool m_bPress = false;
    bool m_bIsChecked =false;
    QPoint m_pressPoint;
    QString m_text;

signals:
    void sigChecked();
    void sigDoubleClicked();
};



#endif // TIMECHOICE_H
