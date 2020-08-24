#include "tlnotice.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QDesktopWidget>
#include <QFontMetrics>
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>


TLNotice::TLNotice(QWidget *parent): QWidget(parent),
    m_showTime(1300),
    m_disApearTime(1000)
{
    m_height = 40*m_nScaleFactor;
    init(parent);
    connect(m_disappearedAnimation, SIGNAL(finished()), this, SLOT(onHideAnimotionFinish()));
    this->hide();
    //如果要在渐隐过程添加其他动作，把这个槽加上。利用QVariant值
    //connect(m_disappearedAnimation,SIGNAL(valueChanged(QVariant)),this,SLOT(onAniValueChange(QVariant)));
}
TLNotice::~TLNotice()
{
    if(m_disappearedAnimation){
        delete m_disappearedAnimation;
        m_disappearedAnimation = nullptr;
    }
    if(m_widgetOpacityEffect){
        delete m_widgetOpacityEffect;
        m_widgetOpacityEffect = nullptr;
    }
}
void TLNotice::notice(QString str, QPoint pos)
{
    this->hide();
    m_noticeMessage = str;
    QFontMetrics metr(this->font());
    int msgLenght =  metr.width(m_noticeMessage) + 30*m_nScaleFactor;
    this->setFixedSize(msgLenght + 6*m_nScaleFactor, m_height);
    this->move(pos.isNull() ? QPoint(m_defaultPos.x() - this->width()/2, m_defaultPos.y() - height()/2)
                            : QPoint(pos.x() - this->width() / 2, pos.y() - height()/2));//移到pos中心
    reset();
}
void TLNotice::notice(QString str)
{
    this->hide();

    m_noticeMessage = str;
    QFontMetrics metr(this->font());
    int msgLenght =  metr.width(m_noticeMessage) + 30*m_nScaleFactor;
    this->setFixedSize(msgLenght + 6*m_nScaleFactor, m_height);
    this->move(m_defaultPos.x() - this->width() / 2, m_defaultPos.y() - height()/2);//移到中心
    //hide和show之间添加延时，防止两次notice切换的时候显示异常
    QTimer::singleShot(100, this, [=]{
        reset();
    });
}
void TLNotice::notice()
{
    this->hide();
    QFontMetrics metr(this->font());
    int msgLenght =  metr.width(m_noticeMessage) + 30*m_nScaleFactor;
    this->resize(msgLenght + 6*m_nScaleFactor,m_height);
    this->move(m_defaultPos.x() - this->width() / 2, m_defaultPos.y() - height()/2);//移到中心
    reset();
}

void TLNotice::setDisAppearedTime(int st)
{
    m_disApearTime = st;
}

int TLNotice::disAppearTime() const
{
    return m_disApearTime;
}


void TLNotice::init(QWidget *parent)
{
    m_defaultPos = (!parent) ? (qApp->desktop()->geometry().topLeft() + QPoint(QApplication::desktop()->screenGeometry().width() / 2,
                                                                               QApplication::desktop()->screenGeometry().height() / 2)) :
                               (QPoint(parent->x() + parent->width() / 2, parent->height() / 2));
    if(!parent){
        setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
        setAttribute(Qt::WA_TranslucentBackground, true);
    }

    m_fontColor = QColor(254, 254, 254);
    m_bgColor = QColor(25, 25, 25, 160);
    m_widgetOpacityEffect = new QGraphicsOpacityEffect(this);
    this->setPixelSize(20*m_nScaleFactor);
    this->setGraphicsEffect(m_widgetOpacityEffect);
    m_widgetOpacityEffect->setOpacity(1.0f);
    m_disappearedAnimation = new QPropertyAnimation(m_widgetOpacityEffect, "opacity", this);//关联透明度于此动画
    m_disappearedAnimation->setEasingCurve(QEasingCurve::Linear);
}
void TLNotice::reset()
{
    m_disappearedAnimation->stop();
    this->show();
    this->raise();          //提升窗口
    this->activateWindow(); //设置窗口为激活状态
    m_widgetOpacityEffect->setOpacity(1.0);
    m_disappearedAnimation->setStartValue((qreal)m_disApearTime/1000.f + m_showTime / 1000.0f);
    m_disappearedAnimation->setDuration(m_disApearTime + m_showTime);
    m_disappearedAnimation->setEndValue(m_disApearTime >0 ? 0:1);
    m_disappearedAnimation->start();
    update();
}
void TLNotice::setPixelSize(int w)
{
    QFont f = this->font();
    f.setPixelSize(w);
    this->setFont(f);
    update();
}

