#include "tllineedit.h"

#include <QTimer>
#include <QPainter>
#include <QPushButton>
#include <QFontMetrics>
#include <QRegExpValidator>

#include "keyboard/tlvirtualkeyboard.h"

TLLineEdit::TLLineEdit(QWidget *parent) : QLineEdit(parent)
{
//    this->setStyleSheet(QString("QLineEdit{border: none;}"));
    m_allowSymbol.clear();

    QTimer* pTimer = new QTimer(this);
    pTimer->setInterval(500);
    connect(pTimer, &QTimer::timeout, this, [=]{
        m_bBlink = !m_bBlink;
        update();
    } );
    pTimer->start();
}

void TLLineEdit::setAllowSymbol(const QString &strSymbol)
{
    m_allowSymbol = strSymbol;
}

void TLLineEdit::setInputLimit(int limit)
{
    m_inputLimit = limit;

    QString letterUpper = QString("");
    QString letterLower = QString("");
    QString number = QString("");
    QString chinese = QString("");
    if (m_inputLimit & Limit_Number) {
        number = QString("0-9");
    }
    if (m_inputLimit & Limit_Letter_Lower) {
        letterLower = QString("a-z");
    }
    if (m_inputLimit & Limit_Letter_Upper) {
        letterUpper = QString("A-Z");
    }
    if (m_inputLimit & Limit_Chinese) {
        chinese = QString("\u4e00-\u9fa5");
    }

    QString strRegExp = QString("[%1%2%3%4%5]+$")
                        .arg(letterUpper)
                        .arg(letterLower)
                        .arg(number)
                        .arg(chinese)
                        .arg(m_allowSymbol);

    QValidator *validator = new QRegExpValidator(QRegExp(strRegExp), this);
    this->setValidator(validator);
}

void TLLineEdit::setScaleFactor(qreal factor)
{
    m_rScaleFactor = factor;
}

void TLLineEdit::setTextColor(const QColor &color)
{
    m_textColor = color;
}

void TLLineEdit::setPlaceholderTextColor(const QColor &color)
{
    m_placeHolderTextColor = color;
}

void TLLineEdit::setClearBtnEnable(bool bEnable, const QString &imgPath)
{
    m_bClearBtn = bEnable;
    if (m_bClearBtn) {
        if (m_pClearBtn == nullptr) {
            m_pClearBtn = new QPushButton(this);
            m_pClearBtn->resize(20 * m_rScaleFactor, 20 * m_rScaleFactor);
            m_pClearBtn->move(this->width() - m_pClearBtn->width(),
                              (this->height() - m_pClearBtn->height()) / 2);
            m_pClearBtn->setCursor(Qt::ArrowCursor);
            m_pClearBtn->setFocusPolicy(Qt::NoFocus);
            update();
            connect(m_pClearBtn, &QPushButton::clicked, [=]{
                this->clear();
                this->setFocus();
                update();
            });
        }

        if (!imgPath.isEmpty()) {
            m_pClearBtn->setStyleSheet(QString("QPushButton{background:rgb(255,255,255); border-image: url(%1);}").arg(imgPath));
        }
    }
}

void TLLineEdit::setVirtualKeyboard(bool bUsed)
{
    m_bUseVirtualKeyboard = bUsed;
}

void TLLineEdit::setBlinkColor(const QColor &color)
{
    m_blickColor = color;
}

void TLLineEdit::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //设置选中背景
    if (this->hasSelectedText()) {
        QFontMetrics fm(this->font());
        QRect rec = fm.boundingRect(this->displayText().left(this->selectionStart()));
        int xPos = rec.width();
        rec = fm.boundingRect(this->displayText().mid(this->selectionStart(), this->selectedText().length()));
        int height = this->font().pixelSize() + 4 * m_rScaleFactor; //这里的4是上下多加一点
        int yPos = (this->height() - height) / 2;
        int width = rec.width() + 2 * m_rScaleFactor;
        painter.fillRect(QRect(xPos, yPos, width, height), m_selectBackColor);
    }

    painter.setFont(this->font());
    //绘制占位文本
    if (this->text().isEmpty()) {
        painter.setPen(m_placeHolderTextColor);
        painter.drawText(this->rect(), Qt::AlignLeft | Qt::AlignVCenter, this->placeholderText());

        if (m_pClearBtn != nullptr)
        {
            m_pClearBtn->hide();
        }
    } else {
        //绘制显示文本
        int pos = this->cursorPosition();
        QFontMetrics fm(this->font());
        QRect rec = fm.boundingRect(this->displayText().left(pos));
        //这个rect是为了实现光标移动
        QRect textRect = fm.boundingRect(this->displayText());
        textRect.setY(0);
        textRect.setHeight(this->height());
        if (rec.width() > this->width())
        {
            int cleanBtnWidth = 0;
            if (m_pClearBtn != nullptr)
            {
                cleanBtnWidth = m_pClearBtn->width();
            }
            textRect.setX(-(rec.width() - this->width() + cleanBtnWidth));
        }
        textRect.setWidth(textRect.width() + 2 * m_rScaleFactor);   //这里的加2是为了防止出现字母画不全的情况，特别是小写p
        painter.setPen(m_textColor);

        painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, this->displayText());
        if (m_bClearBtn)
        {
            if (m_pClearBtn != nullptr)
            {
                m_pClearBtn->show();
            }
        }
    }

    if (this->hasFocus() && m_bBlink) {
        int pos = this->cursorPosition();
        QFontMetrics fm(this->font());
        QRect rec = fm.boundingRect(this->displayText().left(pos));
        //字符串所占的像素宽度,高度
        int xPos = rec.width() + 1;
        if(m_pClearBtn)
        {
            if(xPos > this->width() - m_pClearBtn->width())
            {
                xPos = this->width() - m_pClearBtn->width();
            }
        } else {
            if(xPos > this->width())
            {
                xPos = this->width();
            }
        }

        int height = this->font().pixelSize() + 4 * m_rScaleFactor;   //这里的4是上下多加一点
        int yPos = (this->height() - height) / 2;
        painter.setPen(QPen(m_blickColor));
        painter.drawLine(QPointF(xPos, yPos), QPointF(xPos, yPos + height));
    }

    if(m_usingBgLine){
        painter.setPen(Qt::gray);
        painter.drawRect(this->rect());
    }
}

void TLLineEdit::focusInEvent(QFocusEvent *event)
{
    TLVirtualKeyboard* pKeyboard = TLVirtualKeyboard::keyboardInstance();

 //   pKeyboard->raise();
 //   pKeyboard->show();
    QLineEdit::focusInEvent(event);
}

void TLLineEdit::focusOutEvent(QFocusEvent *event)
{
    if(TLVirtualKeyboard::keyboardInstance()->isVisible())
    {
        TLVirtualKeyboard::keyboardInstance()->hide();
    }

    QLineEdit::focusOutEvent(event);
}
