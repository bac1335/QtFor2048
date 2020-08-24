#ifndef NPDIREXPLORERMANAGER_H
#define NPDIREXPLORERMANAGER_H

#include "c7basewidget.h"
#include "c7commondef.h"

/*
 * @brief   :管理单个文件夹显示
 * @author  :hongcs
 * @date    :2018.11.06
 */

#include <QFileInfoList>

class C7DirExplorer;
class QPropertyAnimation;
class TLPushButton;
class QPushButton;
class C7ExplorerBackButton;

class C7DirExplorerManager : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7DirExplorerManager(SelectMode mode, QSize size, QWidget* parent = nullptr);

    void addFileFilter(const QString& suffix);
    void addFileFilter(const QStringList& lstSuffix);
    //这个是增加类型
    void addFileType(FileType fileType);
    //这个是重置类型
    void setFileType(FileType fileType);
    void setFileType(int fileType);
    void changeFirstDir(const QString& dirPath,bool useDeskIcon = false); //布尔值用于创建同级的桌面图标
    void refreshCurrentDir();
    void setCurrentPath(QString path){m_currentPath = path;}
    void setHasClicked();
    void setHasClicked(bool);

signals:
    void sigSetBtnBackVisible(bool bVisible);
    void sigSetTitleText(const QString& strText, bool showBtnBack, bool isFile,bool isDouble = false);
    void sigSelectFile(const QString& strPath);
public slots:
    void onCdUp();
    void onGetPath_Click(const QString& strPath);
    void onGetPath_DoubleClick(const QString& strPath);
    void onGetSelectFile(const QString& strPath);
    bool getExploreAnimationFlag();

protected:
    void resizeEvent(QResizeEvent* event);

private:
    void setDirPath(const QString& dirPath,bool useDeskFile = false);  //bool值用于桌面图标与c，d盘同级
    void redisplay(); //重新显示这次path,设置新的文件类型后用,少用为好
    //获取所需文件列表
    QFileInfoList getLegalFileList(const QFileInfoList& lstFileInfo);
    QFileInfoList getLegalFileList(const QString& path);

    void getRecentFileInfo();
    void setAnimation(QObject* pObject, QVariant startValue, QVariant endValue);
    void addFileType(int src, FileType fileType);
private slots:
    void onAnimationValueChange(const QVariant& value);
    void onAnimationFinished();
private:
    bool m_bBack = false;
    bool m_bAnimationFinished = false;
    bool m_bUseDesktopIcon = false;
    int m_nFileType = 0;
    int m_nRecentFileMax = 16;
    QString m_currentPath;
    QString m_lastPath;
    SelectMode m_selectMode;
    QStringList m_lstFileFilter;
    QList<C7DirExplorer*> m_lstHistoryDir;
    C7DirExplorer* m_pCurrentDirExplorer = nullptr;
    C7DirExplorer* m_pPreviousDirExplorer = nullptr;
    C7DirExplorer* m_pNextDirExplorer = nullptr;
    QFileInfoList m_lstRecentFileInfo;
    QStringList m_lstRecentFilePath;
    QPropertyAnimation* m_pAnimation = nullptr;
    C7ExplorerBackButton *m_pBackBtn = nullptr;
};


/**
  * @brief 浏览返回控件
  * @author xinde song
  * @date 2019-09-12
  */
class C7ExplorerBackButton : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7ExplorerBackButton(int w, int h, QWidget *parent = nullptr);
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

#endif // NPDIREXPLORERMANAGER_H
