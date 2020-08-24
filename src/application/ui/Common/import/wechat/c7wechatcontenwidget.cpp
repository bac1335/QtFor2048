#include "c7wechatcontenwidget.h"
#include "tlbasecontrol.h"
#include "item/c7contenimageitem.h"
#include "item/c7contentextitem.h"
#include <QScrollArea>
#include <QScrollBar>
#include <QPropertyAnimation>
#include <QPainter>
#include <QTimer>

C7WechatContenWidget::C7WechatContenWidget(int w, int h, QWidget *parent):
    C7BaseWidget(w, h, parent)
{
    initUi();
}

C7WechatContenWidget::~C7WechatContenWidget()
{
    qDebug() << "--->Lynn<---" << __FUNCTION__;
}

void C7WechatContenWidget::addText(const QString str)
{
    C7ContenTextItem *textItem = new C7ContenTextItem(m_pItemTray);
    //设置一个标签，用于多选时区分每个item
    textItem->setCustomType(m_mapText.size() + m_mapImage.size());
    connect(textItem,&C7ContenTextItem::sigItemClick,this,[=](bool f,Item_Type t,QString s){
        if(f){
            WechatContent * content = new WechatContent;
            content->id = textItem->getCustomType();
            content->type = t;
            content->content = s;
            m_selectContent.append(content);
        }
        else{
            removeContents(textItem->getCustomType());
        }

        emit sigItemClick(m_selectContent.size() !=0,m_selectContent);
    });
    if(str == QString(u8"【收到不支持的消息类型，暂无法显示】")){
        textItem->setSelectBtnVisible(false);
    }
    textItem->setText(str);
    if(m_mapText.count() == 0 && m_mapImage.count() == 0)
    {
       textItem->move(42*m_rScaleFactor,20*m_rScaleFactor);
    }
    else{
       if(m_lastItemType == Text_Type)
       {
          textItem->move(42*m_rScaleFactor,20*m_rScaleFactor + m_mapText.last()->y() + m_mapText.last()->height());
       }else{
          textItem->move(42*m_rScaleFactor,20*m_rScaleFactor + m_mapImage.last()->y() + m_mapImage.last()->height());
       }
    }
    textItem->show();
    m_pItemTray->resize(this->width(), textItem->y() + textItem->height());
    m_mapText.push_back(textItem);
    m_lastItemType = Text_Type;
    if(m_pScrollArea->height() < m_pItemTray->height()){
        m_pMoveBackAnimation->setStartValue(m_pItemTray->pos());
        m_pMoveBackAnimation->setEndValue(QPoint(0, m_pScrollArea->height()-m_pItemTray->height()));
        m_pMoveBackAnimation->start();
//        m_pItemTray->move(0,m_pScrollArea->height()-m_pItemTray->height());
    }

}

void C7WechatContenWidget::addImage(const QString str)
{
    m_imgTasks.append(str);
    //每张图片进来时，为确保图片能够正常加载，每隔200ms才去加载一张
    if(!m_pTaskTimer->isActive()){
        m_pTaskTimer->start();
    }
}

void C7WechatContenWidget::setIsReaded(bool isReaded)
{
    m_isReaded = isReaded;
}

void C7WechatContenWidget::_addImgTask(QString str)
{
    C7ContenImageItem *imageItem = new C7ContenImageItem(m_pItemTray);
    //设置一个标签，用于多选时区分每个item
    imageItem->setCustomType(m_mapText.size() + m_mapImage.size());
    connect(imageItem,&C7ContenImageItem::sigItemClick,this,[=](bool f,Item_Type t,QString s){
        if(f){
            WechatContent * content = new WechatContent;
            content->id = imageItem->getCustomType();
            content->type = t;
            content->content = s;
            m_selectContent.append(content);
        }
        else{
            removeContents(imageItem->getCustomType());
        }

        emit sigItemClick(m_selectContent.size() !=0,m_selectContent);
    });

    imageItem->setImage(str);

    if(m_mapText.count() == 0 && m_mapImage.count() == 0)
    {
       imageItem->move(42*m_rScaleFactor,20*m_rScaleFactor);
    }
    else{
       if(m_lastItemType == Text_Type)
       {
          imageItem->move(42*m_rScaleFactor,20*m_rScaleFactor + m_mapText.last()->y() + m_mapText.last()->height());
       }else{
          imageItem->move(42*m_rScaleFactor,20*m_rScaleFactor + m_mapImage.last()->y() + m_mapImage.last()->height());
       }
    }
    imageItem->show();
    m_pItemTray->resize(this->width(), imageItem->y() + imageItem->height());
    m_mapImage.push_back(imageItem);
    m_lastItemType = Image_Type;
    if(m_pScrollArea->height() < m_pItemTray->height()){
        m_pMoveBackAnimation->setStartValue(m_pItemTray->pos());
        m_pMoveBackAnimation->setEndValue(QPoint(0, m_pScrollArea->height()-m_pItemTray->height()));
        m_pMoveBackAnimation->start();
//        m_pItemTray->move(0,m_pScrollArea->height()-m_pItemTray->height());
    }
}

