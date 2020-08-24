#include "c7timechoice.h"
#include <QMouseEvent>
#include "Utility/tlutility.h"
#include <QDateTime>

C7TimeChoice::C7TimeChoice(int w,int h,QWidget *parent):C7BaseWidget(w,h,parent)
{
    initUi();
}

void C7TimeChoice::initPos()
{
    m_dataTime->move(this->x(),this->y() + this->height()+10 * m_rScaleFactor);
}

int C7TimeChoice::getYear()
{
     return m_dataTime->getYear();
}

int C7TimeChoice::getMonth()
{
    return m_dataTime->getMonth();
}

void C7TimeChoice::hideChoiceData()
{
    m_dataTime->hide();
}

void C7TimeChoice::initUi()
{
    m_dataTime = new C7DataTime(this->parentWidget());
    connect(m_dataTime,&C7DataTime::sigData,this,&C7TimeChoice::onData);
    m_dataTime->hide();

    m_timeChoiceBtn = new TLPushButton(this);
    m_timeChoiceBtn->setImage(m_strAppPath + "/skin/import/icon_rili.png");
    m_timeChoiceBtn->resize(24*m_rScaleFactor,24*m_rScaleFactor);
    m_timeChoiceBtn->move(this->width()-m_timeChoiceBtn->width()-10*m_rScaleFactor,
                          this->height()/2.-m_timeChoiceBtn->height()/2.);
    connect(m_timeChoiceBtn,&TLPushButton::sigRelease,this,[=](){
        emit sigTimeChoice(m_dataTime);
    });

    m_dataLabel = new QLabel(this);
    m_dataLabel->resize(this->width() - m_timeChoiceBtn->width(),this->height());
    m_dataLabel->setFont(Uti->getFont(14* m_rScaleFactor));
    m_dataLabel->setAlignment(Qt::AlignCenter);

    m_timeChoiceBtn->raise();

    onData(QDateTime::currentDateTime().date().year(),QDateTime::currentDateTime().date().month());
}

void C7TimeChoice::onData(int year, int month)
{
    m_dataLabel->setText(QString("%1-%2")
                         .arg(year)
                         .arg(month));
}

void C7TimeChoice::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);
    QPen pen = painter.pen();
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    painter.drawRoundedRect(this->rect(),this->height()/2.,this->height()/2.);

}

void C7TimeChoice::hideEvent(QHideEvent *)
{
    if(m_dataTime->isVisible())
        m_dataTime->setVisible(false);
}

C7DataTime::C7DataTime(QWidget* parent ):C7BaseWidget(parent)
{
    initUi();
}

void C7DataTime::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);
    QPen pen = painter.pen();
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    painter.drawRoundedRect(this->rect(),m_radius*2 * m_rScaleFactor,m_radius*2 * m_rScaleFactor);
}

void C7DataTime::initUi()
{

   m_font = Uti->getFont(12 * m_rScaleFactor);
   int height = 40 * m_rScaleFactor;
   m_size = QSize(40 * m_rScaleFactor,40 * m_rScaleFactor);
    for(int i = 1;i<4;++i){
        for(int j = 1;j<5;++j){
            C7DataItem* item = createBtn((i-1)*4+j);
            item->move((j-1) * m_size.width() + m_radius*m_rScaleFactor,(i-1)*m_size.height() + height);
            this->resize(j*m_size.width() + m_radius*m_rScaleFactor*2,i*m_size.height() + height);
        }
    }


    int month = QDateTime::currentDateTime().date().month();
    m_monthData = month;
    C7DataItem* t = m_itemList.at(month-1);
    if(m_itemList.contains(t)){
        t->setChecked(true);
        m_staticItem = t;
    }
    else{
        C7DataItem* t =  m_itemList.first();
        t->setChecked(true);
        m_staticItem = t;
    }

    m_yealLabel = new QLabel(this);
    m_yealLabel->resize(120 * m_rScaleFactor,height);
    m_yealLabel->setAlignment(Qt::AlignCenter);
    m_yealLabel->setFont(m_font);

    int year = QDateTime::currentDateTime().date().year();
    m_yearData = year;
    m_yealLabel->setText(QString::number(year));
    m_yealLabel->move(this->width()/2.-m_yealLabel->width()/2.,0);

    m_pLeftBtn = new TLPushButton(this);
    m_pLeftBtn->setBackColor(Qt::white);
    m_pLeftBtn->resize(height,height);
    m_pLeftBtn->setText("<");
    m_pLeftBtn->move(m_radius*m_rScaleFactor,0);
    connect(m_pLeftBtn,&TLPushButton::sigChecked,this,[=](){
         QString text = QString::number(m_yealLabel->text().toInt() - 1);
         m_yealLabel->setText(text);
         m_yearData = text.toInt();
         emit sigData(m_yearData,m_monthData);
    });

    m_pRightBtn = new TLPushButton(this);
    m_pRightBtn->setBackColor(Qt::white);
    m_pRightBtn->resize(height,height);
    m_pRightBtn->setText(">");
    m_pRightBtn->move(this->width()-m_pLeftBtn->width()-m_radius*m_rScaleFactor,0);
    connect(m_pRightBtn,&TLPushButton::sigChecked,this,[=](){
         QString text = QString::number(m_yealLabel->text().toInt() + 1);
         m_yealLabel->setText(text);
         m_yearData = text.toInt();
         emit sigData(m_yearData,m_monthData);
    });
}

