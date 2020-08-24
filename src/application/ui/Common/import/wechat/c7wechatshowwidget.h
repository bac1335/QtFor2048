#ifndef C7WECHATSHOWWIDGET_H
#define C7WECHATSHOWWIDGET_H

#include "c7basewidget.h"
#include "item/c7wechatuseritem.h"
#include "c7commondef.h"
#include "c7wechatusermanagement.h"

class C7WechatUserWidget;
class QStackedWidget;
class C72dCodeWidget;
class C7WechatContenWidget;
class C7WeChatContenManager;

/**
  * @brief 有用户连上之后的显示窗口
  * @author xinde song
  * @date 2019-09-11
  */
class C7WechatShowWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7WechatShowWidget(int w, int h, QWidget *parent = nullptr);
     C7WechatUserItem * addNewUser(QString headImg); //新微信用户添加

protected:
    void initUi();
    void paintEvent(QPaintEvent *event);

private slots:
    void onItemClick(C7WechatUserItem*);
    void onWechatAddUser(QString openId,QString headImgPath);
    void onWechatRemoveUser(QString openId);
    void onWechatReceiveImg(QString openId,QString picPath);
    void onWechatReceiveTxt(QString openId,QString txt);

signals:
    void sigItemClick(bool,QList<WechatContent*> content);
    void sigFirstUserComing();//第一个微信用户扫码绑定
    void sigLastUserLeaving();//最后一个微信用户离开


private:
    C7WechatUserWidget *m_pUserWidget = nullptr;
    QStackedWidget *m_pStackWidget = nullptr;
    C72dCodeWidget *m_p2dCodeWidget = nullptr;
    C7WeChatContenManager* m_pContentManager = nullptr;
    QMap<C7WechatUserItem*,C7BaseWidget*> m_pMapUser;
    C7WechatUserItem * m_currentUserShow = nullptr;
    QMap<QString,C7WechatUserItem *> m_mapOpenIdToItem;
    C7WechatUserItem * m_pQrcodeUser = nullptr;

};

#endif // C7WECHATSHOWWIDGET_H
