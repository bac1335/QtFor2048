#ifndef Q2048NUMBERMANAGER_H
#define Q2048NUMBERMANAGER_H

#include "q2048managebase.h"

class Q2048NumberManager : public Q2048ManagerBase{
    Q_OBJECT
public:
    explicit Q2048NumberManager(QObject* parent = nullptr);
    void startInit() override;

private:
    void goRight() override;
    void goLeft() override;
    void goUp() override;
    void goDown() override;
    bool isOver() override;
    void insertRand() override;

private:
    bool hasNull();
    void transXToY(QVector<QVector<int>> &list1, QVector<QVector<int>>& list2); //从左向右旋转
    void transYToX(QVector<QVector<int>> &list1, QVector<QVector<int>>& list2); //从右向左旋转

private:
    void  doAction(GameDir dir) override;
};

#endif // Q2048NUMBERMANAGER_H
