#include "c7clouddirexplorer.h"
#include "networkdisk/c7networkdiskmanager.h"
#include <QDebug>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QPropertyAnimation>
#include <QtConcurrent/QtConcurrent>
#include "base/widget/tlbasecontrol.h"
#include "c7clouditem.h"
#include "c7commondef.h"
#include "Utility/tlutility.h"
#include "c7oem.h"
#include "tlinternet.h"

#define FOLDERTYPE "1"

C7CloudDirExplorer::C7CloudDirExplorer(SelectMode mode, QSize size, const QString &displayPath, QWidget *parent)
    : C7BaseWidget(size.width(), size.height(), parent), m_selectMode(mode), m_dispalyPath(displayPath)
{
    m_pHttpDisManager = new C7NetworkDiskManager(this);
    connect(m_pHttpDisManager,&C7NetworkDiskManager::sigFileList,this,&C7CloudDirExplorer::onGetFileList);
    connect(m_pHttpDisManager,&C7NetworkDiskManager::sigNetworkError,this,&C7CloudDirExplorer::sigNetworkError);
    m_defaultScaleFactor = m_rScaleFactor;
    m_nNormalWidth = size.width();
    initUi();
}

void C7CloudDirExplorer::setDisplayInfo(int size,ItemPageType type,QVariant obj)
{

    if(type == PageChoice){
       m_ItemNum = 2;
    }else{
       m_ItemNum = obj.toJsonArray().size();
    }

    if (m_ItemNum <= 0)
    {
        m_pLblNoSelect->show();
    }
    else
    {
        //检测网盘数据与本地数据差异
//        QString str = "";
//        if(type == Yun_Page)
//            str = "/yun";
//        else if(type == Pan_Page)
//            str = "/pan";
        if(type == Yun_Page)
            checkFile(Uti->path(Cache) + "/yundownload/" + QString::number(cOem->getLoginState().userid) + "/yun",obj);

        m_pLblNoSelect->hide();
        countRowAndColumn(m_ItemNum);
        diaplayItem(type,obj);
        //此处修复保存图片时浏览本地文件在4K屏目录名异常问题
        freshItem();
    }
}

void C7CloudDirExplorer::clearDisplayItem()
{
    m_pLastItem = nullptr;
    foreach (C7CloudItem *pItem, m_lstItem) {
        m_lstItem.removeOne(pItem);
        pItem->hide();
        pItem->deleteLater();
    }
}

QString C7CloudDirExplorer::getDispalyPath()
{
    return m_dispalyPath;
}

void C7CloudDirExplorer::setLastItemCheck(bool bCheck)
{
    if (m_pLastItem != nullptr)
    {
        m_pLastItem->setChecked(bCheck);
    }
}

void C7CloudDirExplorer::refresh(const QFileInfoList &lstFileInfo)
{
    setScaleFactor(m_defaultScaleFactor);
    clearDisplayItem();
//    setDisplayInfo(lstFileInfo);
}

void C7CloudDirExplorer::freshItem()
{
    qreal tempScale = m_rScaleFactor;       //保存原有缩放系数
    if (m_bUseAnimation)
    {
        m_rScaleFactor = this->width() * 1.0 / m_nNormalWidth*tempScale;        // 计算现在的缩放系数
    }

    relayoutItem();

    m_rScaleFactor = tempScale; //恢复原有缩放系数
}

void C7CloudDirExplorer::setHasClicked()
{
    if (m_pLastItem != nullptr)
    {
        m_pLastItem->setChecked(false);
    }
}

void C7CloudDirExplorer::setSearChearchSet(QVariant obj)
{
    m_pLblNoSelect->hide();
    clearDisplayItem();
    int num = obj.toJsonArray().size();
    m_pLblNoSelect->setVisible(num <= 0);
    countRowAndColumn(num);
    diaplayItem(obj);
    //此处修复保存图片时浏览本地文件在4K屏目录名异常问题
    freshItem();
}

