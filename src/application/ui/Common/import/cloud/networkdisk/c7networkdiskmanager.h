#ifndef C7NETWORKDISKMANAGER_H
#define C7NETWORKDISKMANAGER_H

#include <QObject>
#include "tlhttpfactory.h"
#include "tlhttpdownload.h"
#include <QNetworkReply>
#include "qreplytimeout.h"


/**
 * @brief The C7NetworkDiskManager class
 * 本类负责网盘的请求和下载。
 */

class C7NetworkDiskManager : public QObject
{
    Q_OBJECT
public:
    explicit C7NetworkDiskManager(QObject *parent = nullptr);
    ~C7NetworkDiskManager();
    void setPage(const int & pagenum);
    void startYun(int page);
    void startDisk(int pageId);
    TLHttpDownLoad::AddResult downloadFile(QString name,QString url,QString type); //下载文件，返回文件路径

signals: 
    void sigDownFinished(QString filePath);
    void sigNetworkError(int errorcode);
    void sigFileList(QVariant obj);
    void sigDownLoadPercent(QString url, qreal percent, QString file);
    void sigError(QString url, int type);

private slots:
    void replyFinished(QNetworkReply *reply);
    void onDownloadFinished(QString url, qreal percent, QString file);

private:

    TLHttpManager * m_pHttpManager = nullptr;
    TLHttpDownLoad * m_pDownload = nullptr;
    QNetworkReply * m_pReply = nullptr;
    QReplyTimeout * m_pReplyTimeOut = nullptr;
    QString m_param = "1";
};


#endif // C7NETWORKDISKMANAGER_H
