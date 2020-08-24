#ifndef TLWAITWIDGET_H
#define TLWAITWIDGET_H

#include <QWidget>

//全屏显示的等待窗口

class TLWaitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TLWaitWidget(int width, int height, QWidget *parent = nullptr);

    void setWaitText(const QString& text);
    void setCurrentFont(const QFont& font);
    void setWaitTime(int time);//设置超时时间，单位ms
protected:
    void paintEvent(QPaintEvent* event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private slots:
    void onCircleTimeout();
private:
    //获取桌面大小
    QSize getDesktopSize(int screen = -1);
    void getScaleFactor();
private:
    bool m_bCircle = false;
    int m_nAngle = 0;
    int m_nCircleCount = 0;
    QFont m_font;
    qreal m_rScaleFactor = 1.0;
    QString m_strWaitText;
    QTimer* m_pCircleTimer = Q_NULLPTR;
    QPixmap m_loadImage;
    int m_nWaitTime = 600;//600*50=30000ms

};

#endif // TLWAITWIDGET_H
