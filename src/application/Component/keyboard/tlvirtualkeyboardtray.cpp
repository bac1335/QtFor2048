#include "tlvirtualkeyboardtray.h"

#include <QScreen>
#include <QPainter>
#include <QGuiApplication>

#include "tlvirtualkeyboardbtn.h"

TLVirtualKeyboardTray::TLVirtualKeyboardTray(qreal rScaleFactor, QWidget *parent)
    : TLBaseControl(parent), m_rScaleFactor(rScaleFactor)
{
    this->setFocusPolicy(Qt::NoFocus);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMoveFlag(TLBaseControl::Flag_MoveByOutSide);

    if (getScreenHeight() < 800) {
        m_nBtnHeight = 60;
    }
    multiScaleFactor();

    m_nWidth = m_nMargin * 2 + m_nBtnHeight * m_nBtnColumnCount + m_nBtnHeight / m_nStanderdBtnHeight * m_nWideBtnWidth + m_nBtnInterval * m_nBtnColumnCount;
    m_nHeight = m_nMargin * 2 + m_nBtnHeight * m_nBtnRowCount + m_nBtnInterval * (m_nBtnRowCount - 1);
    this->setFixedSize(m_nWidth, m_nHeight);

    for (int i = 0; i < KeyName_Count; ++i) {
        TLVirtualKeyboardBtn* pBtn = new TLVirtualKeyboardBtn(this);
        pBtn->setTextColor(Qt::white);
        connect(pBtn, &TLVirtualKeyboardBtn::sigClicked, [=]{
            emit sigKeyPress(pBtn->text(), (KeyName) i);
        });
        connect(pBtn, &TLVirtualKeyboardBtn::sigMoveByAsk, [=](QPoint p) {
            emit sigMoveByAsk(p + this->pos());
        });

        m_keys << pBtn;
    }
    setKeyType();
    layoutBtn();

    m_pCloseBtn = new TLPushButton(this);
    m_pCloseBtn->setMoveFlag(Flag_MoveByOutSide);
    m_pCloseBtn->setFixedSize(35, 35);
    m_pCloseBtn->move(this->width() - m_pCloseBtn->width() - 13, 13);
    m_pCloseBtn->setNormalImage(QString(":/keyboard/image/btn_closed_normal.png"));
    m_pCloseBtn->setHoverImage(QString(":/keyboard/image/btn_closed_hover.png"));
    m_pCloseBtn->setPressImage(QString(":/keyboard/image/btn_closed_press.png"));
    connect(m_pCloseBtn, &TLPushButton::sigRelease, this, &TLVirtualKeyboardTray::sigHide);
    connect(m_pCloseBtn, &TLPushButton::sigMoveByAsk, [=](QPoint p) {
        emit sigMoveByAsk(p + this->pos());
    });
}

void TLVirtualKeyboardTray::changeLetterStyle()
{
    m_bLower = !m_bLower;
    foreach (TLVirtualKeyboardBtn* pBtn, m_keys) {
        pBtn->setLetterStyle(m_bLower);
    }
    update();
}

int TLVirtualKeyboardTray::getScreenHeight()
{
    return qApp->primaryScreen()->size().height();
}

