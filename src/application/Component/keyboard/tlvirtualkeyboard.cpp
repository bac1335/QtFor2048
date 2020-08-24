#include "tlvirtualkeyboard.h"

#include <Windows.h>
#include <shellapi.h>
#include <TlHelp32.h>
#include <QKeyEvent>
#include <QProcess>
#include <QApplication>
#include <QPainter>
#include <QDesktopWidget>

#define DEBUG_OUTPUT

#ifdef DEBUG_OUTPUT
#include <QDebug>
#endif

TLVirtualKeyboard* TLVirtualKeyboard::ms_pTlVirtualKeyBoard = nullptr;

TLVirtualKeyboard::~TLVirtualKeyboard()
{
    Release();
}

TLVirtualKeyboard *TLVirtualKeyboard::keyboardInstance()
{
    if (ms_pTlVirtualKeyBoard == nullptr) {
        ms_pTlVirtualKeyBoard = new TLVirtualKeyboard;
    }

    return ms_pTlVirtualKeyBoard;
}

void TLVirtualKeyboard::Release()
{
    if (ms_pTlVirtualKeyBoard != nullptr) {
        ms_pTlVirtualKeyBoard->deleteLater();
        ms_pTlVirtualKeyBoard = nullptr;
    }
}

bool TLVirtualKeyboard::isSystemKeyboardVisible()
{
    return win32FindHandle("osk.exe");
}

void TLVirtualKeyboard::initPos()
{
    int screenWidth = getDesktopSize().width();
    int screenHeight = getDesktopSize().height();
    this->move((screenWidth - this->width()) / 2., screenHeight - this->height() - 100 * m_rScaleFactor);
}

void TLVirtualKeyboard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setBrush(QColor(Qt::black));

    //下面的加1、减1是为了防止动画中出现间隙
    QRect rect;
    //画上面
    rect.setX(m_cornerSize.width());
    rect.setWidth(m_noneCornerSize.width() + 1);
    rect.setHeight(m_cornerSize.height());
    painter.drawPixmap(rect, m_topMiddle);
    //画下面
    rect.setY(this->height() - m_cornerSize.height());
    rect.setHeight(m_cornerSize.height());          //设置Y值后得重新设置高度，下同
    painter.drawPixmap(rect, m_bottomMiddle);
    //画左边
    rect.setY(m_cornerSize.height());
    rect.setHeight(m_noneCornerSize.height() + 1);
    rect.setX(0);
    rect.setWidth(m_cornerSize.width());
    painter.drawPixmap(rect, m_leftMiddle);
    //画右边
    rect.setX(this->width() - m_cornerSize.width() - 1);
    rect.setWidth(m_cornerSize.width() + 1);        //设置X值后得重新设置宽度，下同
    painter.drawPixmap(rect, m_rightMiddle);

    //后画4个角
    //左上角
    rect.setX(0);
    rect.setY(0);
    rect.setSize(m_cornerSize);
    painter.drawPixmap(rect, m_leftTop);
    //右上角
    rect.setX(this->width() - m_cornerSize.width());
    rect.setWidth(m_cornerSize.width());         //设置X值后得重新设置宽度，下同
    painter.drawPixmap(rect, m_rightTop);
    //左下角
    rect.setX(0);
    rect.setY(this->height() - m_cornerSize.height());
    rect.setSize(m_cornerSize);
    painter.drawPixmap(rect, m_leftBottom);
    //右下角
    rect.setX(this->width() - m_cornerSize.width());
    rect.setWidth(m_cornerSize.width());         //设置X值后得重新设置宽度，下同
    painter.drawPixmap(rect, m_rightBottom);

    rect.setX(this->width() - m_cornerSize.width());
    rect.setWidth(m_cornerSize.width());         //设置X值后得重新设置宽度，下同
    painter.drawPixmap(rect, m_rightBottom);

    //最后画中间
    rect.setX(m_cornerSize.width());
    rect.setY(m_cornerSize.height());
    rect.setSize(m_noneCornerSize + QSize(1, 1));
    painter.drawPixmap(rect, m_middleArea);

    QWidget::paintEvent(event);
}

