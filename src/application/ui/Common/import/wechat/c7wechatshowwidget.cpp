#include "c7wechatshowwidget.h"
#include "c7wechatuserwidget.h"
#include "c72dcodewidget.h"
#include "c7wechatcontenwidget.h"

#include <QStackedWidget>
#include "c7wechatcontenmanager.h"

C7WechatShowWidget::C7WechatShowWidget(int w, int h, QWidget *parent):
    C7BaseWidget(w, h, parent)
{
    initUi();
}

C7WechatUserItem * C7WechatShowWidget::addNewUser(QString headImg)
{
    C7WechatUserItem * newUser = m_pUserWidget->addNewUser(headImg);
    C7WechatContenWidget* contendWidget = m_pContentManager->addNewContentWidget();
    m_pMapUser.insert(newUser,contendWidget);
    return newUser;
}

void C7WechatShowWidget::initUi()
{
    m_pUserWidget = new C7WechatUserWidget(140*m_rScaleFactor, this->height(), this);
    connect(m_pUserWidget,&C7WechatUserWidget::sigItemClick,this,&C7WechatShowWidget::onItemClick);
    m_pUserWidget->move(0, 100*m_rScaleFactor);

    m_pContentManager = new C7WeChatContenManager(this->width() - m_pUserWidget->width(), this->height(),this);
    m_pContentManager->move(m_pUserWidget->x() + m_pUserWidget->width(), 0);
    connect(m_pContentManager,&C7WeChatContenManager::sigItemClick,this,&C7WechatShowWidget::sigItemClick);


    //添加第一个，二维码, 第一个二维码现在在这层，实现始终显示在第一个
    m_pQrcodeUser = new C7WechatUserItem(140*m_rScaleFactor, 90*m_rScaleFactor, this);
    m_pQrcodeUser->setAvatarImage(QPixmap(m_strAppPath + "/skin/import/icon_QRcode.png"));
    m_pQrcodeUser->move(0, 10*m_rScaleFactor);
    m_pQrcodeUser->raise();
    m_pUserWidget->addQrcodeItem(m_pQrcodeUser);
//    C7WechatUserItem * newUser = m_pUserWidget->addNewUser(m_strAppPath + "/skin/import/icon_QRcode.png");
    C72dCodeWidget * p2dCodeWidget = m_pContentManager->addQrCodeWidget();
    m_pMapUser.insert(m_pQrcodeUser,p2dCodeWidget);
    onItemClick(m_pQrcodeUser);

    connect(C7WeChatUserManagement::GetInstance(),&C7WeChatUserManagement::sigAddUser,
            this,&C7WechatShowWidget::onWechatAddUser);
    connect(C7WeChatUserManagement::GetInstance(),&C7WeChatUserManagement::sigRemoveUser,
            this,&C7WechatShowWidget::onWechatRemoveUser);
    connect(C7WeChatUserManagement::GetInstance(),&C7WeChatUserManagement::sigReceiveImg,
            this,&C7WechatShowWidget::onWechatReceiveImg,Qt::QueuedConnection);
    connect(C7WeChatUserManagement::GetInstance(),&C7WeChatUserManagement::sigReceiveTxt,
            this,&C7WechatShowWidget::onWechatReceiveTxt);
}

void C7WechatShowWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#dde0e2"));
    painter.drawRect(0, 0, 140*m_rScaleFactor, 10*m_rScaleFactor);//处理二维码始终显示在第一个
    C7BaseWidget::paintEvent(event);
}

void C7WechatShowWidget::onItemClick(C7WechatUserItem* item)
{
    if(item == nullptr && !m_pMapUser.contains(item))
        return;
    if(m_currentUserShow){
        m_currentUserShow->setChecked(false);
    }
    m_currentUserShow = item;
    m_currentUserShow->setChecked(true);
    m_pContentManager->showCurentItem(m_pMapUser[item]);

}

void C7WechatShowWidget::onWechatAddUser(QString openId, QString headImgPath)
{
    qDebug() << "--->Lynn<---" << __FUNCTION__ <<"openId"<<openId;
    if(m_mapOpenIdToItem.size() == 0){
        emit sigFirstUserComing();
    }
    if(!m_mapOpenIdToItem.keys().contains(openId)){
        C7WechatUserItem * item = addNewUser(headImgPath);
        m_mapOpenIdToItem.insert(openId,item);
    }
}

void C7WechatShowWidget::onWechatRemoveUser(QString openId)
{
    qDebug() << "--->Lynn<---" << __FUNCTION__ ;
    C7WechatUserItem * item = m_mapOpenIdToItem.value(openId);
    if(m_mapOpenIdToItem.size() == 0){
        emit sigLastUserLeaving();
    }else if(m_currentUserShow == item){//退出的和当前显示是同一个，则自动选中二维码扫描
        onItemClick(m_pQrcodeUser);
    }
    m_pContentManager->removeContentWidget(m_pMapUser.value(item));
    m_pUserWidget->removeUser(item);
    m_mapOpenIdToItem.remove(openId);
    m_pMapUser.remove(item);
}

void C7WechatShowWidget::onWechatReceiveImg(QString openId, QString picPath)
{
    qDebug() << "--->Lynn<---" << __FUNCTION__ << "picPath = " <<picPath;
    C7WechatUserItem * item = m_mapOpenIdToItem.value(openId);
    C7WechatContenWidget * widget = dynamic_cast<C7WechatContenWidget*>(m_pMapUser.value(item));
    if(item != m_currentUserShow){
        item->setStatusTip(true);
        m_pUserWidget->setItemTop(item);//移到第一个
    }
    widget->setIsReaded(!item->isReaded());
    widget->addImage(picPath);
}

void C7WechatShowWidget::onWechatReceiveTxt(QString openId, QString txt)
{
    qDebug() << "--->Lynn<---" << __FUNCTION__ <<"openId"<<openId << txt;
    C7WechatUserItem * item = m_mapOpenIdToItem.value(openId,nullptr);
    if(!item){
        qDebug() << "--->Lynn<---" << __FUNCTION__ << "openid is error";
        return;
    }
    C7WechatContenWidget * widget = dynamic_cast<C7WechatContenWidget*>(m_pMapUser.value(item));
    if(item != m_currentUserShow){
        item->setStatusTip(true);
        m_pUserWidget->setItemTop(item);//移到第一个
    }
    qDebug() << __FUNCTION__ << "song" << item->isReaded();
    widget->setIsReaded(!item->isReaded());
    widget->addText(txt);
}

