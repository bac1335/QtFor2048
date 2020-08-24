#include "c7pageselectwidget.h"
#include <QScrollBar>
#include <QPainter>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include "c7pageitem.h"
#include "tlbasecontrol.h"
#include "tlutility.h"

C7PageSelectWidget::C7PageSelectWidget(int branch,QWidget *parent):
    C7BaseWidget (parent),m_nBranch(branch)
{
    initUi();
}

C7PageItem* C7PageSelectWidget::addItem()
{
    if(m_pLastItem){
        m_pLastItem->setChecked(false);
        m_pLastItem->startWaiting();
    }

    C7PageItem *item = new C7PageItem(m_nBranch,m_nPageId, m_pItemTray);
    item->resize(256*m_rScaleFactor, 147*m_rScaleFactor);
    connect(item, &C7PageItem::sigClicked, this, &C7PageSelectWidget::onItemClicked);
    connect(item, &C7PageItem::sigDestroySelf, this, &C7PageSelectWidget::onRemoveItem);
    item->startWaiting();
    m_pLastItem = item;
    m_pLastItem->setChecked(true);
    m_pLastItem->updateBgColor();; //新增页面需要进行背景更新
    m_itemList.append(item);
    m_pItemTray->setFixedHeight(m_itemList.size()*(item->height() + 10*m_rScaleFactor) - 10*m_rScaleFactor);
    item->show();
    resizeUi();
    reLayoutItem();
    m_nPageId++;
    return item;
}

void C7PageSelectWidget::setCurrentPage(int index)
{
    qDebug() << __FUNCTION__ << "song" << index << m_itemList.size();
    Q_ASSERT(index < m_itemList.size());
    if(index > (m_itemList.size() - 1)){
        return;
    }
    if(m_pLastItem){
        m_pLastItem->setChecked(false);
        m_pLastItem->startWaiting();
    }
    C7PageItem *item = m_itemList.at(index);
    item->setChecked(true);
    m_pLastItem = item;
}

void C7PageSelectWidget::updateCurrentPage()
{
    QString imgPath = m_cachePath + QString("/")
                    + QString::number(m_pLastItem->getItemId())
                    + QString("/img/thumbnail.png");

    qDebug() << "--->Lynn<---" << __FUNCTION__ << imgPath;
//    m_pLastItem->startWaiting();
    m_pLastItem->setImage(imgPath);
}

int C7PageSelectWidget::getCurrenPageId()
{
    return /*m_itemList.indexOf(m_pLastItem) + 1;*/m_pLastItem->getItemId();
}

int C7PageSelectWidget::getCurrenPage()
{
    return m_itemList.indexOf(m_pLastItem) + 1;
}

void C7PageSelectWidget::updateItem(QList<int> list,bool isOversea)
{
    foreach(C7PageItem * item,m_itemList){
        if(item == m_itemList[0])
            continue;
        item->deleteLater();
        m_itemList.removeOne(item);
    }

//    int m_currentPageId = *std::min_element(list.begin(),list.end());
    int m_currentPageId = list.first();

    m_itemList[0]->setItemId(m_currentPageId);

    //将页面重置为最大值
//    m_nPageId = *std::max_element(list.begin(),list.end());
    m_nPageId = list.last();
    m_nPageId++;

    //刷新缩略图其他页面
    for(auto& item : list){
        if(item == m_currentPageId)
            continue;
        C7PageItem * pWidget = addItem();
        pWidget->setItemId(item);
        pWidget->setChecked(false);
        pWidget->show();
        if(!isOversea){ //海外版不需要进行保存和分享页面刷新
            emit sigSimAddPage(item);
        }
    }

    m_pLastItem = m_itemList[0];
    m_pLastItem->setChecked(true);

    m_pItemTray->setFixedHeight(m_itemList.size()*(m_pLastItem->height() + 10*m_rScaleFactor) - 10*m_rScaleFactor);
    resizeUi();
    reLayoutItem();
}

void C7PageSelectWidget::updateCurrentItemBg()
{
    if(m_pLastItem){
        m_pLastItem->updateBgColor();
    }
}

void C7PageSelectWidget::onUpdateThumbnail()
{
    if(!this->isVisible()){
        return;
    }
    updateCurrentPage();

#if 1
    //检测其他页面是否需要刷新
    foreach(C7PageItem * item,m_itemList){
        QString imgPath = m_cachePath + QString("/")
                + QString::number(item->getItemId())
                + QString("/img/thumbnail.png");
        if(QFile::exists(imgPath)){
            if(item->getLoadIng()&& item->getItemId() != m_pLastItem->getItemId() ){
                 qDebug() << "--->Lynn<---" << __FUNCTION__ << imgPath;
                 item->setImage(imgPath);
             }
        }
    }
#endif
}

void C7PageSelectWidget::initUi()
{
    QGraphicsDropShadowEffect *pEffect = new QGraphicsDropShadowEffect(this);
    pEffect->setOffset(0, 0);
    pEffect->setColor(QColor(0, 0, 0, 100));
    pEffect->setBlurRadius(5);
    this->setGraphicsEffect(pEffect);

    m_pScrollArea = new QScrollArea(this);
    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    m_pScrollArea->setWidgetResizable(true);
    m_pScrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
    m_pScrollArea->setAlignment(Qt::AlignHCenter);

    m_pItemTray = new TLBaseControl(this);
    m_pItemTray->setMoveFlag(TLBaseControl::Flag_MoveByOutSide);
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

    this->resize(280*m_rScaleFactor, 0);
    addItem();

    m_cachePath = Uti->path(Cache).append("/page");
}

