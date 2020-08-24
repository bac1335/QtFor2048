#ifndef TLGRAPHICSBUTTONINTERFACE_H
#define TLGRAPHICSBUTTONINTERFACE_H

#include "../tlbuttoninterface.h"

class TLGraphicsButtonInterface : public TLButtonInterface
{
public:
    TLGraphicsButtonInterface();

    bool setImageWarp(bool bWrap) { m_bImageWrap = bWrap; }
    void setRounded(qreal xRadius, qreal yRadius);
protected:
    void drawButtonStuff(QPainter* painter, int mode, const QRect& rect,
                         int devicePixelRatio = 1, bool bHover = false);
    void drawButtonImage(QPainter* painter, const QPixmap& currentImage,
                         const QRect& rect, int devicePixelRatio = 1);
    void drawHover(QPainter* painter, const QRect& rect, int devicePixelRatio = 1);
    void drawFixImage(QPainter* painter, const QRect& rect,
                      const QPixmap& currentImage, int devicePixelRatio = 1);

private:
    QRectF prepareRect(QRectF base, QRectF target);

private:
    bool m_bImageWrap = true; //缩放图片以便适应大小
    qreal m_xRadius = -1;
    qreal m_yRadius = -1;
};

#endif // TLGRAPHICSBUTTONINTERFACE_H
