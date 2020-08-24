#include "tlmessagebox.h"

#include <QLabel>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>

#include "maskwidget/tlmaskwidget.h"
#include "button/widget/tlpushbutton.h"
#include "c7commondef.h"

bool TLMessageBox::ms_bInit = false;
int TLMessageBox::ms_nMinWidth = 410;
int TLMessageBox::ms_nMinHeight = 243;
int TLMessageBox::ms_nMaxWidth = 410;
int TLMessageBox::ms_nMaxHeight = 243;
qreal TLMessageBox::ms_rScaleFactor = 1.0;
bool TLMessageBox::ms_bInitFont = false;
QFont TLMessageBox::ms_font;
QString TLMessageBox::ms_strBgPath = QString(":/messagebox/image/bg_message.png");
QString TLMessageBox::ms_strOkBgNormal = QString(":/messagebox/image/btn_confirm_normal.png");
QString TLMessageBox::ms_strOkBgPress = QString(":/messagebox/image/btn_confirm_press.png");
QString TLMessageBox::ms_strCancelBgNormal = QString(":/messagebox/image/btn_cancel_normal.png");
QString TLMessageBox::ms_strCancelBgPress = QString(":/messagebox/image/btn_cancel_press.png");

TLMessageBox::TLMessageBox(QWidget *parent)
    : TLBaseControl(parent)
{
    if (!ms_bInit)
    {
        init();
    }

//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
//    this->setAttribute(Qt::WA_TranslucentBackground, true);
//    this->setWindowModality(Qt::ApplicationModal);

    m_pDisplayLabel = new QLabel(this);
    m_pDisplayLabel->setFont(this->font());
    m_pDisplayLabel->setAlignment(Qt::AlignCenter);
    m_pDisplayLabel->setWordWrap(true);
    QPalette palette;
    palette.setColor(QPalette::Text, Qt::white);
    m_pDisplayLabel->setPalette(palette);
    m_pDisplayLabel->setFont(ms_font);

    m_bg.load(ms_strBgPath);
}

