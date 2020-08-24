#include "c7radiowidget.h"
#include <QLabel>
#include <QPainter>
#include "c7commondef.h"
#include "tlutility.h"

C7RadioWidget::C7RadioWidget(QSize size, qreal rScaleFactor, QWidget *parent)
    : TLBaseControl(parent), m_rScaleFactor(rScaleFactor)
{
    this->resize(size);
    setFont(18 * m_rScaleFactor);
    initUi();
}

void C7RadioWidget::setText(const QString &text)
{
    m_pInfoDisplay->setText(text);
}

void C7RadioWidget::setFont(int pixelSize)
{
    m_font = Uti->getFont(pixelSize);
}

void C7RadioWidget::setChecked(bool status)
{
    QPalette pal;
    if (status)
    {
        pal.setColor(QPalette::Text, QColor(Color_MenuWidget_Text_Normal));
        m_pStatusDisplay->setPixmap(m_checkedPixmap);
    } else {
        pal.setColor(QPalette::Text, QColor(Color_MenuWidget_Text_Normal));
        m_pStatusDisplay->setPixmap(m_normalPixmap);
    }

    m_pInfoDisplay->setPalette(pal);
}

void C7RadioWidget::resizeEvent(QResizeEvent *event)
{

}

void C7RadioWidget::initUi()
{
    QString strAppPath = Uti->path(Apppath);
    m_normalPixmap.load(strAppPath + "/skin/settings/btn_check_normal.png");
    m_checkedPixmap.load(strAppPath + "/skin/settings/btn_check_checked.png");

    m_pStatusDisplay = new QLabel(this);
    m_pStatusDisplay->resize(20 * m_rScaleFactor, 20 * m_rScaleFactor);
    m_pStatusDisplay->move(this->width() - 75 * m_rScaleFactor,
                     (this->height() - m_pStatusDisplay->height()) / 2);
    m_pStatusDisplay->setScaledContents(true);
    m_pStatusDisplay->setPixmap(m_normalPixmap);

    QPalette pal;
    m_pInfoDisplay = new QLabel(this);
    m_pInfoDisplay->resize(this->width() - m_pStatusDisplay->width() - 100 * m_rScaleFactor, 30 * m_rScaleFactor);
    m_pInfoDisplay->move(50 * m_rScaleFactor, (this->height() - m_pInfoDisplay->height()) / 2);
    pal.setColor(QPalette::Text, QColor(255, 255, 255));
    m_pInfoDisplay->setPalette(pal);
    m_pInfoDisplay->setFont(m_font);
}
