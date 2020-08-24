#include "c7simplemessageboxwidget.h"
#include "c7commondef.h"
#include "tlutility.h"
#include "maskwidget/tlmaskwidget.h"
#include <QEventLoop>

C7SimpleMessageBoxWidget::C7SimpleMessageBoxWidget(QWidget *parent):
    C7BaseWidget(parent)
{
    initUi();
    retranslateUi();
}

void C7SimpleMessageBoxWidget::setText(QString str)
{
    m_pContentLabel->setWordWrap(true);
    m_pContentLabel->setText(str);
}

void C7SimpleMessageBoxWidget::setPixmap(const QPixmap &pix)
{
    m_icon = pix;
}

void C7SimpleMessageBoxWidget::questionWithMask(QString str,bool useMask)
{
    this->setText(str);
    QWidget* parent = this->parentWidget();
    QEventLoop loop;
    TLMaskWidget mask;
    if(parent){
        this->move((parent->width()-this->width())/2.,(parent->height()-this->height())/2.);
        QColor color;
        if(useMask)
          color.setAlpha(50);
        else
          color.setAlpha(0);
        QBrush brush(color);
        mask.setModal(true);
        mask.setBackgroundBrush(brush);
        mask.setChild(this);
        mask.show();
    }
    connect(this, &C7SimpleMessageBoxWidget::sigOkClicked,[=,&loop](){
        emit sigQuesTion();  //不能使用sigOkClicked
        loop.quit();
    });

    //重新绑定取消按键的槽函数
    disconnect(m_cancelBtn,&TLPushButton::sigRelease,this,&C7SimpleMessageBoxWidget::hideWithAnimation);
    connect(m_cancelBtn,&TLPushButton::sigRelease,&loop,&QEventLoop::quit);
    loop.exec(QEventLoop::EventLoopExec);
}

void C7SimpleMessageBoxWidget::tipsWithMask(QString str, bool useMask)
{
    this->setText(str);
    m_cancelBtn->hide();
    m_okBtn->move(this->width()/2.-m_okBtn->width()/2.,
                  this->height()-m_okBtn->height()-50 * m_rScaleFactor);
    QWidget* parent = this->parentWidget();
    QEventLoop loop;
    TLMaskWidget mask;
    if(parent){
        this->move((parent->width()-this->width())/2.,(parent->height()-this->height())/2.);
        QColor color;
        if(useMask)
          color.setAlpha(50);
        else
          color.setAlpha(0);
        QBrush brush(color);
        mask.setModal(true);
        mask.setBackgroundBrush(brush);
        mask.setChild(this);
        mask.show();
    }
    connect(this, &C7SimpleMessageBoxWidget::sigOkClicked,[=,&loop](){
        loop.quit();
    });

    loop.exec(QEventLoop::EventLoopExec);
}

void C7SimpleMessageBoxWidget::initUi()
{
    this->resize(515 * m_rScaleFactor,359 * m_rScaleFactor);

    m_cancelBtn = new TLPushButton(this);
    m_okBtn = new TLPushButton(this);
    QFont font = Uti->getFont(18 * m_rScaleFactor);
    int round = 6;

    m_cancelBtn->setFont(font);
    m_cancelBtn->setRadius(round);
    m_cancelBtn->setBorderLine(true);
    m_cancelBtn->setNormalBackColor(Color_CancelButton_Normal);
    m_cancelBtn->setHoverBackColor(Color_CancelButton_Normal);
    m_cancelBtn->setPressBackColor(Color_CancelButton_Press);
    m_cancelBtn->resize(120 * m_rScaleFactor ,50 * m_rScaleFactor);
    m_cancelBtn->move(this->width()-m_cancelBtn->width()-40 * m_rScaleFactor,
                  this->height()-m_cancelBtn->height()-50 * m_rScaleFactor);
    connect(m_cancelBtn,&TLPushButton::sigRelease,this,&C7SimpleMessageBoxWidget::hideWithAnimation);

    m_okBtn->setFont(font);
    m_okBtn->setRadius(round);
    m_okBtn->resize(120 * m_rScaleFactor,50 * m_rScaleFactor);
    m_okBtn->move(this->width()-m_okBtn->width()-60 * m_rScaleFactor,
                  this->height()-m_okBtn->height()-50 * m_rScaleFactor);
    m_okBtn->setNormalBackColor(Color_OkButton_Normal);
    m_okBtn->setHoverBackColor(Color_OkButton_Normal);
    m_okBtn->setPressBackColor(Color_OkButton_Press);
    m_okBtn->setTextColor(Color_OkText_Press);
    connect(m_okBtn,&TLPushButton::sigRelease,this,&C7SimpleMessageBoxWidget::sigOkClicked);

    m_bg.load(m_strAppPath + "/skin/bg/bg_quit.png");

    m_pContentLabel = new QLabel(this);
    m_pContentLabel->setWordWrap(true);
    m_pContentLabel->resize(this->width()* 0.8,160*m_rScaleFactor);
    m_pContentLabel->setAlignment(Qt::AlignHCenter);
    m_pContentLabel->move(this->width()/2.-m_pContentLabel->width()/2.,
                         100 * m_rScaleFactor);
    m_pContentLabel->setFont(m_font);
}

void C7SimpleMessageBoxWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);
    painter.drawPixmap(this->rect(),m_bg);

    if(!m_icon.isNull())
    {
        painter.drawPixmap(QRect((this->width()-m_icon.width())/2.,(this->height() + m_icon.height())/2. - m_icon.height() -60 * m_rScaleFactor,
                                  m_icon.width(),m_icon.height()),m_icon);
    }
}

void C7SimpleMessageBoxWidget::resizeEvent(QResizeEvent *event)
{
    qreal tempScale = m_rScaleFactor;       //保存原有缩放系数
    if (m_bUseAnimation)
        m_rScaleFactor = this->width() * 1.0 / m_nNormalWidth*tempScale;        // 计算现在的缩放系数

    m_okBtn->resize(120 * m_rScaleFactor,50 * m_rScaleFactor);
//    m_okBtn->move(this->width()-m_okBtn->width()-60 * m_rScaleFactor,
//                  this->height()-m_okBtn->height()-50 * m_rScaleFactor);

    m_cancelBtn->resize(120 * m_rScaleFactor,50 * m_rScaleFactor);
    m_cancelBtn->move(60 * m_rScaleFactor,this->height()-m_cancelBtn->height()-50 * m_rScaleFactor);

    m_rScaleFactor = tempScale; //恢复原有缩放系数
}

void C7SimpleMessageBoxWidget::retranslateUi()
{
    m_cancelBtn->setText(tr("cancel"));
    m_okBtn->setText(tr("ok"));
}

