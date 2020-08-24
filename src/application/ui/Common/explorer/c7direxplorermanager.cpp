#include "c7direxplorermanager.h"

#include <QDir>
#include <QDebug>
#include <QPropertyAnimation>
#include <QPushButton>
#include "c7direxplorer.h"
#include "tlutility.h"
#include "tlpushbutton.h"


C7DirExplorerManager::C7DirExplorerManager(SelectMode mode, QSize size, QWidget *parent)
    : C7BaseWidget(size.width(), size.height(), parent), m_selectMode(mode)
{
    m_pBackBtn = new C7ExplorerBackButton(this->width() - 30*m_rScaleFactor, 45*m_rScaleFactor, this);
    m_pBackBtn->setBtnVisible(false);
    connect(m_pBackBtn, &C7ExplorerBackButton::sigBack, this, &C7DirExplorerManager::onCdUp);
}

void C7DirExplorerManager::addFileFilter(const QString &suffix)
{
    m_lstFileFilter.append(suffix);
}

void C7DirExplorerManager::addFileFilter(const QStringList &lstSuffix)
{
    m_lstFileFilter += lstSuffix;
}

void C7DirExplorerManager::addFileType(FileType fileType)
{
    m_nFileType |= fileType;

    QString strSuffix = QString("");
    switch (fileType) {
    case ImageFile:
        strSuffix = IMAGE_SUFFIX;
        break;
    case VideoFile:
        strSuffix = VIDEO_SUFFIX;
        break;
    case AudioFile:
        strSuffix = AUDIO_SUFFIX;
        break;
    case TextFile:
        strSuffix = TEXT_SUFFIX;
        break;
    case WordFile:
        strSuffix = WORD_SUFFIX;
        break;
    case ExcelFile:
        strSuffix = EXCEL_SUFFIX;
        break;
    case PowerpointFile:
        strSuffix = POWERPOINT_SUFFIX;
        break;
    case OfficeFile:
        strSuffix = WORD_SUFFIX + EXCEL_SUFFIX + POWERPOINT_SUFFIX;
        break;
    case YunFile:
        strSuffix = YUN_SUFFIX;
        break;
    default:
        break;
    }

    m_lstFileFilter += strSuffix.split(",");

//    int nRemoveCount = m_lstFileFilter.removeDuplicates();
//    qDebug() << "hongcs" << "repeat count" << __FUNCTION__ << nRemoveCount;
}

void C7DirExplorerManager::setFileType(FileType fileType)
{
    m_nFileType = 0;
    m_lstFileFilter.clear();
    addFileType(fileType);
}

//这个是处理多种类型组合的情况
void C7DirExplorerManager::setFileType(int fileType)
{
    m_nFileType = 0;
    m_lstFileFilter.clear();
    addFileType(fileType, ImageFile);
//    addFileType(fileType, VideoFile);
//    addFileType(fileType, AudioFile);
    addFileType(fileType, WordFile);
    addFileType(fileType, ExcelFile);
    addFileType(fileType, PowerpointFile);
    addFileType(fileType, YunFile);
}

void C7DirExplorerManager::changeFirstDir(const QString &dirPath,bool useDeskIcon)
{
    if (m_pCurrentDirExplorer != nullptr)
    {
        m_pCurrentDirExplorer->deleteLater();
        m_pCurrentDirExplorer = nullptr;
    }
    m_bUseDesktopIcon = useDeskIcon;
    m_lstHistoryDir.clear();

    emit sigSetBtnBackVisible(false);

    this->setDirPath(dirPath,useDeskIcon);
}

void C7DirExplorerManager::refreshCurrentDir()
{
    if (m_pCurrentDirExplorer != nullptr)
    {
        if (m_currentPath == COMPUTER_PATH)
        {
            changeFirstDir(COMPUTER_PATH,m_bUseDesktopIcon);
//            m_pCurrentDirExplorer->refresh(QDir::drives());
        }
        else if (m_currentPath == RECENT_PATH)
        {
            getRecentFileInfo();
            m_pCurrentDirExplorer->refresh(getLegalFileList(m_lstRecentFileInfo));
        }
        else if (m_currentPath == DESKTOP_PATH)
        {
            m_pCurrentDirExplorer->refresh(getLegalFileList(Uti->path(Desktop)));
        }
        else
        {
            //优盘动态插拔时
            QFileInfoList lst = getLegalFileList(m_currentPath);
            if(lst.size() == 0){
                changeFirstDir(COMPUTER_PATH,m_bUseDesktopIcon);
                refreshCurrentDir();
            }
            else {
                m_pCurrentDirExplorer->refresh(lst);
            }
        }
    }
}