void C7WechatContenWidget::removeContents(int id)
{
    for(auto & item:m_selectContent){
        if(item->id == id){
            delete item;
            item = nullptr;
            m_selectContent.removeOne(item);
        }
    }
}

bool C7WechatContenWidget::isCheacked()
{
    bool res = false;
    for(int i = 0;i<m_mapText.size();i++)
    {
        if(m_mapText.at(i)->getChecked())
        {
            res = true;
            break;
        }
    }
    if(res == false){
        for(int i = 0;i<m_mapImage.size();i++)
        {
            if(m_mapImage.at(i)->getChecked())
            {
                res = true;
                break;
            }
        }
    }
    return res;
}

void C7WechatContenWidget::initUi()
{
    m_pScrollArea = new QScrollArea(this);
    m_pScrollArea->resize(this->width(), this->height());
    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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

    m_pTaskTimer = new QTimer(this);
    m_pTaskTimer->setInterval(200);
    connect(m_pTaskTimer,&QTimer::timeout,this,[=](){
        if(!m_imgTasks.isEmpty()){
            _addImgTask(m_imgTasks.takeFirst());
        }
        else {
            m_pTaskTimer->stop();
        }
    });
}

bool C7WechatContenWidget::isSliperOverHead() const
{
    switch (m_sliperAlignment) {
    case 0x1:{
        //存在精度问题，有一定的误差
        //Q_ASSERT(m_pItemTray->height() >= this->height());
        if(m_pScrollArea->height() >= m_pItemTray->height()){
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

bool C7WechatContenWidget::isSliperOverTrail() const
{
    switch (m_sliperAlignment) {
    case 0x1:{
        //存在精度问题，有一定的误差
       // Q_ASSERT(m_pItemTray->height() >= this->height());

        if(m_pScrollArea->height() >= m_pItemTray->height()){
            return false;//当背景比滑动板长的时候，总是不会超越尾部
        }else{
            //qDebug() << m_pItemTray->y() << m_pItemTray->height();
            return (m_pItemTray->y() + m_pItemTray->height() - m_pScrollArea->height())< 0;
        }
        break;
    }
    case 0x2:{
        //存在精度问题，有一定的误差
       // Q_ASSERT(m_pItemTray->width() >= this->width());
        if(this->width() >= m_pItemTray->width()){
            return false;//当背景比滑动板长的时候，总是不会超越尾部
        }else{
            return (m_pItemTray->x() + m_pItemTray->width() - m_pScrollArea->width())< 0;
        }
        break;
    }
    }
    return false;
}

void C7WechatContenWidget::hideEvent(QHideEvent *e)
{
    for(int i = 0;i<m_mapText.size();i++)
    {
        m_mapText.at(i)->setItemCheck(false);
    }
    for(int i = 0;i<m_mapImage.size();i++)
    {
        m_mapImage.at(i)->setItemCheck(false);
    }
}

void C7WechatContenWidget::showEvent(QShowEvent *event)
{
    m_selectContent.clear();
    qDebug() << __FUNCTION__ << "song" << m_isReaded;
    //处理 有新消息点开时
    if(!m_isReaded && m_pScrollArea->height() < m_pItemTray->height()){
        m_pItemTray->move(0,m_pScrollArea->height()-m_pItemTray->height());
        m_isReaded = true;
    }
    C7BaseWidget::showEvent(event);
}

void C7WechatContenWidget::onTrayPress()
{
    m_currentPos = m_pItemTray->pos();
    m_currentTime = QTime::currentTime();
    m_pMoveBackAnimation->stop();
}

void C7WechatContenWidget::onTrayRelease()
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

void C7WechatContenWidget::onTrayMoveByAsk(const QPoint &pos)
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
