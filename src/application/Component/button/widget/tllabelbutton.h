#ifndef TLLabelButton_H
#define TLLabelButton_H

/*
 * @brief   :实现文字式的按钮
 * @author  :Lynn
 * @date    :2019.09.05
 */
#include <QLabel>

class TLLabelButton : public QLabel
{
    Q_OBJECT
public:
    explicit TLLabelButton(QWidget *parent=nullptr);
    explicit TLLabelButton(const QString &text, QWidget *parent=nullptr);
    void setColor(QColor color);
    void setChecked(bool b){m_bChecked = b;}

signals:
    void sigClicked(bool check);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    bool m_bPressed = false;
    bool m_bMoved = false;
    QPoint m_pressPoint;
    bool m_bChecked = false;

};

#endif // TLLabelButton_H
