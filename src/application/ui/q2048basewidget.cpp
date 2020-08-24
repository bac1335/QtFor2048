#include "q2048basewidget.h"
#include "Utility/tlutility.h"
#include <QDebug>

Q2048BaseWidget::Q2048BaseWidget(QWidget *parent):
    QWidget(parent)
{
    m_scaleFactory = Uti->getScaleFactor();
    initColor();
}

void Q2048BaseWidget::initColor()
{
    m_colorMap.insert(2,"#eee5d9");
    m_colorMap.insert(4,"#ece1c8");
    m_colorMap.insert(8,"#f0b378");
    m_colorMap.insert(16,"#f39666");
    m_colorMap.insert(32,"#f67d5d");
    m_colorMap.insert(64,"#f4623c");
    m_colorMap.insert(128,"#ead170");
    m_colorMap.insert(256,"#edce61");
    m_colorMap.insert(512,"#ecc950");
    m_colorMap.insert(1024,"#edc63e");

}
