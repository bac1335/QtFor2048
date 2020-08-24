#include "c7settingscreenwidget.h"
#include "c7commondef.h"
#include "tlutility.h"

C7SettingScreenWidget ::C7SettingScreenWidget(int width,int height,C7BaseWidget *parent)
    :C7BaseWidget(width,height,parent)
{
    initUI();
    retranslateUi();
}

C7SettingScreenWidget ::~C7SettingScreenWidget ()
{

}

void C7SettingScreenWidget ::initUI()
{
    int current = Uti->value(SETTING_GROUP, SCREEN_KEY, 0).toInt();
    m_pSignalScreenWidget = createRadioWidget(current);
    m_pDualScreenWidget = createRadioWidget(current);
}

C7RadioWidget *C7SettingScreenWidget ::createRadioWidget(int currentSelect)
{
    int count = m_lstScreenChoice.size();
    C7RadioWidget* pRadioWidget = new C7RadioWidget(QSize(658 * m_rScaleFactor, 50 * m_rScaleFactor), m_rScaleFactor, this);
    pRadioWidget->move(0, count * (pRadioWidget->height() + m_nNormalSettingInterval * m_rScaleFactor) + 20 * m_rScaleFactor);
    pRadioWidget->setProperty(SCREEN_KEY, count);
    connect(pRadioWidget, &C7RadioWidget::sigClicked, this, &C7SettingScreenWidget ::onRadioWidgetClick);
    pRadioWidget->setChecked(count == currentSelect);
    m_lstScreenChoice.append(pRadioWidget);
    return pRadioWidget;
}

void C7SettingScreenWidget ::retranslateUi()
{
    if(m_pSignalScreenWidget != nullptr){
         m_pSignalScreenWidget->setText(QString(tr("Single screen")));
    }
    if(m_pDualScreenWidget != nullptr){
        m_pDualScreenWidget->setText(QString(tr("Double screen")));
    }
}

void C7SettingScreenWidget ::onRadioWidgetClick()
{
    C7RadioWidget *pWidget = qobject_cast<C7RadioWidget*>(sender());
    if (pWidget != nullptr)
    {
        if (m_lstScreenChoice.contains(pWidget))
        {
            foreach (C7RadioWidget* pRadioWidget, m_lstScreenChoice) {
                pRadioWidget->setChecked(false);
            }
            pWidget->setChecked(true);
            int type = pWidget->property(SCREEN_KEY).toInt();
            Uti->setValue(QString(SETTING_GROUP),SCREEN_KEY, type);
            emit sigCurrentScreenType((ScreenType) type);
        }
    }
}
