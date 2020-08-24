#include "c7clouditem.h"
#include <QPainter>
#include <QMouseEvent>
#include "tlutility.h"

C7CloudItem::C7CloudItem(int w, int h, QWidget *parent):
    C7BaseWidget(w, h, parent)
{
    initUi();
}

C7CloudItem::~C7CloudItem()
{
    if(m_pDownload){
        delete m_pDownload;
    }
}

void C7CloudItem::setText(const QString &strText)
{
    m_name = strText;
    m_displayText = Uti->getElideText(strText, Uti->getFont(14*m_rScaleFactor),
                                      this->width() * m_rScaleFactor,Qt::ElideRight);
    update();
}

void C7CloudItem::setImage(const QPixmap &image)
{
    m_ItemBg = image;
    update();
}

void C7CloudItem::setFileImage(const QString image)
{
    m_fileBg.load(image);
    update();
}

void C7CloudItem::setImageBg(const QString image)
{
    m_cloudBg.load(image);
}

void C7CloudItem::setProgress(int progress)
{
    if(progress > 100){progress = 100;}
    m_progress = progress;
    update();
}

void C7CloudItem::setLoadingState(bool state)
{
    m_isDownloading = state;
    update();
}

void C7CloudItem::setCheckedImage(const QPixmap &image)
{
    m_checkImage = image.scaled(26*m_rScaleFactor,26*m_rScaleFactor,
                                Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    update();
}

void C7CloudItem::setChecked(bool bChecked)
{
    m_bChecked = bChecked;
    update();
}

void C7CloudItem::sethasSelected(bool bChecked)
{
    m_bSelected = bChecked;
    update();
}

void C7CloudItem::setHasDowload(bool f)
{
    m_isDownload = f;
    update();
}

void C7CloudItem::setCheckable(bool bCheckable)
{
    m_bCheckable = bCheckable;
}

void C7CloudItem::setItemID(const ItemType &strId)
{
    m_itemId = strId;
}

TLHttpDownLoad::AddResult C7CloudItem::downloadFile(QString userName)
{
    if(!m_pDownload){
        m_pDownload = new TLHttpDownLoad();
        connect(m_pDownload,&TLHttpDownLoad::sigFinished,this,&C7CloudItem::onDownFinished);
        connect(m_pDownload,&TLHttpDownLoad::sigDownLoadPercent,this,&C7CloudItem::onDownLoadPercent);
    }

    QString name = this->getName();
    QString url = this->getFileUrl();
    QString type = this->getItemType();

    qDebug() << "--->Lynn<---" << __FUNCTION__ <<"userid =" << userName << url;

    QString str = "";
    if(m_itemId == Yun_Item)
        str = "/yun";
    else if(m_itemId == Pan_Item)
        str = "/pan";
    QString path = Uti->path(Cache) + "/yundownload/" + userName + str;

    m_pDownload->setTempDir(path);

    type == ""?type:"."+type;

    QString filePath = QString("%1/%2")
                       .arg(path)
                       .arg(name);

    QFile file(filePath);
    if(file.exists()){
        return TLHttpDownLoad::Exist;
    }

    m_downLoadUrl = url;
    m_downLoadName = name;
    TLHttpDownLoad::AddResult dat = m_pDownload->downLoad(m_downLoadUrl,m_downLoadName);
//    qDebug() << "--->lls<---" << __FUNCTION__ << "download yun file" << "filePath = " << filePath << ":" << dat;
    return dat;
}

void C7CloudItem::setEnableDownLoad(bool on)
{
    m_enableDownLoad = on;
}

void C7CloudItem::initUi()
{
    m_cloudBg.load(m_strAppPath + "/skin/import/icon_yunfiledownload.png");
    m_fileBg.load(m_strAppPath + "/skin/import/icon_yunfile.png");
    m_checkIcon.load(m_strAppPath + "/skin/button/btn_Selection.png");
    m_unCheckIcon.load(m_strAppPath + "/skin/button/btn_Unchecked.png");

    m_pixmap_backGround = QPixmap(QString(m_strAppPath + "/skin/import/icon_yunfiledownload.png"));
    m_pixmap_downLoad = QPixmap(QString(m_strAppPath + "/skin/import/btn_Downloading.png"));
    m_pixmap_stopdownload = QPixmap(QString(m_strAppPath + "/skin/import/icon_stopdownload.png"));

    m_fileItemSize = QSize(85, 85);
}

void C7CloudItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setBrush(QColor(221, 221, 222));
//    painter.drawRoundRect(QRect(0, 0, 138*m_rScaleFactor, 138*m_rScaleFactor), 8*m_rScaleFactor);
    if(m_itemId == PageYunChoiceItem || m_itemId == PagePanChoiceItem)
    {
        painter.drawPixmap(QRect(0, 0, 138*m_rScaleFactor, 138*m_rScaleFactor), m_ItemBg, m_cloudBg.rect());
    }
    else if(!m_isDownload)
    {
        if(!m_isDownloadSuspend)
            painter.drawPixmap(QRect(0, 0, 138*m_rScaleFactor, 138*m_rScaleFactor), m_cloudBg, m_cloudBg.rect());
    }
    else
    {
        painter.drawPixmap(QRect(0, 0, 138*m_rScaleFactor, 138*m_rScaleFactor), m_fileBg, m_fileBg.rect());
        if(m_bSelected)
        {
            painter.drawPixmap(QRect(100*m_rScaleFactor, 100*m_rScaleFactor, 26*m_rScaleFactor, 26*m_rScaleFactor),
                              m_checkIcon, m_checkIcon.rect());
        }
        else
        {
            painter.drawPixmap(QRect(100*m_rScaleFactor, 100*m_rScaleFactor, 26*m_rScaleFactor, 26*m_rScaleFactor),
                              m_unCheckIcon, m_unCheckIcon.rect());
        }
    }

    painter.setFont(Uti->getFont(14*m_rScaleFactor));
    painter.drawText(QRect(0, this->height() - 36*m_rScaleFactor, this->width(), 36*m_rScaleFactor),
                     Qt::AlignHCenter | Qt::AlignBottom, m_displayText);

    if(!m_isDownload){
        if(m_isDownloading){
            painter.drawPixmap(QRect(0, 0, 138*m_rScaleFactor, 138*m_rScaleFactor),m_pixmap_downLoad);
            drawPainterArc(painter);
        }
        else if(m_isDownloadSuspend){
            painter.drawPixmap(QRect(0, 0, 138*m_rScaleFactor, 138*m_rScaleFactor),m_pixmap_stopdownload);
            drawPainterArc(painter);
        }
    }else{
        QRect rect(m_offset,0,m_fileItemSize.width(),m_fileItemSize.height());
        painter.drawPixmap(rect, m_fileIcon);
    }
}

