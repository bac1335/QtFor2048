#ifndef C7CONTENIMAGEITEM_H
#define C7CONTENIMAGEITEM_H

#include "c7basewidget.h"
#include "c7commondef.h"

class QLabel;
class QMouseEvent;

/**
  * @brief 图片内容控件，能够根据图片的大小自适应
  * @author xinde song
  * @date 2019-09-12
  */
class C7ContenImageItem : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7ContenImageItem(QWidget *parent = nullptr);

    void setImage(const QString str);
    bool getChecked(){return  m_bChecked;}
    void setItemCheck(bool f);

protected:
    void initUi();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);

private:
    void adjustWidget(int w, int h);

private:
    int m_nMaxWidth;
    int m_nMaxHeigth;
    int m_nMinWidth;
    int m_nMinHeight;

    QPixmap m_bg;
    QPixmap m_checkIcon;
    QPixmap m_unCheckIcon;

    bool m_bChecked;
    bool m_bMove = false;
    QPoint m_pressPoint;
    bool m_bPress = false;

    QString m_imgStr;

signals:
    void sigItemClick(bool,Item_Type,QString);

};

#endif // C7CONTENIMAGEITEM_H
