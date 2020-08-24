#include "c7direxplorer.h"

#include <QDebug>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QPropertyAnimation>
#include <QtConcurrent/QtConcurrent>
#include "base/widget/tlbasecontrol.h"
#include "c7exploreritem.h"
#include "tlutility.h"

C7DirExplorer::C7DirExplorer(SelectMode mode, QSize size, const QString &displayPath, QWidget *parent)
    : C7BaseWidget(size.width(), size.height(), parent), m_selectMode(mode), m_dispalyPath(displayPath)
{
    m_defaultScaleFactor = m_rScaleFactor;
    m_nNormalWidth = size.width();
    initUi();
}

void C7DirExplorer::setDisplayInfo(const QFileInfoList &lstFileInfo)
{
    m_lstDisplay = lstFileInfo;
    int itemCount = m_lstDisplay.size();
    if (itemCount <= 0)
    {
        m_pLblNoSelect->show();
    }
    else
    {
        m_pLblNoSelect->hide();
        countRowAndColumn(itemCount);
        diaplayItem();

        //此处修复保存图片时浏览本地文件在4K屏目录名异常问题
        freshItem();
    }
}

void C7DirExplorer::clearDisplayItem()
{
    m_pLastItem = nullptr;
    foreach (C7ExplorerItem *pItem, m_lstItem) {
        m_lstItem.removeOne(pItem);
        pItem->hide();
        pItem->deleteLater();
    }
}

QString C7DirExplorer::getDispalyPath()
{
    return m_dispalyPath;
}

void C7DirExplorer::setLastItemCheck(bool bCheck)
{
    if (m_pLastItem != nullptr)
    {
        m_pLastItem->setChecked(bCheck);
    }
}

void C7DirExplorer::refresh(const QFileInfoList &lstFileInfo)
{
    qDebug() << "--->Lynn<---" << __FUNCTION__ << lstFileInfo;
    setScaleFactor(m_defaultScaleFactor);
    clearDisplayItem();
    setDisplayInfo(lstFileInfo);
}

void C7DirExplorer::freshItem()
{
    qreal tempScale = m_rScaleFactor;       //保存原有缩放系数
    if (m_bUseAnimation)
    {
        m_rScaleFactor = this->width() * 1.0 / m_nNormalWidth*tempScale;        // 计算现在的缩放系数
    }

    relayoutItem();

    m_rScaleFactor = tempScale; //恢复原有缩放系数
}

void C7DirExplorer::setHasClicked()
{
    if (m_pLastItem != nullptr)
    {
        m_pLastItem->setChecked(false);
    }
}

void C7DirExplorer::initUi()
{
    //这里不要乘系数，后面缩放动画会乘
    m_itemSize = QSize(138, 180);
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
    m_pLblNoSelect->setStyleSheet(QString("QLabel{color: rgb(255, 0, 0); background:rgb(%1);}")
                                  .arg(ColorString_ScrollArea_Background));
    m_pLblNoSelect->setAlignment(Qt::AlignCenter);
    m_pLblNoSelect->resize(QSize(this->width(), 25 * m_rScaleFactor));
    m_pLblNoSelect->setFont(m_font);
    m_pLblNoSelect->move((m_pDisplayWidget->width() - m_pLblNoSelect->width())/2, m_nTopMargin * m_rScaleFactor);
    m_pLblNoSelect->hide();

    retranslateUi();
}

void C7DirExplorer::countRowAndColumn(int nSize)
{
    m_nRowCount = nSize / m_nOneRowCount;

    //如果刚好是倍数不用++
    if (nSize % m_nOneRowCount != 0)
    {
        ++m_nRowCount;
    }
}

