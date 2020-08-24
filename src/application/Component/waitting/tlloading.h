#ifndef TLLOADING_H
#define TLLOADING_H

#include <QWidget>
#include <QTimerEvent>
#include <QPainter>
#include <QPaintEvent>
/*
 * @brief   :等待加载框，模态
 * @author  :Lynn
 * @date    :2019.11.04
 */

class TLLoading : public QWidget
{
    Q_OBJECT
public:
    explicit TLLoading(bool modal = false,QWidget *parent = nullptr);

    void startWaiting();
    void stopWaiting();
    void setTimeoutTime(int time);
    bool isRunning() const;
    void setColor(const QColor &color);
protected:
    void timerEvent(QTimerEvent *e);
    void paintEvent(QPaintEvent *);

signals:
    /**
     * @brief sigWaitTimeout 等待超时信号
     */
    void sigWaitTimeout();

private:
    QSize m_sCentreSize;
    int m_iAngle = 0;
    int m_iTimerId = -1;
    int m_iTimerTimeoutId = -1;
    int m_iDisplayTime = 60;
    int m_iWaitTimeout = 20000; //超时20s
    QPixmap m_bg;
    QColor m_loadColor = QColor("#32bb85");
    qreal m_rScaleFactor = 1.0;

};

#endif // TLLOADING_H
