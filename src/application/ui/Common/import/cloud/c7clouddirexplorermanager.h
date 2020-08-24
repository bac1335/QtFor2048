#ifndef C7CLOUDDIREXPLORERMANAGER_H
#define C7CLOUDDIREXPLORERMANAGER_H

#include "c7basewidget.h"
#include "c7commondef.h"

/*
 * @brief   :基于页面管理改变的云文件下载
 * @author  :lls
 * @date    :2019.9.17
 */

class C7CloudDirExplorer;
class QPropertyAnimation;
class TLPushButton;
class QPushButton;
class C7CloudExplorerBackButton;
class C7NetworkDiskManager;
class C7CloudItem;
class TLLoading;
class QLabel;

class C7CloudDirExplorerManager : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7CloudDirExplorerManager(SelectMode mode, QSize size, QWidget* parent = nullptr);

    void addFileFilter(const QString& suffix);
    void addFileFilter(const QStringList& lstSuffix);
    //这个是增加类型
    void addFileType(FileType fileType);
    //这个是重置类型
    void setFileType(FileType fileType);
    void setFileType(int fileType);
    void changeFirstDir(const QString& dirPath);
    void setCurrentPath(QString path){m_currentPath = path;}
    void setHasClicked();
    void initState();
    void searchSet(SearchType type,int oriYear = 0,int orimonth = 0,int endYear = 0,int endMonth = 0);
    void searchSetLast(SearchType type);
signals:
    void sigSetBtnBackVisible(bool bVisible);
    void sigSetTitleText(const QString& strText, bool showBtnBack, bool isFile);
    void sigSelectFile(const QString& strPath);
    void sigSelectItem(C7CloudItem*);
    void sigBackBtnClick();
    void sigFlieClicked();
    void sigShowTimeChoice(bool);
    void sigNetworkError(int errorcode);

public slots:
    void onCdUp();
    void onGetPath_Click(ItemType,QVariant);
    void onGetPath_DoubleClick(const QString& strPath);
    void onGetSelectFile(const QString& strPath);
    bool getExploreAnimationFlag();

protected:
    void resizeEvent(QResizeEvent* event);

private:
    void setDirPath(const QString& dirPath,QVariant obj = "");
    void setAnimation(QObject* pObject, QVariant startValue, QVariant endValue);
    void addFileType(int src, FileType fileType);
    QVariant analysisJson(QVariant,int,int,int,int);
    void initUi();

private slots:
    void onAnimationValueChange(const QVariant& value);
    void onAnimationFinished();
private:
    bool m_bBack = false;
    bool m_bAnimationFinished = false;
    int m_nFileType = 0;
    int m_nRecentFileMax = 16;
    QString m_currentPath;
    QString m_lastPath;
    SelectMode m_selectMode;
    QStringList m_lstFileFilter;
    QList<C7CloudDirExplorer*> m_lstHistoryDir;
    C7CloudDirExplorer* m_pCurrentDirExplorer = nullptr;
    C7CloudDirExplorer* m_pPreviousDirExplorer = nullptr;
    C7CloudDirExplorer* m_pNextDirExplorer = nullptr;
    QStringList m_lstRecentFilePath;
    QPropertyAnimation* m_pAnimation = nullptr;
    C7CloudExplorerBackButton *m_pBackBtn = nullptr;
    int m_yunPageNum;
    TLLoading * m_pLoading = nullptr;
    QVariant m_jsonObj;   //记录第一次查询网盘所有数据
    SearchType m_currentSearchType = Type_Search_All;  //记录当前的type
    //记录上一次搜索日期
    int m_oriYear;
    int m_orimonth;
    int m_endYear;
    int m_endMonth;
};


/**
  * @brief 浏览返回控件
  * @author xinde song
  * @date 2019-09-12
  */
class C7CloudExplorerBackButton : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7CloudExplorerBackButton(int w, int h, QWidget *parent = nullptr);
    void setBtnVisible(bool visible);

protected:
    void initUi();
    void retranslateUi();
    void paintEvent(QPaintEvent *event);

signals:
    void sigBack();

private:
    TLPushButton* m_pBackBtn = nullptr;
    QPushButton* m_pBackTextBtn = nullptr;


};

#endif // C7CLOUDDIREXPLORERMANAGER_H