void C7DirExplorerManager::setHasClicked()
{
    m_pCurrentDirExplorer->setHasClicked();
}

void C7DirExplorerManager::setHasClicked(bool b)
{
     m_pCurrentDirExplorer->setLastItemCheck(b);
}

void C7DirExplorerManager::onCdUp()
{
    if (!m_bAnimationFinished)
    {
        return;
    }
    m_bBack = true;
    if (m_pCurrentDirExplorer != nullptr)
    {
        m_pPreviousDirExplorer = m_pCurrentDirExplorer;
        //在动画结束时才做当前explorer的修正
        setAnimation(m_pPreviousDirExplorer,
                     m_pPreviousDirExplorer->pos(), QPoint(this->width(), m_pPreviousDirExplorer->y()));
    }

    m_pCurrentDirExplorer = m_lstHistoryDir.takeLast();
    //m_currentPath = m_lastPath;

    if (m_pCurrentDirExplorer != nullptr)
    {
//        m_pCurrentDirExplorer->raise();  //置顶会挡住返回按钮
        m_pCurrentDirExplorer->show();
        m_pBackBtn->setBtnVisible(!m_lstHistoryDir.isEmpty());
        emit sigSetTitleText(m_pCurrentDirExplorer->getDispalyPath(), !m_lstHistoryDir.isEmpty(), false);
    }
//    //刷新当前页面
//    refreshCurrentDir();
}

void C7DirExplorerManager::onGetPath_Click(const QString &strPath)
{
//    qDebug() << "-->Lynn<--"<< __FUNCTION__ << m_bAnimationFinished  << "strPath = " <<strPath;
    //动画结束时才响应
    if (m_bAnimationFinished)
    {
        QFileInfo fileInfo(strPath);
        if (!strPath.isEmpty() && fileInfo.isFile())
        {
           m_pBackBtn->setBtnVisible(!m_lstHistoryDir.isEmpty());
           emit sigSetTitleText(strPath, !m_lstHistoryDir.isEmpty(), true);
        }
        else
        {
            setDirPath(strPath);
        }
    }
    else
    {
        if (m_pCurrentDirExplorer != nullptr)
        {
            m_pCurrentDirExplorer->setLastItemCheck(false);
        }
    }
}

void C7DirExplorerManager::onGetPath_DoubleClick(const QString &strPath)
{
    qDebug() << "-->lls<--"<< __FUNCTION__ << m_bAnimationFinished;
    //动画结束时才响应
    if (m_bAnimationFinished)
    {
        QFileInfo fileInfo(strPath);
        if (!strPath.isEmpty() && fileInfo.isFile())
        {
           m_pBackBtn->setBtnVisible(!m_lstHistoryDir.isEmpty());
           emit sigSetTitleText(strPath, !m_lstHistoryDir.isEmpty(), true,true);
        }
    }
    else
    {
        if (m_pCurrentDirExplorer != nullptr)
        {
            m_pCurrentDirExplorer->setLastItemCheck(false);
        }
    }
}

void C7DirExplorerManager::onGetSelectFile(const QString &strPath)
{
    //只能选择文件夹的情况，不处理
    if (m_selectMode == Dir_Only)
    {
        return;
    }

    int recentFileCount = m_lstRecentFilePath.count();
    if (!m_lstRecentFilePath.contains(strPath, Qt::CaseInsensitive))
    {
        if (recentFileCount >= m_nRecentFileMax)
        {
            m_lstRecentFilePath.takeFirst();
            --recentFileCount;
        }

        m_lstRecentFilePath.append(strPath);
        ++recentFileCount;
    }
    else
    {
        //后面展示的时候是倒序的，所以最新的加到最后
        m_lstRecentFilePath.removeOne(strPath);
        m_lstRecentFilePath.append(strPath);
    }

    Uti->setValue("recentOpen", "settingIndex", recentFileCount);
    for(int i = 1; i <= m_lstRecentFilePath.size(); i++)
    {
        Uti->setValue("recentOpen", QString::number(i), m_lstRecentFilePath.at(i-1));
    }
}

