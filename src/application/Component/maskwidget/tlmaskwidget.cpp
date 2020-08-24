#include "tlmaskwidget.h"

#include <QPainter>
#include <QSound>
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>

TLMaskWidget::TLMaskWidget(QWidget *parent) : QWidget(parent)
{
    m_brush = QBrush(QColor(0, 0, 0, 80));
}

TLMaskWidget::~TLMaskWidget()
{
    QWidget* child = childWidget();
    if (child != nullptr) {
        child->setParent(this->parentWidget());
    }
}

void TLMaskWidget::setBackgroundBrush(const QBrush &brush)
{
    m_brush = brush;
    update();
}

void TLMaskWidget::setModal(bool bModal)
{
    if(bModal) {
        setWindowModality(Qt::WindowModal);
    } else {
        setWindowModality(Qt::NonModal);
    }
}

void TLMaskWidget::setChild(QWidget *widget, const QPoint &centerPos)
{
    Q_ASSERT(childWidget() == nullptr);
    QWidget* childParent = widget->parentWidget();
    widget->setParent(this);
    widget->installEventFilter(this);

    if (childParent != nullptr) {
        Q_ASSERT(this->parentWidget() == nullptr);
        this->setParent(childParent);
        this->setFixedSize(childParent->size());
        setCenter(widget, centerPos);
        setCenter(this);
    } else {
        this->setParent(nullptr);
        this->setFixedSize(qApp->desktop()->size());
        setCenter(widget);
    }
}

void TLMaskWidget::setClickHide(bool bClickHide)
{
    m_bClickHide = bClickHide;
}

void TLMaskWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(), m_brush);
}

void TLMaskWidget::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    emit sigHide();
}

void TLMaskWidget::mousePressEvent(QMouseEvent *event)
{

    /**
     * 存在传进来的widget()他依旧会下发鼠标事件给parent()，所以这里要判断这个鼠标事件是否是widget()传回来的
     */
    QRect rect;

    QWidget* child = childWidget();
    if (child != nullptr) {
        rect = child->rect();
        rect.moveTo(child->pos());
    }

    if (rect.contains(event->pos())) {
        //是子widget()传回来的事件，忽略
        return;
    }

    emit sigClicked();
    if (isModal()) {

        QSound::play(":/maskwidget/sound/WindowsBackground.wav");
    } else {
        if (m_bClickHide){
            this->hide();
        }
    }
    event->accept();
}

//这个是为了配合子窗口的操作
//比如子窗口hide这个也要跟hide
bool TLMaskWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj->parent() == this) {
        switch (event->type()) {
        case QEvent::Quit:
        case QEvent::Hide:
        case QEvent::Close:
            this->hide();
            break;
        case QEvent::Destroy:
            this->deleteLater();
            break;
        case QEvent::Resize:
            QWidget* parent = this->parentWidget();
            if(parent != nullptr){
                this->setFixedSize(parent->size());
            }
            break;
        }
    }

    return QWidget::eventFilter(obj, event);
}

//因为里面有可能会重新对centerPos赋值，所以不用引用
void TLMaskWidget::setCenter(QWidget *widget, QPoint centerPos)
{
    if (widget == nullptr) {
        return;
    }

    QPoint p;
    //如果没有指定centerPos
    if (centerPos .isNull()) {
        QWidget* parent = widget->parentWidget();
        //有父窗口移到父窗口的中间
        if (parent != nullptr) {
            centerPos = QPoint(parent->width() / 2, parent->height() / 2);
        } else {
            //没有父窗口移到桌面的中间
            QWidget* desktop = qApp->desktop()->screen();
            centerPos = QPoint(desktop->width() / 2, desktop->height() / 2);
        }
    }

    p = centerPos - QPoint(widget->width() / 2, widget->height() / 2);

    widget->move(p);
}

QWidget *TLMaskWidget::childWidget() const
{
    QList<QWidget *> widgets;

    foreach (QObject *obj, this->children()) {
        QWidget* child = qobject_cast<QWidget *> (obj);
        if (child != nullptr) {
            widgets << child;
        }
    }

    Q_ASSERT(widgets.size() <= 1);

    return widgets.isEmpty() ? nullptr : widgets.first();
}
