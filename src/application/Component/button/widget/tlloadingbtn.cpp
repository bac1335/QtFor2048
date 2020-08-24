#include "tlloadingbtn.h"
#include <QPainter>
#include <QTimer>
#include <QDebug>

TLLoadingBtn::TLLoadingBtn(int w,int h,QWidget *parent) :
    QWidget(parent)
{
    resize(w+10,h+10);
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(20);
    connect(m_pTimer,&QTimer::timeout,this,&TLLoadingBtn::onTimerOut);
}

void TLLoadingBtn::setLoadingIcon(const QPixmap &pixmap)
{
    m_loadingIcon = pixmap;
    update();
}

void TLLoadingBtn::setLoadingStatus(bool status)
{
//    qDebug() << "--->Lynn<---" << __FUNCTION__ << "status = " << status;
    if(status){
        m_pTimer->start();
    }
    else {
        if(m_pTimer->isActive()){
            m_pTimer->stop();
        }
        m_rRotate = 0;
        m_nCount = 0;
        repaint();
    }
}

void TLLoadingBtn::setTimeOut(int time)
{
    m_nTimeOut = time;
}

void TLLoadingBtn::mousePressEvent(QMouseEvent *e)
{
    m_bPressed = true;
    QWidget::mousePressEvent(e);
}

void TLLoadingBtn::mouseReleaseEvent(QMouseEvent *e)
{
    if(m_bPressed){
        m_bPressed = false;
        if(!m_pTimer->isActive()){
            m_pTimer->start();
            emit sigClicked();
        }
    }
    QWidget::mouseReleaseEvent(e);
}

void TLLoadingBtn::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QRect rect;
    rect.setX(10);
    rect.setY(10);
    rect.setWidth(this->width()-20);
    rect.setHeight(this->height()-20);
    painter.translate(this->width()/2.,this->height()/2.);
    painter.rotate(m_rRotate);
    painter.translate(-this->width()/2.,-this->height()/2.);
    painter.drawPixmap(rect,m_loadingIcon,m_loadingIcon.rect());
    QWidget::paintEvent(e);
}

void TLLoadingBtn::onTimerOut()
{
    m_nCount++;
    m_rRotate -= 7.2;
    if(m_rRotate <= -360){
        m_rRotate = 0;
    }
    if(m_nCount > 50*m_nTimeOut/1000.){
        setLoadingStatus(false);
        emit sigTimeOut();
    }
    this->repaint();
}
