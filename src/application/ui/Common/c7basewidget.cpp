#include "c7basewidget.h"

#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QEvent>
#include "tlutility.h"

C7BaseWidget::C7BaseWidget(QWidget *parent)
    : QWidget(parent)
{
    commonSetting();
}

C7BaseWidget::C7BaseWidget(int width, int height, QWidget *parent)
    : QWidget(parent)
{
    this->resize(width, height);
    m_nNormalWidth = width;
    commonSetting();
}

void C7BaseWidget::setScaleFactor(qreal rScaleFactor)
{
    m_rScaleFactor = rScaleFactor;

    onScaleFactorChange();
}

void C7BaseWidget::setUseAnimation(bool bUse)
{
    m_bUseAnimation = bUse;
}

void C7BaseWidget::setUseScaleAnimation(bool bUse)
{
    m_bUseScaleAnimation = bUse;
    if (m_pParallelAnimation != nullptr)
    {
        if (m_bUseScaleAnimation)
        {
            initAnimation();
        }
        else
        {
            if (m_pScaleAnimation != nullptr)
            {
                m_pParallelAnimation->removeAnimation(m_pScaleAnimation);
                m_pScaleAnimation->deleteLater();
                m_pScaleAnimation = nullptr;
            }
        }
    }
}

void C7BaseWidget::setUseOpacityAnimation(bool bUse)
{
    m_bUseOpacityAnimation = bUse;

    if (m_pParallelAnimation != nullptr)
    {
        if (m_bUseOpacityAnimation)
        {
            initAnimation();
        }
        else
        {
            if (m_pOpacityAnimation != nullptr)
            {
                m_pParallelAnimation->removeAnimation(m_pOpacityAnimation);
                m_pOpacityAnimation->deleteLater();
                m_pOpacityAnimation = nullptr;
            }
        }
    }
}

void C7BaseWidget::setAnimationOrigin(C7BaseWidget::Animation_Origin origin)
{
    m_animationOrigin = origin;
}

void C7BaseWidget::setAnimationDuration(int duration)
{
    m_nDuration = duration;
}

void C7BaseWidget::showWithAnimation()
{
    this->show();
    initAnimation();

    m_pParallelAnimation->setDirection(QAbstractAnimation::Forward);
    m_pParallelAnimation->start();
}

void C7BaseWidget::hideWithAnimation()
{
    initAnimation();
    m_pParallelAnimation->setDirection(QAbstractAnimation::Backward);
    m_pParallelAnimation->start();
}

void C7BaseWidget::reInitAnimation(QRect rect)
{
    if(!m_pParallelAnimation){
        return;
    }

    if(m_bUseScaleAnimation && m_pScaleAnimation){
        if(m_pParallelAnimation->indexOfAnimation(m_pScaleAnimation) != -1){
            m_pParallelAnimation->removeAnimation(m_pScaleAnimation);
        }

        switch (m_animationOrigin) {
        case Animation_Center:
            m_pScaleAnimation->setStartValue(QRect(rect.x() + rect.width() / 2,
                                                  rect.y() + rect.height() /2, 0, 0));
            break;
        case Animation_BottomCenter:
            m_pScaleAnimation->setStartValue(QRect(rect.x() + rect.width() / 2,
                                                  rect.y() + rect.height(), 0, 0));
            break;
        case Animation_LeftTop:
            m_pScaleAnimation->setStartValue(QRect(rect.x(), rect.y(), 0, 0));
            break;
        case Animation_LeftBottom:
            m_pScaleAnimation->setStartValue(QRect(rect.x(), rect.y() + rect.height(), 0, 0));
            break;
        case Animation_RightTop:
            m_pScaleAnimation->setStartValue(QRect(rect.x() + rect.width(), rect.y(), 0, 0));
            break;
        case Animation_RightBottom:
            m_pScaleAnimation->setStartValue(QRect(rect.x() + rect.width(), rect.y() + rect.height(), 0, 0));
            break;
        default:
            m_pScaleAnimation->setStartValue(QRect(rect.x() + rect.width() / 2,
                                                  rect.y() + rect.height() /2, 0, 0));
            break;
        }

        m_pScaleAnimation->setEndValue(rect);
        m_pParallelAnimation->addAnimation(m_pScaleAnimation);
    }
}


