#include "c7networkdiskmanager.h"
#include "tlutility.h"
#include <QDir>
#include "tlutility.h"
#include <QJsonArray>
#include "c7oem.h"

C7NetworkDiskManager::C7NetworkDiskManager(QObject *parent) : QObject(parent)
{
    m_pHttpManager = TLHttpFactory::GetInstance()->createHttp(); 
    connect(m_pHttpManager,SIGNAL(finished(QNetworkReply*)),
            this,SLOT(replyFinished(QNetworkReply*)));
    m_pDownload = new TLHttpDownLoad();
    connect(m_pDownload,&TLHttpDownLoad::sigFinished,this,&C7NetworkDiskManager::onDownloadFinished);
    connect(m_pDownload,&TLHttpDownLoad::sigError,this,[=](){
         emit sigNetworkError(ERROR_CODE_FILEDOWNLOAD);
    });
    connect(m_pDownload,&TLHttpDownLoad::sigDownLoadPercent,this,&C7NetworkDiskManager::sigDownLoadPercent);
    connect(m_pDownload,&TLHttpDownLoad::sigError,this,&C7NetworkDiskManager::sigError);
}

C7NetworkDiskManager::~C7NetworkDiskManager()
{
    if(m_pDownload){
        delete m_pDownload;
        m_pDownload = nullptr;
    }
    if(m_pHttpManager){
        delete m_pHttpManager;
        m_pHttpManager = nullptr;
    }
    if(m_pReplyTimeOut){
        delete m_pReplyTimeOut;
        m_pReplyTimeOut = nullptr;
    }
    if(m_pReply){
        delete m_pReply;
        m_pReply = nullptr;
    }
}

void C7NetworkDiskManager::setPage(const int &pagenum)
{
    m_param = QString::number(pagenum);
}

void C7NetworkDiskManager::startYun(int page)
{
    QJsonObject json;
    json.insert("user_id",cOem->getLoginState().userid);
    json.insert("page",QString::number(page));
    m_pHttpManager->setUrl(DEF_HTTP_GETCLOUD_ADDR);
    m_pHttpManager->setBody(&json);
    m_pReply =static_cast<QNetworkReply*>(m_pHttpManager->send());

    m_pReplyTimeOut = new QReplyTimeout(m_pReply,20000);//处理超时
    connect(m_pReplyTimeOut,&QReplyTimeout::timeout,[=](){
        qDebug() << "ALWeChatCore reply time out." << m_pReply->error();
        emit sigNetworkError(ERROR_CODE_NETWORKTIMEOUT);
    });
}

void C7NetworkDiskManager::startDisk(int pageId)
{
    QJsonObject json;
    json.insert("user_id",QString::number(cOem->getLoginState().userid));
    json.insert("floder_id",QString::number(pageId));
    m_pHttpManager->setUrl(DEF_HTTP_DISK_ADDR);
    m_pHttpManager->setBody(&json);
    m_pReply =static_cast<QNetworkReply*>(m_pHttpManager->send());

    m_pReplyTimeOut = new QReplyTimeout(m_pReply,20000);//处理超时
    connect(m_pReplyTimeOut,&QReplyTimeout::timeout,[=](){
        qDebug() << "ALWeChatCore reply time out." << m_pReply->error();
        emit sigNetworkError(ERROR_CODE_NETWORKTIMEOUT);
    });
}

void C7NetworkDiskManager::replyFinished(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray ba = reply->readAll();
    qDebug() <<__FUNCTION__<< " server status code = "<< statusCode << " ba=" << ba;

    QJsonParseError jsonpe;
    QJsonDocument json = QJsonDocument::fromJson(ba, &jsonpe);
    if (jsonpe.error == QJsonParseError::NoError && statusCode == 200)
    {
            QJsonObject rootObj = json.object();
            if (rootObj.contains("status_code"))
            {
                if(rootObj.value("status_code").toInt() == API_SUCCESS_CODE){//成功
                    if(rootObj.contains("result"))
                    {
                        QVariant data = rootObj.value("result").toVariant();
                        emit sigFileList(data);
                    }

                }
                else if(rootObj.value("status_code").toInt() == API_ERROR_DEVICE_EXCEPTION){
                    qDebug() <<__FUNCTION__<< "wechat error_code = 500" << "msg = " << rootObj.value("msg").toString();
                    emit sigNetworkError(ERROR_CODE_SERVER_ABNORMAL);
                }
            }
    }
    else
    {
        //请求超时 服务器返回错误，重新请求查询
        qDebug() << "wechat error:" << jsonpe.errorString() << "server return status code = " <<statusCode;
        emit sigNetworkError(ERROR_CODE_NETWORK);
    }

}

void C7NetworkDiskManager::onDownloadFinished(QString url, qreal percent, QString file)
{
    qDebug() << __FUNCTION__<<"file onDownloadFinished " <<file;
//    emit sigWechatQRCode(file);
    if(percent != -1){
        emit sigDownFinished(file);
    }
}

TLHttpDownLoad::AddResult C7NetworkDiskManager::downloadFile(QString name,QString url,QString type)
{
    qDebug() << "--->Lynn<---" << __FUNCTION__ <<"userid =" << cOem->getLoginState().userid << url;
    QString path = Uti->path(Cache) + "/yundownload/" + QString::number(cOem->getLoginState().userid);

    m_pDownload->setTempDir(path);

    type == ""?type:"."+type;

    QString filePath = QString("%1/%2")
                       .arg(path)
                       .arg(name);

    QFile file(filePath);
    if(file.exists()){
        return TLHttpDownLoad::Exist;
    }

    TLHttpDownLoad::AddResult dat = m_pDownload->downLoad(url,name);
    qDebug() << "--->lls<---" << __FUNCTION__ << "download yun file" << "filePath = " << filePath << ":" << dat;
    return dat;

}
