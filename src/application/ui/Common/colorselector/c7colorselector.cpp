#include "c7colorselector.h"

#include <QLabel>
#include <QPainter>
#include <QPropertyAnimation>

#include "c7colorwheel.h"
#include "tlpushbutton.h"
#include "c7commondef.h"

C7ColorSelector::C7ColorSelector(int width, int height, QWidget *parent)
    : C7BaseWidget(width, height, parent)
{
    initUi();
}

void C7ColorSelector::setColor(const QColor &color)
{
    m_lastColor = color;
    onSetColor(color);
    m_pColorWheel->setPositionFormColor(color);
}

void C7ColorSelector::initUi()
{
    m_bg.load(m_strAppPath + "/skin/colorselector/bg_colour.png");
    QPalette palette;
    palette.setColor(QPalette::Text,Color_LabelText_Black);
    //标题
    m_pLblTitle = new QLabel(this);
    m_pLblTitle->setFont(m_font);
    m_pLblTitle->setAlignment(Qt::AlignCenter);
    m_pLblTitle->resize(this->width(), 20 * m_rScaleFactor);
    m_pLblTitle->move(0, 16 * m_rScaleFactor);
    m_pLblTitle->setPalette(palette);

    //色盘
    m_pColorWheel = new C7ColorWheel(121 * m_rScaleFactor, this);
    connect(m_pColorWheel, &C7ColorWheel::sigColorChange,
            this, &C7ColorSelector::onSetColor);
    m_pColorWheel->move(66 * m_rScaleFactor, 94  * m_rScaleFactor);

    //颜色显示
    m_pLblColor = new QLabel(this);
    m_pLblColor->resize(100 * m_rScaleFactor, 100 * m_rScaleFactor);
    m_pLblColor->move(m_pColorWheel->x() + m_pColorWheel->width() +  130 * m_rScaleFactor ,
                      m_pLblTitle->y() + m_pLblTitle->height() + 98 * m_rScaleFactor);

    //颜色数值显示
    m_pLblColorValue = new QLabel(this);
    m_pLblColorValue->setText(QString("#4dad4a"));
    m_pLblColorValue->setAlignment(Qt::AlignCenter);
    m_pLblColorValue->setFont(m_font);
    m_pLblColorValue->resize(100 * m_rScaleFactor, 40 * m_rScaleFactor);
    m_pLblColorValue->move(m_pLblColor->x(), m_pLblColor->y() + m_pLblColor->height() + 45 * m_rScaleFactor);
    m_pLblColorValue->setPalette(palette);

    //确定
    m_pOkBtn = new TLPushButton(this);
    m_pOkBtn->setFixedSize(102 * m_rScaleFactor, 48 * m_rScaleFactor);
    m_pOkBtn->move(this->width() - 44 * m_rScaleFactor - m_pOkBtn->width(),
                   this->height() - 32 * m_rScaleFactor - m_pOkBtn->height());
    m_pOkBtn->setNormalBackColor(Color_MenuButton_Normal);
    m_pOkBtn->setHoverBackColor(Color_MenuButton_Hover);
    m_pOkBtn->setPressBackColor(Color_MenuButton_Press);
    m_pOkBtn->setDisableBackColor(QColor("#666666"));
    m_pOkBtn->setDisableTextColor(Color_MenuButton_Text);
    m_pOkBtn->setTextColor(Color_MenuButton_Text);
    m_pOkBtn->setRadius(6 * m_rScaleFactor);
    m_pOkBtn->setTextColor(Qt::white);
    m_pOkBtn->setFont(m_font);
    connect(m_pOkBtn,SIGNAL(sigClicked()),this,SLOT(onOkClick()));

    //取消
    m_pCancelBtn = new TLPushButton(this);
    m_pCancelBtn->setFixedSize(102 * m_rScaleFactor, 48 * m_rScaleFactor);
    m_pCancelBtn->move(m_pOkBtn->x() - 30 * m_rScaleFactor - m_pCancelBtn->width(), m_pOkBtn->y());
    m_pCancelBtn->setBorderLine(true);
    m_pCancelBtn->setNormalBackColor(Color_CancelButton_Normal);
    m_pCancelBtn->setHoverBackColor(Color_CancelButton_Normal);
    m_pCancelBtn->setPressBackColor(Color_CancelButton_Press);
    m_pCancelBtn->setDisableBackColor(QColor("#666666"));
    m_pCancelBtn->setDisableTextColor(Color_MenuButton_Text);
    m_pCancelBtn->setTextColor(Color_MenuButton_Text);
    m_pCancelBtn->setRadius(6 * m_rScaleFactor);
    m_pCancelBtn->setFont(m_font);
    m_pCancelBtn->setTextColor(Qt::black);
    connect(m_pCancelBtn,SIGNAL(sigClicked()),this,SLOT(onCancelClick()));

    retranslateUi();
}

