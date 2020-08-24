#include "c7settingsitem.h"

#include <QMouseEvent>
#include <QPainter>

C7SettingsItem::C7SettingsItem(int width, int height, QWidget *parent)
    : C7BaseWidget(width, height, parent)
{
    initUi();
}

void C7SettingsItem::setChecked(bool bCheck)
{
    m_bChecked = bCheck;
    update();
}

void C7SettingsItem::setTextColor(const QColor &normal, const QColor &checked)
{
    m_normalTextColor = normal;
    m_checkedTextColor = checked;
}

void C7SettingsItem::setText(const QString &text)
{
    m_text = text;
}

void C7SettingsItem::initUi()
{
    m_checkedBg.load(m_strAppPath + "/skin/settings/item_checked.png");
}

void C7SettingsItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::NoBrush);
    painter.setFont(m_font);

    if (m_bChecked) {
        painter.drawPixmap(QRect(5, 0, 4 * m_rScaleFactor, this->height()),
                           m_checkedBg);
        painter.setPen(m_checkedTextColor);
    } else {
        painter.setPen(m_normalTextColor);
    }
    painter.drawText(QRect(30 * m_rScaleFactor, 0, 125 * m_rScaleFactor, this->height()),
                     m_text, Qt::AlignLeft | Qt::AlignVCenter);


    C7BaseWidget::paintEvent(event);
}

void C7SettingsItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (this->rect().contains(event->pos())) {
        m_bChecked = true;
        emit sigClicked();
        update();
    }

    C7BaseWidget::mouseReleaseEvent(event);
}
