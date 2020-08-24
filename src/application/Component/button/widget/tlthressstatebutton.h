#ifndef TLTHRESSSTATEBUTTON_H
#define TLTHRESSSTATEBUTTON_H

#include "base/widget/tlbasecontrol.h"
#include "base/tlbuttoninterface.h"

class TLThressStateButton : public TLBaseControl, public TLButtonInterface
{
    Q_OBJECT
public:
    enum StateType {
        Normal_State,
        Select_State,
        Press_State
    };
    TLThressStateButton(QWidget* parent = nullptr);

    void setSelectText(const QString& text);
    void setSelectImage(const QString& imagePath);
    void setSelectImage(const QPixmap& image);
    void setSelectBackColor(const QColor& color);
    void setSelectTextColor(const QColor& color);

    void setCurrentState(StateType state);
signals:
    void sigStateChange(StateType stateType);

protected:
    void paintEvent(QPaintEvent* event);

private slots:
    void onClick();

private:
    bool m_bSelect = false;
    StateType m_currentState = Normal_State;
    QString m_selectText;
    QPixmap m_selectImage;
    QColor m_selectBackColor;
    QColor m_selectTextColor;
};

#endif // TLTHRESSSTATEBUTTON_H
