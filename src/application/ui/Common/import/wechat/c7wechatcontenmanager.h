#ifndef C7WECHATCONTENMANAGER_H
#define C7WECHATCONTENMANAGER_H
#include "c7basewidget.h"
#include "c7wechatcontenwidget.h"
#include <QStackedWidget>
#include "c72dcodewidget.h"

class C7WeChatContenManager : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7WeChatContenManager(int w,int h,C7BaseWidget* parent = nullptr);
    C72dCodeWidget * addQrCodeWidget();
    C7WechatContenWidget* addNewContentWidget();
    void showCurentItem(C7BaseWidget *item);
    void removeContentWidget(C7BaseWidget *item);

private:
    QList<C7WechatContenWidget*> m_mapContentWidget;
    QStackedWidget *m_pStackWidget = nullptr;

signals:
    void sigItemClick(bool,QList<WechatContent*> content);

};


#endif // C7WECHATCONTENMANAGER_H
