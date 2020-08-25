#ifndef Q2048MANAGEBASE_H
#define Q2048MANAGEBASE_H

#include <QObject>
#include <QVector>

class Q2048ManagerBase : public QObject{
    Q_OBJECT
public:
    explicit Q2048ManagerBase(QObject* parent = nullptr):QObject(parent){ }

    enum GameType{
        undefined,
        Number_2048,
    };

    enum  GameDir{
        g_Up,
        g_Down,
        g_Left,
        g_Right
    };

    void setBoxMaxBox(int value){ m_boxMax = value;}

    virtual  void startInit() = 0;

protected:
    virtual void init(){}
    virtual void doAction(GameDir dir) = 0;
    virtual bool isOver() = 0;
    virtual void goRight() = 0;
    virtual void goLeft() = 0;
    virtual void goUp() = 0;
    virtual void goDown() = 0;
    virtual void insertRand() = 0;
protected:
    GameType                m_gameType = undefined;
    QVector<QVector<int>>   m_data;
    int                     m_boxMax;
    bool                    m_isOver = false;

signals:
    void sigSendData(QVector<QVector<int>>,bool isOver = false);

public slots:
    void onKeyPress(int key){
        switch (key) {
        case Qt::Key_A:
        case Qt::Key_Left:
            doAction(g_Left);
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            doAction(g_Right);
            break;
        case Qt::Key_W:
        case Qt::Key_Up:
            doAction(g_Up);
            break;
        case Qt::Key_S:
        case Qt::Key_Down:
            doAction(g_Down);
            break;
        }
    }

    void onReinit(){m_data.clear();startInit();}
};

#endif //Q2048MANAGEBASE_H
