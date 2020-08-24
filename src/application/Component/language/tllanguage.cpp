#include "tllanguage.h"

#include <QLocale>
#include <QMetaEnum>
#include <QTranslator>
#include <QApplication>
#include <QFile>

TLLanguage::TLLanguage(bool bUseSystem)
{
    m_pTranslator = new QTranslator(this);
    loadLanguageFiles();
    m_currentLanguage = getSystemLanguage();

    if (bUseSystem) {
        setLanguage(m_currentLanguage);
    }
}


bool TLLanguage::setLanguage(TLLanguage::Language_Type language)
{
    if (!m_languageFileMap.contains(language)) {
        return false;
    }

    QString languageFilePath = QString(":/linguist/") + m_languageFileMap.value(language);
    Q_ASSERT(QFile::exists(languageFilePath));

    if (m_pTranslator->load(languageFilePath)) {
        return qApp->installTranslator(m_pTranslator);
    }

    return false;
}

TLLanguage::Language_Type TLLanguage::getCurrentLanguage() const
{
    return m_currentLanguage;
}

TLLanguage::Language_Type TLLanguage::getSystemLanguage()
{
    Language_Type systemLanguageType;

    QLocale::Language systemLanguage = QLocale::system().language();
    switch (systemLanguage) {
    case QLocale::Chinese: {
        QLocale::Country country = QLocale::system().country();
        if (country == QLocale::China) {
            systemLanguageType = Zh_SM;
        } else {
            systemLanguageType = Zh_TR;
        }
    }
        break;
    case QLocale::English:
        systemLanguageType = English;
        break;

    default:
        systemLanguageType = English;
        break;
    }

    return systemLanguageType;
}

void TLLanguage::loadLanguageFiles()
{
    QMetaEnum enumor = this->metaObject()->enumerator(this->metaObject()->indexOfEnumerator("Language_Type"));
    QString strFilePath;
    for (int i = 0; i < enumor.keyCount(); ++i)
    {
        strFilePath = QString("%1.qm").arg(enumor.key(i));
        m_languageFileMap.insert(enumor.value(i), strFilePath);
    }
}
