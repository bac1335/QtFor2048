#include "c7importwidget.h"
#include "settings/c7settingsitem.h"
#include "c7commondef.h"
#include "tlutility.h"
#include "wechat/c7wechatimportwidget.h"
#include "tlpushbutton.h"
#include "explorer/c7direxplorermanager.h"
#include "c7commondef.h"
#include <QPainter>
#include <QLabel>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "cloud/c7cloudyunmanager.h"
#include "c7application.h"
#include "cloud/c7clouditem.h"
#include "c7oem.h"
#include "c7version.h"

C7ImportWidget::C7ImportWidget(int w, int h, QWidget *parent, int version):
    C7BaseWidget(w, h, parent), m_version(version)
{
    initUi();
    retranslateUi();
}

void C7ImportWidget::changeLoginState(bool b)
{
    m_cloudYunWidget->changeLoginState(b);
}

void C7ImportWidget::setFileType(int type)
{
    m_pExplorer->setFileType(type);
}

void C7ImportWidget::refreshExplorer()
{
    m_pExplorer->refreshCurrentDir();
}

void C7ImportWidget::initUi()
{
    m_bg.load(m_strAppPath + "/skin/import/bg_import.png");

    //标题
    m_pTitleLabel = new QLabel(this);
    m_pTitleLabel->resize(this->width(), 26*m_rScaleFactor);
    m_pTitleLabel->move(0, 28*m_rScaleFactor);
    m_pTitleLabel->setFont(m_font);
    m_pTitleLabel->setAlignment(Qt::AlignCenter);

    //取消
    m_pCancelBtn = new TLPushButton(this);
    m_pCancelBtn->setFont(m_font);
    m_pCancelBtn->setNormalBackColor(Color_CancelButton_Normal);
    m_pCancelBtn->setHoverBackColor(Color_CancelButton_Normal);
    m_pCancelBtn->setPressBackColor(Color_CancelButton_Press);
    m_pCancelBtn->setNormalTextColor(Color_Cancel_Text_Normal);
    m_pCancelBtn->setBorderLine(true);
    m_pCancelBtn->setRadius(6 * m_rScaleFactor);
    m_pCancelBtn->resize(120*m_rScaleFactor, 48*m_rScaleFactor);
    m_pCancelBtn->move(618*m_rScaleFactor, 586*m_rScaleFactor);
    connect(m_pCancelBtn, &TLPushButton::sigClicked, this,[=]()
    {
        emit sigCancelClick();
        this->hideWithAnimation();
    });

    //确认
    m_pOkBtn = new TLPushButton(this);
    m_pOkBtn->setFont(m_font);
    m_pOkBtn->setNormalBackColor(Color_MenuButton_Normal);
    m_pOkBtn->setHoverBackColor(Color_MenuButton_Hover);
    m_pOkBtn->setPressBackColor(Color_MenuButton_Press);
    m_pOkBtn->setDisableBackColor(Color_MenuButton_Disable);
    m_pOkBtn->setTextColor(Color_MenuButton_Text);
    m_pOkBtn->setRadius(6 * m_rScaleFactor);
    m_pOkBtn->resize(120*m_rScaleFactor, 48*m_rScaleFactor);
    m_pOkBtn->move(m_pCancelBtn->x() + m_pCancelBtn->width() + 20*m_rScaleFactor, m_pCancelBtn->y());
    m_pOkBtn->setEnabled(false);
    connect(m_pOkBtn, &TLPushButton::sigClicked, this, &C7ImportWidget::onOkClick);

    QWidget *pLeftToolWidget = new QWidget(this);
    pLeftToolWidget->resize(154*m_rScaleFactor, this->height());

    QVBoxLayout *pLeftVLayout = new QVBoxLayout(pLeftToolWidget);
    pLeftVLayout->addSpacing(117*m_rScaleFactor);
    pLeftVLayout->setSpacing(26*m_rScaleFactor);

    //左侧选择栏
    if(m_version == C7Version::Branch_Oversea){
        m_modeList << Mode_Local;
    }
    else if(m_version == C7Version::Branch_Normal){
        m_modeList << Mode_Wechat << Mode_Cloud << Mode_Local;
    }
    for(int i = 0; i < m_modeList.size(); ++i)
    {
        C7SettingsItem* pItem = new C7SettingsItem(145 * m_rScaleFactor, 30 * m_rScaleFactor, this);
        pItem->setTextColor(Color_MenuItem_Text_Normal, Color_MenuItem_Text_Checked);
        pItem->setFixedSize(145 * m_rScaleFactor, 30 * m_rScaleFactor);
//        pItem->move(6 * m_rScaleFactor, 117*m_rScaleFactor + i*(pItem->height() + 26*m_rScaleFactor));
        pItem->setFont(Uti->getFont(22*m_rScaleFactor, 50));
        pLeftVLayout->addWidget(pItem);
        connect(pItem, &C7SettingsItem::sigClicked, this, &C7ImportWidget::onItemClick);
        m_selectItemList.append(pItem);

        if(!i)
        {
            pItem->setChecked(true);
            m_pLastItem = pItem;
        }
    }

    m_pStackWidget = new QStackedWidget(this);
    m_pStackWidget->resize(724*m_rScaleFactor, 490*m_rScaleFactor);
    m_pStackWidget->move(154*m_rScaleFactor, 75*m_rScaleFactor);

    if(m_modeList.contains(Mode_Wechat)){
        //微信导入
        m_pWeChatImportWidget = new C7WechatImportWidget(m_pStackWidget->width(), m_pStackWidget->height());
        connect(m_pWeChatImportWidget,&C7WechatImportWidget::sigItemClick,this,&C7ImportWidget::onWechatItemClick);
        m_pStackWidget->addWidget(m_pWeChatImportWidget);
    }

    if(m_modeList.contains(Mode_Cloud)){
        //云文件
        m_cloudYunWidget = new C7CloudYunManager(m_pStackWidget->width(), m_pStackWidget->height(),this);
        connect(m_cloudYunWidget,&C7CloudYunManager::sigShowLoginWidget,this,&C7ImportWidget::sigShowLoginWidget);
        connect(m_cloudYunWidget,&C7CloudYunManager::sigSelectItem,this,&C7ImportWidget::onSelectItem);
        connect(m_cloudYunWidget,&C7CloudYunManager::sigBackBtnClick,this,[=](){
            if(m_pClickedItem){
                m_pClickedItem = nullptr;
            }
            m_pOkBtn->setDisabled(true);
        });
        connect(m_cloudYunWidget,&C7CloudYunManager::sigFlieClicked,this,[=](){
            m_pOkBtn->setDisabled(true);
        });
        m_pStackWidget->addWidget(m_cloudYunWidget);
    }


    //本地文件
    m_pExplorer = new C7DirExplorerManager(File_Only, QSize(m_pStackWidget->width(), m_pStackWidget->height()));

    connect(m_pExplorer, &C7DirExplorerManager::sigSetTitleText, this, &C7ImportWidget::onSetCurrentSelectPath);
    connect(m_pExplorer, &C7DirExplorerManager::sigSelectFile, this, [=](const QString& strPath){
        emit sigCurrentSelect(strPath);
        this->hide(); //这里隐藏不需动画，防止多次点击加载多张图片的bug
    });

//    m_pExplorer->setFileType((int)(AllFile));
    m_pExplorer->changeFirstDir(COMPUTER_PATH,true);
    m_pStackWidget->addWidget(m_pExplorer);

    m_pStackWidget->setCurrentIndex(0);
}

