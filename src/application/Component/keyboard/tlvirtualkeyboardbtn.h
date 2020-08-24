#ifndef TLVIRTUALKEYBOARDBTN_H
#define TLVIRTUALKEYBOARDBTN_H

#include "button/widget/tlpushbutton.h"

class QLabel;

class TLVirtualKeyboardBtn : public TLPushButton
{
    Q_OBJECT
public:
    explicit TLVirtualKeyboardBtn(QWidget *parent = nullptr);

    enum KeyType{
        KeyType_Number = 0x01,
        KeyType_Letter = 0x02,
        KeyType_BackSpace = 0x04,
        KeyType_Dot = 0x08,
        KeyType_At = 0x10,
        KeyType_RightArrow = 0x20,
        KeyType_LeftArrow = 0x40,
        KeyType_Arrow = KeyType_LeftArrow | KeyType_RightArrow,
        KeyType_Com = 0x80,
        KeyType_Enter = 0x100,
        KeyType_Caps_Lock = 0x200,
        KeyType_Space = 0x400,
        KeyType_Wide = KeyType_BackSpace | KeyType_Enter | KeyType_Caps_Lock,
        KeyType_KeyBoard = 0x800,
        KeyType_General = KeyType_Com | KeyType_KeyBoard,
        KeyType_Letter_Style = KeyType_Arrow | KeyType_Dot | KeyType_At,
    };

    void setKeyType(KeyType keyType);
    void setIcon(const QString& iconPath);
    void setIconSize(QSize size);
    void setLetterStyle(bool bLower);
private slots:
    void onClickSound();
private:
    KeyType m_keyType = KeyType_Number;
    QLabel* m_pIconLabel = nullptr;
};

#endif // TLVIRTUALKEYBOARDBTN_H
