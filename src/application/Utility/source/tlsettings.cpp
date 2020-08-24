#include "tlsettings.h"

TLSettings::TLSettings(const QString &fileName)
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    m_setting = new QSettings(fileName, QSettings::IniFormat);
}

TLSettings::~TLSettings()
{
    delete m_setting;
    m_setting = 0;
}

QString TLSettings::path() const
{
    return m_setting->fileName();
}

void TLSettings::setValue(const QString &group, const QString &key, const QVariant &value)
{
    m_setting->beginGroup(group);
    m_setting->setValue(key, value);
    m_setting->endGroup();
    m_setting->sync();
}

QVariant TLSettings::value(const QString &group, const QString &key, const QVariant &defaultValue)
{
    m_setting->beginGroup(group);
    QVariant value = m_setting->value(key, defaultValue);
    m_setting->endGroup();
    return value;
}

bool TLSettings::containts(const QString &group, const QString &key)
{
    m_setting->beginGroup(group);
    bool bcontain = m_setting->contains(key);
    m_setting->endGroup();
    return bcontain;
}

void TLSettings::remove(const QString &group, const QString &key)
{
    m_setting->beginGroup(group);
    m_setting->remove(key);
    m_setting->endGroup();
}

void TLSettings::sync()
{
    m_setting->sync();
}
