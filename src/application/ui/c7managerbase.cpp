#include "c7managerbase.h"
#include "tlutility.h"
#include "c7oem.h"
#include <QTimer>

C7ManagerBase::C7ManagerBase(QObject *parent) :
    QObject(parent)
{
    m_rScaleFactor = Uti->getScaleFactor();
//    initParamsMap(); //需屏幕尺寸初始化之后进行map初始化
//    initScreenSize();
}

void C7ManagerBase::saveConfig(QString group, ToolBarPenParams params)
{
    switch (params.penStyle) {
    case Style_Normal_Pen:
        Uti->setValue(group + "_Normal","colorType",params.colorType);
        Uti->setValue(group + "_Normal","color",params.color.name());
        Uti->setValue(group + "_Normal","WidthType",params.widthType);
        Uti->setValue(group + "_Normal","Width",params.width);
        break;
    case Style_Fountain_Pen:
        Uti->setValue(group + "_Fountain","colorType",params.colorType);
        Uti->setValue(group + "_Fountain","color",params.color.name());
        Uti->setValue(group + "_Fountain","WidthType",params.widthType);
        Uti->setValue(group + "_Fountain","Width",params.width);
        break;
    case Style_Pencil:
        Uti->setValue(group + "_Pencil","WidthType",params.widthType);
        Uti->setValue(group + "_Pencil","Width",params.width);
        Uti->setValue(group + "_Pencil","colorType",params.colorType);
        Uti->setValue(group + "_Pencil","color",params.color.name());
        break;
    case Style_Marker_Pen:
        Uti->setValue(group + "_Marker","colorType",params.colorType);
        Uti->setValue(group + "_Marker","color",params.color.name());
        Uti->setValue(group + "_Marker","WidthType",params.widthType);
        Uti->setValue(group + "_Marker","Width",params.width);
        break;
    case Style_Crayon:
        Uti->setValue(group + "_Crayon","colorType",params.colorType);
        Uti->setValue(group + "_Crayon","color",params.color.name());
        Uti->setValue(group + "_Crayon","WidthType",params.widthType);
        Uti->setValue(group + "_Crayon","Width",params.width);
        break;
    case Style_Brush_Pen:
        Uti->setValue(group + "_Brush","colorType",params.colorType);
        Uti->setValue(group + "_Brush","color",params.color.name());
        Uti->setValue(group + "_Brush","WidthType",params.widthType);
        Uti->setValue(group + "_Brush","Width",params.width);
        break;
    case Style_Finger_Eraser:
        Uti->setValue(group + "_Eraser","WidthType",params.widthType);
        Uti->setValue(group + "_Eraser","Width",params.width);
        break;
    }
}

void C7ManagerBase::buildParams(ToolBarPenParams &params, PenStyle style,
                                PenWidthType width, PenColorType colorType,QColor color)
{
    params.penStyle = style;
    params.widthType = width;
    params.width = m_mapPenToWidth.value(style).at((int)width);
    if(colorType != Color_Panel){
        if(params.colorType != colorType){
            params.colorType = colorType;
            params.color = m_mapColor.value(colorType);
        }
    }
    else {
        if(color.isValid()){
            params.colorType = colorType;
            params.color = color;
        }
    }
}

void C7ManagerBase::showHideWithAnimation(C7BaseWidget *widget, bool show)
{
    if(!widget){
        return;
    }
    QWidget* parent = widget->parentWidget();
    if(show)
    {
       if(widget->isVisible())
           return;
       if(parent){
           parent->raise();  //遮罩窗口置顶
           parent->show();
       }
       widget->raise();
       widget->showWithAnimation();
    }
    else{
        if(widget->isVisible())
            widget->hideWithAnimation();
    }
}

TLMaskWidget *C7ManagerBase::addAnimationWidgetMask(QWidget *pWidget, bool model)
{
    TLMaskWidget* pMask = Uti->addMask(pWidget);
    if (pMask != nullptr)
    {
        pMask->setBackgroundBrush(QBrush(QColor(0, 0, 0, 30)));
        pMask->setClickHide(model);
    }
    return pMask;
}

ToolBarPenParams C7ManagerBase::_createParams(PenStyle pen, int width, PenWidthType widthType, QColor color, PenColorType colorType)
{
    ToolBarPenParams params;
    params.penStyle = pen;
    params.width = width;
    params.widthType = widthType;
    params.color = color;
    params.colorType = colorType;
    return  params;
}

