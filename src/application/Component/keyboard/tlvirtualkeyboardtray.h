#ifndef TLVIRTUALKEYBOARDTRAY_H
#define TLVIRTUALKEYBOARDTRAY_H

#include "base/widget/tlbasecontrol.h"

class TLPushButton;
class TLVirtualKeyboardBtn;

class TLVirtualKeyboardTray : public TLBaseControl
{
    Q_OBJECT
public:
    explicit TLVirtualKeyboardTray(qreal rScaleFactor = 1.0, QWidget *parent = nullptr);
    enum KeyName {
        KeyName_1 = 0,
        KeyName_2,
        KeyName_3,
        KeyName_4,
        KeyName_5,
        KeyName_6,
        KeyName_7,
        KeyName_8,
        KeyName_9,
        KeyName_0,
        KeyName_Back_Space,
        KeyName_Q,
        KeyName_W,
        KeyName_E,
        KeyName_R,
        KeyName_T,
        KeyName_Y,
        KeyName_U,
        KeyName_I,
        KeyName_O,
        KeyName_P,
        KeyName_A,
        KeyName_S,
        KeyName_D,
        KeyName_F,
        KeyName_G,
        KeyName_H,
        KeyName_J,
        KeyName_K,
        KeyName_L,
        KeyName_Dot,    //.
        KeyName_Z,
        KeyName_X,
        KeyName_C,
        KeyName_V,
        KeyName_B,
        KeyName_N,
        KeyName_M,
        KeyName_At,     //@
        KeyName_Left_Arrow,
        KeyName_Right_Arrow,
        KeyName_Com,
        KeyName_Keyboard,
        KeyName_Enter,
        KeyName_Caps_Lock,
        KeyName_Space,
        KeyName_Count
    };

    void changeLetterStyle();
signals:
    void sigHide();
    void sigKeyPress(const QString& keyText, KeyName keyName);

private:
    int getScreenHeight();
    void setKeyType();
    void layoutBtn();
    void setLineKey(int start, int end, int row, int xOffset = 0);
    void setOneKey(int nIndex, int xPos, int yPos, bool bWide = false);
    void multiScaleFactor();
private:
    bool m_bLower = true;
    int m_nBtnHeight = 68;  //按键是方形
    int m_nBtnInterval = 4;
    int m_nMargin = 31;
    int m_nWideBtnWidth = 140;  //宽按键默认宽
    int m_nWidth;
    int m_nHeight;
    int m_nBtnColumnCount = 10; //普通按键有10行(不包含宽按键)
    int m_nBtnRowCount = 4;     //总的有4行
    qreal m_rScaleFactor = 1.0;
    qreal m_nStanderdBtnHeight = 68.0;      //标准按键宽，用以计算宽按键的宽
    QSize m_iconSize = QSize(24, 15);
    QList<TLVirtualKeyboardBtn *> m_keys;
    TLPushButton* m_pCloseBtn = nullptr;
};

#endif // TLVIRTUALKEYBOARDTRAY_H
