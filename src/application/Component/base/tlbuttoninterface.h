#ifndef TLBUTTONINTERFACE_H
#define TLBUTTONINTERFACE_H

#include <QFont>
#include <QPixmap>

/*
 * 用于定义按键的通用接口
 * 注意：这里的checked和pressed的显示(文字和图片)不做区别，
 * 后续可根据实际情况决定是否区分。
 *
 */

class TLButtonInterface
{
public:
    TLButtonInterface();
    //设置按键的布局
    enum ButtonLayout {
        Layout_Tiled,           //平铺，文字覆盖在图片上方，文字正中间
        Layout_Tiled2,           //平铺，文字覆盖在图片偏下方
        Layout_LeftToRight,     //图片在左 文字右
        Layout_RightToLeft,     //图片在右 文字左
        Layout_UpToDown,        //图片在上 文字下
        Layout_DownToUp         //图片在下 文字上
    };
    void setButtonLayout(ButtonLayout layout);

    virtual void setText(const QString& text);
    void setImage(const QString& imagePath);
    void setImage(const QPixmap& image);
    void setBackColor(const QColor& color);
    void setTextColor(const QColor& color);
    void setTextFont(const QFont& font);

    void setNormalText(const QString& text);
    void setNormalImage(const QString& imagePath);
    void setNormalImage(const QPixmap& image);
    void setNormalBackColor(const QColor& color);
    void setNormalTextColor(const QColor& color);
    QColor getNormalBackColor(){return m_normalBackColor;}

    void setPressText(const QString& text);
    void setPressImage(const QString& imagePath);
    void setPressImage(const QPixmap& image);
    void setPressBackColor(const QColor& color);
    void setPressTextColor(const QColor& color);

    void setHoverText(const QString& text);
    void setHoverImage(const QString& imagePath);
    void setHoverImage(const QPixmap& image);
    void setHoverBackColor(const QColor& color);
    void setHoverTextColor(const QColor& color);

    void setDisableText(const QString& text);
    void setDisableImage(const QString& imagePath);
    void setDisableImage(const QPixmap& image);
    void setDisableBackColor(const QColor& color);
    void setDisableTextColor(const QColor& color);
    void setRadius(int radius){m_radius = radius;}
    void setBorderLine(bool f){m_borderline = f;}
    void setTextIndent(int indent);
    void setTextHeight(int height);
    void setBorderLineColor(QColor color){borderLineColor = color;}

protected:
    int m_nTextIndent = 0;          //设置文本距离边框的距离和ButtonLayout配合使用
    int m_nTextHeight = -1;          //设置文本的高度，用于调整文字和图片的位置

    int m_radius = 0; //设置边框圆角,默认是0
    ButtonLayout m_btnLayout = Layout_Tiled;
    QFont m_font;
    QString m_normalText;
    QPixmap m_normalImage;
    QColor m_normalBackColor;
    QColor m_normalTextColor;

    QString m_pressText;
    QPixmap m_pressImage;
    QColor m_pressBackColor;
    QColor m_pressTextColor;

    QString m_hoverText;
    QPixmap m_hoverImage;
    QColor m_hoverBackColor;
    QColor m_hoverTextColor;

    QString m_disableText;
    QPixmap m_disableImage;
    QColor m_disableBackColor;
    QColor m_disableTextColor;

    bool m_borderline = 0;  //是否使用边框线，默认是0
    QColor borderLineColor = QColor(0,0,0,125);  //默认黑色半透明

protected:
    virtual void initParams();  //初始化参数
};

#endif // TLBUTTONINTERFACE_H