void TLMessageBox::setUseIcon(bool useIcon, QColor color,QString path, int width , int height,QString text,int round)
{
    m_useIcon = useIcon;
    m_icon.load(path);
    m_icon = m_icon.scaled(width,height,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_IconText = text;
    m_round = round;
    m_bgColor = color;
    m_iconWidth = width;
    m_iconHeight = height;
}

void TLMessageBox::setBoxType(TLMessageBox::MessageBox_Type boxType)
{
    m_boxType = boxType;

    switch (m_boxType) {
    case WaittingBox:
        addCloseButton();
        break;
    case QuestionBox:
    {
        TLPushButton *pEnter = addPushButton(QString(tr("Ok")), Ok_Button);
        connect(pEnter, &TLPushButton::sigRelease, this, &TLMessageBox::sigOk);
        TLPushButton *pCancel = addPushButton(QString(tr("Cancel")), Cancel_Button);
        connect(pCancel, &TLPushButton::sigRelease, this, &TLMessageBox::sigCancel);
        relayout();
    }
        break;
    case InformationBox:
    case AboutBox:
    {
        TLPushButton *pEnter = addPushButton(QString(tr("Ok")), Ok_Button);
        connect(pEnter, &TLPushButton::sigRelease, this, &TLMessageBox::sigOk);
        relayout();
    }
        break;
    default:
        break;
    }
}

void TLMessageBox::setCountDownNumber(int nSecond, TLMessageBox::CountDown_Type countType, bool bStart)
{
    m_nTimerCountMax = nSecond;
    m_nTimerCount = 0;
    m_countType = countType;

    if (bStart) {
        initCountTimer(true);
    }
}

void TLMessageBox::startCountDown()
{
    if (m_pCountTimer == Q_NULLPTR) {
        initCountTimer(true);
    } else {
        m_pCountTimer->start();
    }
}

void TLMessageBox::stopCountDown()
{
    if (m_pCountTimer != Q_NULLPTR) {
        m_pCountTimer->stop();
    }
}

void TLMessageBox::setTitle(const QString &title)
{
    m_strTitle = title;
    update();
}

void TLMessageBox::setMessage(const QString &msg, int nWidth, int nHeight)
{
    m_strMessage = msg;
    m_strDisplayMessage = msg;
    m_pDisplayLabel->setText(m_strDisplayMessage);
    relayout(nWidth, nHeight);
    update();
}

TLPushButton *TLMessageBox::addCloseButton()
{
    if (m_pCloseBtn == Q_NULLPTR)
    {
        m_pCloseBtn = new TLPushButton(this);
        m_pCloseBtn->resize(25 * ms_rScaleFactor, 25 * ms_rScaleFactor);

        m_pCloseBtn->move(this->width() - m_pCloseBtn->width() - 40 * ms_rScaleFactor,
                          40 * ms_rScaleFactor);
        connect(m_pCloseBtn, &TLPushButton::sigRelease, this, [=]{
            this->hide();
            emit sigClose();
        });
    }

    return m_pCloseBtn;
}

TLPushButton *TLMessageBox::addPushButton(const QString &text, TLMessageBox::Button_Type btnType)
{
    TLPushButton* btn = new TLPushButton(this);
    btn->setFixedSize(102 * ms_rScaleFactor, 48 * ms_rScaleFactor);
    btn->setText(text);
    btn->setProperty(ButtonType, btnType);
    btn->setFont(ms_font);
    if (btnType == Ok_Button)
    {
        btn->setNormalBackColor(Color_OkButton_Normal);
        btn->setHoverBackColor(Color_OkButton_Normal);
        btn->setPressBackColor(Color_OkButton_Press);
        btn->setRadius(6 * ms_rScaleFactor);
        btn->setTextColor(Qt::white);
    }
    else
    {
//        btn->setNormalImage(ms_strCancelBgNormal);
//        btn->setHoverImage(ms_strCancelBgNormal);
//        btn->setPressImage(ms_strCancelBgPress);
        btn->setNormalBackColor(Color_CancelButton_Normal);
        btn->setHoverBackColor(Color_CancelButton_Normal);
        btn->setPressBackColor(Color_CancelButton_Press);
        btn->setRadius(6 * ms_rScaleFactor);
    }

    m_btns << btn;
    connect(btn, &TLPushButton::sigRelease, this, &TLMessageBox::onButtonClicked);
    return btn;
}

QList<TLPushButton *> TLMessageBox::getBtns()
{
    return m_btns;
}

void TLMessageBox::relayout(int nWidth, int nHeight)
{
    if (nWidth == -1)
    {
        nWidth = ms_nMinWidth;
    }

    if (nHeight == -1)
    {
        nHeight = ms_nMinHeight;
    }

    int titleWidth = getStringWidth(m_strTitle);
    int msgWidth = getStringWidth(m_strDisplayMessage);
    int width = qMax(titleWidth, msgWidth) + 60 * ms_rScaleFactor;

    int height = 100 * ms_rScaleFactor;

    this->setFixedSize(qMax(width, nWidth),
                       qMax(height, nHeight));
    if (this->width() > ms_nMaxWidth)
    {
        this->setFixedSize(ms_nMaxWidth, this->height());
    }

    m_pDisplayLabel->setFixedSize(this->width() - 40 * 2 * ms_rScaleFactor, 60 * ms_rScaleFactor);

    if (m_boxType == WaittingBox)
    {
        m_pDisplayLabel->move((this->width() - m_pDisplayLabel->width()) / 2,
                            (this->height() - m_pDisplayLabel->height()) / 2);
    }
    else
    {
        if (m_strTitle.isEmpty())
        {
            m_pDisplayLabel->move((this->width() - m_pDisplayLabel->width()) / 2, this->height() * 0.1);
        }
        else
        {
            m_pDisplayLabel->move((this->width() - m_pDisplayLabel->width()) / 2, 50 * ms_rScaleFactor);
        }
    }

    TLPushButton* pBtn = Q_NULLPTR;
    if (m_btns.size() == 1)
    {
        pBtn = m_btns.at(0);
        pBtn->move((this->width() - pBtn->width()) / 2, this->height() - pBtn->height() - 58 * ms_rScaleFactor);
    }
    else if (m_btns.size() == 2)
    {
        pBtn = m_btns.at(0);
        pBtn->move(45 * ms_rScaleFactor, this->height() - 35 * ms_rScaleFactor - pBtn->height());
        pBtn = m_btns.at(1);
        pBtn->move(this->width() - 45  * ms_rScaleFactor - pBtn->width(),
                   this->height() - 35 * ms_rScaleFactor - pBtn->height());
    }
    else if (m_btns.size() == 3)
    {
        TLPushButton* tempBtn = Q_NULLPTR;
        pBtn = m_btns.at(0);
        tempBtn = pBtn;
        tempBtn->setFixedSize(112 * ms_rScaleFactor, pBtn->height());
        pBtn->move(58 * ms_rScaleFactor, this->height() - 58 * ms_rScaleFactor - pBtn->height());
        pBtn = m_btns.at(1);
        pBtn->move(tempBtn->x() + tempBtn->width() + 33 * ms_rScaleFactor, this->height() - 58 * ms_rScaleFactor - pBtn->height());
        pBtn = m_btns.at(2);
        pBtn->move(this->width() - 58 * ms_rScaleFactor - pBtn->width(),
                   this->height() - 58 * ms_rScaleFactor - pBtn->height());
    }


    if (m_pCloseBtn != Q_NULLPTR)
    {
        m_pCloseBtn->move(this->width() - m_pCloseBtn->width() - 40 * ms_rScaleFactor, 40 * ms_rScaleFactor);
    }

    update();
}

int TLMessageBox::showMessage(QWidget *parent, TLMessageBox::MessageBox_Type msgBoxType, const QString &strTitle, const QString &strMsg, bool bMask)
{
    if (!ms_bInit)
    {
        init();
    }

    if (msgBoxType == QuestionBox)
    {
        return question(parent, strTitle, strMsg, bMask);
    }
    else if (msgBoxType == InformationBox || msgBoxType == AboutBox)
    {
        return information(parent, strTitle, strMsg, bMask);
    }
    else
    {
        return 0;
    }
}

void TLMessageBox::init()
{
    ms_bInit = true;
    initScaleFactor();

    ms_nMinWidth *= ms_rScaleFactor;
    ms_nMinHeight *= ms_rScaleFactor;
    ms_nMaxWidth *= ms_rScaleFactor;
    ms_nMaxHeight *= ms_rScaleFactor;

    //从未初始化过字体，使用默认的
    if (!ms_bInitFont) {
        ms_font.setFamily(u8"思源黑体 Medium");
        ms_font.setPixelSize(18 * ms_rScaleFactor);
    }
}

void TLMessageBox::setMinSize(int nMinWidth, int nMinHeight, qreal rScaleFactor)
{
    ms_nMinWidth = nMinWidth * rScaleFactor;
    ms_nMinHeight = nMinHeight * rScaleFactor;
}

void TLMessageBox::setMaxSize(int nMaxWidth, int nMaxHeight, qreal rScaleFactor)
{
    ms_nMaxWidth = nMaxWidth * rScaleFactor;
    ms_nMaxHeight = nMaxHeight * rScaleFactor;
}

void TLMessageBox::setScaleFactor(qreal rScaleFactor)
{
    if (rScaleFactor < 0)
    {
        initScaleFactor();
    }
    else
    {
        ms_rScaleFactor = rScaleFactor;
    }
}

void TLMessageBox::setBgPath(const QString &path)
{
    ms_strBgPath = path;
}

void TLMessageBox::setOkBgNormal(const QString &path)
{
    ms_strOkBgNormal = path;
}

void TLMessageBox::setOkBgPress(const QString &path)
{
    ms_strOkBgPress = path;
}

void TLMessageBox::setCancelBgNormal(const QString &path)
{
    ms_strCancelBgNormal = path;
}

void TLMessageBox::setCancelBgPress(const QString &path)
{
    ms_strCancelBgPress = path;
}

void TLMessageBox::setFont(QFont font)
{
    ms_bInitFont = true;
    ms_font = font;
}

void TLMessageBox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if(!m_useIcon)
    {
          painter.drawPixmap(this->rect(), m_bg);
    }
    else {
        painter.setPen(Qt::NoPen);
        painter.setBrush(m_bgColor);
        painter.drawRoundedRect(this->rect(),m_round,m_round);
        painter.drawPixmap(QRect((this->width()-m_icon.width())/2.,(this->height() + m_icon.height())/2. - m_icon.height() -60 * ms_rScaleFactor,
                                  m_icon.width(),m_icon.height()),m_icon);

        QRect IconRect(0, this->height()/2 -10 * ms_rScaleFactor, this->width(), 20 * ms_rScaleFactor);
        painter.setPen(QPen(Qt::black));
        painter.setFont(ms_font);
        painter.drawText(IconRect, Qt::AlignCenter, m_IconText);
    }

    painter.setPen(QPen(Qt::black));
    painter.setFont(this->font());
    QRect titelRect(0, 44 * ms_rScaleFactor, this->width(), 20 * ms_rScaleFactor);
    painter.drawText(titelRect, Qt::AlignCenter, m_strTitle);
}

