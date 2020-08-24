#ifndef C7PAGEMANAGERTOOLBAR_H
#define C7PAGEMANAGERTOOLBAR_H

#include "c7basewidget.h"
#include "c7version.h"


class TLPushButton;
class TLToggleButton;

/**
  * @brief 操作界面
  * @author xinde song
  * @date 2019-12-20
  */
class C7PageManagerToolBar : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7PageManagerToolBar(int branch,QWidget *parent = nullptr);

    enum BtnType{
        Type_LastPage,
        Type_NextPage,
        Type_AddPage,
    };

    int getCurrentIndex() const;
    void setCurrentIndex(int pageId,int allPage);
    void initToolBarStatus();
    void setBtnCheck(bool);

protected:
    void initUi();
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void onBtnClicked();

signals:
    void sigShowPage(bool isShow);
    void sigPageChange(int index);
    void sigAddPage();

private:
    QPixmap m_bg;
    TLPushButton *m_pLastPageBtn = nullptr;
    TLPushButton *m_pNextPageBtn = nullptr;
    TLPushButton *m_pAddPageBtn = nullptr;
    TLToggleButton *m_pSelectPageBtn = nullptr;
    QList<TLPushButton *> m_btnList;

    int m_nCurPage;
    int m_nTotalPage;
    int m_nBranch = C7Version::Branch_Normal;
};

#endif // C7PAGEMANAGERWIDGET_H