void C7CloudDirExplorer::initUi()
{
    //这里不要乘系数，后面缩放动画会乘
    m_itemSize = QSize(138, 176);
    m_pScrollArea = new QScrollArea(this);
    m_pScrollArea->resize(this->size());

    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pScrollArea->setWidgetResizable(true);
    m_pScrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);

    m_pScrollArea->setStyleSheet(QString("QScrollArea{background-color:rgb(%1);border:none;}")
                                 .arg(ColorString_ScrollArea_Background));
    m_pScrollArea->verticalScrollBar()->setStyleSheet(
        QString("QScrollBar{background-color:rgba(0,0,0,0%); width:%3px; border:none;border-radius:%4px;} \
                QScrollBar::handle{background-color:rgb(%1); width:%3px; border-radius:%4px;} \
                QScrollBar::add-line:vertical{height:0px;} \
                QScrollBar::sub-line:vertical{height:0px;} \
                QScrollBar::sub-page:vertical{background-color:rgb(%2); border:none; \
                                              border-top-left-radius:%4px; border-top-right-radius:%4px;} \
                QScrollBar::add-page:vertical{background-color:rgb(%2); border:none; \
                                              border-bottom-left-radius:%4px; border-bottom-right-radius:%4px;}")
                .arg(Color_ScrollBar_Handle).arg(Color_ScrollBar_Background)
                .arg((int) (12 * m_rScaleFactor)).arg((int) (6 * m_rScaleFactor))
    );

    m_pDisplayWidget = new TLBaseControl(this);
    m_pDisplayWidget->resize(this->width(), 0);
    m_pDisplayWidget->setStyleSheet("background-color: rgb(255,255,255);");

    m_pDisplayWidget->setMoveFlag(TLBaseControl::Flag_MoveByOutSide);
    connect(m_pDisplayWidget, SIGNAL(sigPressed()), this, SLOT(onTrayPress()));
    connect(m_pDisplayWidget, SIGNAL(sigRelease()), this, SLOT(onTrayRelease()));
    connect(m_pDisplayWidget, SIGNAL(sigMoveByAsk(QPoint)), this, SLOT(onTrayMoveByAsk(QPoint)));
    m_pMoveBackAnimation = new QPropertyAnimation(m_pDisplayWidget, "pos", this);
    m_pMoveBackAnimation->setEasingCurve(QEasingCurve::Linear);
    m_pMoveBackAnimation->setTargetObject(m_pDisplayWidget);
    m_pMoveBackAnimation->setDuration(250);

    connect(m_pMoveBackAnimation, &QPropertyAnimation::valueChanged, this,
            [=](const QVariant &value){
        m_pScrollArea->verticalScrollBar()->setSliderPosition(- m_pDisplayWidget->y());
    });

    m_pScrollArea->setWidget(m_pDisplayWidget);

    m_pLblNoSelect = new QLabel(m_pDisplayWidget);
    m_pLblNoSelect->setStyleSheet(QString("QLabel{color: rgb(0, 0, 0); background:rgb(%1);}")
                                  .arg(ColorString_ScrollArea_Background));
    m_pLblNoSelect->setAlignment(Qt::AlignCenter);
    m_pLblNoSelect->resize(QSize(this->width(), 20 * m_rScaleFactor));
    m_pLblNoSelect->setFont(m_font);
    m_pLblNoSelect->move((m_pDisplayWidget->width() - m_pLblNoSelect->width())/2, m_nTopMargin * m_rScaleFactor);
    m_pLblNoSelect->hide();

    retranslateUi();
}

void C7CloudDirExplorer::countRowAndColumn(int nSize)
{
    m_nRowCount = nSize / m_nOneRowCount;

    //如果刚好是倍数不用++
    if (nSize % m_nOneRowCount != 0)
    {
        ++m_nRowCount;
    }
}

void C7CloudDirExplorer::relayoutItem()
{
    int nIndex = 0, itemCount = 0;
    itemCount = m_lstItem.count();
    C7CloudItem *pItem = nullptr;
    for (int i = 0; i < m_nRowCount; ++i)
    {
        for (int j = 0; j < m_nOneRowCount; ++j)
        {
            nIndex = i * m_nOneRowCount + j;
            if (nIndex < itemCount)
            {
                pItem = m_lstItem.at(nIndex);
                pItem->setScaleFactor(m_rScaleFactor);
                pItem->resize(m_itemSize * m_rScaleFactor);
                pItem->move(m_nLeftMargin * m_rScaleFactor + j * (m_itemSize.width() + m_nColumnInterval) * m_rScaleFactor,
                            i * (m_itemSize.height() + m_nRowInterval) * m_rScaleFactor
                            + m_nTopMargin*m_rScaleFactor);

            }
        }
    }
}

