#include "c7contenimageitem.h"
#include <QLabel>
#include <QMouseEvent>

C7ContenImageItem::C7ContenImageItem(QWidget *parent):
    C7BaseWidget(parent), m_nMaxWidth(500*m_rScaleFactor), m_nMaxHeigth(200*m_rScaleFactor),
    m_nMinWidth(250*m_rScaleFactor), m_nMinHeight(80*m_rScaleFactor), m_bChecked(false)
{
    initUi();
}

void C7ContenImageItem::setImage(const QString str)
{
    m_imgStr = str;
    if(m_bg.load(str)){
        adjustWidget(m_bg.width(), m_bg.height());
        m_bg = m_bg.scaled(this->width(), this->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        update();
    }
    else {
        qWarning()<<__FUNCTION__<<"load img failure..";
    }
}

void C7ContenImageItem::setItemCheck(bool f)
{
    m_bChecked = f;
    update();
}

void C7ContenImageItem::initUi()
{
    m_checkIcon.load(m_strAppPath + "/skin/button/btn_Selection.png");
    m_unCheckIcon.load(m_strAppPath + "/skin/button/btn_Unchecked.png");

}

void C7ContenImageItem::mousePressEvent(QMouseEvent *event)
{
//    m_bChecked = !m_bChecked;
//    update();
//    C7BaseWidget::mousePressEvent(event);
    m_bPress = true;
    m_pressPoint = event->pos();
    m_bMove = false;
    C7BaseWidget::mousePressEvent(event);
}

void C7ContenImageItem::mouseMoveEvent(QMouseEvent *event)
{
    //点击item容易触发mouseMove，进行处理
    if((qAbs(event->x() - m_pressPoint.x()) > 5) || (qAbs(event->y() - m_pressPoint.y()) > 5))
    {
       m_bMove = true;
    }
    C7BaseWidget::mouseMoveEvent(event);
}

void C7ContenImageItem::mouseReleaseEvent(QMouseEvent *event)
{
    if(!m_bMove && m_bPress)
    {
        m_bChecked = !m_bChecked;
        update();
        emit sigItemClick(m_bChecked,Image_Type,m_imgStr);
    }
    else //有滑动
    {
        C7BaseWidget::mouseReleaseEvent(event);
    }
    m_bPress = false;
}

void C7ContenImageItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setBrush(Qt::NoBrush);    
    painter.drawPixmap(this->rect(), m_bg);
    if(m_bChecked)
    {
        painter.drawPixmap(QRect(this->width() - 28*m_rScaleFactor, this->height() - 28*m_rScaleFactor, 22*m_rScaleFactor, 22*m_rScaleFactor),
                          m_checkIcon, m_checkIcon.rect());
    }
    else
    {
        painter.drawPixmap(QRect(this->width() - 28*m_rScaleFactor, this->height() - 28*m_rScaleFactor, 22*m_rScaleFactor, 22*m_rScaleFactor),
                          m_unCheckIcon, m_unCheckIcon.rect());
    }
    C7BaseWidget::paintEvent(event);
}

//根据图片大小调整item的大小
void C7ContenImageItem::adjustWidget(int w, int h)
{
    //小于最小限定值
    if(w <= m_nMinWidth || h <= m_nMinHeight)
    {
        //根据原图片的比例，取限定的最小高度计算出对应的宽度,若比最小宽度还小，则使用最小宽度来计算
        if(w*m_nMinHeight/h < m_nMinWidth)
        {
            //使用最小宽度来计算出对应的高度，高度比最大高度小则能直接使用，否则使用最大高度来计算
            if(m_nMinWidth*h/w > m_nMaxHeigth)
            {
                this->resize(w*m_nMaxHeigth/h, m_nMaxHeigth);
            }
            else
            {
                this->resize(m_nMinWidth, m_nMinWidth*h/w);
            }
        }//根据原图片的比例，取限定的最小宽度计算出对应的高度,若比最小高度还小，则使用最小高度来计算
        else if(m_nMinWidth*h/w < m_nMinHeight)
        {
            //使用最小高度计算出相应的宽度，比最大宽度小则能直接使用，否则使用最大宽度来计算
            if(w*m_nMinHeight/h > m_nMaxWidth)
            {
                this->resize(m_nMaxWidth, m_nMaxWidth*h/w);
            }
            else
            {
                this->resize(w*m_nMinHeight/h, m_nMinHeight);
            }
        }
    }//大小在合适范围内
    else if(w > m_nMinWidth && w <= m_nMaxWidth && h > m_nMinHeight && h <= m_nMaxHeigth)
    {
        this->resize(w, h);
    }
    else//比限定的大
    {
        //根据原图片的比例，取限定的最大高度计算出相应的宽度，如果比最大宽度大则使用最大宽度
        if(w*m_nMaxHeigth/h > m_nMaxWidth)
        {
            this->resize(m_nMaxWidth, m_nMaxWidth*h/w);
        }
        else if(m_nMaxWidth*h/w > m_nMaxHeigth)//根据原图片的比例，取限定的最大宽度计算出相应的高度，如果比最大高度大则使用最大高度
        {
            this->resize(w*m_nMaxHeigth/h, m_nMaxHeigth);
        }
    }
}

