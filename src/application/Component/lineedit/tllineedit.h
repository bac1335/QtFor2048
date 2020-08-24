#ifndef TLLINEEDIT_H
#define TLLINEEDIT_H

#include <QLineEdit>

/****************************************************************************
 * @author  :Hongcansi
 * @date    :2018.11.02
 * @brief   :重绘Lineedit，无边框，可设置清空按键图片、占位文本的颜色，调用虚拟键盘等
 * @other   :
****************************************************************************/

class QPushButton;

class TLLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    TLLineEdit(QWidget *parent = nullptr);

    enum InputLimit{
        Limit_Number = 0x01,             //数字
        Limit_Letter_Lower = 0x02,       //小写字母
        Limit_Letter_Upper = 0x04,       //大写字母
        Limit_Symbol = 0x08,             //英文符号，需要配合m_allowSymbol参数使用,默认是全部不可用
        Limit_Chinese = 0x10,            //中文(包括中文符号)
        Limit_Letter = Limit_Letter_Lower | Limit_Letter_Upper,
        Limit_NoChinese = Limit_Number | Limit_Letter | Limit_Symbol,
        Limit_Free = Limit_Number | Limit_Letter | Limit_Symbol | Limit_Chinese,
        Limit_Number_Letter = Limit_Number | Limit_Letter
    };
    void setAllowSymbol(const QString& strSymbol);
    void setInputLimit(int limit);

    void setScaleFactor(qreal factor);
    void setTextColor(const QColor& color);
    void setPlaceholderTextColor(const QColor& color);
    void setClearBtnEnable(bool bEnable, const QString& imgPath);
    void setVirtualKeyboard(bool bUsed);
    //设置光标的颜色
    void setBlinkColor(const QColor& color);
    void setBgLine(bool line){m_usingBgLine = line;}

signals:
    void sigKeyboardVisible(bool bVisible);

protected:
    void paintEvent(QPaintEvent* event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
private:
    bool m_bBlink = true;       //控制光标用
    bool m_bClearBtn = false;   //控制清空按键用
    bool m_bUseVirtualKeyboard = false; //控制是否使用虚拟键盘
    int m_nIndent = 0;
    int m_inputLimit = Limit_Free;
    qreal m_rScaleFactor = 1.0;//缩放系数，多分辨率用
    QColor m_blickColor = QColor(0, 0, 0);
    QColor m_textColor;
    QColor m_placeHolderTextColor = QColor(194, 194, 194);
    QColor m_selectBackColor = QColor(92, 177, 255);
    QPushButton* m_pClearBtn = nullptr;
    QString m_allowSymbol;
    bool m_usingBgLine = false;
};

#endif // TLLINEEDIT_H
