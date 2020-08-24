#ifndef C7PageManager_H
#define C7PageManager_H

#include "Manager/c7baseuimanager.h"

class C7PageManagerToolBar;
class C7PageSelectWidget;

/**
  * @brief 管理添加画面和显示画面
  * @author xinde song
  * @date 2019-12-20
  */
class C7PageManager : public C7BaseUiManager
{
    Q_OBJECT
public:
    explicit C7PageManager(int branch, QWidget *parentWidget, QObject *parent = nullptr);
    void hideThumbnail();
    void updateItem(QList<int>,bool isOversea = false);  //云文件加载需进行页面重置
    void initToolBarStatus();//针对海外版工具栏
    void updateCurrentItemBg();

protected:
    void initUi();

public slots:
    //刷新缩略图
    void onUpdateThumbnail();

private:
    void updatePos(bool onlyOne);

signals:
    //切换当前页,切换页时保存动作的逻辑由主场景中执行
    void sigCurrentPageChanged(int id,bool isDelete,bool update);
    //新增页，新增页时保存动作的逻辑由主场景中执行  isFirst=是否第一次添加
    void sigAddPage(int id,bool isFirst,bool update);
    //删除页，删除页时，切换或者新增页动作由本类中实现
    void sigDeletePage(int id);
    //关闭软件窗口
    void sigClose();
    void sigLoadingOutTime();
    //加载云文件时新增页面，简化上面的addpage操作
    void sigSimAddPage(int id);
    void sigThumbnailShow();

private:
    QWidget *m_pParentWidget = nullptr;
    C7PageSelectWidget *m_pSelectWidget = nullptr;
    C7PageManagerToolBar *m_pManagerWidget = nullptr;
};

#endif // C7PageManager_H
