#ifndef TLSETTINGS_H
#define TLSETTINGS_H

#include <QObject>
#include <QVariant>
#include <QSettings>
/**
 * @brief The TLSettings class
 * 通用型配置类。
 * 对QSetting的再封装，让接口使用更方便
 */
class TLSettings
{
public:
    TLSettings(const QString &fileName);
    ~TLSettings();
    QString path() const;
    void setValue(const QString &group, const QString &key, const QVariant &value);
    QVariant value(const QString &group, const QString &key, const QVariant &defaultValue = QVariant());
    bool containts(const QString &group, const QString &key);
    void remove(const QString &group, const QString &key);
    void sync();
private:
    QSettings *m_setting;
};

#endif // C5SETTINGS_H
