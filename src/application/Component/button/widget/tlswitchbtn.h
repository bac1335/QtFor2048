#ifndef TLSWITCHBTN_H
#define TLSWITCHBTN_H

#include "tlcheckbtn.h"

/*
 * @brief   :开关按键，带动画，图片优先
 * @author  :hongcs
 * @date    :2018.11.06
 */

class QLabel;
class QPropertyAnimation;

class TLSwitchBtn : public TLCheckBtn
{
    Q_OBJECT
public:
    explicit TLSwitchBtn(QWidget* parent = nullptr);

    void setFgNormalImage(const QPixmap& pixmap);
    void setFgChcekedImage(const QPixmap& pixmap);
    void setFgNormalColor(const QColor& color);
    void setFgCheckedColor(const QColor& color);

    void setChecked(bool bChecked);
protected:
    void resizeEvent(QResizeEvent *event);
private slots:
    void onChecked(bool bChecked);
private:
    void setEndValue();
private:
    QPixmap m_fgNormalImage;
    QPixmap m_fgCheckedImage;
    QColor m_fgNormalColor;
    QString m_fgNormalColorString;      //因为颜色是用样式表弄的，所以要有rgb的字符串，下同
    QColor m_fgCheckedColor;
    QString m_fgCheckedColorString;

    QLabel* m_pSwitchLabel = nullptr;
    QPropertyAnimation* m_pAnimation = nullptr;
};

#endif // TLSWITCHBTN_H
