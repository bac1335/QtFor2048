#include "c7wechatuserwidget.h"
#include "tlbasecontrol.h"
#include "item/c7wechatuseritem.h"

#include <QScrollArea>
#include <QScrollBar>
#include <QPropertyAnimation>
#include <QPainter>
#include "tlutility.h"


C7WechatUserWidget::C7WechatUserWidget(int w, int h, QWidget *parent):
    C7BaseWidget(w, h, parent)
{
    initUi();
}

void C7WechatUserWidget::initUi()
{
    m_pScrollArea = new QScrollArea(this);
    m_pScrollArea->resize(this->width(), this->height());
//    m_pScrollArea->move((this->width() - m_pScrollArea->width()) / 2, 80*m_rScaleFactor);
    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    m_pScrollArea->setWidgetResizable(true);
    m_pScrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
    m_pScrollArea->setAlignment(Qt::AlignHCenter);

    m_pItemTray = new TLBaseControl(this);
    m_pItemTray->resize(m_pScrollArea->width(),0);
    m_pItemTray->setMoveFlag(TLBaseControl::Flag_MoveByOutSide);
    m_pItemTray->move(m_pScrollArea->width()/2.-m_pItemTray->width()/2, 0);
    connect(m_pItemTray, SIGNAL(sigPressed()), this, SLOT(onTrayPress()));
    connect(m_pItemTray, SIGNAL(sigRelease()), this, SLOT(onTrayRelease()));
    connect(m_pItemTray, SIGNAL(sigMoveByAsk(QPoint)), this, SLOT(onTrayMoveByAsk(QPoint)));

    m_pMoveBackAnimation = new QPropertyAnimation(m_pItemTray, "pos", this);
    m_pMoveBackAnimation->setEasingCurve(QEasingCurve::Linear);
    m_pMoveBackAnimation->setTargetObject(m_pItemTray);
    m_pMoveBackAnimation->setDuration(250);

    connect(m_pMoveBackAnimation, &QPropertyAnimation::valueChanged, this,
            [=](const QVariant &value){
        m_pScrollArea->verticalScrollBar()->setSliderPosition(- m_pItemTray->y());
    });
    m_pScrollArea->setWidget(m_pItemTray);


//    m_p2dCodeItem = new C7WechatUserItem(this->width(), 90*m_rScaleFactor, m_pItemTray);
//    m_p2dCodeItem->setAvatarImage(QPixmap(m_strAppPath + "/skin/import/icon_QRcode.png"));
//    m_p2dCodeItem->move(0, 10*m_rScaleFactor);
//    for(int i = 0; i < 6; ++i) {
//        addNewUser(m_strAppPath + "/skin/import/icon_QRcode.png");
//    }

//    m_pItemTray->resize(this->width(), 10*m_rScaleFactor + 100*m_rScaleFactor*6);

}

void C7WechatUserWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(221, 224, 226)));
    painter.drawRect(this->rect());
    C7BaseWidget::paintEvent(event);
}

bool C7WechatUserWidget::isSliperOverHead() const
{
    switch (m_sliperAlignment) {
    case 0x1:{
        //存在精度问题，有一定的误差
        //Q_ASSERT(m_pItemTray->height() >= this->height());
        if(this->height() >= m_pItemTray->height()){
            return true;//当背景比滑动板长的时候，总是返回越过头了
        }else{
            //qDebug() << m_pItemTray->y() << m_pItemTray->height();
            return m_pItemTray->y() > 0;
        }
        break;
    }
    case 0x2:{
        //存在精度问题，有一定的误差
       //Q_ASSERT(m_pItemTray->width() >= this->width());
        if(this->width() >= m_pItemTray->width()){
            return true;//当背景比滑动板长的时候，总是返回越过头了
        }else{
            return m_pItemTray->x() > 0;
        }
        break;
    }
    }
    return false;
}

bool C7WechatUserWidget::isSliperOverTrail() const
{
    switch (m_sliperAlignment) {
    case 0x1:{
        //存在精度问题，有一定的误差
       // Q_ASSERT(m_pItemTray->height() >= this->height());

        if(this->height() >= m_pItemTray->height()){
            return false;//当背景比滑动板长的时候，总是不会超越尾部
        }else{
            //qDebug() << m_pItemTray->y() << m_pItemTray->height();
            return (m_pItemTray->y() + m_pItemTray->height() - this->height())< 0;
        }
        break;
    }
    case 0x2:{
        //存在精度问题，有一定的误差
       // Q_ASSERT(m_pItemTray->width() >= this->width());
        if(this->width() >= m_pItemTray->width()){
            return false;//当背景比滑动板长的时候，总是不会超越尾部
        }else{
            return (m_pItemTray->x() + m_pItemTray->width() - this->width())< 0;
        }
        break;
    }
    }
    return false;
}

C7WechatUserItem * C7WechatUserWidget::addNewUser(QString headimg)
{
    C7WechatUserItem *item = new C7WechatUserItem(this->width(), 90*m_rScaleFactor, m_pItemTray);
    connect(item,&C7WechatUserItem::sigItemClick,this,[=]()
    {
        C7WechatUserItem* item = qobject_cast<C7WechatUserItem*>(sender());
        emit sigItemClick(item);
    });

//    item->setAvatarImage(Uti->createRadiusPixmap(QSize(68*m_rScaleFactor,
//                                                       68*m_rScaleFactor),
//                                                 QPixmap(headimg)));
    item->setAvatarImage(QPixmap(headimg));
    item->move(0, 10*m_rScaleFactor + 100*m_rScaleFactor*(m_mapItem.count()));
    item->show();
    m_mapItem.push_back(item);
    relayoutItem();
    return item;
}

