#include "tllinguist.h"
#include <QApplication>
#include <QMetaEnum>
#include <QDebug>
#include <QFile>
TLLinguist::TLLinguist() : QObject()
{
    loadLanguageFiles();
    m_currentLanguage = systemLanguage();
}

/**
 * @brief TLLinguist::loadLanguageFiles
 *  * 翻译文件名对应枚举名
 */
void TLLinguist::loadLanguageFiles()
{
#if 1
    QMetaEnum enumor = this->metaObject()->enumerator(this->metaObject()->indexOfEnumerator("Language"));
    for(int i = 0; i<enumor.keyCount(); i++){
        QString languagefile = QString("%1.qm").arg(enumor.key(i));
        m_languageFileMap.insert(enumor.value(i), languagefile);
    }
#else
    m_languageFileMap.insert(Zh_SM, "Zh_SM.qm");
    m_languageFileMap.insert(Zh_TR, "Zh_TR.qm");
    m_languageFileMap.insert(English, "English.qm");
    m_languageFileMap.insert(Japanese, "Japanese.qm");
    m_languageFileMap.insert(Korean, "Korean.qm");
    m_languageFileMap.insert(Russian, "Russian.qm");
    m_languageFileMap.insert(Arabic, "Arabic.qm");
    m_languageFileMap.insert(Espanol, "Espanol.qm");
#endif
}

/**
 * @brief TLLinguist::setLanguage
 * 设置软件语言，会触发QEvent::LanguageChange 信号
 * @return
 */
bool TLLinguist::setLanguage(TLLinguist::Language index)
{
    if(!m_languageFileMap.contains(index)){
        return false;
    }
    QString languagefile = QString(":/linguist/") + m_languageFileMap.value(index);
    Q_ASSERT(QFile::exists(languagefile));
    if(m_translator.load(languagefile)){
        m_currentLanguage = index;
        qApp->installTranslator(&m_translator);
        return true;
    }
    return false;
}

/**
 * @brief TLLinguist::systemLanguage
 * 返回当前系统的语言类型
 */
TLLinguist::Language TLLinguist::systemLanguage()
{
    QLocale::Language language = QLocale::system().language();
    Language languageindex = English;
    switch (language) {
    case QLocale::Chinese:{
        QLocale::Country country = QLocale::system().country();
        if (country == QLocale::China){
            languageindex = Zh_SM;
        }else{
            languageindex = Zh_TR;
        }
    }
        break;
    case QLocale::English:{
        languageindex = English;
    }
        break;
    case QLocale::Japanese:{
        languageindex = Japanese;
    }
        break;
    case QLocale::Korean:{
        languageindex = Korean;
    }
        break;
    case QLocale::Russian:{
        languageindex = Russian;
    }
        break;
    case QLocale::Arabic:{
        languageindex = Arabic;
    }
        break;
    default:{
        languageindex = English;
    }
        break;
    }
    return languageindex;
}

/**
 * @brief TLLinguist::currentLanguage
 * 当前软件设置的语言类型
 */
TLLinguist::Language TLLinguist::currentLanguage() const
{
    return m_currentLanguage;
}
