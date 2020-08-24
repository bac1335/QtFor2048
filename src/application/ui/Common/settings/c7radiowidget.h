#ifndef C7RadioWidget_H
#define C7RadioWidget_H
#include "base/widget/tlbasecontrol.h"

/*
 * @brief   : 设置radio控件
 * @author  : lls
 * @date    : 2019.9.04
 */

class QLabel;

class C7RadioWidget : public TLBaseControl
{
    Q_OBJECT
public:
    C7RadioWidget(QSize size, qreal rScaleFactor = 1.0, QWidget *parent = nullptr);

    void setText(const QString& text);
    void setFont(int pixelSize);
    void setChecked(bool status);
protected:
    void resizeEvent(QResizeEvent *event);
private:
    void initUi();
private:
    QFont m_font;
    qreal m_rScaleFactor = 1.0;
    QPixmap m_normalPixmap;
    QPixmap m_checkedPixmap;
    QLabel* m_pStatusDisplay = nullptr;
    QLabel* m_pInfoDisplay = nullptr;
};

#endif // C7RadioWidget_H