void C7ImportWidget::retranslateUi()
{
    m_pTitleLabel->setText(tr("Import"));
    m_pOkBtn->setText(tr("Ok"));
    m_pCancelBtn->setText(tr("Cancel"));

    QStringList itemStrList;
    if(m_version == C7Version::Branch_Oversea){
        itemStrList << tr("Local") ;
    }
    else {
        itemStrList << tr("Wechat") << tr("Cloud") << tr("Local") ;
    }
    for(int i = 0; i < m_selectItemList.size(); ++i)
    {
        m_selectItemList.at(i)->setText(itemStrList.at(i));
    }

}

void C7ImportWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setBrush(Qt::NoBrush);
    painter.drawPixmap(this->rect(), m_bg, m_bg.rect());
    C7BaseWidget::paintEvent(event);
}

void C7ImportWidget::resizeEvent(QResizeEvent *event)
{
    qreal tempScale = m_rScaleFactor;       //保存原有缩放系数
    m_rScaleFactor = this->width() * 1.0 / m_nNormalWidth*tempScale;

    {
        //标题
        m_pTitleLabel->resize(this->width(), 26*m_rScaleFactor);
        m_pTitleLabel->move(0, 28*m_rScaleFactor);

        //取消
        m_pCancelBtn->setRadius(6 * m_rScaleFactor);
        m_pCancelBtn->resize(120*m_rScaleFactor, 48*m_rScaleFactor);
        m_pCancelBtn->move(618*m_rScaleFactor, 586*m_rScaleFactor);

        //确认
        m_pOkBtn->setRadius(6 * m_rScaleFactor);
        m_pOkBtn->resize(120*m_rScaleFactor, 48*m_rScaleFactor);
        m_pOkBtn->move(m_pCancelBtn->x() + m_pCancelBtn->width() + 20*m_rScaleFactor, m_pCancelBtn->y());

        for(int i = 0; i < m_selectItemList.size(); ++i)
        {
            C7SettingsItem* pItem = m_selectItemList.at(i);
            pItem->move(6 * m_rScaleFactor, 117*m_rScaleFactor + i*(pItem->height() + 26*m_rScaleFactor));
            pItem->setFont(Uti->getFont(22*m_rScaleFactor, 50));
        }

        m_pStackWidget->resize(724*m_rScaleFactor, 490*m_rScaleFactor);
        m_pStackWidget->move(154*m_rScaleFactor, 75*m_rScaleFactor);

    }

    m_rScaleFactor = tempScale; //恢复原有缩放系数
}

