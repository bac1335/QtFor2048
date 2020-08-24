#ifndef C7PAGESELECTWIDGET_H
#define C7PAGESELECTWIDGET_H
#include <QTime>
#include "c7basewidget.h"
#include "c7version.h"

class C7PageItem;
class QScrollArea;
class QPropertyAnimation;
class TLBaseControl;

/**
  * @brief 显示界面
  * @author xinde song
  * @date 2019-12-20
  */
class C7PageSelectWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7PageSelectWidget(int branch,QWidget *parent = nullptr);

    C7PageItem* addItem();
    void setCurrentPage(int index);
    //刷新当前页面
    void updateCurrentPage();
    int getPageSize(){return m_itemList.size();}
    int getCurrenPageId();
    int getCurrenPage();
    void updateItem(QList<int>,bool isOversea);  //云文件加载需进行页面重置
    void updateCurrentItemBg();

public slots:
    //刷新缩略图
    void onUpdateThumbnail();

protected:
    void initUi();
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent* event);

private slots:
    void onTrayPress();
    void onTrayRelease();
    void onTrayMoveByAsk(const QPoint &pos);
    void onItemClicked();
    void onRemoveItem();

private:
    //是否移动到头
    bool isSliperOverHead() const;
    //是否移动到尾
    bool isSliperOverTrail() const;
    int sliperAlignment() const{return m_sliperAlignment;}

    void resizeUi();
    void reLayoutItem();

signals:
    void sigUpdateCurrentPage(int index, bool isDelete = false);
    void sigDeleteItem(int index,bool onlyOne);
    void sigDeletelastAddItem(int);
    void sigDeleteLast();
    void sigSimAddPage(int); //导入云文件时新增保存和分享页面

private:
    QPoint m_currentPos;
    QTime m_currentTime;
    int m_sliperAlignment = 0x1;
    QScrollArea* m_pScrollArea = nullptr;
    TLBaseControl* m_pItemTray = nullptr;
    QPropertyAnimation *m_pMoveBackAnimation = nullptr;

    QList<C7PageItem *> m_itemList;
    C7PageItem *m_pLastItem = nullptr;

    int m_nPageId = 1;  //页面标识序号，会一直累加
    QString m_cachePath; //缩略图缓存路径

    int m_nBranch = C7Version::Branch_Normal;
};

#endif // C7OSPAGESELECTWIDGET_H
