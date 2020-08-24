#include "c7themeslect.h"
//#include "c7application.h"

C7ThemeSlect::C7ThemeSlect(QString imgPath,qreal scale,QWidget *parent):
    QWidget(parent), m_nScale(scale)
{
    m_bChosen = false;
    m_img.load(imgPath);
}

void C7ThemeSlect::setChecked(bool check)
{
    m_bChosen = check;
    update();
}

void C7ThemeSlect::setChooseFrameImage(const QString &imagePath)
{
    m_chooseFrameImage.load(imagePath);
    m_chooseFrameImage = m_chooseFrameImage.scaled(26 * m_nScale,26 * m_nScale,
                                                   Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void C7ThemeSlect::mouseReleaseEvent(QMouseEvent *e)
{
    emit sigThemeChosen();
}

void C7ThemeSlect::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);

//    painter.drawPixmap(3 * m_nScale, 3 * m_nScale, m_img.width()/2*m_nScale, m_img.height()/2*m_nScale, m_img);
    painter.drawPixmap(this->rect(),m_img);

    if(m_bChosen){
        painter.drawPixmap(QRect((this->width()-m_chooseFrameImage.width())/2.,
                                 (this->height()-m_chooseFrameImage.height())/2.,
                                 m_chooseFrameImage.width(),m_chooseFrameImage.height()),
                           m_chooseFrameImage);
    }

    QWidget::paintEvent(e);
}
