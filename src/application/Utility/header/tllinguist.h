#ifndef TLLinguist_H
#define TLLinguist_H

#include <QObject>
#include <QTranslator>
#include <QMap>
class TLLinguist : public QObject
{
    Q_OBJECT
    Q_ENUMS(Language)//枚举值反射用
public:
    enum Language{
        Zh_SM=1,
        Zh_TR,
        English,
        Japanese,
        Korean,
        Russian,
        Arabic,
        Espanol,
        Zh_TW,
    };
    TLLinguist();
    bool setLanguage(Language index);
    Language currentLanguage() const;
    static Language systemLanguage();
private:
    void loadLanguageFiles();
private:
    QTranslator m_translator;
    Language m_currentLanguage = English;
    QMap<int,QString> m_languageFileMap;
};

#endif // TLLinguist_H