void C7DirExplorerManager::resizeEvent(QResizeEvent *event)
{
    if (m_pCurrentDirExplorer != nullptr)
    {
        m_pCurrentDirExplorer->setScaleFactor(m_rScaleFactor);
        m_pCurrentDirExplorer->resize(this->size());
    }

    C7BaseWidget::resizeEvent(event);
}


void C7DirExplorerManager::setDirPath(const QString &dirPath,bool useDeskFile)
{
    m_lastPath = m_currentPath;
    m_currentPath = dirPath;
    if (m_pCurrentDirExplorer != nullptr)
    {
        if (!m_lstHistoryDir.contains(m_pCurrentDirExplorer))
        {
            m_lstHistoryDir.append(m_pCurrentDirExplorer);
        }
        m_pBackBtn->setBtnVisible(true);
        emit sigSetTitleText(dirPath, true, false);
    }

    m_pNextDirExplorer = new C7DirExplorer(m_selectMode, this->size(), dirPath, this);
    connect(m_pNextDirExplorer, &C7DirExplorer::sigItemClick, this, &C7DirExplorerManager::onGetPath_Click);
    connect(m_pNextDirExplorer, &C7DirExplorer::sigItemDoubleClick, this, &C7DirExplorerManager::onGetPath_DoubleClick);

    QPoint startPoint = QPoint(0, 0);
    if (dirPath == COMPUTER_PATH)
    {
        QFileInfoList fileinfo = QDir::drives();
        QFileInfo file;
        if(useDeskFile){
            file.setFile(Uti->path(Desktop));
            fileinfo.push_front(file);
        }
        m_pNextDirExplorer->setDisplayInfo(fileinfo);
    }
    else if (dirPath == RECENT_PATH)
    {
        getRecentFileInfo();
        m_pNextDirExplorer->setDisplayInfo(getLegalFileList(m_lstRecentFileInfo));
    }
    else if (dirPath == DESKTOP_PATH)
    {
        m_pNextDirExplorer->setDisplayInfo(getLegalFileList(Uti->path(Desktop)));
    }
    else
    {
        m_pBackBtn->raise();
        m_pNextDirExplorer->setDisplayInfo(getLegalFileList(dirPath));
        startPoint = QPoint(m_pNextDirExplorer->x() + this->width(), m_pNextDirExplorer->y());
    }
    m_pNextDirExplorer->show();
    //在动画结束时才做当前explorer的修正
    setAnimation(m_pNextDirExplorer, startPoint, m_pNextDirExplorer->pos());
}

void C7DirExplorerManager::redisplay()
{
    foreach (C7DirExplorer* pDirExplorer, m_lstHistoryDir) {
        //先清掉之前的
        pDirExplorer->clearDisplayItem();
        //重新显示
        QDir dir(pDirExplorer->getDispalyPath());
        dir.setSorting(QDir::Name | QDir::DirsFirst);
        if (m_selectMode == Dir_Only)
        {
            dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
        }
        else
        {
            dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
        }
        pDirExplorer->setDisplayInfo(getLegalFileList(dir.entryInfoList()));
    }
}

QFileInfoList C7DirExplorerManager::getLegalFileList(const QString &path)
{
    QDir dir(path);
    dir.setSorting(QDir::Name | QDir::DirsFirst);

    if (m_selectMode == Dir_Only)
    {
        dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    }
    else
    {
        dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
    }

    return getLegalFileList(dir.entryInfoList());
}

QFileInfoList C7DirExplorerManager::getLegalFileList(const QFileInfoList &lstFileInfo)
{

    if (m_lstFileFilter.isEmpty())
    {
        return lstFileInfo;
    }
    else
    {
        QFileInfoList rLstFileInfo;
        foreach (QFileInfo info, lstFileInfo) {
            if (info.isDir())
            {
                rLstFileInfo.append(info);
            }
            else if (m_lstFileFilter.contains(info.suffix(), Qt::CaseInsensitive))
            {
                rLstFileInfo.append(info);
            }
        }
        return rLstFileInfo;
    }
}