void TLMessageBox::onCountTimeout()
{
    ++m_nTimerCount;
    QString strCount = getTimeString(m_nTimerCountMax - m_nTimerCount);

    m_strDisplayMessage = m_strMessage + strCount;
    //第一次设置
    if (m_nTimerCount == 1)
    {
        int msgWidth = getStringWidth(m_strDisplayMessage);
        msgWidth += (60 * ms_rScaleFactor);
        this->setFixedSize(qMax(msgWidth, this->width()),
                           this->height());
        if (this->width() > ms_nMaxWidth)
        {
            this->setFixedSize(ms_nMaxWidth, this->height());
        }

        relayout();
    }
    m_pDisplayLabel->setText(m_strDisplayMessage);

    update();

    if (m_nTimerCount == m_nTimerCountMax)
    {
        m_pCountTimer->stop();
        emit sigTimeout();
    }
}

void TLMessageBox::onButtonClicked()
{
    TLPushButton *pBtn = qobject_cast<TLPushButton *>(sender());
    if(pBtn != Q_NULLPTR){
        emit sigButtonClicked(pBtn);
        emit sigButtonType((Button_Type)pBtn->property(ButtonType).toInt());
    }
}

TLMaskWidget *TLMessageBox::addMask(QWidget *widget)
{
    QWidget* parent = widget->parentWidget();

    TLMaskWidget* maskWidget = qobject_cast<TLMaskWidget *> (parent);
    //说明已经有遮罩
    if (maskWidget != Q_NULLPTR) {
        Q_ASSERT(parent->metaObject()->className() == "TLMaskWidget");
        widget->show();
        return maskWidget;
    }

    maskWidget = new TLMaskWidget;
    maskWidget->setChild(widget);
    maskWidget->hide();
    return maskWidget;
}