void TLNotice::setDefinedFont(QFont f)
{
    this->setFont(f);
    update();
}

void TLNotice::setNoticetHeight(int h)
{
    m_height = h;
    this->resize(this->width(), m_height);
    update();
}

void TLNotice::setDefaultPos()
{
    QWidget *parent = (QWidget *)this->parent();
    m_defaultPos = (!parent) ? (qApp->desktop()->geometry().topLeft() + QPoint(QApplication::desktop()->screenGeometry().width() / 2,
                                                                               QApplication::desktop()->screenGeometry().height() * 2 / 3)) :
                               (QPoint(parent->x() + parent->width() / 2, parent->height() * 2 / 3));
}
int TLNotice::getNoticeMessageLenght()
{
    QFontMetrics metr(this->font());
    return metr.width(m_noticeMessage);
}
void TLNotice::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPainterPath path;
    QPen p;
    p.setBrush(m_fontColor);
    painter.setRenderHint(QPainter::Antialiasing);

    path.addRoundedRect(QRectF(0, 0, this->width(), this->height()), this->height()/2., this->height()/2.);

    painter.fillPath(path, m_bgColor);
    painter.setPen(p);
    painter.setFont(this->font());
    painter.drawText(QRectF(0, 0, this->width(), this->height()), Qt::TextWordWrap | Qt::AlignCenter,
                     m_noticeMessage);
}
void TLNotice::mousePressEvent(QMouseEvent* e)
{
    if(e->pos().x() > 5){
        QWidget::mousePressEvent((e));
    }
    this->hide();
}

void TLNotice::onActiveHideAnimation()
{
    m_disappearedAnimation->start();
}

void TLNotice::onHideAnimotionFinish()
{
    emit sigFinished();
    hide();
}


//-------------
CbGraphicsNotice::CbGraphicsNotice(QGraphicsItem *parent): QGraphicsWidget(parent),
    m_showTime(1300),
    m_height(40)
{
    init(parent);
    connect(m_disappearedAnimation, SIGNAL(finished()), this, SLOT(onHideAnimotionFinish()));
    this->hide();
    //如果要在渐隐过程添加其他动作，把这个槽加上。利用QVariant值
    //connect(m_disappearedAnimation,SIGNAL(valueChanged(QVariant)),this,SLOT(onAniValueChange(QVariant)));
}
CbGraphicsNotice::~CbGraphicsNotice()
{
    if(m_disappearedAnimation){
        delete m_disappearedAnimation;
        m_disappearedAnimation = nullptr;
    }
    if(m_widgetOpacityEffect){
        delete m_widgetOpacityEffect;
        m_widgetOpacityEffect = nullptr;
    }
}
void CbGraphicsNotice::notice(QString str, QPoint pos)
{
    m_noticeMessage = str;
    QFontMetrics metr(this->font());
    int msgLenght =  metr.width(m_noticeMessage) + 30*m_nScaleFactor;
    this->resize(msgLenght + 6*m_nScaleFactor, m_height);
    this->setPos(pos.isNull() ? QPoint(m_defaultPos.x() - this->width()/2, m_defaultPos.y() - height()/2)
                              : QPoint(pos.x() - this->width() / 2, pos.y() - height()/2));//移到pos中心
    reset();
}
void CbGraphicsNotice::notice(QString str)
{
    m_noticeMessage = str;
    QFontMetrics metr(this->font());
    int msgLenght =  metr.width(m_noticeMessage) + 30*m_nScaleFactor;
    this->resize(msgLenght + 6*m_nScaleFactor, m_height);
    this->setPos(m_defaultPos.x() - this->width() / 2, m_defaultPos.y() - height()/2);//移到中心
    reset();
}
void CbGraphicsNotice::notice()
{
    QFontMetrics metr(this->font());
    int msgLenght =  metr.width(m_noticeMessage) + 30*m_nScaleFactor;
    this->resize(msgLenght + 6*m_nScaleFactor, m_height);
    this->setPos(m_defaultPos.x() - this->width() / 2, m_defaultPos.y() - height()/2);//移到中心
    reset();
}

