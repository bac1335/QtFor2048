#ifndef NPDIREXPLORER_H
#define NPDIREXPLORER_H

#include "c7basewidget.h"
#include "c7commondef.h"

/*
 * @brief   :单个文件夹显示
 * @author  :hongcs
 * @date    :2018.11.06
 */

#include <QTime>
#include <QFileInfoList>

class QLabel;
class QPropertyAnimation;
class QScrollArea;

class TLBaseControl;
class C7ExplorerItem;

class C7DirExplorer : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7DirExplorer(SelectMode mode, QSize size, const QString& displayPath, QWidget* parent = nullptr);

    void setDisplayInfo(const QFileInfoList& lstFileInfo);
    void clearDisplayItem();
    QString getDispalyPath();
    void setLastItemCheck(bool bCheck);
    //这个类只负责显示，所以文件列表要从外部传进来
    void refresh(const QFileInfoList& lstFileInfo);

    void freshItem();  //更新界面item显示，此时刷新不放在resizeEvent事件，刷新内容多，打开文件容易崩溃
    void setHasClicked();
signals:
    void sigItemClick(const QString& strPath);
    void sigItemDoubleClick(const QString& strPath);
protected:
    void initUi();
    void countRowAndColumn(int nSize);
    void relayoutItem();
    void diaplayItem();
    QPixmap getDirImage(bool isRoot);
    QString getDirImagePath(bool isRoot);
    QPixmap getImage(const QString &suffix);
    QString getImagePath(const QString& suffix);
    bool isSliperOverHead() const;
    bool isSliperOverTrail() const;
    int sliperAlignment() const{return m_sliperAlignment;}
    void retranslateUi();
    void resizeEvent(QResizeEvent *event);
private slots:
    void onItemClick(const QString& strPath);
    void onItemDoubleClick(const QString& strPath);
    void onTrayPress();
    void onTrayRelease();
    void onTrayMoveByAsk(const QPoint &pos);
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
    C7ExplorerItem* m_pLastItem = nullptr;
    QList<C7ExplorerItem*> m_lstItem;
    C7ExplorerItem *m_pItem = nullptr;
};

#endif // NPDIREXPLORER_H