void C7CloudDirExplorer::diaplayItem(ItemPageType type,QVariant obj)
{
    QJsonArray array = obj.toJsonArray();

    int fileCount = m_ItemNum;
    int nIndex = 0;
    for (int i = 0; i < m_nRowCount; ++i)
    {
        for (int j = 0; j < m_nOneRowCount; ++j)
        {
            nIndex = i * m_nOneRowCount + j;
            if (nIndex < fileCount)
            {
                C7CloudItem *pItem = new C7CloudItem(m_itemSize.width(),m_itemSize.height(), m_pDisplayWidget);
                m_lstItem.append(pItem);

                if (type == PageChoice)
                {
                    if(j == 0)
                    {
                      pItem->setItemID(PageYunChoiceItem);
                      pItem->setImage(m_strAppPath + "/skin/explorer/icon_yunfiled.png");
                      pItem->setText(tr("Yun File"));
                      pItem->setItemFloder(FOLDERTYPE);
                      m_pYunItem = pItem;
                    }
                    else
                    {
                      pItem->setItemID(PagePanChoiceItem);
                      pItem->setImage(m_strAppPath + "/skin/explorer/icon_drive.png");
                      pItem->setText(tr("Network"));
                      pItem->setItemFloder(FOLDERTYPE);
                      m_pPanItem = pItem;
                    }
                }
                else if(Yun_Page == type)
                {
                    QJsonValue data = array.at(nIndex);
                    if(data.isNull())
                        return;

                    initYunData(pItem,data.toVariant());
                }
                else if(Pan_Page == type)
                {
                    QJsonValue data = array.at(nIndex);
                    if(data.isNull())
                        return;

                    initPanData(pItem,data.toVariant());
                }

                pItem->setCheckedImage(QPixmap(m_strAppPath + "/skin/explorer/btn_Selection.png"));
                pItem->move(m_nLeftMargin * m_rScaleFactor + j * (m_itemSize.width() + m_nColumnInterval) * m_rScaleFactor,
                            i * (m_itemSize.height() + m_nRowInterval) * m_rScaleFactor + m_nTopMargin*m_rScaleFactor);
                connect(pItem, &C7CloudItem::sigItemClick, this, &C7CloudDirExplorer::onItemClick);
                pItem->show();
            }
            else
            {
                break;
            }
        }
    }

    m_pDisplayWidget->setMinimumSize(m_pDisplayWidget->width(), m_nRowCount * (m_itemSize.height() + m_nRowInterval) * m_rScaleFactor + m_nRowInterval + 40 * m_rScaleFactor);
}

void C7CloudDirExplorer::diaplayItem(QVariant obj)
{
    QJsonArray array = obj.toJsonArray();
    int fileCount = obj.toJsonArray().size();
    int nIndex = 0;
    for (int i = 0; i < m_nRowCount; ++i)
    {
        for (int j = 0; j < m_nOneRowCount; ++j)
        {
            nIndex = i * m_nOneRowCount + j;
            if (nIndex < fileCount)
            {
                C7CloudItem *pItem = new C7CloudItem(m_itemSize.width(),m_itemSize.height(), m_pDisplayWidget);
                m_lstItem.append(pItem);

                QJsonValue data = array.at(nIndex);
                if(data.isNull())
                        return;

                initYunData(pItem,data.toVariant());

                pItem->setCheckedImage(QPixmap(m_strAppPath + "/skin/explorer/btn_Selection.png"));
                pItem->move(m_nLeftMargin * m_rScaleFactor + j * (m_itemSize.width() + m_nColumnInterval) * m_rScaleFactor,
                            i * (m_itemSize.height() + m_nRowInterval) * m_rScaleFactor + m_nTopMargin*m_rScaleFactor);
                connect(pItem, &C7CloudItem::sigItemClick, this, &C7CloudDirExplorer::onItemClick);
                pItem->show();
            }
            else
            {
                break;
            }
        }
    }

    m_pDisplayWidget->setMinimumSize(m_pDisplayWidget->width(), m_nRowCount * (m_itemSize.height() + m_nRowInterval) * m_rScaleFactor + m_nRowInterval +40 * m_rScaleFactor);
}


