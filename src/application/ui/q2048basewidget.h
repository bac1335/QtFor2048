#ifndef Q2048BASEWIDGET_H
#define Q2048BASEWIDGET_H

#include <QWidget>
#include <QMap>
#include <QGridLayout>

class Q2048BoxItem;
class Q2048BaseWidget : public QWidget{
    Q_OBJECT
public:
    explicit Q2048BaseWidget(QWidget* parent = nullptr);
    void setBoxVale(int value){m_boxMax = value;}
    int boxVale(){return m_boxMax;}

private:
    void initColor();

protected:
    virtual Q2048BoxItem* creatBox(){return nullptr;}

protected:
    qreal                   m_scaleFactory = 1.0;
    QMap<int,QString>       m_colorMap;
    QVector<QVector<int>>   m_data;
    int                     m_boxMax = 5;
    QGridLayout*            m_boxLayout = nullptr;
};


#endif // Q2048BASEWIDGET_H
