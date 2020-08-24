#ifndef TLLANGUAGE_H
#define TLLANGUAGE_H

#include <QObject>
#include <QMap>

class QTranslator;

class TLLanguage : public QObject
{
    Q_OBJECT
    Q_ENUMS(Language_Type)
public:
    enum Language_Type {
        Zh_SM=1,
        Zh_TR,
        English,
    };
    explicit TLLanguage(bool bUseSystem = false);

    bool setLanguage(Language_Type language);
    Language_Type getCurrentLanguage() const;

    static Language_Type getSystemLanguage();
private:
    void loadLanguageFiles();
private:
    QTranslator *m_pTranslator;
    Language_Type m_currentLanguage;
    QMap<int, QString> m_languageFileMap;
};

#endif // TLLANGUAGE_H