void C7CloudDirExplorer::onTrayPress()
{
    m_currentPos = m_pDisplayWidget->pos();
    m_currentTime = QTime::currentTime();
    m_pMoveBackAnimation->stop();
}

void C7CloudDirExplorer::onTrayRelease()
{
    {
        //回弹
        if(isSliperOverHead()){
            m_pMoveBackAnimation->setStartValue(m_pDisplayWidget->pos());
            m_pMoveBackAnimation->setEndValue(QPoint(0, 0));
            m_pMoveBackAnimation->start();
        } else if(isSliperOverTrail()) {
            switch (sliperAlignment()) {
            case 0x1:{
                m_pMoveBackAnimation->setStartValue(m_pDisplayWidget->pos());
                m_pMoveBackAnimation->setEndValue(QPoint(0,
                                                        this->height() - m_pDisplayWidget->height()));
            }break;
            case 0x2:{
                m_pMoveBackAnimation->setStartValue(m_pDisplayWidget->pos());
                m_pMoveBackAnimation->setEndValue(QPoint(this->width() - m_pDisplayWidget->width(),
                                                        0));
            }break;
            default:
                return;
            }
            m_pMoveBackAnimation->start();
        } else {
            QPoint tmpRealse = m_pDisplayWidget->pos() - m_currentPos;
            int tmpMinute = QTime::currentTime().minute() - m_currentTime.minute();
            int tmpSecond = QTime::currentTime().second() - m_currentTime.second();
            int tmpMsec = QTime::currentTime().msec() - m_currentTime.msec();
            int tmpTime = tmpMinute * 60 * 1000 + tmpSecond * 1000 + tmpMsec;
            int tmpMaxH = this->height() - m_pDisplayWidget->height();
            int tmpMaxW = this->width() - m_pDisplayWidget->width();
            QPoint tmpPos;
            if(sliperAlignment() == 0x1) {
                if(qAbs(tmpRealse.y()) > tmpTime*2) {
                    m_pMoveBackAnimation->setStartValue(m_pDisplayWidget->pos());
                    if(tmpRealse.y() > 0) {
                        tmpPos = m_pDisplayWidget->pos() + QPoint(0, 3*qAbs(tmpRealse.y()));
                        m_pMoveBackAnimation->setEndValue(QPoint(tmpPos.x(), tmpPos.y() < 0 ? tmpPos.y() : 0));
                    } else {
                        tmpPos = m_pDisplayWidget->pos() - QPoint(0, 3*qAbs(tmpRealse.y()));
                        m_pMoveBackAnimation->setEndValue(QPoint(tmpPos.x(), tmpPos.y() > tmpMaxH ? tmpPos.y() : tmpMaxH));
                    }
                    m_pMoveBackAnimation->start();
                }
            } else if(sliperAlignment() == 0x2){
                if(qAbs(tmpRealse.x()) > tmpTime) {
                    m_pMoveBackAnimation->setStartValue(m_pDisplayWidget->pos());
                    if(tmpRealse.x() > 0) {
                        tmpPos = m_pDisplayWidget->pos() + QPoint(3*qAbs(tmpRealse.x()), 0);
                        m_pMoveBackAnimation->setEndValue(QPoint(tmpPos.x() < 0 ? tmpPos.x() : 0, 0));
                    } else {
                        tmpPos = m_pDisplayWidget->pos() - QPoint(3*qAbs(tmpRealse.x()), 0);
                        m_pMoveBackAnimation->setEndValue(QPoint(tmpPos.x() > tmpMaxW ? tmpPos.x() : tmpMaxW, 0));
                    }
                    m_pMoveBackAnimation->start();
                }
            }
        }
    }
}

