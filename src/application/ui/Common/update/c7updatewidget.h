#ifndef C7UPDATEWIDGET_H
#define C7UPDATEWIDGET_H

#include "c7basewidget.h"

class QLabel;
class TLPushButton;
class QTextEdit;

struct UpdateVersionInfo{
    QString curVersion;
    QString newVersion;
    QString descridText;
    QString packagePath;
};

class C7UpdateWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7UpdateWidget(int w,int h,QWidget *parent = nullptr);

    void setUpdateInfo(const UpdateVersionInfo &info);

protected:
    void initUi();
    void retranslateUi();
    void paintEvent(QPaintEvent *e);

signals:
    void sigUpdate(const QString &path);

private:
    QImage m_bg;

    QLabel *m_pIconLabel = nullptr;
    QLabel *m_pTitleLabel = nullptr;
    QLabel *m_pDetailLabel = nullptr;
    QTextEdit *m_pDescribEdit = nullptr;
    TLPushButton *m_pUpdateBtn = nullptr;
    TLPushButton *m_pCancelBtn = nullptr;

    QString m_packagePath;
    QString m_curVersion;
    QString m_newVersion;
};

#endif // C7UPDATEWIDGET_H
