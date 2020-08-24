#ifndef C7SETTINGPARAMSWIDGET_H
#define C7SETTINGPARAMSWIDGET_H

#include "c7basewidget.h"
#include <QLabel>
#include "tlswitchbtn.h"
#include "c7commondef.h"
#include "tlhttpmanager.h"
#include "tlhttpfactory.h"
/*
 * @brief   :参数设置页面，本页面数据跟随登录账号
 * @author  :Lynn
 * @date    :2020.03.10
 */
class CSwitchItem;
class C7SettingParamsWidget : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit C7SettingParamsWidget(int width,int height,C7BaseWidget * parent = nullptr);
    ~C7SettingParamsWidget();
    void setPalmEraserVisible(bool visible);
    void pullInitParams();  //从服务器请求账号绑定的数据
    void pushInitParams(ParamsSettingsType type, bool check);  //更新服务器数据
    void initConfig();//从文件中读取参数并进行初始化

protected:
    void initUi() Q_DECL_OVERRIDE;
    void retranslateUi() Q_DECL_OVERRIDE;

signals:
    void sigParamsChanged(const ParamsSettingsType &type,bool check);
    void sigNetworkError(int code);
private slots:
    void onItemClicked(bool check);
    void replyFinished(QNetworkReply *reply);
private:
    CSwitchItem * createItem(const QString &text,
                             const ParamsSettingsType &type);
    void setParams(const ParamsSettingsType &type,bool check);
    void updateConfig(ParamsSettingsType type,bool check);//更新配置
    void parseData(QJsonObject data);
    void updatePos(bool havePalm);
private:
    CSwitchItem * m_pStrokeItem = nullptr;                      //笔锋
    CSwitchItem * m_pChalkItem = nullptr;                       //粉笔灰
    CSwitchItem * m_pGestureItem = nullptr;                     //手势
    CSwitchItem * m_pEraserProfileItem = nullptr;               //板擦框
    CSwitchItem * m_pDefaultGraphicInfoItem = nullptr;          //默认显示图形信息
    CSwitchItem * m_pPalmEraserItem = nullptr;                  //手掌擦除
    QLabel * m_pGestureContent = nullptr;                       //手势说明
    QMap<CSwitchItem*,ParamsSettingsType> m_mapItemToType;
    TLHttpManager * m_pHttpManager = nullptr;
    bool m_bPalmVisible = true;
};

//页面中的每一个item元素

class CSwitchItem : public C7BaseWidget
{
    Q_OBJECT
public:
    explicit CSwitchItem(int width,int height,C7BaseWidget * parent = nullptr);
    ~CSwitchItem();
    void setText(const QString & text);
    void setChecked(bool check);
protected:
    void initUi() Q_DECL_OVERRIDE;

signals:
    void sigItemClicked(bool check);

private:
    QLabel * m_pText = nullptr;
    TLSwitchBtn * m_pSWitchBtn = nullptr;
};

#endif // C7SETTINGPARAMSWIDGET_H