void C7CloudItem::mousePressEvent(QMouseEvent *event)
{
    if(!m_enabled)
        return;

    m_bPress = true;
    m_pressPoint = event->pos();
    m_bMove = false;
    C7BaseWidget::mousePressEvent(event);
}

void C7CloudItem::mouseMoveEvent(QMouseEvent *event)
{
    //点击item容易触发mouseMove，进行处理
    if((qAbs(event->x() - m_pressPoint.x()) >5) || (qAbs(event->y() - m_pressPoint.y()) >5))
    {
       m_bMove = true;
    }
    C7BaseWidget::mouseMoveEvent(event);
}

void C7CloudItem::mouseReleaseEvent(QMouseEvent *event)
{
    if(!m_bMove && m_bPress)
    {
         if(m_enableDownLoad && !m_isDownload)
         {
            if(m_isDownloading){   //下载中,点击暂停
                 m_isDownloading = false;
                 m_bPress = false;
                 m_isDownloadSuspend = true;
                 m_pDownload->stopDownLoad(m_downLoadUrl);
                 update();
                 return;
            }
            else if(m_isDownloadSuspend){  //暂停中,点击下载
                 m_isDownloading = true;
                 m_bPress = false;
                 m_isDownloadSuspend = false;
                 m_pDownload->downLoad(m_downLoadUrl,m_downLoadName);
                 update();
                 return;
            }
            else{  //进行下载
                m_isDownloading = true;
                m_isDownload = false;
                update();
            }
         }
         emit sigItemClick(m_itemId);
    }
    else //有滑动
    {
        C7BaseWidget::mouseReleaseEvent(event);
    }
    m_bPress = false;
}

