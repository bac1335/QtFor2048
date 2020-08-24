#include "tlvirtualkeyboardbtn.h"

#include <QLabel>
#include <QSound>

TLVirtualKeyboardBtn::TLVirtualKeyboardBtn(QWidget *parent)
    : TLPushButton(parent)
{
    this->setFocusPolicy(Qt::NoFocus);

    m_font = QFont(u8"思源黑体 Medium", 10);
    m_font.setBold(true);

    m_pIconLabel = new QLabel(this);
    connect(this, &TLVirtualKeyboardBtn::sigClicked, this, &TLVirtualKeyboardBtn::onClickSound);
}

void TLVirtualKeyboardBtn::setKeyType(TLVirtualKeyboardBtn::KeyType keyType)
{
    m_keyType = keyType;

    QString strNormalImage;
    QString strHoverImage;
    QString strPressImage;

    switch (m_keyType) {
    case KeyType_Number:
        strNormalImage = QString(":/keyboard/image/keyboard_number_normal.png");
        strHoverImage = QString(":/keyboard/image/keyboard_number_hover.png");
        strPressImage = QString(":/keyboard/image/keyboard_number_press.png");
        break;
    case KeyType_Letter:
        strNormalImage = QString(":/keyboard/image/keyboard_letter_normal.png");
        strHoverImage = QString(":/keyboard/image/keyboard_letter_hover.png");
        strPressImage = QString(":/keyboard/image/keyboard_letter_press.png");
        break;
    case KeyType_BackSpace:
//        setIcon(QString(":/keyboard/image/icon_delete.png"));
        break;
    case KeyType_LeftArrow:
        setIcon(QString(":/keyboard/image/icon_left.png"));
        break;
    case KeyType_RightArrow:
        setIcon(QString(":/keyboard/image/icon_right.png"));
        break;
    case KeyType_KeyBoard:
        setIcon(QString(":/keyboard/image/icon_keyboard.png"));
        break;
    default:
        break;
    }

    if (m_keyType & KeyType_Wide) {
        strNormalImage = QString(":/keyboard/image/keyboard_wide_normal.png");
        strHoverImage = QString(":/keyboard/image/keyboard_wide_hover.png");
        strPressImage = QString(":/keyboard/image/keyboard_wide_press.png");
    } else if (m_keyType & KeyType_Letter_Style) {
        strNormalImage = QString(":/keyboard/image/keyboard_letter_normal.png");
        strHoverImage = QString(":/keyboard/image/keyboard_letter_hover.png");
        strPressImage = QString(":/keyboard/image/keyboard_letter_press.png");
    } else if (m_keyType & KeyType_General) {
        strNormalImage = QString(":/keyboard/image/keyboard_general_normal.png");
        strHoverImage = QString(":/keyboard/image/keyboard_general_hover.png");
        strPressImage = QString(":/keyboard/image/keyboard_general_press.png");
    }

    setNormalImage(strNormalImage);
    setHoverImage(strHoverImage);
    setPressImage(strPressImage);
}

void TLVirtualKeyboardBtn::setIcon(const QString &iconPath)
{
    m_pIconLabel->setStyleSheet(QString("border-image:url(%1);").arg(iconPath));
}

void TLVirtualKeyboardBtn::setIconSize(QSize size)
{
    m_pIconLabel->resize(size);
    int xPos = (this->width() - m_pIconLabel->width())/2;
    if (m_keyType == KeyType_BackSpace) {
        xPos = 5;
    }

    m_pIconLabel->move(xPos, (this->height() - m_pIconLabel->height()) / 2);
}

void TLVirtualKeyboardBtn::setLetterStyle(bool bLower)
{
    if (m_keyType == KeyType_Letter || m_keyType == KeyType_Com)
    {
        if (bLower) {
            this->setText(this->text().toLower());
        } else {
            this->setText(this->text().toUpper());
        }
    }
}

void TLVirtualKeyboardBtn::onClickSound()
{
    if(this->text() >= "0" && this->text() <= "9") {
        QSound::play(QString(":/keyboard/sound/%1.wav").arg(this->text()));
    } else if(this->text() == "Back Space") {
        QSound::play(":/keyboard/sound/star.wav");
    }
}