void C7BaseWidget::onScaleFactorChange()
{
    int pixsize = 18 * m_rScaleFactor;
    m_font.setPixelSize(pixsize <= 0 ? 1 : pixsize);
}

void C7BaseWidget::showNoAnimation()
{
    m_bNoAnimatonFlag = false;
    this->show();
}

void C7BaseWidget::retranslateUi()
{
    this->setWindowTitle(tr("Natural Board"));
}

void C7BaseWidget::showEvent(QShowEvent *event)
{
    if (m_bUseAnimation && m_bNoAnimatonFlag) {
        showWithAnimation();
    }
    m_bNoAnimatonFlag = true;
    QWidget::showEvent(event);
}

void C7BaseWidget::hideEvent(QHideEvent *event)
{
    if (m_bUseAnimation) {
        hideWithAnimation();
    }

    QWidget::hideEvent(event);
}

void C7BaseWidget::commonSetting()
{
    m_strAppPath = Uti->path(Apppath);
    m_rScaleFactor = Uti->getScaleFactor();
    m_font = Uti->getFont(18 * m_rScaleFactor);

    this->setWindowFlags(Qt::FramelessWindowHint/* | Qt::WindowStaysOnTopHint*/);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

void C7BaseWidget::initAnimation()
{
    if (m_pParallelAnimation == nullptr)
    {
        m_pParallelAnimation = new QParallelAnimationGroup(this);
        connect(m_pParallelAnimation, &QPropertyAnimation::finished, this, [=]{
            if (m_pParallelAnimation->direction() == QAbstractAnimation::Backward) {
                this->hide();
                emit sigHideFinished();
            }

        });
    }

    if (m_bUseScaleAnimation && m_pScaleAnimation == nullptr) {
        m_pScaleAnimation = new QPropertyAnimation(this);

        m_pScaleAnimation->setTargetObject(this);
        m_pScaleAnimation->setPropertyName(QByteArray("geometry"));
        switch (m_animationOrigin) {
        case Animation_Center:
            m_pScaleAnimation->setStartValue(QRect(this->x() + this->width() / 2,
                                                  this->y() + this->height() /2, 0, 0));
            break;
        case Animation_BottomCenter:
            m_pScaleAnimation->setStartValue(QRect(this->x() + this->width() / 2,
                                                  this->y() + this->height(), 0, 0));
            break;
        case Animation_LeftTop:
            m_pScaleAnimation->setStartValue(QRect(this->x(), this->y(), 0, 0));
            break;
        case Animation_LeftBottom:
            m_pScaleAnimation->setStartValue(QRect(this->x(), this->y() + this->height(), 0, 0));
            break;
        case Animation_RightTop:
            m_pScaleAnimation->setStartValue(QRect(this->x() + this->width(), this->y(), 0, 0));
            break;
        case Animation_RightBottom:
            m_pScaleAnimation->setStartValue(QRect(this->x() + this->width(), this->y() + this->height(), 0, 0));
            break;
        default:
            m_pScaleAnimation->setStartValue(QRect(this->x() + this->width() / 2,
                                                  this->y() + this->height() /2, 0, 0));
            break;
        }

        m_pScaleAnimation->setEndValue(this->geometry());
        m_pScaleAnimation->setDuration(m_nDuration);
        m_pParallelAnimation->addAnimation(m_pScaleAnimation);
    }

    if (m_bUseOpacityAnimation && m_pOpacityAnimation == nullptr)
    {
        QGraphicsOpacityEffect* pEffect = new QGraphicsOpacityEffect(this);

        this->setGraphicsEffect(pEffect);
        m_pOpacityAnimation = new QPropertyAnimation(this);
        m_pOpacityAnimation->setTargetObject(pEffect);
        m_pOpacityAnimation->setPropertyName(QByteArray("opacity"));
        m_pOpacityAnimation->setStartValue(0.0);
        m_pOpacityAnimation->setEndValue(1.0);
        m_pOpacityAnimation->setDuration(m_nDuration);
        m_pParallelAnimation->addAnimation(m_pOpacityAnimation);
    }
}

void C7BaseWidget::changeEvent(QEvent *e)
{
    if(e->type() == QEvent::LanguageChange){
        retranslateUi();
    }
    QWidget::changeEvent(e);
}
