#ifndef C7THEMEWIDGET_H
#define C7THEMEWIDGET_H

#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "c7basewidget.h"
#include "c7themeslect.h"
#include "tlpushbutton.h"
#include "c7commondef.h"
#include  "../../c7lib_code/c7lib/include/c7pendef.h"


/**
  * @brief 主题选择界面
  * @author lls
  * @date 2019-09-11
  */

class C7ThemeWidget : public C7BaseWidget
{
    Q_OBJECT
public:

    C7ThemeWidget(int w, int h, QWidget *parent = nullptr);
    ~C7ThemeWidget();
    //外部初始化时
    void initTheme(ThemeType type);
    //加载云文件后设置背景
    void setTheme(ThemeType themeType);
    //是否使用商业背景
    void setBusinessBg(bool);

protected:
    void initUi();
    void retranslateUi();
    void paintEvent(QPaintEvent *e);

private:
    void initTextLabelLayout();

private slots:
    void onThemeChoose();

signals:
    void sigThemeChanged(ThemeType);

private:
    QImage m_bg;
    QList<QLabel *> m_textLabelList;
    C7ThemeSlect *m_preThemeBtn = nullptr;
    QList<C7ThemeSlect *> m_themeBtnList;
    ThemeType m_curThemeType;
    TLPushButton* m_pCloseBtn = nullptr;
    int m_colorBgNum;
};

#endif // C7THEMEWIDGET_H
