#include "c7pagemanager.h"
#include "c7pagemanagertoolbar.h"
#include "c7pageselectwidget.h"

C7PageManager::C7PageManager(int branch,QWidget *parentWidget, QObject *parent):
    C7BaseUiManager (branch, parent), m_pParentWidget (parentWidget)
{
    initUi();
}

void C7PageManager::hideThumbnail()
{
    if(m_pSelectWidget->isVisible()){
       m_pSelectWidget->hide();
    }
}

void C7PageManager::updateItem(QList<int> list,bool isOversea)
{
   m_pSelectWidget->updateItem(list,isOversea);
   m_pManagerWidget->setCurrentIndex(m_pSelectWidget->getCurrenPage(),m_pSelectWidget->getPageSize());
   updatePos(list.size() == 1);
}

void C7PageManager::initToolBarStatus()
{
    m_pManagerWidget->initToolBarStatus();
}

void C7PageManager::updateCurrentItemBg()
{
    m_pSelectWidget->updateCurrentItemBg();
}

void C7PageManager::initUi()
{
    //显示画面
    m_pSelectWidget = new C7PageSelectWidget(m_branch,m_pParentWidget);
    connect(m_pSelectWidget,&C7PageSelectWidget::sigSimAddPage,this,&C7PageManager::sigSimAddPage);
    updatePos(true);
    m_pSelectWidget->hide();

    //操作界面
    m_pManagerWidget = new C7PageManagerToolBar(m_branch,m_pParentWidget);

    if(m_branch == C7Version::Branch_Normal){
        m_pManagerWidget->resize(76*m_rScaleFactor, 294*m_rScaleFactor);
        m_pManagerWidget->move(m_pParentWidget->width() - m_pManagerWidget->width(),
                               m_pParentWidget->height() - 120*m_rScaleFactor - m_pManagerWidget->height());
    }
    else if(m_branch == C7Version::Branch_Oversea){
        m_pManagerWidget->resize(261*m_rScaleFactor, 72*m_rScaleFactor);
        m_pManagerWidget->move(m_pSelectWidget->x() - 8*m_rScaleFactor, m_pSelectWidget->y() + m_pSelectWidget->height() + 10*m_rScaleFactor);
    }
    connect(m_pManagerWidget, &C7PageManagerToolBar::sigShowPage, this, [=](bool isShow){
        m_pSelectWidget->setVisible(isShow);
        if(!isShow){
            emit sigClose();
        }
        else {
            emit sigThumbnailShow();
        }
    });
    connect(m_pManagerWidget, &C7PageManagerToolBar::sigAddPage, this, [=]{
        m_pSelectWidget->addItem();
        m_pSelectWidget->hide();
        //重新调整位置
        updatePos(false);

        m_pManagerWidget->setCurrentIndex(m_pSelectWidget->getCurrenPage(),m_pSelectWidget->getPageSize());
        emit sigAddPage(m_pSelectWidget->getCurrenPageId(), false,true);
    });
    connect(m_pManagerWidget, &C7PageManagerToolBar::sigPageChange, this, [=](int index){
        m_pSelectWidget->hide();//切换页隐藏
        m_pSelectWidget->setCurrentPage(index-1);  //数组对应的元素需减一
        m_pManagerWidget->setCurrentIndex(m_pSelectWidget->getCurrenPage(),m_pSelectWidget->getPageSize());
        m_pManagerWidget->initToolBarStatus();
        emit sigCurrentPageChanged(m_pSelectWidget->getCurrenPageId(), false,true);
    });
    connect(m_pSelectWidget, &C7PageSelectWidget::sigUpdateCurrentPage, this, [=](int index, bool isDelete){

        updatePos(false);
        m_pManagerWidget->setCurrentIndex(m_pSelectWidget->getCurrenPage(),m_pSelectWidget->getPageSize());
        emit sigCurrentPageChanged(index, isDelete,true);
//        m_pSelectWidget->hide();//切换页不隐藏
    });
    connect(m_pSelectWidget, &C7PageSelectWidget::sigDeleteItem, this,[=](int index,bool onlyone){
        emit sigDeletePage(index);
        updatePos(onlyone);
    });
    connect(m_pSelectWidget, &C7PageSelectWidget::sigDeletelastAddItem, this, [=](int index){
        emit sigDeletePage(index);
        m_pSelectWidget->addItem(); //删除最后一页后新增一页
        //重新调整位置
        updatePos(true);
        m_pManagerWidget->setCurrentIndex(m_pSelectWidget->getCurrenPage(),m_pSelectWidget->getPageSize());
        emit sigCurrentPageChanged(m_pSelectWidget->getCurrenPageId(),true,false);
        emit sigAddPage(m_pSelectWidget->getCurrenPageId(), false,true);
    });

    connect(m_pSelectWidget, &C7PageSelectWidget::sigDeleteLast, this, [=](){
        C7BaseWidget* widget = qobject_cast<C7BaseWidget*>(sender());
        if(widget)
            widget->hide();
        m_pManagerWidget->setBtnCheck(false);
    });

}

void C7PageManager::onUpdateThumbnail()
{
    m_pSelectWidget->onUpdateThumbnail();
}

void C7PageManager::updatePos(bool onlyOne)
{
    if(m_branch == C7Version::Branch_Normal){
        if(onlyOne){
            m_pSelectWidget->move(m_pParentWidget->width() - m_pSelectWidget->width() - 80*m_rScaleFactor,
                                  m_pParentWidget->height() - m_pSelectWidget->height() - 200*m_rScaleFactor);
        }
        else {
            m_pSelectWidget->move(m_pParentWidget->width() - m_pSelectWidget->width() - 80*m_rScaleFactor,
                                  m_pParentWidget->height() - m_pSelectWidget->height() - 123*m_rScaleFactor);
        }
    }
    else if(m_branch == C7Version::Branch_Oversea){
        m_pSelectWidget->move(m_pParentWidget->width() - m_pSelectWidget->width(),
                              m_pParentWidget->height() - m_pSelectWidget->height() - 105*m_rScaleFactor);
    }
}
