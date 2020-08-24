#ifndef TLPATHS_H
#define TLPATHS_H

#include <QObject>
#include <QMap>
class TLPaths
{
public:
    explicit TLPaths();

    static QString path(int type);

private:
    void init();
    QMap<int, QString> m_paths;
};

#endif // C5PATHS_H
