#ifndef C7SIMPLEMESSAGEBOXWIDGET_H
#define C7SIMPLEMESSAGEBOXWIDGET_H

/*
 * @brief   :页面弹窗提示窗
 * @author  :lls
 * @date    :2019.11.08
 */

#include "c7basewidget.h"
#include "tlpushbutton.h"
#include <QLabel>

class C7SimpleMessageBoxWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7SimpleMessageBoxWidget(QWidget* parent = nullptr);
    void setText(QString str);
    void setPixmap(const QPixmap& pix);
    void questionWithMask(QString str,bool useMask);
    void tipsWithMask(QString str,bool useMask);  //消息提示框


private:
    void initUi();
    void paintEvent(QPaintEvent* e);

protected:
    void resizeEvent(QResizeEvent* event);
    void retranslateUi();

private:
    TLPushButton* m_cancelBtn = nullptr;
    TLPushButton* m_okBtn = nullptr;
    QPixmap m_bg;
    int m_round = 0;
    QPixmap m_icon;
    QLabel * m_pContentLabel = nullptr;

signals:
    void sigCancelClicked();
    void sigOkClicked();
    void sigQuesTion();
    void sigQuesTionQuit();

};

#endif // C7SIMPLEMESSAGEBOXWIDGET_H
