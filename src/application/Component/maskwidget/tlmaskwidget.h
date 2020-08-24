#ifndef TLMASKWIDGET_H
#define TLMASKWIDGET_H

#include <QWidget>

class TLMaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TLMaskWidget(QWidget *parent = nullptr);
    ~TLMaskWidget();

    void setBackgroundBrush(const QBrush& brush);
    void setModal(bool bModal);
    void setChild(QWidget* widget, const QPoint& centerPos = QPoint());
    void setClickHide(bool bClickHide);
signals:
    void sigClicked();
    void sigHide();
protected:
    void paintEvent(QPaintEvent* event);
    void hideEvent(QHideEvent* event);
    void mousePressEvent(QMouseEvent* event);
    bool eventFilter(QObject *obj, QEvent *event);

    void setCenter(QWidget* widget, QPoint centerPos = QPoint());
private:
    QWidget* childWidget() const;
private:
    bool m_bClickHide = true;
    QBrush m_brush;
};

#endif // TLMASKWIDGET_H
