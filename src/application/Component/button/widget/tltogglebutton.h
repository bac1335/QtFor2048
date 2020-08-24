#ifndef TLTOGGLEBUTTON_H
#define TLTOGGLEBUTTON_H

/*
 * @brief   :开关按钮
 * 每次点击都会切换状态，只有两种状态checked <--> unckecked
 * 每次点击发出sigCheck(bool)信号
 * 该按钮 上面是图标，下面是文字描述
 * @author  :Lynn
 * @date    :2019.09.17
 */

#include "tlbasecontrol.h"
#include "tlbuttoninterface.h"

class TLToggleButton : public TLBaseControl, public TLButtonInterface
{
    Q_OBJECT
public:
    TLToggleButton(QWidget* parent = nullptr);

    void setChecked(bool bchecked){
        m_bChecked = bchecked;
        update();
    }
    bool isChecked() const{
        return m_bChecked;
    }

    void setTextIndent(int nIndent) { m_nTextIndent = nIndent; }//这个是设置文字距离底部的像素

    void setText(const QString& text) override;
signals:
    void sigChecked(bool checked);
public slots:
    virtual void onButtonClicked();
protected:
    void paintEvent(QPaintEvent *e);

private:
    bool m_bChecked = false;
//    int m_nTextIndent = 0;
};

#endif // TLTOGGLEBUTTON_H
