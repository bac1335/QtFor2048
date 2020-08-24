#ifndef C7MANAGERBASE_H
#define C7MANAGERBASE_H

#include <QObject>
#include "c7commondef.h"
#include <QColor>
#include "c7basewidget.h"
#include "tlmaskwidget.h"
#include "c7mainview.h"
#include "c7vendorscreensize.h"


/*
 * @brief   :页面管理基类，提取共同部分，派生去初始化国内版和海外版相关界面
 * @author  :Lynn
 * @date    :2019.12.19
 */

class C7OsPalmEraseWidget;
class C7NormalPenWidget;
class C7LargePenWidget;
class C7FingerWidget;
class C7EraserWidget;
class C7ThemeWidget;
class C7ImportWidget;
class C7ColorSelector;
class C7SaveWidget;
class C7LoginWidget;
class C7ShareWidget;
class C7ShareQRWidget;
class C7MainToolBar;
class C7LoginToolBarWidget;
class C7SimpleMessageBoxWidget;
class C7UIManager;
class C7VoteQRWidget;
class C7BoardViewVoteManager;
class C7QtRoManager;
class C7Screenshot2;
class C7ShareCore;
class TLNotice;
class C7SettingAccountWidget;
class C7UpdateWidget;
class C7GuidePage;
class C7MainToolBarManager;
class C7MoreMenuManager;
class C7ThumbnailManager;
class QtMqttCore;

class C7ManagerBase : public QObject
{
    Q_OBJECT
public:
    explicit C7ManagerBase(QObject *parent = nullptr);
    void saveConfig(QString group,ToolBarPenParams params);
    void buildParams(ToolBarPenParams & params,
                     PenStyle style,
                     PenWidthType width,
                     PenColorType colorType,
                     QColor color = QColor());
    //动画显示界面
    void showHideWithAnimation(C7BaseWidget* widget,bool show = true);
    //窗口添加遮罩和动画
    TLMaskWidget *addAnimationWidgetMask(QWidget *pWidget,bool model = true); //添加遮罩，因为隐藏时需要动画，所以添加这个接口
    ToolBarPenParams _createParams(PenStyle pen,int width,
                                  PenWidthType widthType,QColor color,
                                   PenColorType colorType);
    void hideAllWidget(QList<QWidget*> lst);
    //以前颜色值是黄色，后来改成蓝色。这里为了兼容之前的配置文件，将黄色改成蓝色
    void fixColor(QColor & color);

protected slots:
    virtual void onNotice(QString) = 0;    //文字提示
    void onShapeGestureMode(int mode);//进入图形识别的手势模式

protected:
    qreal m_rScaleFactor = 1.0;                             //缩放系数
    QMap<PenColorType,QColor> m_mapColor;  //颜色值对应
    QMap<PenStyle,QList<int>> m_mapPenToWidth;  //每种笔型对应的三种笔宽实际值
    C7VendorScreenSize* m_ScreenSize = nullptr;
    QMap<ThemeType,QColor> m_mapThemeToColor; //主题背景对应的颜色，由于海外版和国内版颜色不一样，所以全部由UI端控制
    qreal m_mmPerPixel = 1;

protected:
    //初始化笔参数值
    void initParamsMap();
};

#endif // C7MANAGERBASE_H