void TLVirtualKeyboard::onKeyPress(const QString &text, TLVirtualKeyboardTray::KeyName keyName)
{
    switch (keyName) {
    case TLVirtualKeyboardTray::KeyName_Left_Arrow:
        if(qApp->focusWidget())
        {
            QApplication::postEvent(qApp->focusWidget(),
                                    new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier));
        }
        break;
    case TLVirtualKeyboardTray::KeyName_Right_Arrow:
        if(qApp->focusWidget())
        {
            QApplication::postEvent(qApp->focusWidget(),
                                    new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier));
        }
        break;
    case TLVirtualKeyboardTray::KeyName_Keyboard:
        if(!isSystemKeyboardVisible()) {
            windowsTabTip();
        }
        break;
    case TLVirtualKeyboardTray::KeyName_Back_Space:
        if(qApp->focusWidget())
        {
            QApplication::postEvent(qApp->focusWidget(),
                                    new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier));
        }
        break;
    case TLVirtualKeyboardTray::KeyName_Enter:
//        if(qApp->focusWidget())
//        {
//            QApplication::postEvent(qApp->focusWidget(),
//                                    new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier));
            if(qApp->focusWidget() && qApp->focusWidget()->inherits("QLineEdit"))
            {
                qApp->focusWidget()->clearFocus();
            }
//        }
        break;
    case TLVirtualKeyboardTray::KeyName_Caps_Lock:
        m_pKeyBoardTray->changeLetterStyle();
        break;
    default:
        if(qApp->focusWidget())
        {
            qDebug() << "hongcs" << __FUNCTION__ << "" << qApp->focusWidget();
            QApplication::postEvent(qApp->focusWidget(),
                                    new QKeyEvent(QEvent::KeyPress,QChar('0').unicode(), Qt::NoModifier, text));
        }
        break;
    }
}

TLVirtualKeyboard::TLVirtualKeyboard(QWidget *parent)
    : TLBaseControl(parent)
{
    getScaleFactor();
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFocusPolicy(Qt::NoFocus);
    m_pKeyBoardTray = new TLVirtualKeyboardTray(m_rScaleFactor, this);
    connect(m_pKeyBoardTray, &TLVirtualKeyboardTray::sigKeyPress,
            this, &TLVirtualKeyboard::onKeyPress);
    this->setFixedSize(m_pKeyBoardTray->size());

    this->setMoveFlag(Flag_MoveSelf);
    this->setMoveLimit(MoveLimit_Free);
    connect(m_pKeyBoardTray, &TLVirtualKeyboardTray::sigMoveByAsk, [=](QPoint offset){
        this->move(offset + this->pos());
    });
    //隐藏的时候把获取焦点lineedit的焦点清空
    connect(m_pKeyBoardTray, &TLVirtualKeyboardTray::sigHide, this, [=](){
        this->hide();
        if(qApp->focusWidget() && qApp->focusWidget()->inherits("QLineEdit"))
        {
            qApp->focusWidget()->clearFocus();
        }
    });

    m_leftTop.load(":/keyboard/image/bg_lefttop.png");
    m_rightTop.load(":/keyboard/image/bg_righttop.png");
    m_leftBottom.load(":/keyboard/image/bg_leftbottom.png");
    m_rightBottom.load(":/keyboard/image/bg_rightbottom.png");
    m_leftMiddle.load(":/keyboard/image/bg_leftmiddle.png");
    m_rightMiddle.load(":/keyboard/image/bg_rightmiddle.png");
    m_topMiddle.load(":/keyboard/image/bg_topmiddle.png");
    m_bottomMiddle.load(":/keyboard/image/bg_bottommiddle.png");
    m_middleArea.load(":/keyboard/image/bg_middlearea.png");

    m_cornerSize = m_leftTop.size() / 2;
    //因为切的是两倍图，所以这里不需要乘2，如果是一倍图则需要乘2，因为左右上下各两个角落
    m_noneCornerSize = QSize(this->width() - m_leftTop.width(),
                             this->height() - m_leftTop.height());

    initPos();
}

