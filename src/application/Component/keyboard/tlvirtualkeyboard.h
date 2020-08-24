#ifndef TLVIRTUALKEYBOARD_H
#define TLVIRTUALKEYBOARD_H

#include "base/widget/tlbasecontrol.h"

/*单例
 *
 *
*/

#include "tlvirtualkeyboardtray.h"

class TLVirtualKeyboard : public TLBaseControl
{
    Q_OBJECT
public:
    ~TLVirtualKeyboard();
    static TLVirtualKeyboard* keyboardInstance();
    static void Release();

    bool isSystemKeyboardVisible();

    void initPos();
protected:
    void paintEvent(QPaintEvent* event);

private slots:
    void onKeyPress(const QString& text, TLVirtualKeyboardTray::KeyName keyName);
private:
    explicit TLVirtualKeyboard(QWidget* parent = nullptr);

    bool win32FindHandle(const QString& proName);
    void windowsTabTip();

    //获取桌面大小
    QSize getDesktopSize(int screen = -1);
    void getScaleFactor();
private:
    TLVirtualKeyboardTray* m_pKeyBoardTray = nullptr;
    static TLVirtualKeyboard* ms_pTlVirtualKeyBoard;

    qreal m_rScaleFactor = 1.0;
    QSize m_cornerSize; //用来计算背景图corner的大小，防止重复计算，影响性能
    QSize m_noneCornerSize; //用来计算背景图非corner的大小，防止重复计算，影响性能
    QPixmap m_leftTop;
    QPixmap m_rightTop;
    QPixmap m_leftBottom;
    QPixmap m_rightBottom;
    QPixmap m_leftMiddle;
    QPixmap m_rightMiddle;
    QPixmap m_topMiddle;
    QPixmap m_bottomMiddle;
    QPixmap m_middleArea;
};

#endif // TLVIRTUALKEYBOARD_H
