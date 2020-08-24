#include "tlwaitwidget.h"

#include <QTimer>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>

TLWaitWidget::TLWaitWidget(int width, int height, QWidget *parent)
    : QWidget(parent)
{
    this->resize(width, height);
    this->getScaleFactor();
    m_font = QFont(u8"思源黑体 Medium", 18 * m_rScaleFactor);
    m_loadImage.load(":/waitting/image/icon_loading.png");
    m_pCircleTimer = new QTimer(this);
    m_pCircleTimer->setInterval(50);
    connect(m_pCircleTimer, &QTimer::timeout, this, &TLWaitWidget::onCircleTimeout);
}

void TLWaitWidget::setWaitText(const QString &text)
{
    m_strWaitText = text;
}

void TLWaitWidget::setCurrentFont(const QFont &font)
{
    m_font = font;
}

void TLWaitWidget::setWaitTime(int time)
{
    m_nWaitTime = time/50;
}

void TLWaitWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(0, 0, 0, 77)));
    painter.setPen(Qt::NoPen);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    painter.drawRect(this->rect());
    painter.setPen(Qt::white);
    painter.setFont(m_font);

    painter.drawText(0, this->height()/2 - m_loadImage.height()/4*m_rScaleFactor + m_loadImage.height()/2*m_rScaleFactor + 20*m_rScaleFactor,
                     this->width(), 40*m_rScaleFactor, Qt::AlignCenter, m_strWaitText);
    if(m_bCircle)
    {
        painter.translate(this->width() / 2, this->height() / 2);          //设置旋转中心
        painter.rotate(m_nAngle);          //旋转
        painter.translate(-(this->width() / 2), -(this->height() / 2));        //将原点复位
    }
    painter.drawPixmap(QRect(this->width()/2 - m_loadImage.width()/4*m_rScaleFactor, this->height()/2 - m_loadImage.height()/4*m_rScaleFactor,
                            m_loadImage.width()/2*m_rScaleFactor, m_loadImage.height()/2*m_rScaleFactor), m_loadImage);

    QWidget::paintEvent(event);
}

void TLWaitWidget::showEvent(QShowEvent *event)
{
    this->raise();
    m_bCircle = true;
    m_nAngle = 0;
    m_pCircleTimer->start();

    QWidget::showEvent(event);
}

void TLWaitWidget::hideEvent(QHideEvent *event)
{
    m_bCircle = false;
    m_nCircleCount = 0;
    m_pCircleTimer->stop();

    QWidget::hideEvent(event);
}

void TLWaitWidget::onCircleTimeout()
{
    ++m_nCircleCount;
    if (m_nAngle >= 360) {
        m_nAngle = 0;
    }
    m_nAngle += 10;

    //超过50*600ms = 30000ms = 30s
    if (m_nCircleCount >= m_nWaitTime) {
        this->hide();
    }
    update();
}

QSize TLWaitWidget::getDesktopSize(int screen)
{
    return qApp->desktop()->screenGeometry(screen).size();
}

void TLWaitWidget::getScaleFactor()
{
    int width = 1920.;
    int height = 1080.;
    int deskWidth = getDesktopSize().width();
    int deskHeight = getDesktopSize().height();

    qreal normalScale = width / height;
    qreal curScale = deskWidth / deskHeight;
    qreal rRet = 1.0;
    if(normalScale <= curScale)
    {
        rRet = deskHeight / 1080.;
    }
    else
    {
        rRet = deskWidth / 1920.;
    }

    m_rScaleFactor = rRet;
}