void C7WechatUserWidget::removeUser(C7WechatUserItem *item)
{
    m_mapItem.removeOne(item);
    if(item){
        delete item;
        item = nullptr;
    }
    relayoutItem();
}

void C7WechatUserWidget::addQrcodeItem(C7WechatUserItem *item)
{
    Q_ASSERT(item);
    if(!item){
        return;
    }
    connect(item,&C7WechatUserItem::sigItemClick,this,[=]()
    {
        C7WechatUserItem* item = qobject_cast<C7WechatUserItem*>(sender());
        emit sigItemClick(item);
    });
    m_mapItem.push_back(item);
}

void C7WechatUserWidget::setItemTop(C7WechatUserItem *item)
{
    Q_ASSERT(item);
    if(!item){
        return;
    }
    m_mapItem.removeOne(item);
    m_mapItem.insert(1, item);
    relayoutItem();
}

void C7WechatUserWidget::onTrayPress()
{
    m_currentPos = m_pItemTray->pos();
    m_currentTime = QTime::currentTime();
    m_pMoveBackAnimation->stop();
}

void C7WechatUserWidget::onTrayRelease()
{
    //回弹
    if(isSliperOverHead()){
        m_pMoveBackAnimation->setStartValue(m_pItemTray->pos());
        m_pMoveBackAnimation->setEndValue(QPoint(0, 0));
        m_pMoveBackAnimation->start();
    } else if(isSliperOverTrail()) {
        switch (sliperAlignment()) {
        case 0x1:{
            m_pMoveBackAnimation->setStartValue(m_pItemTray->pos());
            m_pMoveBackAnimation->setEndValue(QPoint(0,
                                                    m_pScrollArea->height() - m_pItemTray->height()));
        }break;
        case 0x2:{
            m_pMoveBackAnimation->setStartValue(m_pItemTray->pos());
            m_pMoveBackAnimation->setEndValue(QPoint(m_pScrollArea->width() - m_pItemTray->width(),
                                                    0));
        }break;
        default:
            return;
        }
        m_pMoveBackAnimation->start();
    } else {
        QPoint tmpRealse = m_pItemTray->pos() - m_currentPos;
        int tmpMinute = QTime::currentTime().minute() - m_currentTime.minute();
        int tmpSecond = QTime::currentTime().second() - m_currentTime.second();
        int tmpMsec = QTime::currentTime().msec() - m_currentTime.msec();
        int tmpTime = tmpMinute * 60 * 1000 + tmpSecond * 1000 + tmpMsec;
        int tmpMaxH = this->height() - m_pItemTray->height();
        int tmpMaxW = this->width() - m_pItemTray->width();
        QPoint tmpPos;
        if(sliperAlignment() == 0x1) {
            if(qAbs(tmpRealse.y()) > tmpTime*2) {
                m_pMoveBackAnimation->setStartValue(m_pItemTray->pos());
                if(tmpRealse.y() > 0) {
                    tmpPos = m_pItemTray->pos() + QPoint(0, 3*qAbs(tmpRealse.y()));
                    m_pMoveBackAnimation->setEndValue(QPoint(tmpPos.x(), tmpPos.y() < 0 ? tmpPos.y() : 0));
                } else {
                    tmpPos = m_pItemTray->pos() - QPoint(0, 3*qAbs(tmpRealse.y()));
                    m_pMoveBackAnimation->setEndValue(QPoint(tmpPos.x(), tmpPos.y() > tmpMaxH ? tmpPos.y() : tmpMaxH));
                }
                m_pMoveBackAnimation->start();
            }
        } else if(sliperAlignment() == 0x2){
            if(qAbs(tmpRealse.x()) > tmpTime) {
                m_pMoveBackAnimation->setStartValue(m_pItemTray->pos());
                if(tmpRealse.x() > 0) {
                    tmpPos = m_pItemTray->pos() + QPoint(3*qAbs(tmpRealse.x()), 0);
                    m_pMoveBackAnimation->setEndValue(QPoint(tmpPos.x() < 0 ? tmpPos.x() : 0, 0));
                } else {
                    tmpPos = m_pItemTray->pos() - QPoint(3*qAbs(tmpRealse.x()), 0);
                    m_pMoveBackAnimation->setEndValue(QPoint(tmpPos.x() > tmpMaxW ? tmpPos.x() : tmpMaxW, 0));
                }
                m_pMoveBackAnimation->start();
            }
        }
    }
}

void C7WechatUserWidget::onTrayMoveByAsk(const QPoint &pos)
{
    QPoint fixpos = pos;
    //斥力效果
    if(isSliperOverHead()){
        if(pos.x() > 0){
            fixpos.setX(pos.x()/2);
        }
        if(pos.y() > 0){
            fixpos.setY(pos.y()/2);
        }
    }
    if(isSliperOverTrail()){
        if(pos.x() < 0){
            fixpos.setX(pos.x()/2);
        }
        if(pos.y() < 0){
            fixpos.setY(pos.y()/2);
        }
    }

    QPoint p = m_pItemTray->pos() + fixpos;
    m_pItemTray->move(m_pItemTray->pos().x(), p.y());
    m_pScrollArea->verticalScrollBar()->setSliderPosition(-m_pItemTray->y());
}

void C7WechatUserWidget::relayoutItem()
{
    for (int i = 1; i < m_mapItem.size() ; ++i) {
        C7WechatUserItem * pItem = m_mapItem.at(i);
        pItem->move(0, 10*m_rScaleFactor + (i - 1)*(pItem->height() + 10*m_rScaleFactor));
    }
    m_pItemTray->resize(m_pScrollArea->width(), (m_mapItem.size() - 1)*100*m_rScaleFactor + 10*m_rScaleFactor);
}
