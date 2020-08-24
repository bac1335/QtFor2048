#include "tlgraphicsbuttoninterface.h"

#include <QPainter>

#include "../tlcontrolinterface.h"

TLGraphicsButtonInterface::TLGraphicsButtonInterface()
{

}

void TLGraphicsButtonInterface::setRounded(qreal xRadius, qreal yRadius)
{
    m_xRadius = xRadius;
    m_yRadius = yRadius;
}

/**
 * @brief QtButtonInterface::drawButtonStuff
    画背景颜色+按下颜色
 */
void TLGraphicsButtonInterface::drawButtonStuff(QPainter *painter, int mode, const QRect &rect, int devicePixelRatio, bool bHover)
{
    switch (mode) {
    case TLControlInterface::Mode_Click:
        if (!m_pressImage.isNull()) {
            drawFixImage(painter, rect, m_pressImage, devicePixelRatio);
            break;//不需要画背景
        } else {
            if(m_pressBackColor.isValid()){
                if(m_xRadius > 0 && m_yRadius > 0) {
                    QPainterPath path;
                    path.addRoundedRect(rect, m_xRadius, m_yRadius);
                    painter->fillPath(path, m_pressBackColor);
                } else {
                    painter->fillRect(rect, m_pressBackColor);
                }
                break;//不需要画背景
            }
        }
    default:
        if(bHover) {
            drawHover(painter, rect, devicePixelRatio);
        } else {
            if (!m_normalImage.isNull()) {
                drawFixImage(painter, rect, m_normalImage, devicePixelRatio);
            } else {
                if(m_normalBackColor.isValid()){
                    if(m_xRadius > 0 && m_yRadius > 0) {
                        QPainterPath path;
                        path.addRoundedRect(rect, m_xRadius, m_yRadius);
                        painter->fillPath(path, m_normalBackColor);
                    } else {
                        painter->fillRect(rect, m_normalBackColor);
                    }
                }
            }
        }
    }
}

void TLGraphicsButtonInterface::drawButtonImage(QPainter *painter, const QPixmap &currentImage, const QRect &rect, int devicePixelRatio)
{
    drawFixImage(painter, rect, currentImage, devicePixelRatio);
}

/**
 * @brief QtButtonInterface::drawHover
   当触发悬浮的时候可以调用该事件画出悬浮颜色
 */
void TLGraphicsButtonInterface::drawHover(QPainter *painter, const QRect &rect, int devicePixelRatio)
{
    if (!m_hoverImage.isNull()) {
        drawFixImage(painter, rect, m_hoverImage, devicePixelRatio);
    } else {
        if (m_hoverBackColor.isValid()) {
            if(m_xRadius >0 && m_yRadius>0) {
                QPainterPath path;
                path.addRoundedRect(rect, m_xRadius, m_yRadius);
                painter->fillPath(path, m_hoverBackColor);
            } else {
                painter->fillRect(rect, m_hoverBackColor);
            }
        }
        else {
            m_hoverImage = m_normalImage;
            drawFixImage(painter, rect, m_hoverImage, devicePixelRatio);
        }
    }
}

/**
 * @brief TLGraphicsButtonInterface::drawFixImage
 * 根据m_bImageWrap调整图片显示大小，当m_bImageWrap为假，不进行调节；当m_bImageWrap为真，进行缩放居中处理
 * @param painter
 * @param rect
 * @param image
 * @param devicepixelratio
 */
void TLGraphicsButtonInterface::drawFixImage(QPainter *painter, const QRect &rect, const QPixmap &currentImage, int devicePixelRatio)
{
    if (currentImage.isNull()) {
        return;
    }

    if (m_bImageWrap) {
        QRectF fRect = prepareRect(rect, QRectF(0, 0, currentImage.width(), currentImage.height()));
        QPixmap temp = currentImage.scaled(fRect.width() * devicePixelRatio,
                                   fRect.height() * devicePixelRatio,
                                   Qt::KeepAspectRatio,
                                   Qt::SmoothTransformation);
        temp.setDevicePixelRatio(devicePixelRatio);
        painter->drawPixmap(fRect.topLeft(), temp);
    } else {
        painter->drawPixmap(rect, currentImage);
    }
}

/**
 * @brief： prepareRect
 * @param： base 背景的矩形,显示区域
 * @param： target 要调整的矩形
 * @return： QRectF 返回要在!suitR上显示的区域
 */
QRectF TLGraphicsButtonInterface::prepareRect(QRectF base, QRectF target)
{
    bool bw = (target.width()/base.width()) > (target.height()/base.height());

    bool bscare = base.width()<target.width() || base.height()<target.height();

    if(bscare){
        //图像缩放.
        int truesizeW = base.width();
        int truesizeH = base.height();
        int x_,y_;
        if(bw){
            qreal mscalepercent =  (qreal)base.width()/target.width();
            truesizeH = target.height()*mscalepercent;//缩放比例
            x_ = 0;
            y_ = base.height()/2-truesizeH/2;
        }else{
            qreal mscalepercent =  (qreal)base.height()/target.height();
            truesizeW= target.width()*mscalepercent;//缩放比例
            y_ = 0;
            x_ = base.width()/2 - truesizeW/2;
        }
        return QRectF(x_,y_,truesizeW,truesizeH);
    }else{
        //否则画原图，找中心点
        return QRectF(base.width()/2-target.width()/2, base.height()/2-target.height()/2,target.width(),target.height());
    }
}
