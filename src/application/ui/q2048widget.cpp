#include "q2048widget.h"
#include <QPainter>
#include "q2048boxitem.h"
#include <QKeyEvent>
#include <QDebug>
#include <QMessageBox>

Q2048Widget::Q2048Widget(QWidget *parent):
    Q2048BaseWidget(parent)
{

}

Q2048Widget::Q2048Widget(int w, int h, QWidget *parent):
    Q2048BaseWidget(parent),m_size(w,h)
{
    init();
}

void Q2048Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(this->rect(),"#bbafa0");
}

void Q2048Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_A:
        case Qt::Key_W:
        case Qt::Key_S:
        case Qt::Key_D:
        emit sigKeyPress(event->key());
        break;
    }
}

void Q2048Widget::init()
{
    int scale = m_boxMax/4;

    this->setFixedSize(m_size.width() * scale,m_size.height() * scale);

    m_boxLayout = new QGridLayout;
    m_boxLayout->setSpacing(15 * m_scaleFactory);

    for(int i = 0;i< m_boxMax * m_boxMax;i++){
        Q2048BoxItem* box = creatBox();
        m_boxLayout->addWidget(box,i/m_boxMax,i%m_boxMax);
        m_listBox.push_back(box);
    }

    this->setLayout(m_boxLayout);
}

Q2048BoxItem* Q2048Widget::creatBox()
{
    Q2048BoxItem* box = new Q2048BoxItem(this);
    return box;
}

void Q2048Widget::onSendData(QVector<QVector<int> > data,bool isOver)
{
    for(int i = 0;i<m_boxMax;i++){
        if( i < data.count()){
            QVector<int> data2 = data.at(i);
            for(int j = 0;j<m_boxMax;j++){
                Q2048BoxItem*  item =  m_listBox.at( i * m_boxMax +j);
                if(j < data2.count()){
                    int index = data2.at(j);
                    QString color;
                    if(index == -1){
                       item->reInit();
                    }
                    else{
                        color = m_colorMap.value(index,"cbc2b2");
                        item->setBgText(color,QString::number(index));
                    }
                }
                else{
                    item->reInit();
                }
            }
        }
    }

    if(isOver){
        QMessageBox::warning(nullptr,"WARNING","gave  over!");
    }
}