void C7CloudDirExplorer::onTrayMoveByAsk(const QPoint &pos)
{
    QPoint fixpos = pos;
    //斥力效果
    if(isSliperOverHead()){
        if(pos.x() > 0){
            fixpos.setX(pos.x()/2);
        }
        if(pos.y() > 0){
            fixpos.setY(pos.y()/2);
        }
    }
    if(isSliperOverTrail()){
        if(pos.x() < 0){
            fixpos.setX(pos.x()/2);
        }
        if(pos.y() < 0){
            fixpos.setY(pos.y()/2);
        }
    }

    QPoint p = m_pDisplayWidget->pos() + fixpos;
    m_pDisplayWidget->move(m_pDisplayWidget->pos().x(), p.y());
    m_pScrollArea->verticalScrollBar()->setSliderPosition(-m_pDisplayWidget->y());
}

void C7CloudDirExplorer::onGetFileList(QVariant obj)
{
    if(!m_pClickedItem)
        return;

    emit sigShowWaitBox(false);

    emit sigItemClick(m_pClickedItem->getItemId(),obj);
    if(m_pLastItem)
        m_pLastItem->sethasSelected(false);
}

void C7CloudDirExplorer::initPanData(C7CloudItem *pItem, QVariant value)
{
    QJsonObject data = value.toJsonValue().toObject();

    pItem->setItemID(Pan_Item);

    if(data.contains("filename")){
        QString nameStr = data.value("filename").toString();
        pItem->setText(nameStr);

        QFileInfo file(nameStr);
        pItem->setItemType(file.suffix());
    }

//    if(data.contains("type")){
//        QString type = data.value("type").toString();
//        pItem->setItemType(type);
//    }

    if(data.contains("floder_id")){
        QString nameId = data.value("floder_id").toString();
        pItem->setItemFloderID(nameId);
    }

    if(data.contains("floder")){
        QString floderId = data.value("floder").toString();
        if(floderId == FOLDERTYPE){ //1表示文件夹，2表示文件
             pItem->setItemFloder(floderId);
             pItem->setImageBg(m_strAppPath + "/skin/explorer/icon_folder.png");
        }else{
             QString path = Uti->path(Cache) + "/yundownload/" + QString::number(cOem->getLoginState().userid)+ "/pan";
             QString filePath = path+ "/" + pItem->getName();
             QFile file(filePath);
             if(file.exists()){
                 pItem->setHasDowload(true);
                 QString type = pItem->getItemType();
                 initPanIcon(pItem,type);
             }
             else{
                 pItem->setHasDowload(false);
                 pItem->setEnableDownLoad(true);
             }

        }
    }

    if(data.contains("fileurl")){
        QString url = data.value("fileurl").toString();
             pItem->setFileUrl(url);
    }

}

void C7CloudDirExplorer::initYunData(C7CloudItem *pItem, QVariant value)
{
    QJsonObject data = value.toJsonValue().toObject();

    pItem->setItemID(Yun_Item);

    if(data.contains("filename")){
        QString nameStr = data.value("filename").toString();
        pItem->setText(nameStr);
    }

    pItem->setItemType("yun");

    if(data.contains("fileurl")){
        QString url = data.value("fileurl").toString();
             pItem->setFileUrl(url);
    }

    QString path = Uti->path(Cache) + "/yundownload/" + QString::number(cOem->getLoginState().userid) + "/yun";
    QString filePath = path+ "/" + pItem->getName();
    QFile file(filePath);
    if(file.exists()){
        pItem->setHasDowload(true);
        QString type = pItem->getItemType();
        initPanIcon(pItem,type);
    }
    else{
        pItem->setHasDowload(false);
        pItem->setEnableDownLoad(true);
    }
}

void C7CloudDirExplorer::initPanIcon(C7CloudItem *pItem, QString str)
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

void C7CloudDirExplorer::checkFile(QString path,QVariant& obj)
{
    QDir dir(path);
    if(!dir.exists())
        return;

    dir.setSorting(QDir::Name | QDir::DirsFirst);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    QFileInfoList fileList = dir.entryInfoList();
    QJsonArray array = obj.toJsonArray();

    foreach(auto file, fileList){

        QString localFileName = file.fileName();
        bool fileExit = false;
        foreach(auto item,array){
             QJsonObject data = item.toObject();
             if(data.contains("filename")){
                 QString nameStr = data.value("filename").toString();
                 if(localFileName == nameStr){
                     fileExit = true;
                     break;
                 }
             }
        }

        if(!fileExit){  //删除本地不存在于网盘文件
            QFile::remove(file.absoluteFilePath());
        }

    }

}

