#include "c7wechatimportwidget.h"
#include <QLabel>
#include <QStackedWidget>

#include "c7wechatshowwidget.h"
#include "c72dcodewidget.h"

C7WechatImportWidget::C7WechatImportWidget(int w, int h, QWidget *parent):
    C7BaseWidget(w, h, parent)
{
    this->resize(w, h);
    initUi();
}

void C7WechatImportWidget::initUi()
{
    QPixmap pix(600, 600);
    m_2dPixmap=pix;

    m_pStackWidget = new QStackedWidget(this);
    m_pStackWidget->resize(this->width(), this->height());

    //未有用户登录的时候，二维码的显示
    m_p2dCodeWidget = new C72dCodeWidget(this->width(), this->height());
    m_p2dCodeWidget->set2dCodeImage(m_2dPixmap);
    m_p2dCodeWidget->set2dCodeSize(294*m_rScaleFactor, 294*m_rScaleFactor);
    m_pStackWidget->addWidget(m_p2dCodeWidget);

    //有用户连接后的显示窗口
    m_pWechatShowWidget = new C7WechatShowWidget(this->width(), this->height());
    connect(m_pWechatShowWidget,&C7WechatShowWidget::sigItemClick,this,&C7WechatImportWidget::sigItemClick);
    m_pStackWidget->addWidget(m_pWechatShowWidget);

    m_pStackWidget->setCurrentWidget(m_p2dCodeWidget);

    connect(m_pWechatShowWidget,&C7WechatShowWidget::sigFirstUserComing,this,[=](){
        m_pStackWidget->setCurrentIndex(1);
    });
    connect(m_pWechatShowWidget,&C7WechatShowWidget::sigLastUserLeaving,this,[=](){
        m_pStackWidget->setCurrentIndex(0);
    });
}

