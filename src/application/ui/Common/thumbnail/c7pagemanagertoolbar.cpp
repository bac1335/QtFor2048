#include "c7pagemanagertoolbar.h"
#include <QPainter>
#include "tlpushbutton.h"
#include "tltogglebutton.h"
#include "tlutility.h"

C7PageManagerToolBar::C7PageManagerToolBar(int branch, QWidget *parent):
    C7BaseWidget (parent), m_nCurPage (1), m_nTotalPage (1),m_nBranch(branch)
{
    initUi();
}

int C7PageManagerToolBar::getCurrentIndex() const
{
    return m_nCurPage;
}

void C7PageManagerToolBar::setCurrentIndex(int pageId, int allPage)
{
    m_nCurPage = pageId;
    m_nTotalPage = allPage;

    if(allPage == 1){
        m_pNextPageBtn->setEnabled(false);
        m_pLastPageBtn->setEnabled(false);
    }
    else{
        if(m_nCurPage <= 1){
            m_pLastPageBtn->setEnabled(false);
            m_pNextPageBtn->setEnabled(true);
        }
        else if(m_nCurPage >= m_nTotalPage){
            m_pNextPageBtn->setEnabled(false);
            m_pLastPageBtn->setEnabled(true);
        }
        else{
            m_pNextPageBtn->setEnabled(true);
            m_pLastPageBtn->setEnabled(true);
        }
    }
    m_pSelectPageBtn->setText(QString("%1/%2").arg(pageId).arg(allPage));
}

void C7PageManagerToolBar::initToolBarStatus()
{
    if(m_pSelectPageBtn->isChecked()){
        m_pSelectPageBtn->setChecked(false);
    }
}

void C7PageManagerToolBar::setBtnCheck(bool check)
{
    m_pSelectPageBtn->setChecked(check);
}

void C7PageManagerToolBar::initUi()
{
    QString fileName;
    if(m_nBranch == C7Version::Branch_Normal){
        fileName = "internal";
    }
    else if (m_nBranch == C7Version::Branch_Oversea) {
        fileName = "oversea";
    }
    m_bg.load(m_strAppPath + QString("/skin/%1/pageManager/bg_page.png").arg(fileName));

    m_pLastPageBtn = new TLPushButton(this);
    m_pNextPageBtn = new TLPushButton(this);
    m_pAddPageBtn = new TLPushButton(this);
    m_pNextPageBtn->setEnabled(false);
    m_pLastPageBtn->setEnabled(false);
    m_btnList << m_pLastPageBtn << m_pNextPageBtn << m_pAddPageBtn;

    QStringList iconList;
    iconList << "pageup" << "pagedown" << "addpg";

    int index = 0;
    foreach (auto btn, m_btnList) {
        btn->setNormalImage(m_strAppPath +
                            QString("/skin/%1/pageManager/icon_%2_normal.png").arg(fileName).arg(iconList.at(index)));
        btn->setPressImage(m_strAppPath +
                           QString("/skin/%1/pageManager/icon_%2_press.png").arg(fileName).arg(iconList.at(index)));
        btn->setHoverImage(m_strAppPath +
                           QString("/skin/%1/pageManager/icon_%2_normal.png").arg(fileName).arg(iconList.at(index)));
        if(index != 2){
            btn->setDisableImage(m_strAppPath +
                                 QString("/skin/%1/pageManager/icon_%2_Unavailable.png").arg(fileName).arg(iconList.at(index)));
        }
        if(m_nBranch == C7Version::Branch_Normal){
            btn->resize(68*m_rScaleFactor, 64*m_rScaleFactor);
        }
        else if(m_nBranch == C7Version::Branch_Oversea){
            btn->resize(56*m_rScaleFactor, 56*m_rScaleFactor);
        }
        index++;
        connect(btn, &TLPushButton::sigClicked, this, &C7PageManagerToolBar::onBtnClicked);
    }

    //显示
    m_pSelectPageBtn = new TLToggleButton(this);
    m_pSelectPageBtn->setNormalImage(m_strAppPath + QString("/skin/%1/pageManager/icon_slt_normal.png").arg(fileName));
    m_pSelectPageBtn->setPressImage(m_strAppPath + QString("/skin/%1/pageManager/icon_slt_press.png").arg(fileName));
    m_pSelectPageBtn->setHoverImage(m_strAppPath + QString("/skin/%1/pageManager/icon_slt_normal.png").arg(fileName));
    m_pSelectPageBtn->setFont(Uti->getFont(12*m_rScaleFactor));
    if(m_nBranch == C7Version::Branch_Normal){
        m_pSelectPageBtn->resize(68*m_rScaleFactor, 64*m_rScaleFactor);
    }
    else if(m_nBranch == C7Version::Branch_Oversea){
        m_pSelectPageBtn->resize(56*m_rScaleFactor, 56*m_rScaleFactor);
    }
    m_pSelectPageBtn->setText(QString("%1/%2").arg(m_nCurPage).arg(m_nTotalPage));
    connect(m_pSelectPageBtn, &TLToggleButton::sigChecked, this, &C7PageManagerToolBar::sigShowPage);
}