bool C7CloudDirExplorer::isSliperOverHead() const
{
    switch (m_sliperAlignment) {
    case 0x1:{
        //存在精度问题，有一定的误差
        //Q_ASSERT(m_pDisplayWidget->height() >= this->height());
        if(this->height() >= m_pDisplayWidget->height()){
            return true;//当背景比滑动板长的时候，总是返回越过头了
        }else{
            //qDebug() << m_pDisplayWidget->y() << m_pDisplayWidget->height();
            return m_pDisplayWidget->y() > 0;
        }
    }break;
    case 0x2:{
        //存在精度问题，有一定的误差
       //Q_ASSERT(m_pDisplayWidget->width() >= this->width());
        if(this->width() >= m_pDisplayWidget->width()){
            return true;//当背景比滑动板长的时候，总是返回越过头了
        }else{
            return m_pDisplayWidget->x() > 0;
        }
    }break;
    }
    return false;
}

bool C7CloudDirExplorer::isSliperOverTrail() const
{
    switch (m_sliperAlignment) {
    case 0x1:{
        //存在精度问题，有一定的误差
       // Q_ASSERT(m_pDisplayWidget->height() >= this->height());

        if(this->height() >= m_pDisplayWidget->height()){
            return false;//当背景比滑动板长的时候，总是不会超越尾部
        }else{
            //qDebug() << m_pDisplayWidget->y() << m_pDisplayWidget->height();
            return (m_pDisplayWidget->y() + m_pDisplayWidget->height() - this->height())< 0;
        }
    }break;
    case 0x2:{
        //存在精度问题，有一定的误差
       // Q_ASSERT(m_pDisplayWidget->width() >= this->width());
        if(this->width() >= m_pDisplayWidget->width()){
            return false;//当背景比滑动板长的时候，总是不会超越尾部
        }else{
            return (m_pDisplayWidget->x() + m_pDisplayWidget->width() - this->width())< 0;
        }
    }break;
    }
    return false;
}

void C7CloudDirExplorer::retranslateUi()
{
    if (m_selectMode != Dir_Only)
    {
        m_pLblNoSelect->setText(tr("No file"));
    }
    else
    {
        m_pLblNoSelect->setText(tr("No floder"));
    }

    if(m_pYunItem){
         m_pYunItem->setText(tr("Yun File"));
    }

    if(m_pPanItem){
         m_pPanItem->setText(tr("Network"));
    }
}

void C7CloudDirExplorer::resizeEvent(QResizeEvent *event)
{
    C7BaseWidget::resizeEvent(event);
}

void C7CloudDirExplorer::onItemClick(ItemType type)
{
    C7CloudItem* item = qobject_cast<C7CloudItem*> (sender());
    m_pClickedItem = item;

    if(!cInternet->isConnected()  && !item->getHasDowloadState()){ //已经下载的文件无需报无网络异常
        if(item->getItemFloder() != FOLDERTYPE){
            item->setLoadingState(false);
        }
       emit sigNetworkError(ERROR_CODE_NETWORK);
       return;
    }

    if(item->getItemFloder() == FOLDERTYPE){
        emit sigShowWaitBox(true);

        if(type == PageYunChoiceItem){
            m_pHttpDisManager->startYun(1);
        }
        else if(type == PagePanChoiceItem){
            m_pHttpDisManager->startDisk(0);
        }
        else if(type == Pan_Item){
            m_pHttpDisManager->startDisk(item->getItemFloderID().toInt());
        }

    }
    else{
         m_pLastItem = item;
         TLHttpDownLoad::AddResult res = item->downloadFile(QString::number(cOem->getLoginState().userid));
         if(res == TLHttpDownLoad::Exist)
         {
             emit sigSelectItem(item);
         }
   }

}




