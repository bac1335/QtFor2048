#include "c7pageitem.h"
#include <QPainter>
#include "tlpushbutton.h"
#include "tlutility.h"
#include <QtConcurrent/QtConcurrent>
#include "c7commondef.h"

C7PageItem::C7PageItem(int branch,int id, QWidget *parent):
    C7BaseWidget (parent),m_nBranch(branch), m_nId(id)
{
    initUi();
    m_pFutureWatcher = new QFutureWatcher<void>(this);
    connect(m_pFutureWatcher, SIGNAL(finished()), this, SLOT(onFutureFinished()));
}

void C7PageItem::setImage(const QString &path)
{
    QFuture<void> Future = QtConcurrent::run([=](){
        QPixmap pixmap(path);
        m_bgPixmap = pixmap.scaled(m_itemSize.width() * 4, m_itemSize.height() * 4,
                                          Qt::IgnoreAspectRatio,
                                          Qt::FastTransformation)
                           .scaled(m_itemSize.width(), m_itemSize.height(),
                                   Qt::IgnoreAspectRatio,
                                   Qt::SmoothTransformation);
    });
    m_pFutureWatcher->setFuture(Future);
}

void C7PageItem::setChecked(bool checked)
{
    qDebug() << __FUNCTION__ << "song" << checked;
    m_bChecked = checked;
    update();
    m_pRemoveBtn->setVisible(m_bChecked);
}

void C7PageItem::startWaiting()
{
    m_loading = true;
    update();
}

void C7PageItem::setLoadIng(bool state)
{
    m_loading = state;
    update();
}

void C7PageItem::updateBgColor()
{
    int themeType = Uti->value(SETTING_GROUP,THEME).toInt();
    switch(themeType)
    {
    case 0:
        m_bgColor = QColor(92,145,124);
        break;
    case 1:
        m_bgColor = QColor("#39393c");
        break;
    case 2:
        m_bgColor = QColor("#3aaea6");
        break;
    case 3:
        m_bgColor = QColor("#6fbdc4");
        break;
    case 4:
        m_bgColor = QColor("#c7eecc");
        break;
    case 5:
        m_bgColor = QColor("#f5f5f5");
        break;
    default:
        m_bgColor= QColor("#ffffff");
        break;
    }
}

void C7PageItem::initUi()
{
    m_itemSize = QSize(250*m_rScaleFactor, 141*m_rScaleFactor);

    QString fileName;
    if(m_nBranch == C7Version::Branch_Normal){
        fileName = "internal";
    }
    else if (m_nBranch == C7Version::Branch_Oversea) {
        fileName = "oversea";
    }
    //移除按钮
    m_pRemoveBtn = new TLPushButton(this);
    m_pRemoveBtn->setNormalImage(m_strAppPath + QString("/skin/%1/pageManager/icon_delet_normal.png").arg(fileName));
    m_pRemoveBtn->setPressImage(m_strAppPath + QString("/skin/%1/pageManager/icon_delet_press.png").arg(fileName));
    m_pRemoveBtn->setHoverImage(m_strAppPath + QString("/skin/%1/pageManager/icon_delet_normal.png").arg(fileName));
    m_pRemoveBtn->resize(40*m_rScaleFactor, 40*m_rScaleFactor);
    m_pRemoveBtn->hide();
    connect(m_pRemoveBtn, &TLPushButton::sigRelease, this, &C7PageItem::sigDestroySelf);

    m_ft = Uti->getFont(26*m_rScaleFactor);
}

void C7PageItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QRect imgRect = this->rect().adjusted(6*m_rScaleFactor,6*m_rScaleFactor,-6*m_rScaleFactor,-6*m_rScaleFactor);

//    if(!m_loading){
//        painter.drawPixmap(imgRect, m_bgPixmap, m_bgPixmap.rect());
//    }else{
//        painter.fillRect(imgRect,QBrush(m_bgColor));
//    }

    if(m_bgPixmap.isNull()){
        painter.fillRect(imgRect,QBrush(m_bgColor));
    }
    else{
       painter.drawPixmap(imgRect, m_bgPixmap, m_bgPixmap.rect());
    }

    painter.setPen(QPen(Qt::gray,1));
    painter.drawRect(this->rect().adjusted(8*m_rScaleFactor,8*m_rScaleFactor,-8*m_rScaleFactor,-8*m_rScaleFactor));
    if (m_bChecked)
    {
        painter.setPen(QPen(QColor(50,186,134), 2));  //这里设计图是2个像素，但是应该是从起点对称的，所以多画2个像素
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(this->rect().adjusted(8*m_rScaleFactor,8*m_rScaleFactor,-8*m_rScaleFactor,-8*m_rScaleFactor));
    }
#ifdef QT_DEBUG
    painter.setPen(Qt::red);
    QFont font = this->font();
    font.setPixelSize(30);
    painter.setFont(font);
    painter.drawText(this->rect(),Qt::AlignCenter,QString::number(m_nId));
#endif
    if(m_loading){
        painter.setPen(Qt::white);
        QFont font = this->font();
        font.setPixelSize(18 * m_rScaleFactor);
        painter.setFont(font);
        painter.drawText(this->rect(),Qt::AlignCenter,tr("Loading..."));
    }
}

void C7PageItem::mousePressEvent(QMouseEvent *e)
{
    m_bPress = true;
    m_pressPoint = e->pos();
    m_bMove = false;
    C7BaseWidget::mousePressEvent(e);
}

void C7PageItem::mouseMoveEvent(QMouseEvent *e)
{
    //点击item容易触发mouseMove，进行处理
    if((qAbs(e->x() - m_pressPoint.x()) >5) || (qAbs(e->y() - m_pressPoint.y()) >5))
    {
       m_bMove = true;
    }
    C7BaseWidget::mouseMoveEvent(e);
}

void C7PageItem::mouseReleaseEvent(QMouseEvent *e)
{
    if(!m_bMove && m_bPress && !m_bChecked)
    {
        m_bChecked = true;
        update();
        emit sigClicked();
    }
    else //有滑动
    {
        C7BaseWidget::mouseReleaseEvent(e);
    }
    m_pRemoveBtn->setVisible(m_bChecked);
    m_bPress = false;
}

void C7PageItem::resizeEvent(QResizeEvent *event)
{
    m_pRemoveBtn->move(this->width() - m_pRemoveBtn->width() - 10*m_rScaleFactor,
                       this->height() - m_pRemoveBtn->height() - 10*m_rScaleFactor);
    C7BaseWidget::resizeEvent(event);
}

void C7PageItem::onFutureFinished()
{
    m_loading = false;
    update();
}
