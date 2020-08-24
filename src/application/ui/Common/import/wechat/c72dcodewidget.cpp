#include "c72dcodewidget.h"
#include <QLabel>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include "tlpushbutton.h"
#include "tlutility.h"
#include "c7application.h"
#include "tlinternet.h"
#include "c7oem.h"

C72dCodeWidget::C72dCodeWidget(int w, int h, QWidget *parent):
    C7BaseWidget(w, h, parent)
{
    initUi();
    retranslateUi();
}

void C72dCodeWidget::set2dCodeSize(int w, int h)
{
    m_p2dCodeLabel->resize(w, h);
    m_p2dCodeLabel->move(this->width()/2 - m_p2dCodeLabel->width()/2, 76*m_rScaleFactor);
    m_p2dCodeText->move(0, m_p2dCodeLabel->y() + m_p2dCodeLabel->height() + 24*m_rScaleFactor);

}

void C72dCodeWidget::set2dCodeImage(const QPixmap &pixmap)
{
    m_2dPixmap = pixmap;
    m_p2dCodeLabel->setPixmap(m_2dPixmap);
}

void C72dCodeWidget::setConnectType(ConnectType type)
{
    m_pStackWidget->setCurrentIndex((int)type);
}

void C72dCodeWidget::initUi()
{
    m_pStackWidget = new QStackedWidget(this);
    m_pStackWidget->resize(this->width(), this->height());

    //未有用户登录的时候，二维码的显示
    m_p2d = new QWidget(this);
    m_p2d->resize(this->width(), this->height());
    m_pStackWidget->addWidget(m_p2d);

    //二维码
    m_p2dCodeLabel = new QLabel(m_p2d);
    m_p2dCodeLabel->setPixmap(m_2dPixmap);
    m_p2dCodeLabel->setScaledContents(true);
    m_p2dCodeLabel->resize(294*m_rScaleFactor, 294*m_rScaleFactor);
    m_p2dCodeLabel->move(this->width()/2 - m_p2dCodeLabel->width()/2, 76*m_rScaleFactor);

    //文字
    m_p2dCodeText = new QLabel(m_p2d);
    m_p2dCodeText->setAlignment(Qt::AlignCenter);
    m_p2dCodeText->setFont(Uti->getFont(22*m_rScaleFactor));
    m_p2dCodeText->setWordWrap(true);
    m_p2dCodeText->resize(this->width(), 30*m_rScaleFactor);
    m_p2dCodeText->move(0, m_p2dCodeLabel->y() + m_p2dCodeLabel->height() + 14*m_rScaleFactor);

    //刷新窗口
    m_pFlushWidget = new QWidget(this);
    m_pFlushWidget->resize(this->width(), this->height());
    m_pStackWidget->addWidget(m_pFlushWidget);

    //刷新提示语
    m_pFlushTipsLabel = new QLabel(m_pFlushWidget);
    m_pFlushTipsLabel->setAlignment(Qt::AlignCenter);
    m_pFlushTipsLabel->setFont(Uti->getFont(22*m_rScaleFactor));
    m_pFlushTipsLabel->setWordWrap(true);
    m_pFlushTipsLabel->resize(this->width()-30*m_rScaleFactor, 50*m_rScaleFactor);
    m_pFlushTipsLabel->move(0, m_p2dCodeLabel->y() + m_p2dCodeLabel->height() + 14*m_rScaleFactor);

    m_pStackWidget->setCurrentIndex(1);

    m_pWechatCode = new C7WeChatCodeManager(this);
    connect(m_pWechatCode,&C7WeChatCodeManager::sigWechatQRCode,
            this,&C72dCodeWidget::onWechatQRCode);
    connect(m_pWechatCode,&C7WeChatCodeManager::sigNetworkError,this,[=](int code){
        m_pStackWidget->setCurrentIndex(1);
        m_pLoadingBtn->setLoadingStatus(false);
        qDebug() << "--->Lynn<---" << "C72dCodeWidget sigNetworkError" << TLHttpManager::getErrorStr(code);
//        setScanTips(TLHttpManager::getErrorStr(code));
        setScanTips(tr("The network is abnormal,please check"));
    });
    m_pWechatCode->setUrl(DEF_HTTP_WECHAT_TRANS_BIND_QRCODE_ADDR);

    m_pLoadingBtn = new TLLoadingBtn(48*m_rScaleFactor,48*m_rScaleFactor,m_pFlushWidget);
    m_pLoadingBtn->setTimeOut(20000);
    m_pLoadingBtn->move(this->width()/2-m_pLoadingBtn->width()/2.,
                        200*m_rScaleFactor);

    m_pProTips = new QLabel(this);
    m_pProTips->resize(m_pStackWidget->width(),30*m_rScaleFactor);
    m_pProTips->setFont(Uti->getFont(18*m_rScaleFactor));
    m_pProTips->move(this->width()/2.-m_pProTips->width()/2.,
                     this->height()/2.-m_pProTips->height()/2.);
    m_pProTips->setAlignment(Qt::AlignCenter);
    m_pStackWidget->addWidget(m_pProTips);


    connect(m_pLoadingBtn,&TLLoadingBtn::sigClicked,this,[=](){
        if(!cInternet->isConnected()){
//            cInternet->check();
            setScanTips(tr("The network is abnormal,please check"));
            m_pLoadingBtn->setLoadingStatus(false);
        }
        else if(!cApp->isTcpConnected()){
            cApp->reConnectTcp();
            m_pCheckNetWorkTimer->start();
        }
    });
    //超时后文字提示
    connect(m_pLoadingBtn,&TLLoadingBtn::sigTimeOut,this,[=](){
        qDebug() << "--->Lynn<---" << __FUNCTION__;
        setScanTips(tr("Timed out,Click the refresh button to update"));
    });
    m_pLoadingBtn->setLoadingIcon(QPixmap(Uti->path(Apppath) +
                                          "/skin/refresh/icon_refresh_normal.png"));

    //检查tcp连接
    m_pCheckNetWorkTimer = new QTimer(this);
    m_pCheckNetWorkTimer->setInterval(1500);
    connect(m_pCheckNetWorkTimer,&QTimer::timeout,[=](){
        if(cApp->isTcpConnected()){
            m_pWechatCode->start();
            m_pCheckNetWorkTimer->stop();
            m_pLoadingBtn->setLoadingStatus(false);
        }
    });

}

