#include "c7commonlineedit.h"
#ifdef Q_OS_WIN32
#include "Windows.h"
#include <ShellAPI.h>
#endif
#include <QRegExpValidator>
#include <QDebug>

QGraphicsView* C7CommonLineEdit::m_parentView = nullptr;

C7CommonLineEdit::C7CommonLineEdit(QWidget *parent):
    QLineEdit(parent), m_nXRadius(4), m_nYRadius(4), m_color(80, 82, 91)
{
    this->setStyleSheet("QMenu{background:white;}"
                        "QMenu:item:selected{background:rgb(155, 202, 239);}");
    QFont fontTMP = QFont("Microsoft YaHei", 14, QFont::Normal);
    fontTMP.setPixelSize(17);
    this->setFont(fontTMP);
}

C7CommonLineEdit::~C7CommonLineEdit()
{
//    qDebug() << __FUNCTION__;
    if(m_validator) {
        delete m_validator;
        m_validator = nullptr;
    }
}

void C7CommonLineEdit::setRadius(int xRadius, int yRadius)
{
    m_nXRadius = xRadius;
    m_nYRadius = yRadius;
}

void C7CommonLineEdit::setColor(QColor color)
{
    m_color = color;
    update();
}

void C7CommonLineEdit::setShowFrame(bool b)
{
    m_frame = b;
    update();
}

void C7CommonLineEdit::setDefaultText(QString str)
{
    m_defaultText = str;
    update();
}

void C7CommonLineEdit::setNoInputChinese()
{
    if(!m_validator) {
        m_validator = new QRegExpValidator(QRegExp("^[^\u4e00-\u9fa5]{0,}$"), this);
    }
    this->setValidator(m_validator);
}

void C7CommonLineEdit::setCurrentParentView(QGraphicsView *pParentView)
{
    m_parentView = pParentView;
}

void C7CommonLineEdit::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        emit sigEnterPressed();
        return;
    }
    if (!m_key) {
        QLineEdit::keyPressEvent(event);
        return;
    }
    if(event->matches(QKeySequence::Paste)) {
        m_paste = true;
        QLineEdit::keyPressEvent(event);
        return ;
    } else {
        QLineEdit::keyPressEvent(event);//QKeySequence::SelectAll || event->matches(QKeySequence::Copy)
    }
}

void C7CommonLineEdit::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QFont fontTMP = QFont("Microsoft YaHei", 14, QFont::Normal);
    fontTMP.setPixelSize(17);
    if(m_frame) {
        painter.setPen(m_color);
        painter.drawRoundedRect(QRect(0, 0, this->width(), this->height()), m_nXRadius, m_nYRadius);
    }
    painter.setPen(QColor(212, 212, 212));
    painter.setFont(fontTMP);
    if(!m_foucus && this->text().isEmpty()) {
        painter.drawText(QRect(20, 0, this->width(), this->height()),
                         Qt::AlignLeft | Qt::AlignVCenter, m_defaultText);
    }
    QLineEdit::paintEvent(event);
}

void C7CommonLineEdit::focusInEvent(QFocusEvent *event)
{
    TLVirtualKeyboard *m_pKeyboard = TLVirtualKeyboard::keyboardInstance();

    if (m_parentView != nullptr)
    {
        m_pKeyboard->move(m_parentView->x() + (m_parentView->width() - m_pKeyboard->width()) / 2,
                      m_parentView->height() - m_pKeyboard->height());
    }


    QLineEdit::focusInEvent(event);
}

void C7CommonLineEdit::focusOutEvent(QFocusEvent *event)
{
    if(TLVirtualKeyboard::keyboardInstance()->isVisible())
    {
        TLVirtualKeyboard::keyboardInstance()->hide();
    }

    QLineEdit::focusOutEvent(event);
}

void C7CommonLineEdit::hideEvent(QHideEvent *e)
{
    if(TLVirtualKeyboard::keyboardInstance()->isVisible())
    {
        TLVirtualKeyboard::keyboardInstance()->hide();
    }
}

void C7CommonLineEdit::windowsTabTip()
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
