#include "c7doubleselectwidget.h"
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>

C7DoubleSelectWidget::C7DoubleSelectWidget(int w, int h, QWidget *parent,int x,int r,int radius):
    C7BaseWidget(w, h, parent), m_x(x * m_rScaleFactor),m_radius(radius),
    m_position(Pos_Left)
{
    m_widthOffset = m_x;
    m_heightOffset = r * m_rScaleFactor;
    m_width = w/2.- m_widthOffset;
    m_height = h;
    this->resize(w, h);
    initUi();
}

void C7DoubleSelectWidget::setBgText(const QString &leftText, const QString &rightText)
{
    m_leftText = leftText;
    m_rightText = rightText;
}

void C7DoubleSelectWidget::setChceked(bool check)
{
    m_bChecked = check;

    if(check){
        m_x = m_width + m_widthOffset;
        m_position = Pos_Right;
    }
    else{
        m_x = m_widthOffset;
        m_position = Pos_Left;
    }

    update();
}

void C7DoubleSelectWidget::setBg(QString str)
{
    m_bg.load(str);
    update();
}


void C7DoubleSelectWidget::setLeftBg(const QString &normal, const QString &select)
{
    m_leftNormalBg.load(normal);
    m_leftSelectBg.load(select);
}

void C7DoubleSelectWidget::setRightBg(const QString &normal, const QString &select)
{
    m_rightNormalBg.load(normal);
    m_rightSelectBg.load(select);
}

void C7DoubleSelectWidget::initUi()
{
    m_bg.load(m_strAppPath + "/skin/bg/bg_slide.png");

    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(20);

    connect(m_pTimer, &QTimer::timeout, this, [=](){
        if(m_bChecked)
        {
            m_x += m_widthOffset;
            if(m_x >= (m_width+ 5*m_rScaleFactor))
            {
                m_pTimer->stop();
                m_position = Pos_Right;
            }
        }
        else
        {
            m_x -= m_widthOffset;
            if(m_x <= (m_widthOffset+5*m_rScaleFactor))
            {
                m_pTimer->stop();
                m_position = Pos_Left;
            }
        }
        update();
    });
}

void C7DoubleSelectWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setBrush(Qt::NoBrush);
    painter.drawImage(this->rect(), m_bg, m_bg.rect());
    painter.setBrush(QBrush(QColor(50, 187, 133)));
    painter.setPen(QPen(Qt::NoPen));
    painter.drawRoundedRect(QRect(m_x == 0 ? 20*m_rScaleFactor:m_x, m_heightOffset, m_width, (m_height-2*m_heightOffset)),
                            m_radius*m_rScaleFactor, m_radius*m_rScaleFactor);
    if(m_leftNormalBg.isNull()){
        painter.drawRoundedRect(QRect(m_x, m_heightOffset, m_width, (m_height-2*m_heightOffset)),
                                m_radius*m_rScaleFactor, m_radius);
        painter.setFont(m_font);
        painter.setPen(QPen(Qt::white));
        if(m_position == Pos_Right)
        {
            painter.setPen(QPen(Qt::black));
        }

        painter.drawText(QRect(m_widthOffset, m_heightOffset, m_width, (m_height-2*m_heightOffset)),
                         Qt::AlignCenter, m_leftText);

        painter.setPen(QPen(Qt::white));
        if(m_position == Pos_Left)
        {
            painter.setPen(QPen(Qt::black));
        }
        painter.drawText(QRect((m_widthOffset+m_width), m_heightOffset, m_width, (m_height-2*m_heightOffset)),
                         Qt::AlignCenter, m_rightText);
    }
    else{
        if(m_bChecked){
            painter.drawPixmap(QRect((m_width/2 - 18*m_rScaleFactor) + 20*m_rScaleFactor,
                                     m_heightOffset + (m_height-2*m_heightOffset)/2 - 18*m_rScaleFactor,
                                     36*m_rScaleFactor, 36*m_rScaleFactor), m_leftNormalBg);
            painter.drawPixmap(QRect(m_widthOffset + m_width + (m_width/2 - 18*m_rScaleFactor),
                                     m_heightOffset + (m_height-2*m_heightOffset)/2 - 18*m_rScaleFactor,
                                     36*m_rScaleFactor, 36*m_rScaleFactor), m_rightSelectBg);
        }
        else {
            painter.drawPixmap(QRect((m_width/2 - 18*m_rScaleFactor) + 20*m_rScaleFactor,
                                     m_heightOffset + (m_height-2*m_heightOffset)/2 - 18*m_rScaleFactor,
                                     36*m_rScaleFactor, 36*m_rScaleFactor), m_leftSelectBg);
            painter.drawPixmap(QRect(m_widthOffset + m_width + (m_width/2 - 18*m_rScaleFactor),
                                     m_heightOffset + (m_height-2*m_heightOffset)/2 - 18*m_rScaleFactor,
                                     36*m_rScaleFactor, 36*m_rScaleFactor), m_rightNormalBg);
        }
    }

    C7BaseWidget::paintEvent(event);
}

void C7DoubleSelectWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->x() >= this->width()/2 && m_position == Pos_Left)
    {
        m_bChecked = true;
        m_pTimer->start();
    }
    else if(event->x() < this->width()/2 && m_position == Pos_Right)
    {
        m_bChecked = false;
        m_pTimer->start();
    }
    emit sigChecked(m_bChecked);
}