void C7DirExplorer::relayoutItem()
{
    int nIndex = 0, itemCount = 0;
    itemCount = m_lstItem.count();
    C7ExplorerItem *pItem = nullptr;
    for (int i = 0; i < m_nRowCount; ++i)
    {
        for (int j = 0; j < m_nOneRowCount; ++j)
        {
            nIndex = i * m_nOneRowCount + j;
            if (nIndex < itemCount)
            {
              //  qDebug() << __FUNCTION__ << nIndex;
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

void C7DirExplorer::diaplayItem()
{
    int fileCount = m_lstDisplay.count();
    QFileInfo fileInfo;
    int nIndex = 0;
    for (int i = 0; i < m_nRowCount; ++i)
    {
        for (int j = 0; j < m_nOneRowCount; ++j)
        {
            nIndex = i * m_nOneRowCount + j;
            if (nIndex < fileCount)
            {
                fileInfo = m_lstDisplay.at(nIndex);
                C7ExplorerItem *pItem = new C7ExplorerItem(m_itemSize, m_pDisplayWidget);
                m_lstItem.append(pItem);
                pItem->setItemID(fileInfo.canonicalFilePath());

                if(fileInfo.filePath() == Uti->path(Desktop))
                {
                     pItem->setImage(m_strAppPath + "/skin/explorer/icon_desktop.png");
                }
                else if (fileInfo.isDir())
                {
                    pItem->setImage(getDirImage(fileInfo.isRoot()));
                }
                else
                {
                     pItem->setImage(getImage(fileInfo.suffix()));
                     pItem->setCheckable(true);
                }

                if (fileInfo.isRoot())
                {
                    pItem->setText(fileInfo.filePath().replace("/", "").replace(":", ""));
                }
                else
                {

                    if(fileInfo.filePath() == Uti->path(Desktop))
                    {
                         m_pItem = pItem;
                         pItem->setText(tr("desktop"));
                    }
                    else{
                        pItem->setText(fileInfo.fileName());
                    }
                }

//                pItem->setCheckedImage(QPixmap(m_strAppPath + "/skin/explorer/btn_Unchecked.png"));
                pItem->setCheckedImage(QPixmap(m_strAppPath + "/skin/explorer/btn_Selection.png"));

                pItem->move(m_nLeftMargin * m_rScaleFactor + j * (m_itemSize.width() + m_nColumnInterval) * m_rScaleFactor,
                            i * (m_itemSize.height() + m_nRowInterval) * m_rScaleFactor + m_nTopMargin*m_rScaleFactor);

                connect(pItem, &C7ExplorerItem::sigItemClick, this, &C7DirExplorer::onItemClick);
                connect(pItem, &C7ExplorerItem::sigItemDoubleClick, this, &C7DirExplorer::onItemDoubleClick);
                pItem->show();
            }
            else
            {
                break;
            }
        }
    }
//    qDebug() << "hongcs" << __FUNCTION__ << "" << m_nRowCount * (m_itemSize.height() + m_nRowInterval) * m_rScaleFactor;
    // m_nRowInterval +40 * m_rScaleFactor 为底部修正
    m_pDisplayWidget->setMinimumSize(m_pDisplayWidget->width(), m_nRowCount * (m_itemSize.height() + m_nRowInterval) * m_rScaleFactor + m_nRowInterval +40 * m_rScaleFactor);
}

QPixmap C7DirExplorer::getDirImage(bool isRoot)
{
    return QPixmap(getDirImagePath(isRoot));
}

QString C7DirExplorer::getDirImagePath(bool isRoot)
{
    if (isRoot)
    {
        return m_strAppPath + "/skin/explorer/icon_drive.png";
    }
    else
    {
        return m_strAppPath + "/skin/explorer/icon_folder.png";
    }
}

QPixmap C7DirExplorer::getImage(const QString &suffix)
{
    return QPixmap(getImagePath(suffix));
}

QString C7DirExplorer::getImagePath(const QString &suffix)
{
    if (IMAGE_SUFFIX.contains(suffix, Qt::CaseInsensitive))
    {
        return m_strAppPath + "/skin/explorer/icon_png.png";
    }
    else if (VIDEO_SUFFIX.contains(suffix, Qt::CaseInsensitive))
    {
        return m_strAppPath + "/skin/explorer/icon_video.png";
    }
    else if (AUDIO_SUFFIX.contains(suffix, Qt::CaseInsensitive))
    {
        return m_strAppPath + "/skin/explorer/icon_audio.png";
    }
    else if (TEXT_SUFFIX.contains(suffix, Qt::CaseInsensitive))
    {
        return m_strAppPath + "/skin/explorer/icon_text.png";
    }
    else if (WORD_SUFFIX.contains(suffix, Qt::CaseInsensitive))
    {
        return m_strAppPath + "/skin/explorer/icon_doc.png";
    }
    else if (EXCEL_SUFFIX.contains(suffix, Qt::CaseInsensitive))
    {
        return m_strAppPath + "/skin/explorer/icon_excel.png";
    }
    else if (POWERPOINT_SUFFIX.contains(suffix, Qt::CaseInsensitive))
    {
        return m_strAppPath + "/skin/explorer/icon_powerpoint.png";
    }
    else if (PDF_SUFFIX.contains(suffix, Qt::CaseInsensitive))
    {
        return m_strAppPath + "/skin/explorer/icon_pdf.png";
    }
    else if (YUN_SUFFIX.contains(suffix, Qt::CaseInsensitive))
    {
        return m_strAppPath + "/skin/explorer/icon_yunfile.png";
    }
    else
    {
        return QString("");
    }
}

void C7DirExplorer::onItemClick(const QString &strPath)
{
    C7ExplorerItem* item = qobject_cast<C7ExplorerItem*> (sender());
    if (m_pLastItem != nullptr)
    {
        m_pLastItem->setChecked(false);
    }

    if (item != nullptr)
    {
        item->setChecked(true);
    }

    m_pLastItem = item;

    emit sigItemClick(strPath);
}

void C7DirExplorer::onItemDoubleClick(const QString &strPath)
{
    C7ExplorerItem* item = qobject_cast<C7ExplorerItem*> (sender());
    if (m_pLastItem != nullptr)
    {
        m_pLastItem->setChecked(false);
    }

    if (item != nullptr)
    {
        item->setChecked(false);
    }

    m_pLastItem = item;

    emit sigItemDoubleClick(strPath);
}

void C7DirExplorer::onTrayPress()
{
    m_currentPos = m_pDisplayWidget->pos();
    m_currentTime = QTime::currentTime();
    m_pMoveBackAnimation->stop();
}

void C7DirExplorer::onTrayRelease()
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

void C7DirExplorer::onTrayMoveByAsk(const QPoint &pos)
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

bool C7DirExplorer::isSliperOverHead() const
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

bool C7DirExplorer::isSliperOverTrail() const
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

void C7DirExplorer::retranslateUi()
{
    if (m_selectMode != Dir_Only)
    {
        m_pLblNoSelect->setText(tr("No file"));
    }
    else
    {
        m_pLblNoSelect->setText(tr("No floder"));
    }
    if(m_pItem)
        m_pItem->setText(tr("desktop"));
}

void C7DirExplorer::resizeEvent(QResizeEvent *event)
{
    C7BaseWidget::resizeEvent(event);
}




