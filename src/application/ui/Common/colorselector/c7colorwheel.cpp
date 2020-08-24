#include "c7colorwheel.h"

#include <QDebug>
#include <QPainter>
#include <QMouseEvent>

#define PI 3.141592653

C7ColorWheel::C7ColorWheel(int nRadius, QWidget *parent)
    : QWidget(parent), m_nRadius(nRadius), m_nValue(255)
{
    m_nWidth = 3;
    this->resize(m_nRadius * 2 + m_nWidth * 4, m_nRadius * 2 + m_nWidth * 4);
}

void C7ColorWheel::setScaleFactor(qreal rScaleFactor)
{
    m_rScaleFactor = rScaleFactor;
}

void C7ColorWheel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //生成角度渐变
    QConicalGradient conicalGradient(0, 0, 0);
    conicalGradient.setColorAt(0.0, Qt::red);
    conicalGradient.setColorAt(60.0/360.0, Qt::yellow);
    conicalGradient.setColorAt(120.0/360.0, Qt::green);
    conicalGradient.setColorAt(180.0/360.0, Qt::cyan);
    conicalGradient.setColorAt(240.0/360.0, Qt::blue);
    conicalGradient.setColorAt(300.0/360.0, Qt::magenta);
    conicalGradient.setColorAt(1.0, Qt::red);
    //转换坐标
    painter.translate(m_nRadius + m_nWidth * 2, m_nRadius + m_nWidth * 2);
    QBrush brush(conicalGradient);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);
    painter.drawEllipse(QPoint(0, 0), m_nRadius, m_nRadius);

    //设置为叠加模式
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    //生成辐射渐变
    QRadialGradient radialGradient(0, 0, m_nRadius, 0, 0);
    //创建了一个QRadialGradient对象实例，参数分别为中心坐标，半径长度和焦点坐标,如果需要对称那么中心坐标和焦点坐标要一致
    //从里到外透明度变化
    radialGradient.setColorAt(0, QColor(255, 255, 255, 255));
    radialGradient.setColorAt(1.0,QColor(255, 255, 255, 0));
    painter.setBrush(QBrush(radialGradient));
    painter.drawEllipse(QPoint(0, 0), m_nRadius, m_nRadius);//在相应的坐标画出来
    painter.setPen(QPen(Qt::black));
    painter.setBrush(Qt::NoBrush);

    if (m_bSelectVisible)
    {
        painter.drawEllipse(QPointF(m_colorX - m_nRadius, m_colorY - m_nRadius),
                            m_nWidth * 2, m_nWidth * 2);
    }
}

void C7ColorWheel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->buttons() & Qt::LeftButton)
    {
        m_colorX = ev->pos().x();
        m_colorY = ev->pos().y();
        qDebug() << "hongcs" << m_colorX << m_colorY;
        //因为色盘的圆心是（m_nRadius，m_nRadius），这里计算到色盘圆心的距离
        float fLength = countLength(m_colorX - m_nRadius, m_colorY - m_nRadius);
        if (fLength <= m_nRadius)
        {
            QLineF currentLine(QPoint(m_nRadius, m_nRadius), QPoint(m_nRadius * 2, m_nRadius));

            int nAngle = currentLine.angleTo(QLineF(ev->pos(), QPoint(m_nRadius, m_nRadius)));

            //这里本意是色盘右边为起点，但是好像是从左边作为起点，所以要做以下处理
            nAngle -= 180;
            if (nAngle < 0)
            {
                nAngle = nAngle + 180;
                int intervale = qAbs(nAngle);
                nAngle = 180 + intervale;
            }

            //角度是颜色类型，距离圆心的距离是颜色深度
            m_nHue = nAngle;
            m_nSaturation = (int)(fLength / m_nRadius * 255);
            emit sigColorChange(QColor::fromHsv(m_nHue, m_nSaturation, m_nValue));
            update();
        }
    }
}

void C7ColorWheel::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons() & Qt::LeftButton)
    {
        m_colorX = ev->pos().x();
        m_colorY = ev->pos().y();

        float fLength = countLength(m_colorX - m_nRadius, m_colorY - m_nRadius);

        QLineF currentLine(QPoint(m_nRadius, m_nRadius), QPoint(m_nRadius * 2, m_nRadius));
        int nAngle = currentLine.angleTo(QLineF(ev->pos(), QPoint(m_nRadius, m_nRadius)));
        nAngle -= 180;
        if (nAngle < 0)
        {
            nAngle = nAngle + 180;
            int intervale = qAbs(nAngle);
            nAngle = 180 + intervale;
        }

        m_nHue = nAngle;
        if (fLength > m_nRadius)
        {
            fLength = m_nRadius;
            setPosition(m_nRadius, nAngle);
        }

        m_nSaturation = (int)(fLength / m_nRadius * 255);
        emit sigColorChange(QColor::fromHsv(m_nHue, m_nSaturation, m_nValue));
        update();
    }
}

/****************************************************************************
 * @author  :Hongcansi
 * @date    :2018.04.20
 * @brief   :计算点到圆心的距离
 * @other   :
****************************************************************************/
float C7ColorWheel::countLength(int x, int y)
{
    return sqrt((x * x) + (y * y));
}

void C7ColorWheel::setPositionFormColor(const QColor &color)
{
    m_nSaturation = color.toHsv().saturation();
    int nLength = m_nSaturation / 255.0 * m_nRadius;

//    qDebug() << "hongcs" << "m_nRadius" << __FUNCTION__ << m_nRadius;
//    qDebug() << "hongcs" << "nLength" << __FUNCTION__ << nLength;

    m_nHue = color.toHsv().hue();

    setPosition(nLength, m_nHue);
}

void C7ColorWheel::setRadius(int nRadius)
{
    m_nRadius = nRadius;
    this->resize(m_nRadius * 2 + m_nWidth * 4, m_nRadius * 2 + m_nWidth * 4);
//    qDebug() << "hongcs" << "m_nRadius" << __FUNCTION__ << m_nRadius;
    update();
}

void C7ColorWheel::setSelectRadius(int nRadius)
{
    m_nWidth = nRadius;
}

void C7ColorWheel::setSelectVisible(bool bVisible)
{
    m_bSelectVisible = bVisible;
}

void C7ColorWheel::setPosition(int nLength, int nAngle)
{
    double arc_angle = nAngle / 180.0 * PI;
    if (arc_angle > PI)
    {
        m_colorY = m_nRadius + qAbs(nLength * sin(arc_angle));
    }
    else
    {
        m_colorY = m_nRadius - (nLength * sin(arc_angle));
    }

    if (arc_angle > PI * 0.5 && arc_angle < 1.5 * PI)
    {
        m_colorX = m_nRadius - qAbs(nLength * cos(arc_angle));
//        qDebug() << "minus" << nLength * cos(arc_angle);
    }
    else
    {
        m_colorX = m_nRadius + (nLength * cos(arc_angle));
//        qDebug() << "add" << nLength * cos(arc_angle);
    }

//    qDebug() << m_nHue << m_nSaturation << nLength << arc_angle;

    update();
}
