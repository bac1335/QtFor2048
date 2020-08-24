#include "../header/tlpaths.h"
#include <QStandardPaths>
#include <QDir>
#include <QApplication>
#include "header/tlutilitydef.h"

Q_GLOBAL_STATIC(TLPaths, tl_paths)//全局静态类

TLPaths::TLPaths()
{
    init();
}

QString TLPaths::path(int type)
{
    QString path = tl_paths()->m_paths.value(type);

    QDir dir(path);
    if(!dir.exists()){
        dir.mkpath(path);
    }
    return path;
}
void TLPaths::init()
{
    QString fileName = qApp->applicationName();

    // Config
#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
    m_paths.insert(Config, QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).append("/"));
#elif defined(Q_OS_MAC)
    m_paths.insert(Config, QDir::homePath() + QLatin1String("/Library/Application Support/") + fileName);
#endif

    m_paths.insert(HomePath, QDir::homePath()+ QLatin1String("/") + fileName);

    m_paths.insert(Profiles, m_paths.value(Config) + QLatin1String("profiles"));

    m_paths.insert(Cache, m_paths.value(Config) + QLatin1String("cache"));

    m_paths.insert(Temp, QDir::tempPath() + QLatin1String("/") + fileName);;
    m_paths.insert(Documents, QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                   + QLatin1String("/") + fileName);

    m_paths.insert(Apppath, QApplication::applicationDirPath());
    m_paths.insert(Desktop, QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
}
