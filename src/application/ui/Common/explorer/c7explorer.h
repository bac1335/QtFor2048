#ifndef NPEXPLORER_H
#define NPEXPLORER_H

#include "c7basewidget.h"
#include "c7commondef.h"
#include <QFileInfoList>

class QLabel;
class TLPushButton;
class C7SettingsItem;
class C7DirExplorerManager;

/*
 * @brief   :本地文件/路径浏览窗口
 * @author  :Lynn
 * @date    :2019.9.11
 */

class C7Explorer : public C7BaseWidget
{
    Q_OBJECT
public:
    enum DirType{
        DirRecent,
        DirDesktop,
        DirComputer
    };

    explicit C7Explorer(SelectMode mode, int width, int height,
                        int fileType, QWidget* parent = nullptr);

    void setDisplayFileType(FileType fileType, bool bClearOldType = false);
    bool getDirExploreAnimation();

public slots:
    void onSetCurrentSelectPath(const QString& strPath, bool bShowBtnBack, bool isFile,bool isDouble);
    void refresh();
signals:
    void sigBack();
    void sigCurrentSelect(const QString& strPath);
protected:
    void initUi();

    void paintEvent(QPaintEvent *event);
    void retranslateUi();
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);
private slots:
//    void onBtnEnterClick();
    void onItemClick();
    void onOkAndCancelBtnClicked();
private:
    void setTitleVisible(bool bVisible);
    void layoutItems(bool bIinit = false);
    C7SettingsItem* createItem();
private:
    int m_nFileType;
    int m_nItemHeight;
    int m_nItemInterval;
    SelectMode m_selectMode;
    QString m_selectPath;
    QString m_currentPath;
    QPixmap m_bg;

    QLabel* m_pDisplayLabel = nullptr;
    TLPushButton* m_pBackBtn = nullptr;
    TLPushButton* m_pOkBtn = nullptr;
    TLPushButton* m_pCancelBtn = nullptr;
    QLabel* m_pTitleLabel = nullptr;  //标题

    C7SettingsItem* m_pRecentItem = nullptr;
    C7SettingsItem* m_pDesktopItem = nullptr;
    C7SettingsItem* m_pComputerItem = nullptr;
    C7SettingsItem* m_pCurrentItem = nullptr;
    QList<C7SettingsItem*> m_items;

    C7DirExplorerManager* m_pDirManager = nullptr;
};

#endif // NPEXPLORER_H