int TLMessageBox::getStringWidth(const QString &str)
{
    QFontMetrics fontMetrics(ms_font);
    return fontMetrics.width(str);
}

void TLMessageBox::initScaleFactor(qreal width, qreal height)
{
    int deskWidth = qApp->desktop()->width();
    int deskHeight = qApp->desktop()->height();

    qreal normalScale = width / height;
    qreal curScale = deskWidth / deskHeight;

    if(normalScale <= curScale)
    {
        ms_rScaleFactor = deskHeight / 1080.;
    }
    else
    {
        ms_rScaleFactor = deskWidth / 1920.;
    }
}

void TLMessageBox::initCountTimer(bool bStart)
{
    if (m_pCountTimer == Q_NULLPTR)
    {
        m_pCountTimer = new QTimer(this);
        m_pCountTimer->setInterval(1000);
        m_pCountTimer->setTimerType(Qt::PreciseTimer);
        connect(m_pCountTimer, &QTimer::timeout, this, &TLMessageBox::onCountTimeout);
    }

    if (bStart && m_pCountTimer != Q_NULLPTR)
    {
        m_pCountTimer->start();
    }
}

QString TLMessageBox::getTimeString(int count)
{
    if (m_countType == Second)
    {
        return QString("(%1)").arg(count, 2, 10, QLatin1Char('0')); //这个表示2位数，10进制，不足前面补0，下同
    }
    else if (m_countType == Minute)
    {
        int second, minute;
        second = count % 60;
        minute = count / 60;

        return QString("(%1:%2)")
                .arg(minute, 2, 10, QLatin1Char('0'))
                .arg(second, 2, 10, QLatin1Char('0'));
    }
    else if (m_countType == Hour)
    {
        int second, minute, hour;
        hour = count / 3600;
        minute = count % 3600 / 60;
        second = count % 60;

        return QString("(%1:%2:%3)")
                .arg(hour, 2, 10, QLatin1Char('0'))
                .arg(minute, 2, 10, QLatin1Char('0'))
                .arg(second, 2, 10, QLatin1Char('0'));
    }

    return QString("");
}

