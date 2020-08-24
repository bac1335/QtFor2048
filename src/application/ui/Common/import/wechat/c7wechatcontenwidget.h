#ifndef C7WECHATCONTENWIDGET_H
#define C7WECHATCONTENWIDGET_H

#include "c7basewidget.h"
#include <QTime>
#include "c7commondef.h"

class QScrollArea;
class TLBaseControl;
class QPropertyAnimation;
class C7ContenImageItem;
class C7ContenTextItem;

/**
  * @brief 微信用户内容显示窗口
  * @author xinde song
  * @date 2019-09-11
  */

class C7WechatContenWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7WechatContenWidget(int w, int h, QWidget *parent = nullptr);
    ~C7WechatContenWidget();
    void addText(const QString str);
    void addImage(const QString str);

    void setIsReaded(bool isReaded);
private:
    bool isCheacked();
    void _addImgTask(QString str);
    void removeContents(int id);
protected:
    void initUi();
    //是否移动到头
    bool isSliperOverHead() const;
    //是否移动到尾
    bool isSliperOverTrail() const;
    int sliperAlignment() const{return m_sliperAlignment;}
    void hideEvent(QHideEvent* e);
    void showEvent(QShowEvent *event);

private slots:
    void onTrayPress();
    void onTrayRelease();
    void onTrayMoveByAsk(const QPoint &pos);

private:
    QPoint m_currentPos;
    QTime m_currentTime;
    int m_sliperAlignment = 0x1;
    QScrollArea* m_pScrollArea = nullptr;
    TLBaseControl* m_pItemTray = nullptr;
    QPropertyAnimation *m_pMoveBackAnimation = nullptr;
    QList<C7ContenTextItem*> m_mapText;
    QList<C7ContenImageItem*> m_mapImage;
    Item_Type m_lastItemType;
    C7BaseWidget* m_lastItem = nullptr;

    QStringList m_imgTasks;//添加进来的图片队列，执行完添加动作后会清空
    QTimer * m_pTaskTimer = nullptr;
    bool m_isReaded = false;
//    QList<QPair<Item_Type, QString> > m_selectContent; //选中的内容
//    QMap<int,QList<QPair<Item_Type, QString> > > m_mapSelectContent;
    QList<WechatContent*> m_selectContent;

signals:
     void sigItemClick(bool,QList<WechatContent*> content);

};

#endif // C7WECHATCONTENWIDGET_H
