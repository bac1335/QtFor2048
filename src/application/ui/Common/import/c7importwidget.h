#ifndef C7IMPORTWIDGET_H
#define C7IMPORTWIDGET_H

#include "c7basewidget.h"
#include "c7commondef.h"

class C7SettingsItem;
class QLabel;
class QStackedWidget;
class C7WechatImportWidget;
class TLPushButton;
class C7DirExplorerManager;
class C7CloudYunManager;
class C7CloudItem;

enum ImportMode{
    Mode_Wechat,
    Mode_Local,
    Mode_Cloud,
};

/**
  * @brief 导入窗口
  * @author xinde song
  * @date 2019-09-11
  */
class C7ImportWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7ImportWidget(int w, int h, QWidget *parent = nullptr, int version = 0);
    void changeLoginState(bool b);
    void setFileType(int type);
    void refreshExplorer(); //优盘动态检测刷新文件列表

protected:
    void initUi();
    void retranslateUi();
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void onItemClick();

private:
    QPixmap m_bg;
    QLabel *m_pTitleLabel = nullptr;
    TLPushButton *m_pOkBtn = nullptr;
    TLPushButton *m_pCancelBtn = nullptr;

    C7SettingsItem *m_pLastItem = nullptr;
    QList<C7SettingsItem *> m_selectItemList;
    QList<ImportMode> m_modeList;

    QStackedWidget *m_pStackWidget = nullptr;
    C7WechatImportWidget *m_pWeChatImportWidget = nullptr;
    C7DirExplorerManager *m_pExplorer = nullptr;
    C7CloudYunManager* m_cloudYunWidget = nullptr;

    //页面导入类型
    ImportType m_importType;

    //本地导入类型
    QString m_selectPath;
    QString m_currentPath;

    //微信导入类型
    Item_Type WeChatType;
    QList<WechatContent*> m_selectWechatContent;

    //网盘导入
    C7CloudItem* m_pClickedItem = nullptr;  //用来标记选中文件

    int m_version = 1;

signals:
    void sigCurrentSelect(QString str);
    void sigCurrentSelectWechatText(QList<WechatContent*> content);
    void sigCancelClick();
    void sigShowLoginWidget(bool bTransparency, bool bMainWindow);

private slots:
    void onSetCurrentSelectPath(const QString& strPath, bool bShowBtnBack, bool isFile,bool isDouble);
    void onOkClick();
    void onWechatItemClick(bool, QList<WechatContent*> content);
    void onSelectItem(C7CloudItem*);
};

#endif // C7IMPORTWIDGET_H
