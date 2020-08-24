#ifndef C7CONTENTEXTITEM_H
#define C7CONTENTEXTITEM_H

#include "c7basewidget.h"
#include "c7commondef.h"

class QMouseEvent;
class QTextEdit;

/**
  * @brief 文字内容控件，能够根据文字长度自适应
  * @author xinde song
  * @date 2019-09-12
  */
class C7ContenTextItem : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7ContenTextItem(QWidget *parent = nullptr);
    void setText(const QString &text);
    bool getChecked(){return m_bChecked;}
    void setItemCheck(bool f);
    void setSelectBtnVisible(bool visible); //选中控件是否显示

protected:
    void initUi();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QString m_text;
    bool m_bChecked;
    QPixmap m_checkIcon;
    QPixmap m_unCheckIcon;
    int m_nMaxWidth;
    int m_nMinWidth;
    int m_lineNum = 1;

//    bool m_bChecked = false;
    bool m_bMove = false;
    QPoint m_pressPoint;
    bool m_bPress = false;
    bool m_bSelectVisible = true;

    QTextEdit *m_pTextEdit = nullptr;
    QWidget *m_pMaskWidget = nullptr;

signals:
    void sigItemClick(bool,Item_Type,QString);

};

#endif // C7CONTENTEXTITEM_H
