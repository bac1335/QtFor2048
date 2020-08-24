#include "c7contentextitem.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFontMetrics>
#include <QTextEdit>

C7ContenTextItem::C7ContenTextItem(QWidget *parent):
    C7BaseWidget(parent), m_bChecked(false), m_nMaxWidth(440*m_rScaleFactor), m_nMinWidth(100*m_rScaleFactor)
{
    initUi();
}

void C7ContenTextItem::setText(const QString &text)
{
    m_text = text;

    QString str = text;
    QStringList list = str.split("\n");
    int width = 0;
    int height = 0;

    for(auto item : list){
        QFontMetrics metrics(m_font);
        int strWidth = metrics.width(item);
        int strHeight = metrics.height();

        if(strWidth <= m_nMaxWidth)
        {
            width = qMax(width,strWidth);
            height += strHeight;
        }
        else{
            width = m_nMaxWidth;
            height += (strWidth/m_nMaxWidth+1)*(strHeight);
        }
    }
    this->resize(width+ 80*m_rScaleFactor,height+10*m_rScaleFactor);
    m_pTextEdit->resize(width+ 60*m_rScaleFactor, this->height());
    m_pMaskWidget->resize(this->width(), this->height());
    m_pTextEdit->setText(m_text);
}

void C7ContenTextItem::setItemCheck(bool f)
{
    m_bChecked = f;
    update();
}

void C7ContenTextItem::setSelectBtnVisible(bool visible)
{
    m_bSelectVisible = visible;
    update();
}

void C7ContenTextItem::initUi()
{
    m_checkIcon.load(m_strAppPath + "/skin/button/btn_Selection.png");
    m_unCheckIcon.load(m_strAppPath + "/skin/button/btn_Unchecked.png");

    //文字显示使用textedit，方便自动换行
    m_pTextEdit = new QTextEdit(this);
//    m_pTextEdit->setAlignment(Qt::AlignCenter);
    m_pTextEdit->setFont(m_font);
    m_pTextEdit->setStyleSheet("background:rgb(0,0,0,0);border:none;text-align:center;");
    m_pTextEdit->move(20*m_rScaleFactor, 0);
    m_pTextEdit->setReadOnly(true);
    m_pTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //TextEdit鼠标点击不会触发父窗口的mousepress事件，这里在上面添加一层遮罩，通过点击遮罩触发
    m_pMaskWidget = new QWidget(this);

}

void C7ContenTextItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(this->rect(), 8*m_rScaleFactor, 8*m_rScaleFactor);
//    painter.drawText(this->rect(), Qt::AlignCenter, m_text);
    if(m_bSelectVisible){
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
    }
    C7BaseWidget::paintEvent(event);
}

void C7ContenTextItem::mousePressEvent(QMouseEvent *event)
{
    if(m_bSelectVisible){
        m_bPress = true;
        m_pressPoint = event->pos();
        m_bMove = false;
    }
    C7BaseWidget::mousePressEvent(event);

}

void C7ContenTextItem::mouseMoveEvent(QMouseEvent *event)
{
    //点击item容易触发mouseMove，进行处理
    if((qAbs(event->x() - m_pressPoint.x()) >5) || (qAbs(event->y() - m_pressPoint.y()) >5))
    {
       m_bMove = true;
    }
    C7BaseWidget::mouseMoveEvent(event);
}

void C7ContenTextItem::mouseReleaseEvent(QMouseEvent *event)
{
    if(!m_bMove && m_bPress)
    {
        m_bChecked = !m_bChecked;
        update();
        emit sigItemClick(m_bChecked,Text_Type,m_text);
    }
    else //有滑动
    {
        C7BaseWidget::mouseReleaseEvent(event);
    }
    m_bPress = false;
}
