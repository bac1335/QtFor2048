#include "c7settingpalmwidget.h"
#include "tlutility.h"

C7SettingPalmWidget ::C7SettingPalmWidget(int width,int height,C7BaseWidget *parent)
    :C7BaseWidget(width,height,parent)
{
    initUI();
    retranslateUi();
}

C7SettingPalmWidget ::~C7SettingPalmWidget ()
{

}

void C7SettingPalmWidget ::initUI()
{
    int current = Uti->value(SETTING_GROUP, PALM_ERASE_KEY, 0).toInt();
    m_pPalmEraseWidget = createRadioWidget(current);
    m_pNoPalmWidget = createRadioWidget(current);
}

C7RadioWidget *C7SettingPalmWidget ::createRadioWidget(int currentSelect)
{
    int count = m_lstChoice.size();
    C7RadioWidget* pRadioWidget = new C7RadioWidget(QSize(658 * m_rScaleFactor, 50 * m_rScaleFactor), m_rScaleFactor, this);
    pRadioWidget->move(0, count * (pRadioWidget->height() + m_nNormalSettingInterval * m_rScaleFactor) + 20 * m_rScaleFactor);
    pRadioWidget->setProperty(PALM_ERASE_KEY, count);
    connect(pRadioWidget, &C7RadioWidget::sigClicked, this, &C7SettingPalmWidget ::onRadioWidgetClick);
    pRadioWidget->setChecked(count == currentSelect);
    m_lstChoice.append(pRadioWidget);
    return pRadioWidget;
}

void C7SettingPalmWidget ::retranslateUi()
{
    if(m_pNoPalmWidget != nullptr){

         m_pNoPalmWidget->setText(QString(tr("No Palm")));
    }
    if(m_pPalmEraseWidget != nullptr){
        m_pPalmEraseWidget->setText(QString(tr("Palm Erase")));
    }
}

void C7SettingPalmWidget ::onRadioWidgetClick()
{
    C7RadioWidget *pWidget = qobject_cast<C7RadioWidget*>(sender());
    if (pWidget != nullptr)
    {
        if (m_lstChoice.contains(pWidget))
        {
            foreach (C7RadioWidget* pRadioWidget, m_lstChoice) {
                pRadioWidget->setChecked(false);
            }
            pWidget->setChecked(true);
            int type = pWidget->property(PALM_ERASE_KEY).toInt();
            Uti->setValue(QString(SETTING_GROUP),PALM_ERASE_KEY, type);
            emit sigCurrentPalmType((PalmType) type);
        }
    }
}
