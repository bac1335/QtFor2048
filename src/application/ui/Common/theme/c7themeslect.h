#ifndef C7THEMESLECT_H
#define C7THEMESLECT_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPixmap>
#include <QLabel>

/**
  * @brief 主题选择item
  * @author lls
  * @date 2019-09-11
  */

class C7ThemeSlect : public QWidget
{
    Q_OBJECT
public:    
    C7ThemeSlect(QString imgPath, qreal scale,QWidget *parent = nullptr);
    void setChecked(bool check);

    void setChooseFrameImage(const QString &imagePath);
protected:
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void paintEvent(QPaintEvent *e);
signals:
    void sigThemeChosen();
public slots:
private:
    QString m_imgPath;
    bool m_bChosen;
    QPixmap m_img;

    QPixmap m_chooseFrameImage;
    qreal m_nScale;

};

#endif // C7THEMESLECT_H
