#include "c7explorer.h"

#include <QLabel>
#include <QPainter>
#include <QVariant>
#include <QDebug>

#include "c7commondef.h"
#include "settings/c7settingsitem.h"
#include "button/widget/tlpushbutton.h"
#include "c7direxplorermanager.h"
#include "tlutility.h"

#define DIR_TYPE "DirType"

C7Explorer::C7Explorer(SelectMode mode, int width, int height, int fileType, QWidget* parent)
    : C7BaseWidget(width, height, parent), m_selectMode(mode), m_nFileType(fileType)
{
    initUi();
}

void C7Explorer::setDisplayFileType(FileType fileType, bool bClearOldType)
{
    if (bClearOldType)
    {
        m_pDirManager->setFileType(fileType);
    }
    else
    {
        m_pDirManager->addFileType(fileType);
    }
}

void C7Explorer::onSetCurrentSelectPath(const QString &strPath, bool bShowBtnBack, bool isFile,bool isDouble)
{
    QFileInfo fileInfo(strPath);
    //此时路径保存的是非空和非文件的的地址目录
    if(fileInfo.isDir()&&(!(fileInfo.isFile()))){
        m_currentPath = strPath;
        m_pDirManager->setCurrentPath(strPath);
    }
    else if(DESKTOP_PATH == strPath ||RECENT_PATH== strPath || COMPUTER_PATH == strPath)
    {
        //此处判断非文件的地址，若返回到根目录，此时DirManager保存的路径是绝对路径，造成界面再次进入显示异常
        m_pDirManager->setCurrentPath(strPath);
    }

    if (strPath == DESKTOP_PATH)
    {
        m_selectPath = Uti->path(Desktop);
        m_currentPath =  Uti->path(Desktop);
    }
    else
    {
        m_selectPath = strPath;
    }

    m_pDisplayLabel->setText(strPath.split("/", QString::SkipEmptyParts).last());
    if (bShowBtnBack)
    {
        m_pDisplayLabel->setVisible(true);
        m_pDisplayLabel->move(m_pBackBtn->x() + m_pBackBtn->width() + 5 * m_rScaleFactor, m_pDisplayLabel->y());
    }
    else
    {
        m_pDisplayLabel->setVisible(isFile);
        m_pDisplayLabel->move(m_pBackBtn->x(), m_pDisplayLabel->y());
    }

    m_pDisplayLabel->hide();
    m_pBackBtn->setVisible(false);
//    m_pBackBtn->setVisible(bShowBtnBack);

    if (m_selectMode == File_Only)
    {
        m_pOkBtn->setEnabled(isFile);
    }
    else
    {
        //如果是系统根目录，确定按键设置为不可用
        if (m_selectPath == COMPUTER_PATH)
        {
            m_pOkBtn->setEnabled(false);
        } else {
            m_pOkBtn->setEnabled(true);
        }
    }
}