int TLMessageBox::question(QWidget *parent, const QString &strTitle, const QString &strMsg, bool bMask, bool bModel)
{
    int nReturn;

    TLMessageBox msgBox(parent);
    msgBox.setTitle(strTitle);
    msgBox.setMessage(strMsg);
    msgBox.addPushButton(QString(tr("Ok")), Ok_Button);
    msgBox.addPushButton(QString(tr("Cancel")), Cancel_Button);
    msgBox.relayout();

    //没有父窗口的话，不能也没必要设置mask
    if (parent == Q_NULLPTR)
    {
        msgBox.setMoveFlag(MoveFlag::Flag_MoveSelf);
        msgBox.show();
        QEventLoop loop;

        connect(&msgBox, &TLMessageBox::sigButtonClicked,[&nReturn, &loop](TLPushButton *pBtn){
            if (pBtn != Q_NULLPTR)
            {
                nReturn = pBtn->property(ButtonType).toInt();
            }
            loop.quit();
        });

        loop.exec(QEventLoop::DialogExec);
        return nReturn;
    }
    else
    {
        TLMaskWidget mask;
        QColor color;
        if (bMask)
        {
            color.setAlpha(80);
        }
        else
        {
            color.setAlpha(0);
        }

        QBrush brush(color);
        //原生态ppt会有问题，所以默认不用模态的。
        mask.setModal(bModel);
        mask.setBackgroundBrush(brush);
        mask.setChild(&msgBox);
        mask.show();

        QEventLoop loop;

        connect(&msgBox, &TLMessageBox::sigButtonClicked,[&nReturn, &loop, &mask](TLPushButton *pBtn){
            if (pBtn != Q_NULLPTR)
            {
                nReturn = pBtn->property(ButtonType).toInt();
            }
            loop.quit();
//            mask.removeChild();
        });

        loop.exec(QEventLoop::DialogExec);
        return nReturn;
    }
}

int TLMessageBox::information(QWidget *parent, const QString &strTitle, const QString &strMsg, bool bMask)
{
    int nReturn;
    TLMessageBox msgBox(parent);
    msgBox.setTitle(strTitle);
    msgBox.setMessage(strMsg);
    msgBox.addPushButton(QString(tr("Ok")), Ok_Button);
    msgBox.relayout();
    msgBox.show();

    if (parent == Q_NULLPTR)
    {
        msgBox.setMoveFlag(MoveFlag::Flag_MoveSelf);
        msgBox.show();
        QEventLoop loop;

        connect(&msgBox, &TLMessageBox::sigButtonClicked,[&nReturn, &loop](TLPushButton *pBtn){
            if (pBtn != Q_NULLPTR)
            {
                nReturn = pBtn->property(ButtonType).toInt();
            }
            loop.quit();
        });

        loop.exec(QEventLoop::DialogExec);
        return nReturn;
    }
    else
    {
        TLMaskWidget mask;
        QColor color;
        if (bMask)
        {
            color.setAlpha(80);
        }
        else
        {
            color.setAlpha(0);
        }
        QBrush brush(color);
        mask.setBackgroundBrush(brush);
        mask.setChild(&msgBox/*, ms_centerPos*/);
        mask.setModal(true);
        mask.show();
        QEventLoop loop;

        connect(&msgBox, &TLMessageBox::sigButtonClicked,[&nReturn, &loop, &mask](TLPushButton *pBtn){
            if (pBtn != Q_NULLPTR)
            {
                nReturn = pBtn->property(ButtonType).toInt();
            }
            loop.quit();
//            mask.removeChild();
        });

        loop.exec(QEventLoop::DialogExec);
        return nReturn;
    }
}
