#include "uimanager.h"
#include "Utility/tlutility.h"

Uimanager::Uimanager(QObject *parent):
    QObject (parent)
{
    init();
}

void Uimanager::init()
{
    m_scaleFactory = Uti->getScaleFactor();
    m_mainWidget = new Q2048Widget(480 * m_scaleFactory,480 * m_scaleFactory);
    m_mainWidget->show();
    m_manager = new Q2048NumberManager(this);
    m_manager->setBoxMaxBox(m_mainWidget->boxVale());
    connect(m_mainWidget,&Q2048Widget::sigKeyPress,m_manager,&Q2048NumberManager::onKeyPress);
    connect(m_manager,&Q2048NumberManager::sigSendData,m_mainWidget,&Q2048Widget::onSendData);
    connect(m_mainWidget,&Q2048Widget::sigReinit,m_manager,&Q2048NumberManager::onReinit);


    m_manager->startInit();
}