void C7Explorer::initUi()
{
    m_nItemHeight = 30;   //这里没有乘缩放系数，所以后面引用到这个变量的时候要乘
    m_nItemInterval = 30;   //这里没有乘缩放系数，所以后面引用到这个变量的时候要乘
    m_bg.load(m_strAppPath + "/skin/explorer/bg_explorer.png");

    m_pBackBtn = new TLPushButton(this);
    m_pBackBtn->setNormalImage(m_strAppPath + "/skin/explorer/btn_return_normal.png");
    m_pBackBtn->setHoverImage(m_strAppPath + "/skin/explorer/btn_return_normal.png");
    m_pBackBtn->setPressImage(m_strAppPath + "/skin/explorer/btn_return_press.png");
    connect(m_pBackBtn, &TLPushButton::sigRelease, this, &C7Explorer::sigBack);
    m_pDisplayLabel = new QLabel(this);
    m_pTitleLabel = new QLabel(this);

    QPalette palette;
    palette.setColor(QPalette::Text, Qt::black);
    m_pDisplayLabel->setPalette(palette);
    m_pDisplayLabel->setFont(m_font);
    m_pTitleLabel->setPalette(palette);
    m_pTitleLabel->setFont(m_font);
    setTitleVisible(false);

    m_font.setPixelSize(18*m_rScaleFactor);
    m_pOkBtn = new TLPushButton(this);
    m_pOkBtn->setFont(m_font);
    m_pOkBtn->setTextColor(Color_OkText_Press);
    m_pOkBtn->setNormalBackColor(Color_OkButton_Normal);
    m_pOkBtn->setHoverBackColor(Color_OkButton_Normal);
    m_pOkBtn->setPressBackColor(Color_OkButton_Press);
    m_pOkBtn->setDisableBackColor(Color_Button_Disable);
    m_pOkBtn->setRadius(6 * m_rScaleFactor);
    m_pOkBtn->setEnabled(m_selectMode != File_Only);
    connect(m_pOkBtn,&TLPushButton::sigClicked,
            this,&C7Explorer::onOkAndCancelBtnClicked);

    m_pCancelBtn = new TLPushButton(this);
    m_pCancelBtn->setFont(m_font);
    m_pCancelBtn->setBorderLine(true);
    m_pCancelBtn->setNormalBackColor(Color_CancelButton_Normal);
    m_pCancelBtn->setHoverBackColor(Color_CancelButton_Normal);
    m_pCancelBtn->setPressBackColor(Color_CancelButton_Press);
    m_pCancelBtn->setRadius(6 * m_rScaleFactor);
    connect(m_pCancelBtn,&TLPushButton::sigClicked,
            this,&C7Explorer::onOkAndCancelBtnClicked);


    m_pDirManager = new C7DirExplorerManager(m_selectMode, QSize(736 * m_rScaleFactor, 492 * m_rScaleFactor), this);
    m_pDirManager->setFileType(m_nFileType);
    connect(this, &C7Explorer::sigBack, m_pDirManager, &C7DirExplorerManager::onCdUp);
    connect(m_pDirManager, &C7DirExplorerManager::sigSetTitleText, this, &C7Explorer::onSetCurrentSelectPath);
    connect(m_pDirManager, &C7DirExplorerManager::sigSelectFile, this, [=](const QString& strPath){
        emit sigCurrentSelect(strPath);
        this->hide(); //这里隐藏不需动画，防止多次点击加载多张图片的bug
    });
    connect(this, &C7Explorer::sigCurrentSelect, m_pDirManager, &C7DirExplorerManager::onGetSelectFile);

    if (m_selectMode != Dir_Only)
    {
        m_pRecentItem = createItem();
        m_pRecentItem->setProperty(DIR_TYPE, DirRecent);
    }
    m_pDesktopItem = createItem();
    m_pDesktopItem->setProperty(DIR_TYPE, DirDesktop);
    m_pComputerItem = createItem();
    m_pComputerItem->setProperty(DIR_TYPE, DirComputer);

    layoutItems(true);

    retranslateUi();
}

void C7Explorer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(this->rect(), m_bg, m_bg.rect());

    QWidget::paintEvent(event);
}

void C7Explorer::retranslateUi()
{
    m_pOkBtn->setText(QString(tr("Ok")));
    m_pCancelBtn->setText(QString(tr("Cancel")));

    if (m_pRecentItem != nullptr)
    {
        m_pRecentItem->setText(QString(tr("Recent")));
    }

    m_pDesktopItem->setText(QString(tr("Desktop")));
    m_pComputerItem->setText(QString(tr("Computer")));
    m_pTitleLabel->setText(tr("Choose a directory"));
}

void C7Explorer::resizeEvent(QResizeEvent *event)
{
    qreal tempScale = m_rScaleFactor;       //保存原有缩放系数
    if (m_bUseAnimation)
    {
        m_rScaleFactor = this->width() * 1.0 / m_nNormalWidth*tempScale;        // 计算现在的缩放系数
        onScaleFactorChange();
    }

    m_pBackBtn->setFixedSize(20 * m_rScaleFactor, 28 * m_rScaleFactor);
    m_pBackBtn->move(162 * m_rScaleFactor, 25 * m_rScaleFactor);

    m_pDisplayLabel->resize(this->width() - m_pDisplayLabel->x(), 26 * m_rScaleFactor);
    m_pDisplayLabel->move(m_pBackBtn->x() + m_pBackBtn->width() + 5 * m_rScaleFactor,
                           m_pBackBtn->y() - (m_pDisplayLabel->height() - m_pBackBtn->height()) / 2);

    m_pDisplayLabel->setFont(m_font);
    m_pTitleLabel->setFont(m_font);
    m_pTitleLabel->move(this->width()/2.-m_pTitleLabel->sizeHint().width()/2.,
                        26 * m_rScaleFactor);

    QFileInfo fileInfo(m_currentPath);
    if(fileInfo.isFile())
    {
        m_pDisplayLabel->setText(fileInfo.fileName());
    }
    else if(fileInfo.isDir())
    {
        m_pDisplayLabel->setText(m_currentPath.split("/", QString::SkipEmptyParts).last());
    }
    else
    {
        m_pDisplayLabel->setText("");
    }

    if(!fileInfo.exists() || fileInfo.isDir()){
        m_pOkBtn->setEnabled(m_selectMode == Dir_Only);
    }

    m_pDirManager->setScaleFactor(this->m_rScaleFactor);
    m_pDirManager->resize(736 * m_rScaleFactor, 492 * m_rScaleFactor);
    m_pDirManager->move(153 * m_rScaleFactor, 76 * m_rScaleFactor);

    m_pOkBtn->resize(121*m_rScaleFactor,48*m_rScaleFactor);
    m_pOkBtn->move(this->width() - 47*m_rScaleFactor - m_pOkBtn->width(),
                   this->height()-90*m_rScaleFactor);
    m_pCancelBtn->resize(121*m_rScaleFactor,48*m_rScaleFactor);
    m_pCancelBtn->move(m_pOkBtn->x() - 20*m_rScaleFactor - m_pCancelBtn->width(),
                       this->height()-90*m_rScaleFactor);

    layoutItems();

    if (m_bUseAnimation)
    {
        m_rScaleFactor = tempScale; //恢复原有缩放系数
    }
    QWidget::resizeEvent(event);
}

