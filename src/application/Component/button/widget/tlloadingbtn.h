#ifndef TLLOADINGBTN_H
#define TLLOADINGBTN_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTransform>
#include <QPixmap>
#include <QTimer>

class TLLoadingBtn : public QWidget
{
    Q_OBJECT
public:
    explicit TLLoadingBtn(int w, int h, QWidget *parent = nullptr);
    void setLoadingIcon(const QPixmap &pixmap);
    void setLoadingStatus(bool status);
    void setTimeOut(int time);
    bool isActive(){
        return  m_pTimer->isActive();
    }

protected:
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
private slots:
    void onTimerOut();
signals:
    void sigTimeOut();
    void sigClicked();
private:
    QPropertyAnimation * m_pRoundAnimation = nullptr;
    bool m_bPressed = false;
    QTimer * m_pTimer = nullptr;
    QPixmap m_loadingIcon;
    qreal m_rRotate = 0;
    int m_nTimeOut = 20000;
    int m_nCount = 0;
};

#endif // TLLOADINGBTN_H
