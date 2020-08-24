#ifndef C7LINEEDiTWIDGET_H
#define C7LINEEDiTWIDGET_H

#include <QLineEdit>
#include <QPainter>
#include <QFocusEvent>
#include <QProcess>
#include "keyboard/tlvirtualkeyboard.h"
//#include "c7application.h"
#include <QGraphicsView>

/**
 * @brief The CbCommonLineEdit class
 * 该类copy自类CbLineEdit，把原来使用照片做样式改为自绘，其他逻辑功能不变
 * @author xinde.song
 * @date 2018.3.28
 */

class C7CommonLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit C7CommonLineEdit(QWidget *parent = nullptr);
    ~C7CommonLineEdit();
    inline void setKey(bool key) {m_key = key;}//键盘特殊处理
    inline void setPaste(bool paste) {m_paste = paste;}
    inline bool getPaste() const {return m_paste;}//是否可以粘贴
    void setRadius(int xRadius,int yRadius);
    void setColor(QColor  color);
    void setShowFrame(bool b);
    void setDefaultText(QString str);
    void setNoInputChinese();//设置不能输入中文
    bool isHaveFocus(){return m_foucus;}
    static void setCurrentParentView(QGraphicsView* pParentView);
signals:
    void sigToClear();
    void sigPressed();
    void sigGetFocus();
    void sigLoseFocus();
    void sigEnterPressed();
protected:
    //输入框全选、复制、粘贴功能
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);
    void hideEvent(QHideEvent *e);
private:
    void windowsTabTip();//调用微软接口
private:
    QString m_defaultText;
    QValidator* m_validator = nullptr;
    QColor m_color = QColor(41,187,235);
    bool m_foucus = false;	//焦点
    bool m_key = false;	//键盘特殊处理
    bool m_paste = false; //正在粘贴
    bool m_frame = true;
    int m_count = 0;

    int m_nXRadius;
    int m_nYRadius;

    static QGraphicsView* m_parentView ;
};

#endif // C7LINEEDiTWIDGET_H