void C7Explorer::showEvent(QShowEvent *event)
{
    if (m_pDirManager != nullptr)
    {
        m_pDirManager->refreshCurrentDir();
    }
    C7BaseWidget::showEvent(event);
}

//void C7Explorer::onBtnEnterClick()
//{
//    emit sigCurrentSelect(m_selectPath);
//    if (m_bUseAnimation)
//    {
//        this->hideWithAnimation();
//    }
//    else
//    {
//        this->hide();
//    }
//    qDebug() << "hongcs" << __FUNCTION__ << m_selectPath;
//}

void C7Explorer::onItemClick()
{
    C7SettingsItem * item = qobject_cast<C7SettingsItem *>(sender());
    setTitleVisible(false);
    if (item != nullptr)
    {
        if (item != m_pCurrentItem)
        {
            m_pCurrentItem->setChecked(false);
            m_pCurrentItem = item;
        }
        DirType type = (DirType) m_pCurrentItem->property(DIR_TYPE).toInt();
        switch (type) {
        case DirRecent:
        {
            //没有直接选中文件或者路径，所以设置确定按键不可用
            m_pDirManager->changeFirstDir(RECENT_PATH);
            m_pOkBtn->setEnabled(false);
            break;
        }
        case DirDesktop:
        {
            m_selectPath = Uti->path(Desktop);
            m_pDirManager->changeFirstDir(DESKTOP_PATH);
            if (m_selectMode != File_Only)
            {
                m_pOkBtn->setEnabled(true);
            }
            break;
        }
        case DirComputer:
        {
            //没有直接选中文件或者路径，所以设置确定按键不可用
            m_pDirManager->changeFirstDir(COMPUTER_PATH);
            m_pOkBtn->setEnabled(false);
            break;
        }
        default:
            break;
        }
    }
}

void C7Explorer::onOkAndCancelBtnClicked()
{
    TLPushButton * btn = qobject_cast<TLPushButton*>(sender());
    this->hide();
    if(btn == m_pOkBtn){
        emit sigCurrentSelect(m_selectPath);
    }
    if (m_bUseAnimation)
    {
        this->hideWithAnimation();
    }
    else
    {
        this->hide();
    }
}

void C7Explorer::setTitleVisible(bool bVisible)
{
    m_pBackBtn->setVisible(false);
//    m_pBackBtn->setVisible(bVisible);
//    m_pDisplayLabel->setVisible(bVisible);
    m_pDisplayLabel->setVisible(false);
    m_currentPath = "";
}

void C7Explorer::layoutItems(bool bIinit)
{
    C7SettingsItem* pItem = nullptr;
    for (int i = 0, count = m_items.count(); i < count; ++i)
    {
        pItem = m_items.at(i);
        if (m_bUseAnimation)
        {
            pItem->setScaleFactor(m_rScaleFactor);
            pItem->resize(140 * m_rScaleFactor, m_nItemHeight * m_rScaleFactor);
        }
        pItem->move(5 * m_rScaleFactor,
                     i * ((m_nItemHeight + m_nItemInterval) * m_rScaleFactor) + 112 * m_rScaleFactor);
    }

    if (bIinit)
    {
        emit m_pCurrentItem->sigClicked();
    }
}

C7SettingsItem *C7Explorer::createItem()
{
    C7SettingsItem* pItem = new C7SettingsItem(140*m_rScaleFactor, m_nItemHeight * m_rScaleFactor, this);
    pItem->setTextColor(Color_MenuItem_Text_Normal, Color_MenuItem_Text_Checked);
    connect(pItem, &C7SettingsItem::sigClicked, this, &C7Explorer::onItemClick);
    if (m_pCurrentItem == nullptr)
    {
        m_pCurrentItem = pItem;
        m_pCurrentItem->setChecked(true);
    }

    m_items << pItem;
    return pItem;
}

 bool C7Explorer::getDirExploreAnimation()
 {
     return m_pDirManager->getExploreAnimationFlag();
 }

 void C7Explorer::refresh()
 {
     if(this->isVisible()){
         if(m_pDirManager){
             m_pDirManager->refreshCurrentDir();
         }
     }
 }