void C7DirExplorerManager::getRecentFileInfo()
{
    m_lstRecentFileInfo.clear();
    m_lstRecentFilePath.clear();

    int fileCount = Uti->value("recentOpen", "settingIndex").toInt();
    if(fileCount != 0)
    {
        QString fileName;
        //这里是倒序的，所以最新的加到最后
        for(int i = fileCount; i > 0; --i)
        {
            fileName = Uti->value("recentOpen", QString::number(i)).toString();
            QFileInfo fileInfo(fileName);
            if(fileInfo.exists())
            {
                m_lstRecentFileInfo.append(fileInfo);
                m_lstRecentFilePath.append(fileName);
            }
        }
    }
}

void C7DirExplorerManager::setAnimation(QObject* pObject, QVariant startValue, QVariant endValue)
{
    if (m_pAnimation == nullptr)
    {
        m_pAnimation = new QPropertyAnimation(this);
        m_pAnimation->setDuration(300);
        connect(m_pAnimation, &QPropertyAnimation::valueChanged,
                this, &C7DirExplorerManager::onAnimationValueChange);
        connect(m_pAnimation, &QPropertyAnimation::finished,
                this, &C7DirExplorerManager::onAnimationFinished);
    }
    else
    {
        if (m_pAnimation->state() == QAbstractAnimation::Running)
        {
            return;
        }
    }

    m_pAnimation->setTargetObject(pObject);
    m_pAnimation->setPropertyName("pos");
    m_pAnimation->setEasingCurve(QEasingCurve::Linear);
    m_pAnimation->setStartValue(startValue);
    m_pAnimation->setEndValue(endValue);
    m_bAnimationFinished = false;
    m_pAnimation->start();
}

void C7DirExplorerManager::addFileType(int src, FileType fileType)
{
    if ((src & fileType) == fileType)
    {
        addFileType(fileType);
    }
}

void C7DirExplorerManager::onAnimationValueChange(const QVariant &value)
{
    if (m_pCurrentDirExplorer != nullptr)
    {
        QPoint p = value.toPoint();
        m_pCurrentDirExplorer->move(QPoint(p.x() - this->width(), m_pCurrentDirExplorer->y()));
    }
}

bool C7DirExplorerManager::getExploreAnimationFlag()
{
    return m_bAnimationFinished;
}

void C7DirExplorerManager::onAnimationFinished()
{
    if (m_bBack)
    {
        if (m_pPreviousDirExplorer != nullptr)
        {
            m_pPreviousDirExplorer->deleteLater();
            m_pPreviousDirExplorer = nullptr;
        }
        m_bBack = false;
    }
    else
    {
        m_pCurrentDirExplorer = m_pNextDirExplorer;
    }

    m_bAnimationFinished = true;
}


/*********************************************************************************************************************************/

C7ExplorerBackButton::C7ExplorerBackButton(int w, int h, QWidget *parent):
    C7BaseWidget(w, h, parent)
{
    initUi();
    retranslateUi();
}

void C7ExplorerBackButton::setBtnVisible(bool visible)
{
    m_pBackBtn->setVisible(visible);
//    m_pBackTextBtn->setVisible(visible);
}

void C7ExplorerBackButton::initUi()
{
    m_pBackBtn = new TLPushButton(this);
    m_pBackBtn->setNormalImage(m_strAppPath + "/skin/explorer/cloud_return_normal.png");
    m_pBackBtn->setHoverImage(m_strAppPath + "/skin/explorer/cloud_return_normal.png");
    m_pBackBtn->setPressImage(m_strAppPath + "/skin/explorer/cloud_return_press.png");
    m_pBackBtn->resize(45 * m_rScaleFactor, 45 * m_rScaleFactor);
    m_pBackBtn->move(20*m_rScaleFactor, 0);
    connect(m_pBackBtn, &TLPushButton::sigRelease, this, &C7ExplorerBackButton::sigBack);
}

void C7ExplorerBackButton::retranslateUi()
{
//    m_pBackTextBtn->setText(tr("Back"));
}

void C7ExplorerBackButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setBrush(QBrush(QColor(245, 245, 245)));
    painter.setPen(Qt::NoPen);
    painter.drawRect(this->rect());
    C7BaseWidget::paintEvent(event);
}