void C7ColorSelector::retranslateUi()
{
    m_pLblTitle->setText(tr("Custom"));
    m_pOkBtn->setText(tr("Ok"));
    m_pCancelBtn->setText(tr("Cancel"));
}

void C7ColorSelector::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPixmap(this->rect(), m_bg, m_bg.rect());
    QWidget::paintEvent(event);
}

void C7ColorSelector::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    m_pColorWheel->setPositionFormColor(m_currentColor);
    onSetColor(m_currentColor);

    C7BaseWidget::showEvent(event);
}

void C7ColorSelector::resizeEvent(QResizeEvent *event)
{
    qreal tempScale = m_rScaleFactor;       //保存原有缩放系数
    if (m_bUseAnimation)
    {
        m_rScaleFactor = this->width() * 1.0 / m_nNormalWidth*tempScale;        // 计算现在的缩放系数
        onScaleFactorChange();
    }

    m_pLblTitle->resize(this->width(), 20 * m_rScaleFactor);
    m_pLblTitle->move(0, 16 * m_rScaleFactor);
    m_pLblTitle->setFont(m_font);

    m_pColorWheel->setScaleFactor(m_rScaleFactor);
//    m_pColorWheel->setSelectRadius(3 * m_rScaleFactor);
    m_pColorWheel->setRadius(121 * m_rScaleFactor);

    if (m_rScaleFactor == tempScale)
    {
        m_pColorWheel->setSelectVisible(true);
        m_pColorWheel->setPositionFormColor(m_currentColor);
    }
    else
    {
        m_pColorWheel->setSelectVisible(false);
    }

    m_pColorWheel->move(66 * m_rScaleFactor, 94  * m_rScaleFactor);

    m_pLblColor->resize(100 * m_rScaleFactor, 100 * m_rScaleFactor);
    m_pLblColor->move(m_pColorWheel->x() + m_pColorWheel->width() +  130 * m_rScaleFactor ,
                      m_pLblTitle->y() + m_pLblTitle->height() + 98 * m_rScaleFactor);

    m_pLblColorValue->resize(100 * m_rScaleFactor, 40 * m_rScaleFactor);
    m_pLblColorValue->move(m_pLblColor->x(), m_pLblColor->y() + m_pLblColor->height() + 45 * m_rScaleFactor);
    m_pLblColorValue->setFont(m_font);

    m_pOkBtn->setFixedSize(102 * m_rScaleFactor, 48 * m_rScaleFactor);
    m_pOkBtn->move(this->width() - 44 * m_rScaleFactor - m_pOkBtn->width(),
                   this->height() - 32 * m_rScaleFactor - m_pOkBtn->height());
    m_pOkBtn->setFont(m_font);

    m_pCancelBtn->setFixedSize(102 * m_rScaleFactor, 48 * m_rScaleFactor);
    m_pCancelBtn->move(m_pOkBtn->x() - 30 * m_rScaleFactor - m_pCancelBtn->width(), m_pOkBtn->y());
    m_pCancelBtn->setFont(m_font);

    if (m_bUseAnimation)
    {
        m_rScaleFactor = tempScale; //恢复原有缩放系数
    }

    QWidget::resizeEvent(event);
}

void C7ColorSelector::onSetColor(const QColor &color)
{
    m_currentColor = color;
    m_pLblColor->setStyleSheet(QString("background:%1;border-radius:%2px;")
                               .arg(color.name())
                               .arg(10 * m_rScaleFactor));
    m_pLblColorValue->setText(QString("%1").arg(color.name()));
}

void C7ColorSelector::onOkClick()
{
    m_lastColor = m_currentColor;
    emit sigColorChange(m_currentColor,getCustomType());

    if (m_bUseAnimation)
    {
        this->hideWithAnimation();
    }
    else
    {
        this->hide();
    }
}

void C7ColorSelector::onCancelClick()
{
    emit sigCancel(getCustomType());
    if (m_bUseAnimation)
    {
        this->hideWithAnimation();
    }
    else
    {
        this->hide();
    }
}
