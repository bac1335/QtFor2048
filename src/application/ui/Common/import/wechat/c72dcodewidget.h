#ifndef C72DCODEWIDGET_H
#define C72DCODEWIDGET_H

#include "c7basewidget.h"
#include "c7wechatcodemanager.h"
#include "tlloadingbtn.h"

class QLabel;
class QStackedWidget;
class TLPushButton;
class QPropertyAnimation;

enum ConnectType{
    Connect_Normal,
    Connect_Not,
};

/**
  * @brief 二维码窗口，包含重连
  * @author xinde song
  * @date 2019-09-11
  */
class C72dCodeWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C72dCodeWidget(int w, int h, QWidget *parent = nullptr);
    void set2dCodeSize(int w, int h);
    void set2dCodeImage(const QPixmap &pixmap);

    void setConnectType(ConnectType type);

protected:
    void initUi();
    void retranslateUi();
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private slots:
    void onWechatQRCode(QString codeUrl);
private:
    void setScanTips(QString tips);
private:
    QPixmap m_2dPixmap;

    QWidget *m_p2d;
    QLabel *m_p2dCodeLabel = nullptr;
    QLabel *m_p2dCodeText = nullptr;

    QStackedWidget *m_pStackWidget = nullptr;

    QWidget *m_pFlushWidget = nullptr;
    QLabel *m_pFlushTipsLabel = nullptr;

    QPropertyAnimation *m_pAnimation = nullptr;

    C7WeChatCodeManager * m_pWechatCode = nullptr; //负责微信二维码请求
    TLLoadingBtn * m_pLoadingBtn = nullptr;
    QTimer * m_pCheckNetWorkTimer = nullptr;
    QLabel * m_pProTips = nullptr;//升级专业版提示
};

#endif // C72DCODEWIDGET_H