void C7ManagerBase::hideAllWidget(QList<QWidget *> lst)
{
    foreach (QWidget *pWidget, lst) {
        if(pWidget->isVisible()){
            C7BaseWidget* w = dynamic_cast<C7BaseWidget*>(pWidget);
            if(w){
                w->hideWithAnimation();
            }
        }
    }
}

void C7ManagerBase::fixColor(QColor &color)
{
    if(color == QColor("#e6cd01")){  //兼容之前的颜色值
        color = QColor("#00b2e5");
    }
}

void C7ManagerBase::onShapeGestureMode(int mode)
{
    qDebug() << "--->Lynn<---" << __FUNCTION__ << "mode"<<mode;
    QString text;
    switch (mode) {
    case 1:
        //已进入绘制圆弧/圆的模式
        text = tr("Entered arc/circle drawing mode");//绘制圆弧和圆
        break;
    case 2:
        //已进入绘制直线模式
        text = tr("Entered the line drawing mode");//绘制直线
        break;
    case 3:
        //已进入组合多边形模式
        text = tr("Entered combined polygon mode");//组合多边形
        break;
    case 4:
        //已进入几何图形智能识别模式
        text = tr("Entered the geometry intelligent recognition mode(Circle/Rectangle/Triangle/Line)");//图形识别
        break;
    default:
        break;
    }
    onNotice(text);
}

void C7ManagerBase::initParamsMap()
{
    m_mapColor.insert(Color_Black,QColor(Qt::black));
    m_mapColor.insert(Color_Red,QColor(Qt::red));
    m_mapColor.insert(Color_Blue,QColor(0,178,229));
    m_mapPenToWidth.insert(Style_Normal_Pen,
                           QList<int>() << round(NormalPen_Width_0  / m_mmPerPixel )<<round(NormalPen_Width_1 / m_mmPerPixel)<<round(NormalPen_Width_2 / m_mmPerPixel));
    m_mapPenToWidth.insert(Style_Fountain_Pen,
                           QList<int>() << round(FountainPen_Width_0  / m_mmPerPixel)<<round(FountainPen_Width_1 / m_mmPerPixel)<<round(FountainPen_Width_2 / m_mmPerPixel));
    m_mapPenToWidth.insert(Style_Pencil,
                           QList<int>() << round(Pencil_Width_0 / m_mmPerPixel) <<round(Pencil_Width_1 / m_mmPerPixel)<<round(Pencil_Width_2 / m_mmPerPixel));
    m_mapPenToWidth.insert(Style_Marker_Pen,
                           QList<int>() << round(MarkerPen_Width_0 / m_mmPerPixel) <<round(MarkerPen_Width_1 / m_mmPerPixel)<<round(MarkerPen_Width_2 / m_mmPerPixel));
    m_mapPenToWidth.insert(Style_Crayon,
                           QList<int>() << round(CrayonPen_Width_0 / m_mmPerPixel) <<round(CrayonPen_Width_1 / m_mmPerPixel)<<round(CrayonPen_Width_2 / m_mmPerPixel));
    m_mapPenToWidth.insert(Style_Brush_Pen,
                           QList<int>() << round(BrushPen_Width_0 / m_mmPerPixel) <<round(BrushPen_Width_1 / m_mmPerPixel)<<round(BrushPen_Width_2 / m_mmPerPixel));
    m_mapPenToWidth.insert(Style_Finger_Eraser,
                           QList<int>() << round(Eraser_Width_0 / m_mmPerPixel) <<round(Eraser_Width_1  / m_mmPerPixel) <<round(Eraser_Width_2 / m_mmPerPixel));
    m_mapThemeToColor.insert(Themecolor1,QColor(92,145,124));
    m_mapThemeToColor.insert(Themecolor3,QColor("#3aaea6"));
    m_mapThemeToColor.insert(Themecolor4,QColor("#6fbdc4"));
    m_mapThemeToColor.insert(Themecolor5,QColor("#c7eecc"));
    m_mapThemeToColor.insert(ThemeEisenhower,QColor("#ffffff"));
    m_mapThemeToColor.insert(ThemeFishBone,QColor("#ffffff"));
    m_mapThemeToColor.insert(ThemePDCA,QColor("#ffffff"));
    m_mapThemeToColor.insert(ThemeSWOTAnalysis,QColor("#ffffff"));

    if(cOem->version().branch == C7Version::Branch_Normal){
        m_mapThemeToColor.insert(Themecolor2,QColor("#39393c"));
        m_mapThemeToColor.insert(Themecolor6,QColor("#f5f5f5"));
    }
    else {
        m_mapThemeToColor.insert(Themecolor2,QColor("#000000"));
        m_mapThemeToColor.insert(Themecolor6,QColor("#ffffff"));
    }
}