void CbGraphicsNotice::setDisAppearedTime(int st)
{
    m_disappearedAnimation->setDuration(st);
}

void CbGraphicsNotice::init(QGraphicsItem *parent)
{
    m_defaultPos = (!parent) ? (QPoint(QApplication::desktop()->screenGeometry().width() / 2,
                                       QApplication::desktop()->screenGeometry().height() * 2 / 3)) :
                               (QPoint(parent->boundingRect().width() / 2, parent->boundingRect().width() * 2 / 3));
    m_fontColor = QColor(254, 254, 254);
    m_bgColor = QColor(25, 25, 25, 160);
    m_widgetOpacityEffect = new QGraphicsOpacityEffect(this);
    this->setPixelSize(20*m_nScaleFactor);
    this->setGraphicsEffect(m_widgetOpacityEffect);
    m_widgetOpacityEffect->setOpacity(1.0f);
    m_disappearedAnimation = new QPropertyAnimation(m_widgetOpacityEffect, "opacity", this);//关联透明度于此动画
    m_disappearedAnimation->setEasingCurve(QEasingCurve::Linear);
}
void CbGraphicsNotice::reset()
{
    m_disappearedAnimation->stop();
    this->show();

    m_widgetOpacityEffect->setOpacity(1.0);
    m_disappearedAnimation->setStartValue(1.0f + m_showTime / 1000.0f);
    m_disappearedAnimation->setDuration(1000 + m_showTime);
    m_disappearedAnimation->setEndValue(0.0f);
    m_disappearedAnimation->start();
    update();
}
void CbGraphicsNotice::setPixelSize(int w)
{
    QFont f = this->font();
    f.setPixelSize(w);
    this->setFont(f);
    update();
}
void CbGraphicsNotice::setNoticetHeight(int h)
{
    m_height = h;
    this->resize(size().width(), m_height);
    update();
}

void CbGraphicsNotice::setBackgroupColor(const QColor &color){
    m_bgColor = color;
    update();
}

int CbGraphicsNotice::width(){
    return size().width();
}

int CbGraphicsNotice::height(){
    return size().height();
}

void CbGraphicsNotice::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath path;
    QPen p;
    p.setBrush(m_fontColor);
    painter->setRenderHint(QPainter::Antialiasing);
    if(m_backgroupStype == Style_Ellipse){
        path.addRoundedRect(boundingRect(), m_height/2., m_height/2.);

    }else if(m_backgroupStype == Style_Rect){
        path.addRoundedRect(boundingRect(), 2*m_nScaleFactor, 2*m_nScaleFactor);
    }
    painter->fillPath(path, m_bgColor);

    p.setBrush(m_fontColor);
    painter->setPen(p);
    painter->setFont(this->font());
    painter->drawText(boundingRect(), Qt::TextWordWrap | Qt::AlignCenter, m_noticeMessage);
}

int CbGraphicsNotice::getNoticeMessageLenght()
{
    QFontMetrics metr(this->font());
    return metr.width(m_noticeMessage);
}

void CbGraphicsNotice::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    e->accept();
    this->hide();
}

void CbGraphicsNotice::onActiveHideAnimation()
{
    m_disappearedAnimation->start();
}

void CbGraphicsNotice::onHideAnimotionFinish()
{
    emit sigFinished();
    hide();
}