bool TLVirtualKeyboard::win32FindHandle(const QString &proName)
{
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32* processInfo = new PROCESSENTRY32;
    processInfo->dwSize = sizeof(PROCESSENTRY32);
    int index = 0;
    while(Process32Next(hSnapShot, processInfo) != FALSE) {
        index++;
        int size = WideCharToMultiByte(CP_ACP, 0, processInfo->szExeFile, -1, NULL, 0, NULL, NULL);
        char *ch = new char[size+1];
        if(WideCharToMultiByte(CP_ACP, 0, processInfo->szExeFile, -1, ch, size, NULL, NULL)){
            if(strstr(ch, proName.toUtf8())) {//进程还在
                delete[] ch;
                CloseHandle(hSnapShot);
                delete processInfo;
                return true;
            }
        }
        delete[] ch;
    }
    CloseHandle(hSnapShot);
    delete processInfo;
    return false;
}

void TLVirtualKeyboard::windowsTabTip()
{
    bool ret = false;
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    char* windir = getenv("windir");
    char winroot = 'c';
    if (!windir || strcmp(windir, "")) {
        winroot = windir[0];
    }
    char tabtip[260];
    char tabtipworksp[260];
    if (osvi.dwMajorVersion <= 5) {// XP
        sprintf(tabtip, "%c:\\Windows\\System32\\osk.exe ", winroot);
        sprintf(tabtipworksp, "%c:\\Windows\\System32", winroot);
        ret = QProcess::startDetached(tabtip, QStringList(), tabtipworksp);
    } else if (osvi.dwMajorVersion >= 6) {
        if (osvi.dwMinorVersion >= 2) { // win8, win8.1, Windows Server 2012
            PVOID OldValue;
            BOOL bRet = Wow64DisableWow64FsRedirection(&OldValue);
            ShellExecuteA(NULL, "open", "osk.exe", 0, 0, SW_SHOW);
            if (bRet) {
                Wow64RevertWow64FsRedirection(OldValue);
            }
            return ;
        }
        // vista win7
        sprintf(tabtip, "%c:\\Program Files\\Common Files\\Microsoft Shared\\ink\\TabTip.exe ", winroot);
        sprintf(tabtipworksp, "%c:\\Program Files\\Common Files\\Microsoft Shared\\ink", winroot);
        ret = QProcess::startDetached(tabtip, QStringList(), tabtipworksp);
        if (!ret) {
            sprintf(tabtip, "%c:\\Program Files (x86)\\Common Files\\Microsoft Shared\\ink\\TabTip32.exe ", winroot);
            sprintf(tabtipworksp, "%c:\\Program Files (x86)\\Common Files\\Microsoft Shared\\ink", winroot);
            ret = QProcess::startDetached(tabtip, QStringList(), tabtipworksp);
            if (!ret) {
                HWND hTabTip = ::FindWindowA("IPTip_Main_Window", 0);
                if (hTabTip) {
                    ShowWindow(hTabTip, SW_NORMAL);
                    DWORD WM_DESKBAND_CLICKED = ::RegisterWindowMessage(L"TabletInputPanelDeskBandClicked");
                    ::PostMessage(hTabTip, WM_DESKBAND_CLICKED, 0, 0);
                } else {
                    sprintf(tabtip, "%c:\\Windows\\System32\\osk.exe ", winroot);
                    sprintf(tabtipworksp, "%c:\\Windows\\System32", winroot);
                    ret = QProcess::startDetached(tabtip, QStringList(), tabtipworksp);
                }
            }
        }
    }
}

QSize TLVirtualKeyboard::getDesktopSize(int screen)
{
    return qApp->desktop()->screenGeometry(screen).size();
}

void TLVirtualKeyboard::getScaleFactor()
{
    int width = 1920.;
    int height = 1080.;
    int deskWidth = getDesktopSize().width();
    int deskHeight = getDesktopSize().height();

    qreal normalScale = width / height;
    qreal curScale = deskWidth / deskHeight;
    qreal rRet = 1.0;
    if(normalScale <= curScale)
    {
        rRet = deskHeight / 1080.;
    }
    else
    {
        rRet = deskWidth / 1920.;
    }

    m_rScaleFactor = rRet;
}
