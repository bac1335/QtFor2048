#ifndef C7SETTINGSITEM_H
#define C7SETTINGSITEM_H

#include "c7basewidget.h"

/*
 * @brief   : 设置左侧按键
 * @author  : lls
 * @date    : 2019.9.4
 */

class C7SettingsItem : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7SettingsItem(int width, int height, QWidget* parent = nullptr);

    void setChecked(bool bCheck);
    void setTextColor(const QColor& normal, const QColor& checked);
    void setText(const QString& text);
signals:
    void sigClicked();

private:
    void initUi();
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    bool m_bChecked = false;
    QPixmap m_checkedBg;
    QString m_text;
    QColor m_normalTextColor;
    QColor m_checkedTextColor;

};

#endif // C7SETTINGSITEM_H
