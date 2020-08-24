#ifndef C7WECHATIMPORTWIDGET_H
#define C7WECHATIMPORTWIDGET_H

#include "c7basewidget.h"
#include "c7commondef.h"

class QLabel;
class QStackedWidget;
class C7WechatShowWidget;
class C72dCodeWidget;

/**
  * @brief 微信导入顶层窗口
  * @author xinde song
  * @date 2019-09-11
  */
class C7WechatImportWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7WechatImportWidget(int w, int h, QWidget *parent = nullptr);

protected:
    void initUi();


private:
    QPixmap m_2dPixmap;

    QStackedWidget *m_pStackWidget = nullptr;
    C72dCodeWidget *m_p2dCodeWidget = nullptr;
    C7WechatShowWidget *m_pWechatShowWidget = nullptr;

signals:
    void sigItemClick(bool,QList<WechatContent*> content);
};

#endif // C7WECHATIMPORTWIDGET_H
