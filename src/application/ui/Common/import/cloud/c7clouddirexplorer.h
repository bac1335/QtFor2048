#ifndef NPDIREXPLORER_H
#define NPDIREXPLORER_H

#include "c7basewidget.h"
#include "c7commondef.h"

/*
 * @brief   :云文件单个文件夹显示
 * @author  :lls
 * @date    :2019.9.21
 */

#include <QTime>
#include <QFileInfoList>

class QLabel;
class QPropertyAnimation;
class QScrollArea;
class TLBaseControl;
class C7CloudItem;
class C7NetworkDiskManager;

class C7CloudDirExplorer : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7CloudDirExplorer(SelectMode mode, QSize size, const QString& displayPath, QWidget* parent = nullptr);
    void setDisplayInfo(int size,ItemPageType type,QVariant obj = "");
    void clearDisplayItem();
    QString getDispalyPath();
    void setLastItemCheck(bool bCheck);
    //这个类只负责显示，所以文件列表要从外部传进来
    void refresh(const QFileInfoList& lstFileInfo);
    void freshItem();  //更新界面item显示，此时刷新不放在resizeEvent事件，刷新内容多，打开文件容易崩溃
    void setHasClicked();
    void setSearChearchSet(QVariant obj);

signals:
    void sigItemClick(ItemType,QVariant&);
    void sigItemDoubleClick(const QString& strPath);
    void sigSelectItem(C7CloudItem*);
    void sigShowWaitBox(bool);
    void sigNetworkError(int errorcode);

private:
    void initUi();
    void countRowAndColumn(int nSize);
    void relayoutItem();
    void diaplayItem(ItemPageType type,QVariant obj);
    void diaplayItem(QVariant obj);
    QPixmap getImage(const QString &suffix);
    QString getImagePath(const QString& suffix);
    bool isSliperOverHead() const;
    bool isSliperOverTrail() const;
    int sliperAlignment() const{return m_sliperAlignment;}
    void retranslateUi();
    void resizeEvent(QResizeEvent *event);
    void initPanData(C7CloudItem *pItem,QVariant value);
    void initYunData(C7CloudItem *pItem,QVariant value);
    void initPanIcon(C7CloudItem *pItem,QString str);
    void checkFile(QString,QVariant&);

private slots:
    void onItemClick(ItemType type);
    void onTrayPress();
    void onTrayRelease();
    void onTrayMoveByAsk(const QPoint &pos);
    void onGetFileList(QVariant);

private:
    int m_nRowCount;
    int m_nOneRowCount = 4;
    int m_nRowInterval = 28;
    int m_nColumnInterval = 34;
    int m_nLeftMargin = 35;
    int m_sliperAlignment = 0x1;
    int m_nTopMargin = 75;
    //这个是刷新时用
    qreal m_defaultScaleFactor = 1.0;
    SelectMode m_selectMode;
    QPoint m_currentPos;
    QTime m_currentTime;
    QString m_dispalyPath;
    QSize m_itemSize;
    QLabel* m_pLblNoSelect = nullptr;
    QScrollArea* m_pScrollArea = nullptr;
    TLBaseControl* m_pDisplayWidget = nullptr;
    QPropertyAnimation *m_pMoveBackAnimation = nullptr;
    QFileInfoList m_lstDisplay;
    int m_ItemNum;
    C7CloudItem* m_pLastItem = nullptr;
    QList<C7CloudItem*> m_lstItem;
    C7NetworkDiskManager* m_pHttpDisManager = nullptr;
    C7CloudItem* m_pClickedItem = nullptr;
    C7CloudItem *m_pYunItem = nullptr;
    C7CloudItem *m_pPanItem = nullptr;
};




#endif // NPDIREXPLORER_H
