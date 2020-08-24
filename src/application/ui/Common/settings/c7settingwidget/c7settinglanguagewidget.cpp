#include "c7settinglanguagewidget.h"
#include "c7commondef.h"
#include "tlutility.h"
#include "c7application.h"

C7SettingLanguageWidget::C7SettingLanguageWidget(int width,int height,C7BaseWidget *parent)
    :C7BaseWidget(width,height,parent)
{
    initUI();
    retranslateUi();
}

C7SettingLanguageWidget::~C7SettingLanguageWidget()
{

}

void C7SettingLanguageWidget::initUI()
{
    int current = Uti->value(SETTING_GROUP, LANGUAGE, 0).toInt();
    m_pSimplifiedWidget = createRadioWidget(current);
    m_pComplexWidget = createRadioWidget(current);
    m_pEnglishWidget = createRadioWidget(current);
}

C7RadioWidget *C7SettingLanguageWidget::createRadioWidget(int currentSelect)
{
    int count = m_lstLanguageChoice.size();
    C7RadioWidget* pRadioWidget = new C7RadioWidget(QSize(658 * m_rScaleFactor, 50 * m_rScaleFactor), m_rScaleFactor, this);
    pRadioWidget->move(0, count * (pRadioWidget->height() + m_nNormalSettingInterval * m_rScaleFactor) + 20 * m_rScaleFactor);
    pRadioWidget->setProperty(LANGUAGE, count);
    connect(pRadioWidget, &C7RadioWidget::sigClicked, this, &C7SettingLanguageWidget::onRadioWidgetClick);
    pRadioWidget->setChecked(count == currentSelect);
    m_lstLanguageChoice.append(pRadioWidget);
    return pRadioWidget;
}

void C7SettingLanguageWidget::retranslateUi()
{
    if(m_pSimplifiedWidget != nullptr){
         m_pSimplifiedWidget->setText(tr("Simplified Chinese"));
    }
    if(m_pComplexWidget != nullptr){
        m_pComplexWidget->setText(tr("Traditional Chinese"));
    }
    if(m_pEnglishWidget != nullptr){
        m_pEnglishWidget->setText(tr("English"));
    }
}

void C7SettingLanguageWidget::onRadioWidgetClick()
{
    C7RadioWidget *pWidget = qobject_cast<C7RadioWidget*>(sender());
    if (pWidget != nullptr)
    {
        if (m_lstLanguageChoice.contains(pWidget))
        {
            foreach (C7RadioWidget* pRadioWidget, m_lstLanguageChoice) {
                pRadioWidget->setChecked(false);
            }
            pWidget->setChecked(true);
            int sizeType = pWidget->property(LANGUAGE).toInt();

            Uti->setValue(QString(SETTING_GROUP), LANGUAGE, sizeType);
            cApp->linguist()->setLanguage(static_cast<TLLinguist::Language>(sizeType + 1));
        }
    }
}

