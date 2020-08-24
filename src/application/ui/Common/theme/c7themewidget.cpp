#include "c7themewidget.h"
#include "tlutility.h"
#include "messagebox/c7simplemessageboxwidget.h"
#include <QEventLoop>

C7ThemeWidget::C7ThemeWidget(int w, int h, QWidget *parent):
    C7BaseWidget(w, h, parent), m_curThemeType(Themecolor1)
{
    initUi();
    retranslateUi();
}

C7ThemeWidget::~C7ThemeWidget()
{

}

void C7ThemeWidget::setTheme(ThemeType themeType)
{
    int index = (int)(themeType);
    index = (index >= m_themeBtnList.size() || index < 0)?0:index;
    m_curThemeType = themeType; 
    C7ThemeSlect *themBtn = m_themeBtnList.at(index);

    if(m_preThemeBtn)
    {
        m_preThemeBtn->setChecked(false);
    }
    themBtn->setChecked(true);
    m_preThemeBtn = themBtn;
}

void C7ThemeWidget::setBusinessBg(bool useBusiness)
{
    m_textLabelList.at(2)->setVisible(useBusiness);
    for(int i = m_colorBgNum;i < m_themeBtnList.size();++i){
        m_themeBtnList.at(i)->setVisible(useBusiness);
    }
}

void C7ThemeWidget::initUi()
{
    m_bg.load(m_strAppPath + "/skin/theme/bg_theme.png");

    //标题
    initTextLabelLayout();

    //纯色背景
    for(int i = 0; i < 6; ++i)
    {
        C7ThemeSlect *bgBtn = new C7ThemeSlect(m_strAppPath + "/skin/theme/bg_color-"
                                             + QString::number(i + 1) + ".png",m_rScaleFactor,this);
        connect(bgBtn,SIGNAL(sigThemeChosen()),this,SLOT(onThemeChoose()));
        bgBtn->setChooseFrameImage(m_strAppPath + "/skin/button/btn_Selection.png");
        m_themeBtnList.append(bgBtn);
        bgBtn->resize(60 * m_rScaleFactor,60 * m_rScaleFactor);
        //默认选择第一个
        if(!i)
        {
            QPoint p1 = m_textLabelList.at(1)->pos();
            bgBtn->setChecked(true);
            bgBtn->move(p1.x() , p1.y() + 40 * m_rScaleFactor);
            m_preThemeBtn = bgBtn;
        }
        else{
            QPoint pos = m_themeBtnList.at(i - 1)->pos();
            bgBtn->move(pos.x() + bgBtn->width() + 20 * m_rScaleFactor,pos.y());
        }
    }
    m_colorBgNum = m_themeBtnList.size();


    //商业背景
    for(int i = 0; i < 4; ++i)
    {
        C7ThemeSlect *exerciseBtn = new C7ThemeSlect(m_strAppPath + "/skin/theme/pic_exercisebook" +
                                                   QString::number(i + 1) + ".png",m_rScaleFactor,this);
        m_themeBtnList.append(exerciseBtn);
        exerciseBtn->setChooseFrameImage(m_strAppPath + "/skin/button/btn_Selection.png");
        connect(exerciseBtn,SIGNAL(sigThemeChosen()),this,SLOT(onThemeChoose()));
        exerciseBtn->resize(60 * m_rScaleFactor,60 * m_rScaleFactor);

        if(!i)
        {
            QPoint p1 = m_textLabelList.at(2)->pos();
            exerciseBtn->move(p1.x() , p1.y() + 40 * m_rScaleFactor);
        }
        else{
            QPoint pos = m_themeBtnList.at(m_colorBgNum + i - 1)->pos();
            exerciseBtn->move(pos.x() + exerciseBtn->width() + 20 * m_rScaleFactor,pos.y());
        }
    }

    m_pCloseBtn = new TLPushButton(this);
    m_pCloseBtn->setFixedSize(24 * m_rScaleFactor, 24 * m_rScaleFactor);
    m_pCloseBtn->move(this->width() - m_pCloseBtn->width() - 26*m_rScaleFactor,
                      26*m_rScaleFactor);
    m_pCloseBtn->setImage(m_strAppPath + "/skin/button/btn_close.png");
    connect(m_pCloseBtn,&TLPushButton::sigRelease,this,&C7ThemeWidget::hideWithAnimation);

}

void C7ThemeWidget::retranslateUi()
{
    QStringList labelText;
    labelText << tr("Background Setting") << tr("Pure Background") << tr("Business Background  (Can't move canvas)");
    for(int i = 0; i < labelText.count(); ++i)
    {
        m_textLabelList.at(i)->setText(labelText.at(i));
    }
}

void C7ThemeWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawImage(this->rect(),m_bg);
    QWidget::paintEvent(e);
}

//创建文字标题
void C7ThemeWidget::initTextLabelLayout()
{
    QPalette pa;
    pa.setColor(QPalette::Text,QColor(80, 82, 91));
    for(int i = 0; i < 3; ++i)
    {
        QLabel *explainLabel = new QLabel(this);
        explainLabel->setFont(Uti->getFont(18*m_rScaleFactor));
        explainLabel->resize(this->width() - 40*m_rScaleFactor,explainLabel->sizeHint().height());
        explainLabel->setPalette(pa);
        m_textLabelList.append(explainLabel);
        if(!i)
        {
            explainLabel->move(20*m_rScaleFactor,25*m_rScaleFactor);
            explainLabel->setAlignment(Qt::AlignCenter);
        }
        else if(i == 1){
            m_textLabelList.at(i)->move(60* m_rScaleFactor,90 * m_rScaleFactor);
        }
        else{
            QPoint pos = m_textLabelList.at(i - 1)->pos();
            m_textLabelList.at(i)->move(pos.x(),pos.y() + 150 * m_rScaleFactor);
        }
    }
}

void C7ThemeWidget::initTheme(ThemeType type)
{
    if(m_preThemeBtn)
         m_preThemeBtn->setChecked(false);

     m_curThemeType = (ThemeType)(type);
     C7ThemeSlect * seleted = m_themeBtnList.at((int)m_curThemeType);
     if(!m_themeBtnList.contains(seleted))
         seleted = m_themeBtnList.at(0);
     seleted->setChecked(true);
     m_preThemeBtn = seleted;
}

void C7ThemeWidget::onThemeChoose()
{
    C7SimpleMessageBoxWidget messageBox(this->parentWidget());
    connect(&messageBox, &C7SimpleMessageBoxWidget::sigQuesTion,[=](){
        C7ThemeSlect *themBtn = qobject_cast<C7ThemeSlect *>(sender());
        emit sigThemeChanged((ThemeType)m_themeBtnList.indexOf(themBtn));
        if(m_preThemeBtn)
        {
            m_preThemeBtn->setChecked(false);
        }
        themBtn->setChecked(true);
        m_preThemeBtn = themBtn;
        this->hide();
    });
    messageBox.questionWithMask(tr("Switching the background will clear the current all content, do you want to switch?"),true);
}
