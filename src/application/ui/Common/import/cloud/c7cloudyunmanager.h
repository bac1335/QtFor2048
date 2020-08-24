#ifndef C7CLOUDYUNMANAGER_H
#define C7CLOUDYUNMANAGER_H

#include "c7basewidget.h"
#include <QStackedWidget>
#include "../cloud/c7clouddirexplorermanager.h"
#include "c7settingwidget/c7settingaccountwidget.h"
#include "slideWidget/c7doubleselectwidget.h"
#include "c7timechoice.h"
#include "notice/tlnotice.h"

class C7TimeChoiceWidget;

/*
 * @brief   : yun文件导入界面，登陆时显示云文件下载界面，反之显示登陆界面
 * @author  : lls
 * @date    : 2019.9.25
 */

class C7CloudYunManager : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7CloudYunManager(int w, int h,C7BaseWidget* parent = nullptr);
    void setCurrentWidget(bool b);
    void changeLoginState(bool b);

private:
    void setDataChoixeVisble(bool);
    void initUi();

private:
     QStackedWidget *m_pStackWidget = nullptr;
     C7CloudDirExplorerManager *m_pCloudImportWidget = nullptr;
     C7SettingAccountWidget* m_accountLogin = nullptr;
     C7TimeChoiceWidget* m_fileChoice = nullptr;
     C7TimeChoice* m_timeChoiceOrigin = nullptr;
     C7TimeChoice* m_timeChoiceEnd = nullptr;
     QLabel* m_textLabel = nullptr;
     TLPushButton* m_dataSureBtn = nullptr;
     SearchType m_searchType;  //记录当前的选择类型
     C7DataTime* m_currentTimeChoice = nullptr;  //当前时间选择器
     //网络异常文字提示
     TLNotice* m_noticeText = nullptr;

protected:
     void retranslateUi();

Q_SIGNALS:
     void sigShowLoginWidget(bool bTransparency, bool bMainWindow);
     void sigSelectItem(C7CloudItem*);
     void sigBackBtnClick();
     void sigFlieClicked();

private slots:
     void onSearchSet(SearchType type);
     void onDataSureClicked();
     void onTimeChoice(C7DataTime*);
     void onNetworkError(int errorcode);

};

class C7TimeChoiceWidget : public C7DoubleSelectWidget
{
    Q_OBJECT
public:
    explicit C7TimeChoiceWidget(int w, int h, QWidget *parent = nullptr);
    void setFont(QFont f){m_font = f;}

protected:
    void retranslateUi();

signals:
    void sigSearchSet(SearchType type);

};

#endif // C7CLOUDYUNMANAGER_H
