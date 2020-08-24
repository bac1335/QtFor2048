#include "tlloading.h"
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include "tlutility.h"
#include "maskwidget/tlmaskwidget.h"

TLLoading::TLLoading(bool modal, QWidget *parent) :
    QWidget(parent)
{
    this->hide();
    m_bg = QPixmap(":/waitting/image/bg_loading.png");
    m_rScaleFactor = Uti->getScaleFactor();
    this->resize(182*m_rScaleFactor,182*m_rScaleFactor);

    if(modal){//加载模态
        setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);
        setWindowModality(Qt::WindowModal);
        QRect rect = qApp->desktop()->screenGeometry();
        this->move(rect.width()/2.-width()/2.,
                         rect.height()/2.-height()/2.);
    }
    else {   //加载遮罩
        TLMaskWidget* pMask = Uti->addMask(this);
        if (pMask != nullptr)
        {
            pMask->setWindowFlags(Qt::FramelessWindowHint |Qt::WindowStaysOnTopHint);
            pMask->setBackgroundBrush(QBrush(QColor(0, 0, 0, 30)));
            pMask->setClickHide(false);
        }
    }
}

void TLLoading::startWaiting()
{
    qDebug() <<__FUNCTION__ ;

    if(this->isVisible())
        return;

    this->show();
    QWidget * parent = this->parentWidget();
    if(parent){
       parent->show();
       parent->raise();
    }
    m_iAngle =0;
    if(m_iTimerId == -1)
    {
        m_iTimerId = startTimer(m_iDisplayTime);
    }
    if(m_iTimerTimeoutId == -1)
    {
        m_iTimerTimeoutId = startTimer(m_iWaitTimeout);
    }
}

void TLLoading::stopWaiting()
{
    qDebug() << "TLLoading::stopWaiting()";

    if(!this->isVisible())
        return;

    this->hide();

    if(m_iTimerId != -1)
    {
        killTimer(m_iTimerId);
    }
    if(m_iTimerTimeoutId != -1)
    {
        killTimer(m_iTimerTimeoutId);
    }
    m_iTimerId =-1;
    m_iTimerTimeoutId = -1;
    update();
}

void TLLoading::setTimeoutTime(int time)
{
    m_iWaitTimeout = time;
}

void TLLoading::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == m_iTimerId)
    {
        m_iAngle =(m_iAngle - 30) % 360;
        update();
    }
    else if(e->timerId() == m_iTimerTimeoutId)
    {
        stopWaiting();
        emit sigWaitTimeout();
    }
   QWidget::timerEvent(e);
}

void TLLoading::paintEvent(QPaintEvent *)
{
    if(!isRunning())
        return;

    //动态圈圈的直径大小
    int width= 80*m_rScaleFactor;

    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);

    p.drawPixmap(this->rect(),m_bg,m_bg.rect());

    int outer_radius = (width - 1) * 0.4;
    int inner_radius = (width - 1) * 0.3;

    int capsule_height = 8*m_rScaleFactor;
    int capsule_width  = 8*m_rScaleFactor;
    int capsule_radius = capsule_width/2;

    for(int i=0; i<12; i++)
    {
        m_loadColor.setAlphaF(1.0f - (i/12.0f));
        p.setPen(Qt::NoPen);
        p.setBrush(m_loadColor);
        p.save();
        p.translate(rect().center());
        p.rotate(m_iAngle - i*30.0f);
        p.drawRoundedRect(-capsule_width*0.5,-(inner_radius + capsule_height), capsule_width, capsule_height,capsule_radius, capsule_radius);
        p.restore();
    }
}

bool TLLoading::isRunning() const
{
    return (m_iTimerId != -1);
}

void TLLoading::setColor(const QColor &color)
{
    m_loadColor = color;
    update();
}