void TLVirtualKeyboardTray::setKeyType()
{
    for (int i = 0; i < KeyName_Back_Space; ++i) {
        m_keys[i]->setKeyType(TLVirtualKeyboardBtn::KeyType_Number);
        m_keys[i]->setText(QString("%1").arg(i + 1));
    }
    m_keys[KeyName_0]->setText(QString("0"));

    m_keys[KeyName_Back_Space]->setKeyType(TLVirtualKeyboardBtn::KeyType_BackSpace);
    m_keys[KeyName_Back_Space]->setText(QString(tr("Back Space")));


    QStringList lstLetter;
    lstLetter << QString("q") << QString("w") << QString("e") << QString("r") << QString("t")
              << QString("y") << QString("u") << QString("i") << QString("o") << QString("p")
              << QString("a") << QString("s") << QString("d") << QString("f") << QString("g")
              << QString("h") << QString("j") << QString("k") << QString("l") << QString(".")
              << QString("z") << QString("x") << QString("c") << QString("v") << QString("b")
              << QString("n") << QString("m");

    for (int i = KeyName_Q; i < KeyName_At; ++i) {
        m_keys[i]->setKeyType(TLVirtualKeyboardBtn::KeyType_Letter);
        m_keys[i]->setText(lstLetter[i - KeyName_Q]);
    }

    m_keys[KeyName_Dot]->setKeyType(TLVirtualKeyboardBtn::KeyType_Dot);

    m_keys[KeyName_At]->setKeyType(TLVirtualKeyboardBtn::KeyType_At);
    m_keys[KeyName_At]->setText(QString("@"));


    m_keys[KeyName_Left_Arrow]->setKeyType(TLVirtualKeyboardBtn::KeyType_LeftArrow);
    m_keys[KeyName_Right_Arrow]->setKeyType(TLVirtualKeyboardBtn::KeyType_RightArrow);

    m_keys[KeyName_Com]->setKeyType(TLVirtualKeyboardBtn::KeyType_Com);
    m_keys[KeyName_Com]->setText(QString(tr(".com")));

    m_keys[KeyName_Keyboard]->setKeyType(TLVirtualKeyboardBtn::KeyType_KeyBoard);

    m_keys[KeyName_Enter]->setKeyType(TLVirtualKeyboardBtn::KeyType_Enter);
    m_keys[KeyName_Enter]->setText(QString(tr("Enter")));

    m_keys[KeyName_Caps_Lock]->setKeyType(TLVirtualKeyboardBtn::KeyType_Caps_Lock);
    m_keys[KeyName_Caps_Lock]->setText(QString(tr("Caps Lock")));

    m_keys[KeyName_Space]->setKeyType(TLVirtualKeyboardBtn::KeyType_Space);
}

void TLVirtualKeyboardTray::layoutBtn()
{
    setLineKey(KeyName_1, KeyName_Back_Space, 0);
    setOneKey(KeyName_Back_Space, m_nMargin + m_nBtnColumnCount * (m_nBtnHeight + m_nBtnInterval), m_nMargin, true);

    setLineKey(KeyName_Q, KeyName_A, 1);
    setOneKey(KeyName_Enter, m_nMargin + m_nBtnColumnCount * (m_nBtnHeight + m_nBtnInterval),
              m_nMargin + 1 * (m_nBtnHeight + m_nBtnInterval), true);

    setLineKey(KeyName_A, KeyName_Z, 2);
    setOneKey(KeyName_Com, m_nMargin + m_nBtnColumnCount * (m_nBtnHeight + m_nBtnInterval),
              m_nMargin + 2 * (m_nBtnHeight + m_nBtnInterval));
    setOneKey(KeyName_Keyboard, m_nMargin + 11 * (m_nBtnHeight + m_nBtnInterval),
              m_nMargin + 2 * (m_nBtnHeight + m_nBtnInterval));

    setOneKey(KeyName_Caps_Lock, m_nMargin, m_nMargin + 3 * (m_nBtnHeight + m_nBtnInterval), true);
    setLineKey(KeyName_Z, KeyName_Com, 3, m_nBtnHeight/m_nStanderdBtnHeight*m_nWideBtnWidth + m_nBtnInterval);
    m_keys[KeyName_Space]->hide();
}

void TLVirtualKeyboardTray::setLineKey(int start, int end, int row, int xOffset)
{
    for (int i = start; i < end; ++i) {
        setOneKey(i, m_nMargin + xOffset + (i - start) * (m_nBtnHeight + m_nBtnInterval),
                      m_nMargin + row * (m_nBtnHeight + m_nBtnInterval));
    }
}

void TLVirtualKeyboardTray::setOneKey(int nIndex, int xPos, int yPos, bool bWide)
{
    if (bWide) {
        m_keys[nIndex]->setFixedSize(m_nBtnHeight / m_nStanderdBtnHeight * m_nWideBtnWidth,
                                     m_nBtnHeight);
    } else {
        m_keys[nIndex]->setFixedSize(m_nBtnHeight, m_nBtnHeight);
    }

    m_keys[nIndex]->setIconSize(m_iconSize);
    m_keys[nIndex]->move(xPos, yPos);
}

void TLVirtualKeyboardTray::multiScaleFactor()
{
    m_nBtnHeight *= m_rScaleFactor;  //按键是方形
    m_nBtnInterval *= m_rScaleFactor;
    m_nMargin *= m_rScaleFactor;
    m_nWideBtnWidth *= m_rScaleFactor;  //宽按键默认宽
    m_nStanderdBtnHeight *= m_rScaleFactor;
}
