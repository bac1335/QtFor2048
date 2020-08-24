#ifndef TLMESSAGEBOX_H
#define TLMESSAGEBOX_H

#include "base/widget/tlbasecontrol.h"

/****************************************************************************
 * @author  :Hongcansi
 * @date    :2018.11.07
 * @brief   :对话框
 * @other   :在使用前根据需要设置一些静态变量的值
****************************************************************************/

#include <QTimer>

#define ButtonType "ButtonType"

class QLabel;
class TLPushButton;
class TLMaskWidget;

class TLMessageBox : public TLBaseControl
{
    Q_OBJECT
public:
    enum  MessageBox_Type {
        QuestionBox,
        InformationBox,
        AboutBox,
        WaittingBox,
    };

    enum Button_Type{
        Cancel_Button,
        Ok_Button,
        Other_Button,
    };

    enum CountDown_Type{
        Second,
        Minute,
        Hour
    };

    explicit TLMessageBox(QWidget *parent = 0);

    //增加图片icon提示接口
    void setUseIcon(bool useIcon,QColor color,QString path, int width , int height,QString text,int round = 0);
    void setBoxType(MessageBox_Type boxType);
    void setCountDownNumber(int nSecond, CountDown_Type countType = Minute, bool bStart = true);
    void startCountDown();
    void stopCountDown();

    void setTitle(const QString& title);
    void setMessage(const QString& msg, int nWidth = -1, int nHeight = -1);

    TLPushButton* addCloseButton();
    TLPushButton* addPushButton(const QString& text, Button_Type btnType = Ok_Button);
    QList<TLPushButton*> getBtns();

    //根据传入的大小重新布局
    void relayout(int nWidth = -1, int nHeight = -1);
    //根据对话框类型，快速显示对话框
    static int showMessage(QWidget* parent, MessageBox_Type msgBoxType, const QString& strTitle,
                           const QString& strMsg, bool bMask = true);
    //初始化一些静态变量
    static void init();
    //以下是设置静态变量的接口
    static void setMinSize(int nMinWidth, int nMinHeight, qreal rScaleFactor = 1.0);
    static void setMaxSize(int nMaxWidth, int nMaxHeight, qreal rScaleFactor = 1.0);
    static void setScaleFactor(qreal rScaleFactor = -1);
    static void setBgPath(const QString& path);
    static void setOkBgNormal(const QString& path);
    static void setOkBgPress(const QString& path);
    static void setCancelBgNormal(const QString& path);
    static void setCancelBgPress(const QString& path);
    static void setFont(QFont font);
signals:
    void sigClose();
    void sigTimeout();
    void sigOk();
    void sigCancel();
    void sigButtonClicked(TLPushButton *pBtn);
    void sigButtonType(Button_Type btnType);

protected:
    void paintEvent(QPaintEvent *);
private slots:
    void onCountTimeout();
    void onButtonClicked();
private:
    TLMaskWidget* addMask(QWidget *widget);
    int getStringWidth(const QString& str);
    void initCountTimer(bool bStart = false);
    QString getTimeString(int count);

    static int question(QWidget* parent, const QString& strTitle, const QString& strMsg,
                        bool bMask = true, bool bModel = false);
    static int information(QWidget* parent, const QString& strTitle, const QString& strMsg,
                           bool bMask = true);
    static void initScaleFactor(qreal width = 1920.0, qreal height = 1080);
private:
    QPixmap m_bg;
    QPixmap m_icon;
    QColor m_bgColor = QColor(92,145,124);
    bool m_useIcon = true;
    int m_round = 12;
    int m_iconWidth;
    int m_iconHeight;
    QString m_IconText;
    MessageBox_Type m_boxType;
    QString m_strTitle;
    QString m_strMessage;       //这个是传入的
    QLabel* m_pDisplayLabel = Q_NULLPTR;
    TLPushButton* m_pCloseBtn = Q_NULLPTR;
    QList<TLPushButton*> m_btns;

    int m_nTimerCount = 0;
    int m_nTimerCountMax = -1;
    CountDown_Type m_countType;
    QTimer* m_pCountTimer = Q_NULLPTR;
    QString m_strDisplayMessage; //实际显示的字符串，用于倒计时加上秒数

    static bool ms_bInit;
    static int ms_nMinWidth;
    static int ms_nMinHeight;
    static int ms_nMaxWidth;
    static int ms_nMaxHeight;
    static qreal ms_rScaleFactor;
    static bool ms_bInitFont;
    static QFont ms_font;
    static QString ms_strBgPath;
    static QString ms_strOkBgNormal;
    static QString ms_strOkBgPress;
    static QString ms_strCancelBgNormal;
    static QString ms_strCancelBgPress;
};

#endif // TLMESSAGEBOX_H