void C72dCodeWidget::retranslateUi()
{
    m_p2dCodeText->setText(tr("Please use wechat scan code to connect"));
    m_pProTips->setText(tr("Please activate to higher version!"));
}

void C72dCodeWidget::showEvent(QShowEvent *e)
{
    if(cOem->oemType() < C7OEM::OEM_Professional){
        m_pStackWidget->setCurrentIndex(2);
    }
    else {
//        cInternet->check();
        m_pStackWidget->setCurrentIndex(1);
        //必须要有tcp连接才行
        if(cApp->isTcpConnected() && cInternet->isConnected()){
            m_pWechatCode->start();
            m_pLoadingBtn->setLoadingStatus(true);
        }
        else {
            setScanTips(tr("The network is abnormal,please check"));
        }
    }
    QWidget::showEvent(e);
}

void C72dCodeWidget::hideEvent(QHideEvent *e)
{
    if(m_pLoadingBtn->isActive()){
        m_pLoadingBtn->setLoadingStatus(false);
    }
    if(m_pCheckNetWorkTimer->isActive()){
        m_pCheckNetWorkTimer->stop();
    }
    set2dCodeImage(QPixmap());
    QWidget::hideEvent(e);
}

void C72dCodeWidget::onWechatQRCode(QString codeUrl)
{
    qDebug() << "--->Lynn<---" << __FUNCTION__ << codeUrl;
    m_pLoadingBtn->setLoadingStatus(false);
    m_pStackWidget->setCurrentIndex(0);
    set2dCodeImage(QPixmap(codeUrl));
}

void C72dCodeWidget::setScanTips(QString tips)
{
    m_pFlushTipsLabel->setText(tips);
}
