#ifndef C7SETTINGABOUTWIDGET_H
#define C7SETTINGABOUTWIDGET_H
#include "c7basewidget.h"
#include <QLabel>

/*
 * @brief   : 关于界面
 * @author  : lls
 * @date    : 2019.9.05
 */

class C7SettingAboutWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7SettingAboutWidget(int width,int height,C7BaseWidget* parent = nullptr);
    ~C7SettingAboutWidget();
    void setVerson(qreal verson);
    void changeScreenSize(int size);

private:
    void initUi();

protected:
    void resizeEvent(QResizeEvent *event);
    void retranslateUi();
    void paintEvent(QPaintEvent *event);

private:
    int m_sSize = 0;
    QString m_versionStr;

    QPixmap m_logoBg;
    QLabel *m_pLinkLabel = nullptr;
    int m_version;
};

#endif // C7SETTINGABOUTWIDGET_H
