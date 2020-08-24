#ifndef C7PageItem_H
#define C7PageItem_H

#include <QMouseEvent>
#include "c7basewidget.h"
#include <QFuture>
#include "c7version.h"


class TLPushButton;

/**
  * @brief 页面管理的item
  * @author xinde song
  * @date 2019-12-20
  */
class C7PageItem : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7PageItem(int branch,int id, QWidget *parent = nullptr);

    void setImage(const QString &path);
    void setChecked(bool checked);
    void setItemId(int id){
        m_nId = id;
    }
    int getItemId(){
        return m_nId;
    }
    void startWaiting();
    bool getLoadIng(){return m_loading;}
    void setLoadIng(bool state);
    void updateBgColor();

protected:
    void initUi();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *event);

signals:
    void sigClicked();
    void sigDestroySelf();

private:
    bool m_bChecked = false;
    bool m_bMove = false;
    QPoint m_pressPoint;
    bool m_bPress = false;
    int m_nId = -1;
    TLPushButton *m_pRemoveBtn = nullptr;
    QFont m_ft;
    QPixmap m_bgPixmap;
    QSize m_itemSize;
    QFutureWatcher<void>* m_pFutureWatcher = nullptr;
    bool m_loading = false;
    QColor m_bgColor = QColor("#3aaea6");
    int m_nBranch = C7Version::Branch_Normal;

private slots:
    void onFutureFinished();
};

#endif // C7PageItem_H
