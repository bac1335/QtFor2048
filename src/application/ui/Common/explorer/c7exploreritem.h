#ifndef NPEXPLORERITEM_H
#define NPEXPLORERITEM_H

#include "c7basewidget.h"
#include <QTimer>

/*
 * @brief   :文件/文件夹item
 * @author  :hongcs
 * @date    :2018.11.06
 */

class C7ExplorerItem : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7ExplorerItem(QSize size, QWidget* parent = nullptr);
    void setText(const QString& strText);
    void setImage(const QPixmap& image);
    void setCheckedImage(const QPixmap& image);
    void setChecked(bool bChecked);
    void setCheckable(bool bCheckable);
    //这个是用来设置标识item的信息，灵活使用，如果是路径/文件选择用，就是完整路径
    void setItemID(const QString& strId);
signals:
    void sigItemId(const QString& itemPath, bool isDoubleClick);
    void sigItemDoubleClick(const QString& itemId);
    void sigItemClick(const QString& itemId);

protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void onScaleFactorChange();

private:
    bool m_bChecked = false;
    bool m_bCheckable = false;
    bool m_bMove = false;
    int m_nTextHeight = 50;
    int m_nNormalTextHeight = 50;
    QPoint m_pressPoint;
    QPixmap m_image;
    QPixmap m_checkImage;
    QString m_displayText;
    QString m_itemId;
    bool m_bIsClick = false;
};

#endif // NPEXPLORERITEM_H