void C7CloudItem::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    //emit sigItemDoubleClick(m_itemId);
}

void C7CloudItem::gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QRgb color)
{
    // 渐变色
    QRadialGradient gradient(0, 0, radius);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1.0, color);
    painter->setBrush(gradient);

    // << 1（左移1位）相当于radius*2 即：150*2=300
    QRectF rect(-radius, -radius, radius << 1, radius << 1);
    QPainterPath path;
    path.arcTo(rect, startAngle, angleLength);

    QPainterPath subPath;
    subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));

    // path为扇形 subPath为椭圆
    path -= subPath;

    painter->setPen(Qt::NoPen);
    painter->drawPath(path);
}

void C7CloudItem::initPanIcon(C7CloudItem *pItem, QString str)
{
    QString imgPath;

    if(str == ""){
        imgPath = m_strAppPath + "/skin/explorer/icon_unknown.png";
        pItem->setItemEanled(false);
    }
    else if (IMAGE_SUFFIX.contains(str, Qt::CaseInsensitive))
    {
        imgPath =  m_strAppPath + "/skin/explorer/icon_png.png";
    }
    else if (VIDEO_SUFFIX.contains(str, Qt::CaseInsensitive))
    {
        imgPath = m_strAppPath + "/skin/explorer/icon_video.png";
    }
    else if (AUDIO_SUFFIX.contains(str, Qt::CaseInsensitive))
    {
        imgPath = m_strAppPath + "/skin/explorer/icon_audio.png";
    }
    else if (TEXT_SUFFIX.contains(str, Qt::CaseInsensitive))
    {
        imgPath = m_strAppPath + "/skin/explorer/icon_text.png";
    }
    else if (WORD_SUFFIX.contains(str, Qt::CaseInsensitive))
    {
        imgPath = m_strAppPath + "/skin/explorer/icon_doc.png";
    }
    else if (EXCEL_SUFFIX.contains(str, Qt::CaseInsensitive))
    {
        imgPath = m_strAppPath + "/skin/explorer/icon_excel.png";
    }
    else if (POWERPOINT_SUFFIX.contains(str, Qt::CaseInsensitive))
    {
        imgPath = m_strAppPath + "/skin/explorer/icon_powerpoint.png";
    }
    else if (PDF_SUFFIX.contains(str, Qt::CaseInsensitive))
    {
        imgPath = m_strAppPath + "/skin/explorer/icon_pdf.png";
    }
    else if (YUN_SUFFIX.contains(str, Qt::CaseInsensitive))
    {
        imgPath = m_strAppPath + "/skin/explorer/icon_yunfile.png";
    }
    else{
        imgPath = m_strAppPath + "/skin/explorer/icon_unknown.png";
        pItem->setItemEanled(false);
    }

    pItem->setFileImage(imgPath);
}

void C7CloudItem::drawPainterArc(QPainter &painter)
{
    int radius = 44 * m_rScaleFactor;
    int arcHeight = 8;
    //位置
    painter.translate(this->width()/2., this->width()/2.);

    /**
         * 参数二：半径
         * 参数三：开始的角度
         * 参数四：指扫取的角度-顺时针（360度 / 8 = 45度）
         * 参数五：圆环的高度
         * 参数六：填充色
        **/
    gradientArc(&painter, radius, 0,  360, arcHeight, qRgb(255,255,255));
    int angle = m_progress*3.6;
    gradientArc(&painter, radius, 90 - angle, angle, arcHeight, qRgb(58,173,166));
}

void C7CloudItem::onDownLoadPercent(QString url, qreal percent, QString file)
{
    this->setProgress(percent *100);
    if(percent >=1){
       m_isDownload= true;
       update();
    }
}

void C7CloudItem::onDownFinished(QString filePath)
{
    initPanIcon(this,this->getItemType());
}

void C7CloudItem::onDownloadError(QString url, int type)
{

}
