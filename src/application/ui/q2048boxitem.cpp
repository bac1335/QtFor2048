#include <q2048boxitem.h>
#include <QPainter>
#include "Utility/tlutility.h"

Q2048BoxItem::Q2048BoxItem(int w, int h, QWidget *parent):
    QWidget(parent)
{
    this->resize(w,h);
    init();
}

Q2048BoxItem::Q2048BoxItem(QWidget *parent):
    QWidget(parent)
{
    init();
}

void Q2048BoxItem::setBgText(QString bg, QString text)
{
    m_bg = bg;
    m_text = text;

    update();
}

void Q2048BoxItem::reInit()
{
    m_bg = "#cbc2b2";
    m_text.clear();
    update();
}

void Q2048BoxItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(this->rect(),m_bg);


    if(!m_text.isNull()){
        QFont font;
        font.setPixelSize(24 * m_scaleFactory);
        font.setBold(true);
        painter.setFont(font);
        painter.drawText(this->rect(),Qt::AlignCenter,m_text);
    }
}

void Q2048BoxItem::init()
{
    m_scaleFactory = Uti->getScaleFactor();
}
