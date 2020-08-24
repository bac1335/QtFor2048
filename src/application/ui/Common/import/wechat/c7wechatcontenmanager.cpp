#include "c7wechatcontenmanager.h"

C7WeChatContenManager::C7WeChatContenManager(int w,int h,C7BaseWidget *parent):
    C7BaseWidget (w,h,parent)
{
    m_pStackWidget = new QStackedWidget(this);
    m_pStackWidget->setFixedSize(w,h);
}

C72dCodeWidget *C7WeChatContenManager::addQrCodeWidget()
{
    C72dCodeWidget * p2dCodeWidget = new C72dCodeWidget(this->width(),this->height(),m_pStackWidget);
    m_pStackWidget->addWidget(p2dCodeWidget);
    return p2dCodeWidget;
}

C7WechatContenWidget *C7WeChatContenManager::addNewContentWidget()
{
    C7WechatContenWidget* item = new C7WechatContenWidget(m_pStackWidget->width(),
                                                          m_pStackWidget->height(),
                                                          m_pStackWidget);

    connect(item,&C7WechatContenWidget::sigItemClick,this,&C7WeChatContenManager::sigItemClick);
    m_pStackWidget->addWidget(item);
    return item;
}

void C7WeChatContenManager::showCurentItem(C7BaseWidget *item)
{
    Q_ASSERT(item!=nullptr);
    m_pStackWidget->setCurrentWidget(item);
}

void C7WeChatContenManager::removeContentWidget(C7BaseWidget *item)
{
    m_pStackWidget->removeWidget(item);
    if(item){
        delete item;
        item = nullptr;
    }
}
