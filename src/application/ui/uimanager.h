#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "QObject"
#include "q2048widget.h"
#include "q2048numbermanager.h"

class Uimanager : public QObject{
    Q_OBJECT
public:
    explicit Uimanager(QObject* parent = nullptr);

private:
    void init();

private:
    Q2048Widget*        m_mainWidget = nullptr;
    qreal               m_scaleFactory = 1.;
    Q2048ManagerBase*   m_manager = nullptr;
};

#endif // UIMANAGER_H
