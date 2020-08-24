#ifndef C7REGISTERWIDGET_H
#define C7REGISTERWIDGET_H
#include "c7basewidget.h"
#include "../c7settingdef.h"
#include <QLabel>
#include "tlpushbutton.h"
#include "lineeditwidget/c7commonlineedit.h"
#include "c7oem.h"
#include "tlloadingbtn.h"
#include "tlloading.h"

/*
 * @brief   : 激活界面
 * @author  : lls
 * @date    : 2019.9.05
 */

class QStackedWidget;

class C7RegisterWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7RegisterWidget(int width, int height, C7BaseWidget* parent = nullptr);
    ~C7RegisterWidget();
    void initUI();

protected:
    void showEvent(QShowEvent *ev);
    void hideEvent(QHideEvent *ev);
    void resizeEvent(QResizeEvent *event);
    void retranslateUi();

private:
    bool checkIsOemPrivateStr(QString licence);
    void updateUI();
    void updateOemTypeByLicens();
    bool updateOemTypeByDevice();
    QString fixInputText(const QString &text);
    QString getVerType(int oemType);
    void setDoubleScreenEnable(bool bEnable);
    void setScanTips(QString tips);
    void _showRegisterCode();

private Q_SLOTS:
    void onVerifyResult(int result);
    void onTextChange(QString str);
    void onUpdate();
    void onClear();

signals:
    void sigShowKeyBoard();
    void sigHideKeyBoard();
    void sigVersionChange(C7OEM::OEMType oemType);

private:
    //显示窗口用
    bool m_bDoubleScreen = false;
    QLabel* m_registerText = nullptr;
    QLabel* m_serialNumberText = nullptr;
    TLPushButton* m_okButton = nullptr;
    TLPushButton* m_clearButton = nullptr;
    C7CommonLineEdit* m_lineEdit = nullptr;
    QLabel * m_pTipsLabel = nullptr;  //红色文字提示序列号相关
    QLabel * m_pQRCodeImg = nullptr;  //二维码
    QLabel * m_pTipsScan = nullptr;   //扫码激活
    TLLoadingBtn * m_pLoadingBtn = nullptr;
    QTimer * m_pCheckNetWorkTimer = nullptr;
    QString m_strUrl; //生成的二维码地址
    TLLoading * m_pLoading = nullptr;
    QWidget *m_pRegisterWidget = nullptr;
    QWidget *m_pRegisterResultWidget = nullptr;
    QLabel *m_pRegisterResultLabel = nullptr;
    QStackedWidget *m_pStackWidget = nullptr;

    int m_version = 0;

};


#endif // C7REGISTERWIDGET_H