void C7PageSelectWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    QPainterPath path;

    if(m_nBranch == C7Version::Branch_Oversea){
        path.moveTo(this->width(), 0);
        path.lineTo(this->width(), this->height());
        path.lineTo(16*m_rScaleFactor, this->height());
        path.arcTo(QRect(0, this->height() - 32*m_rScaleFactor, 32*m_rScaleFactor, 32*m_rScaleFactor), -90, -90);
        path.lineTo(0, 16*m_rScaleFactor);
        path.arcTo(QRect(0, 0, 32*m_rScaleFactor, 32*m_rScaleFactor), 180, -90);
    }
    else if(m_nBranch == C7Version::Branch_Normal){
        path.moveTo(this->width(), this->height());
        path.lineTo(16*m_rScaleFactor, this->height());
        path.arcTo(QRect(0, this->height() - 32*m_rScaleFactor, 32*m_rScaleFactor, 32*m_rScaleFactor), -90, -90);
        path.lineTo(0, 16*m_rScaleFactor);
        path.arcTo(QRect(0, 0, 32*m_rScaleFactor, 32*m_rScaleFactor), 180, -90);
        path.lineTo(width() - 16*m_rScaleFactor, 0);
        path.arcTo(QRect(this->width()-32*m_rScaleFactor, 0, 32*m_rScaleFactor, 32*m_rScaleFactor), 90, -90);
        path.closeSubpath();
    }

    path.setFillRule(Qt::WindingFill);
    painter.drawPath(path);

    C7BaseWidget::paintEvent(event);
}

void C7PageSelectWidget::resizeEvent(QResizeEvent *event)
{
    m_pScrollArea->move(0, 16*m_rScaleFactor);
    m_pScrollArea->resize(this->width(), this->height() - 32*m_rScaleFactor);
    m_pItemTray->move(0, 0);
    m_pItemTray->setFixedWidth(this->width());
}

void C7PageSelectWidget::showEvent(QShowEvent *event)
{
    m_pLastItem->startWaiting();
}

void C7PageSelectWidget::onTrayPress()
{
    m_currentPos = m_pItemTray->pos();
    m_currentTime = QTime::currentTime();
    m_pMoveBackAnimation->stop();
}

void C7PageSelectWidget::onTrayRelease()
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

void C7PageSelectWidget::onTrayMoveByAsk(const QPoint &pos)
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

void C7PageSelectWidget::onItemClicked()
{
    C7PageItem *item = qobject_cast<C7PageItem *>(sender());
    if(m_pLastItem && m_pLastItem != item){
        m_pLastItem->setChecked(false);
    }
    m_pLastItem = item;
    emit sigUpdateCurrentPage(item->getItemId());
}

void C7PageSelectWidget::onRemoveItem()
{
    C7PageItem *item = qobject_cast<C7PageItem *>(sender());
    int removeIndex = m_itemList.indexOf(item);
    m_itemList.removeOne(item);
    //重新设置选中的item
    if(removeIndex == 0){
        if(m_itemList.isEmpty()){
//            this->hide();
            emit sigDeleteLast();
            m_pLastItem = nullptr;
        }
        else {
            m_pLastItem = m_itemList.at(0);
        }
    }
    else {
        m_pLastItem = m_itemList.at(--removeIndex);
    }
    if(m_pLastItem){
        //重置当前页面
        emit sigUpdateCurrentPage(m_pLastItem->getItemId(),true);
        m_pItemTray->setFixedHeight(m_itemList.size()*(item->height() + 10*m_rScaleFactor) - 10*m_rScaleFactor);
        m_pLastItem->setChecked(true);
        resizeUi();
        reLayoutItem();
        //清除本地缓存
        emit sigDeleteItem(item->getItemId(),m_itemList.size() == 1);
    }
    else{
        emit sigDeletelastAddItem(item->getItemId());
    }
    delete item;
    item = nullptr;
}

bool C7PageSelectWidget::isSliperOverHead() const
{
    switch (m_sliperAlignment) {
    case 0x1:{
        //存在精度问题，有一定的误差
        //Q_ASSERT(m_pItemTray->height() >= this->height());
//        qDebug() << "--->Lynn<---" << __FUNCTION__ << this->height() <<m_pItemTray->height();
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

bool C7PageSelectWidget::isSliperOverTrail() const
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

void C7PageSelectWidget::resizeUi()
{
    if(m_itemList.size() > 5){
        this->resize(this->width(), 880*m_rScaleFactor);
    }
    else {
//        qDebug() << __FUNCTION__ << "song" << m_pItemTray->height();
        this->resize(this->width(), m_pItemTray->height() + 32*m_rScaleFactor);
    }
}

void C7PageSelectWidget::reLayoutItem()
{
    for(int i = 0; i < m_itemList.size(); ++i){
        C7PageItem *item = m_itemList.at(i);
        item->move(12*m_rScaleFactor, i*(item->height() + 10*m_rScaleFactor));
//        qDebug() << __FUNCTION__ << "song" << item->pos();
    }
}
