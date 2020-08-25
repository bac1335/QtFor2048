#ifndef Q2048WIDGET_H
#define Q2048WIDGET_H

#include "q2048basewidget.h"
#include <QMenu>

class Q2048Widget : public Q2048BaseWidget{
    Q_OBJECT
public:
    explicit Q2048Widget(QWidget* parent = nullptr);
    explicit Q2048Widget(int w,int h,QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent*) override;

private:
    void init();
    Q2048BoxItem* creatBox() override;

signals:
    void sigKeyPress(int);
    void sigReinit();

public slots:
    void onSendData(QVector<QVector<int>>,bool);

private:
    QSize                   m_size;
    QSize                   m_boxSize;
    QList<Q2048BoxItem*>    m_listBox;
};


#endif // Q2048WIDGET_H