C7DataItem* C7DataTime::createBtn(int num)
{
   C7DataItem* item = new C7DataItem(m_size.width(),m_size.height(),this);

   connect(item,&C7DataItem::sigChecked,this,[=](){
       C7DataItem* it = qobject_cast<C7DataItem*>(sender());
       if(m_staticItem != nullptr){
           m_staticItem->setChecked(false);
       }
       m_staticItem = it;
       m_staticItem->setChecked(true);
       m_monthData = it->getText().toInt();
       emit sigData(m_yearData,m_monthData);
   });

   connect(item,&C7DataItem::sigDoubleClicked,this,[=](){
       C7DataItem* it = qobject_cast<C7DataItem*>(sender());
       if(m_staticItem != nullptr){
           m_staticItem->setChecked(false);
       }
       m_staticItem = it;
       m_staticItem->setChecked(true);
       m_monthData = it->getText().toInt();
       emit sigData(m_yearData,m_monthData);
       this->hide();
   });

   m_itemList.push_back(item);
   item->setText(QString::number(num));
   return item;
}


C7DataItem::C7DataItem(int w,int h,QWidget *parent):C7BaseWidget(w,h,parent)
{

}

void C7DataItem::setChecked(bool checked)
{
    m_bIsChecked = checked;
    update();
}

void C7DataItem::setText(QString str)
{
    m_text = str;
    update();
}

void C7DataItem::mouseReleaseEvent(QMouseEvent *event)
{
    if(!m_bMove && m_bPress)
    {
        m_bIsChecked = !m_bIsChecked;
        emit sigChecked();

        update();
    }
    else //有滑动
    {
        C7BaseWidget::mouseReleaseEvent(event);
    }
    m_bPress = false;
}

void C7DataItem::mouseMoveEvent(QMouseEvent *event)
{
    //点击item容易触发mouseMove，进行处理
    if((qAbs(event->x() - m_pressPoint.x()) >5) || (qAbs(event->y() - m_pressPoint.y()) >5))
    {
       m_bMove = true;
    }
    C7BaseWidget::mouseMoveEvent(event);
}

void C7DataItem::mousePressEvent(QMouseEvent *event)
{
    m_bPress = true;
    m_pressPoint = event->pos();
    m_bMove = false;
    C7BaseWidget::mousePressEvent(event);
}

void C7DataItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if(m_bIsChecked)
    {
        painter.fillRect(this->rect(),QBrush(QColor("#2fbe84")));
    }
    painter.setFont(Uti->getFont(12 * m_rScaleFactor));
    painter.drawText(this->rect(),Qt::AlignCenter,m_text + QString(u8"月"));
    C7BaseWidget::paintEvent(event);
}

void C7DataItem::mouseDoubleClickEvent(QMouseEvent *)
{
    emit sigDoubleClicked();
}