void C7ImportWidget::onItemClick()
{
    C7SettingsItem *item = qobject_cast<C7SettingsItem *>(sender());
    if(m_pLastItem && m_pLastItem != item)
    {
        m_pLastItem->setChecked(false);
        m_pLastItem = item;
    }

    int index = m_selectItemList.indexOf(item);

    if(index == m_pStackWidget->indexOf(m_cloudYunWidget))
    {
        m_cloudYunWidget->setCurrentWidget(cOem->getLoginState().isLogin);
    }

    if(m_pClickedItem)
        m_pClickedItem->sethasSelected(false);
    m_pExplorer->setHasClicked(false);
    m_pOkBtn->setDisabled(true);
    m_pStackWidget->setCurrentIndex(index);

}

void C7ImportWidget::onSetCurrentSelectPath(const QString &strPath, bool bShowBtnBack, bool isFile,bool isDouble)
{
    m_importType = Local_type;

    QFileInfo fileInfo(strPath);

    m_pOkBtn->setEnabled(false);
    if(fileInfo.isDir()&&(!(fileInfo.isFile()))){
        m_currentPath = strPath;
        m_pExplorer->setCurrentPath(strPath);
    }
    else if(DESKTOP_PATH == strPath ||RECENT_PATH== strPath || COMPUTER_PATH == strPath)
    {
        //此处判断非文件的地址，若返回到根目录，此时DirManager保存的路径是绝对路径，造成界面再次进入显示异常
        m_pExplorer->setCurrentPath(strPath);
    }
    else
    {
        m_pOkBtn->setEnabled(true);
        m_selectPath = strPath;
    }

    if (strPath == DESKTOP_PATH)
    {
        m_selectPath = Uti->path(Desktop);
        m_currentPath =  Uti->path(Desktop);
    }

    if(isDouble){
        m_pOkBtn->setEnabled(false);
        this->hide();
        emit sigCurrentSelect(m_selectPath);
    }

}

void C7ImportWidget::onOkClick()
{
    if(m_importType == Local_type || m_importType == Cloud_type)
    {
        emit sigCurrentSelect(m_selectPath);
        if(m_pClickedItem)
            m_pClickedItem->sethasSelected(false);
    }
    else if(m_importType == WeChat_type)
    {
        emit sigCurrentSelectWechatText(m_selectWechatContent);
    }
    this->hide();
    m_pOkBtn->setEnabled(false);
    m_pExplorer->setHasClicked();
}

void C7ImportWidget::onWechatItemClick(bool f, QList<WechatContent*> content)
{
    m_importType = WeChat_type;
    m_pOkBtn->setDisabled(!f);
    m_selectWechatContent = content;
}

void C7ImportWidget::onSelectItem(C7CloudItem *item)
{
    if(!m_pClickedItem){
        m_pClickedItem = item;
        m_pClickedItem->sethasSelected(true);
    }
    else{
        if(m_pClickedItem == item){
            m_pClickedItem = nullptr;
            item->sethasSelected(false);
        }
        else{
            m_pClickedItem->sethasSelected(false);
            m_pClickedItem = item;
            m_pClickedItem->sethasSelected(true);
        }
    }

    QString str = "/";
    if(item->getItemId() == Yun_Item)
        str = "/yun/";
    else if(item->getItemId() == Pan_Item)
        str = "/pan/";

    m_importType = Cloud_type;
    QString path = Uti->path(Cache) + "/yundownload/" + QString::number(cOem->getLoginState().userid);
    m_selectPath = QString("%1%2%3")
            .arg(path)
            .arg(str)
            .arg(item->getName());
    m_pOkBtn->setDisabled(m_pClickedItem != item);
}
