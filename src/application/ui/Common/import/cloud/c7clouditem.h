#ifndef C7CLOUDITEM_H
#define C7CLOUDITEM_H

#include "c7basewidget.h"
#include "c7commondef.h"
#include "tlhttpdownload.h"

class QMouseEvent;

class C7CloudItem : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7CloudItem(int w, int h, QWidget *parent = nullptr);
    ~C7CloudItem();
    void setText(const QString& strText);
    void setImage(const QPixmap& image);
    void setFileImage(const QString image);
    void setCheckedImage(const QPixmap& image);
    void setChecked(bool bChecked);
    void sethasSelected(bool bChecked);
    void setHasDowload(bool f);
    void setCheckable(bool bCheckable);
    void setImageBg(const QString image);

    //下载动画接口
    void setEnableDownLoad(bool on);
    void setProgress(int progress);
    void setLoadingState(bool state);

    //获取文件信息 //这个是用来设置标识item的信息，灵活使用，如果是路径/文件选择用，就是完整路径
    void setItemID(const ItemType& strId);
    ItemType getItemId(){return m_itemId;}
    void setItemFloderID(const QString id){m_floder_id = id;}
    QString getItemFloderID(){return m_floder_id;}
    void setItemFloder(const QString floder){m_floder = floder;}
    QString getItemFloder(){return m_floder;}
    void setFileUrl(QString url){m_fileurl = url;}
    QString getFileUrl(){return m_fileurl;}
    QString getName(){return m_name;}
    void setItemType(const QString type){m_type = type;}
    QString getItemType(){return m_type;}
    void setItemEanled(bool f){m_enabled = f;}
    TLHttpDownLoad::AddResult downloadFile(QString userName);
    bool getHasDowloadState(){return m_isDownload;}
    void setCreatTime(const QString time){m_created_at = time;}
    QString getCreatTime(){return m_created_at;}

protected:
    void initUi();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *e);

private:
    QPixmap m_cloudBg;
    QPixmap m_fileBg;
    QPixmap m_checkIcon;
    QPixmap m_unCheckIcon;

    //下载相关
    int m_progress = 0;
    QPixmap m_pixmap_downLoad;
    QPixmap m_pixmap_stopdownload;
    QPixmap m_pixmap_backGround;
    QPixmap m_pixmap_start;
    QPixmap m_pixmap_stop;
    QPixmap m_fileIcon;
    QString m_downLoadUrl;
    QString m_downLoadName;

    //文件路径
    QString m_fileUrl;
    QString m_filePath;
    QString m_fileName;
    QSize m_fileItemSize;
    int m_offset = 0;

    QPixmap m_ItemBg;
    bool m_bChecked = false;
    bool m_bSelected = false;
    bool m_bMove = false;
    QPoint m_pressPoint;
    bool m_bPress = false;

    QString m_displayText;
    QPixmap m_checkImage;

    bool m_bCheckable;

    //yun文件数据信息
    ItemType m_itemId;
    QString m_floder_id;
    QString m_name;
    QString m_path;
    QString m_fileurl;
    QString m_created_at;
    QString m_type;
    QString m_floder;

    bool m_isDownloading = false;   //云文件下载中
    bool m_isDownloadSuspend = false;   //云文件下载暂停中
    bool m_enableDownLoad = false;  //云文件是否可以下载
    bool m_isDownload = false;      //文件是否下载完成

    bool m_enabled = true;          //文件是否点击

    TLHttpDownLoad * m_pDownload = nullptr;

private:
    void gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QRgb color);
    void initPanIcon(C7CloudItem *pItem,QString str);
    void drawPainterArc(QPainter&);

signals:
    void sigItemDoubleClick(ItemType);
    void sigItemFolderClick(C7CloudItem*);
    void sigItemClick(ItemType);
//    void sigItemClick(fileType);

private slots:
    void onDownLoadPercent(QString url, qreal percent, QString file);
    void onDownFinished(QString filePath);
    void onDownloadError(QString url, int type);


};

#endif // C7CLOUDITEM_H