void C7PageManagerToolBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(this->rect(), m_bg);
    C7BaseWidget::paintEvent(event);
}

void C7PageManagerToolBar::resizeEvent(QResizeEvent *event)
{
    if(m_nBranch == C7Version::Branch_Normal){
        m_pLastPageBtn->move(this->width() - m_pLastPageBtn->width(), 19*m_rScaleFactor);
        m_pSelectPageBtn->move(m_pLastPageBtn->x(), m_pLastPageBtn->y() + m_pLastPageBtn->height());
        m_pNextPageBtn->move(m_pLastPageBtn->x(), m_pSelectPageBtn->y() + m_pSelectPageBtn->height());
        m_pAddPageBtn->move(m_pLastPageBtn->x(), m_pNextPageBtn->y() + m_pNextPageBtn->height());
    }
    else if(m_nBranch == C7Version::Branch_Oversea){
        m_pLastPageBtn->move(14*m_rScaleFactor, this->height()/2 - m_pLastPageBtn->height()/2);
        m_pSelectPageBtn->move(m_pLastPageBtn->x() + m_pLastPageBtn->width(), m_pLastPageBtn->y());
        m_pNextPageBtn->move(m_pSelectPageBtn->x() + m_pSelectPageBtn->width(), m_pLastPageBtn->y());
        m_pAddPageBtn->move(m_pNextPageBtn->x() + m_pNextPageBtn->width() + 9*m_rScaleFactor, m_pLastPageBtn->y());
    }
    C7BaseWidget::resizeEvent(event);
}

void C7PageManagerToolBar::onBtnClicked()
{
    TLPushButton *btn = qobject_cast<TLPushButton *>(sender());
    int btnIndex = m_btnList.indexOf(btn);
    switch (btnIndex) {
    case Type_LastPage:
        if(!m_pNextPageBtn->isEnabled()){
            m_pNextPageBtn->setEnabled(true);
        }
        if(m_nCurPage == 1){
            m_pLastPageBtn->setEnabled(false);
            break;
        }
        m_nCurPage--;
        emit sigPageChange(m_nCurPage);
        break;
    case Type_NextPage:
        if(!m_pLastPageBtn->isEnabled()){
            m_pLastPageBtn->setEnabled(true);
        }
        if(m_nCurPage >= m_nTotalPage){
            m_pNextPageBtn->setEnabled(false);
            break;
        }
        m_nCurPage++;
        emit sigPageChange(m_nCurPage);
        break;
    case Type_AddPage:
        m_pSelectPageBtn->setChecked(false);
        emit sigAddPage();
        break;
    default:
        break;
    }
}
