#include "c7settingparamswidget.h"
#include "tlutility.h"
#include "c7oem.h"


C7SettingParamsWidget::C7SettingParamsWidget(int width,
                                             int height,
                                             C7BaseWidget *parent):
    C7BaseWidget (width,height,parent)
{
    initUi();
    retranslateUi();
}

C7SettingParamsWidget::~C7SettingParamsWidget()
{

}

void C7SettingParamsWidget::setPalmEraserVisible(bool visible)
{
    m_bPalmVisible = visible;
    m_pPalmEraserItem->setVisible(visible);
    updatePos(visible);
}

void C7SettingParamsWidget::pullInitParams()
{
    qDebug() << "--->Lynn<---" << __FUNCTION__;
    QJsonObject json;
    json.insert("user_id",QString::number(cOem->getLoginState().userid));

    m_pHttpManager->setBody(&json);
    m_pHttpManager->setUrl(DEF_HTTP_GET_INIT_PARAMS);
    m_pHttpManager->send();
}

void C7SettingParamsWidget::pushInitParams(ParamsSettingsType type, bool check)
{
    if(!cOem->getLoginState().isLogin){
        return;
    }
    QJsonObject json;
    json.insert("user_id",QString::number(cOem->getLoginState().userid));

    switch (type) {
    case Params_Stroke:
        json.insert("penStatus",check);
        break;
    case Params_Chalk:
        json.insert("chalkStatus",check);
        break;
    case Params_Gesture:
        json.insert("gestureStatus",check);
        break;
    case Params_EraserProfile:
        json.insert("eraserStatus",check);
        break;
    case Params_GraphicInfo:
        json.insert("graphStatus",check);
        break;
    case Params_PalmEraser:
        json.insert("handStatus",check);
        break;
    default:
        break;
    }
    m_pHttpManager->setBody(&json);
    m_pHttpManager->setUrl(DEF_HTTP_SET_PARAMS);
    m_pHttpManager->send();
}

void C7SettingParamsWidget::initConfig()
{
    bool stroke = Uti->value(SETTING_GROUP,PARAM_STROKE,false).toBool();
    bool chalk = Uti->value(SETTING_GROUP,PARAM_CHALK,true).toBool();
    bool gesture = Uti->value(SETTING_GROUP,PARAM_GESTURE,true).toBool();
    bool eraserProfile = Uti->value(SETTING_GROUP,PARAM_ERASERPROFILE,false).toBool();
    bool graphicInfo = Uti->value(SETTING_GROUP,PARAM_GRAPHICINFO,false).toBool();
    bool plamEraser = Uti->value(SETTING_GROUP,PARAM_PALMERASER,true).toBool();
    setParams(Params_Stroke,stroke);
    setParams(Params_Chalk,chalk);
    setParams(Params_Gesture,gesture);
    setParams(Params_EraserProfile,eraserProfile);
    setParams(Params_GraphicInfo,graphicInfo);
    setParams(Params_PalmEraser,plamEraser);
}

void C7SettingParamsWidget::setParams(const ParamsSettingsType &type, bool check)
{
    if(type == Params_PalmEraser && !m_bPalmVisible){
        return;
    }
    CSwitchItem * item = m_mapItemToType.key(type,nullptr);
    if(item){
        item->setChecked(check);
        updateConfig(type,check);
    }
}

void C7SettingParamsWidget::updateConfig(ParamsSettingsType type, bool check)
{
    switch (type) {
    case Params_Stroke:
        Uti->setValue(SETTING_GROUP,PARAM_STROKE,check);
        break;
    case Params_Chalk:
        Uti->setValue(SETTING_GROUP,PARAM_CHALK,check);
        break;
    case Params_Gesture:
        Uti->setValue(SETTING_GROUP,PARAM_GESTURE,check);
        break;
    case Params_EraserProfile:
        Uti->setValue(SETTING_GROUP,PARAM_ERASERPROFILE,check);
        break;
    case Params_GraphicInfo:
        Uti->setValue(SETTING_GROUP,PARAM_GRAPHICINFO,check);
        break;
    case Params_PalmEraser:
        Uti->setValue(SETTING_GROUP,PARAM_PALMERASER,check);
        break;
    default:
        break;
    }
    emit sigParamsChanged(type,check);
}

void C7SettingParamsWidget::parseData(QJsonObject data)
{
    bool stroke = data["penStatus"].toBool(false);
    bool chalk = data["chalkStatus"].toBool(true);
    bool gesture = data["gestureStatus"].toBool(true);
    bool eraserProfile = data["eraserStatus"].toBool(false);
    bool graphicInfo = data["graphStatus"].toBool(false);
    bool plamEraser = data["handStatus"].toBool(true);
    setParams(Params_Stroke,stroke);
    setParams(Params_Chalk,chalk);
    setParams(Params_Gesture,gesture);
    setParams(Params_EraserProfile,eraserProfile);
    setParams(Params_GraphicInfo,graphicInfo);
    setParams(Params_PalmEraser,plamEraser);
}

void C7SettingParamsWidget::updatePos(bool havePalm)
{
    QList<CSwitchItem*> m_items;

#if 0
    if(havePalm){ //国内版需要手掌擦除
        m_items << m_pStrokeItem << m_pChalkItem <<m_pEraserProfileItem
                <<m_pDefaultGraphicInfoItem<<m_pPalmEraserItem << m_pGestureItem;
    }
    else {
        m_items << m_pStrokeItem << m_pChalkItem <<m_pEraserProfileItem
                <<m_pDefaultGraphicInfoItem << m_pGestureItem;
    }
#else //板擦轮廓暂时屏蔽
    if(havePalm){ //国内版需要手掌擦除
        m_items << m_pStrokeItem <<m_pPalmEraserItem << m_pChalkItem
                <<m_pDefaultGraphicInfoItem<< m_pGestureItem;
    }
    else {
        m_items << m_pStrokeItem << m_pChalkItem
                <<m_pDefaultGraphicInfoItem << m_pGestureItem;
    }
    m_pEraserProfileItem->hide();
#endif
    qreal temp = 35*m_rScaleFactor;
    m_items.at(0)->move(temp,20*m_rScaleFactor);
    for (int i = 1; i < m_items.size(); ++i) {
        m_items.at(i)->move(temp,m_items.at(i-1)->y() +
                            m_items.at(i-1)->height() + 10*m_rScaleFactor);
    }
    m_pGestureContent->move(temp,m_pGestureItem->y() + m_pGestureItem->height() + 5*m_rScaleFactor);
}

void C7SettingParamsWidget::initUi()
{
    m_pStrokeItem = createItem(tr("Stroke"),Params_Stroke);
    m_pChalkItem = createItem(tr("Chalk"),Params_Chalk);
    m_pGestureItem = createItem(tr("Gesture"),Params_Gesture);
    m_pEraserProfileItem = createItem(tr("Eraser Profile"),Params_EraserProfile);
    m_pDefaultGraphicInfoItem = createItem(tr("Default Graphic Info"),Params_GraphicInfo);
    m_pPalmEraserItem = createItem(tr("Palm Eraser"),Params_PalmEraser);
    m_pGestureContent = new QLabel(this);
    m_pGestureContent->setWordWrap(true);
    m_pGestureContent->setFixedWidth(410*m_rScaleFactor);
    QFont font = Uti->getFont(15*m_rScaleFactor);
    m_pGestureContent->setFont(font);
    m_pGestureContent->setText(tr("(Gestures include: moving the main scene with two fingers, drawing an arc with one finger, and drawing a straight line with three fingers)"));

    updatePos(true);

    m_pHttpManager = TLHttpFactory::GetInstance()->createHttp();
    connect(m_pHttpManager,SIGNAL(finished(QNetworkReply*)),
            this,SLOT(replyFinished(QNetworkReply*)));
}

void C7SettingParamsWidget::retranslateUi()
{
    m_pGestureContent->setText(tr("(Gestures include: moving the main scene with two fingers, drawing an arc with one finger, and drawing a straight line with three fingers)"));
    m_pStrokeItem->setText(tr("Stroke"));
    m_pChalkItem->setText(tr("Chalk"));
    m_pGestureItem->setText(tr("Gesture"));
    m_pEraserProfileItem->setText(tr("Eraser Profile"));
    m_pDefaultGraphicInfoItem->setText(tr("Default Graphic Info"));
    m_pPalmEraserItem->setText(tr("Palm Eraser"));
}

void C7SettingParamsWidget::onItemClicked(bool check)
{
    auto item = qobject_cast<CSwitchItem *>(sender());
    ParamsSettingsType type = m_mapItemToType.value(item);
//    emit sigParamsChanged(type,check);
    //更新本地配置文件
    updateConfig(type,check);
    //更新数据到服务器
    pushInitParams(type,check);
}

void C7SettingParamsWidget::replyFinished(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray ba = reply->readAll();
    qDebug() << __FUNCTION__ << " server status code = "<< statusCode << " ba=" << QString(ba);
    if(ba.isEmpty()){
        emit sigNetworkError(ERROR_CODE_NETWORK);
        return;
    }
    QJsonParseError jsonpe;
    QJsonDocument json = QJsonDocument::fromJson(ba, &jsonpe);
    if (jsonpe.error == QJsonParseError::NoError && statusCode == 200)
    {
        if (json.isObject())
        {
            QJsonObject obj = json.object();
            if (obj.contains("status_code"))
            {
                if(obj["status_code"] == API_SUCCESS_CODE)
                {
                    if(obj.contains("data")){
                        QJsonObject data = obj["data"].toObject();
                        parseData(data);
                    }
                }
                else {
                    emit sigNetworkError(ERROR_CODE_STATUSCODE);
                }
            }
            else {
                emit sigNetworkError(ERROR_CODE_PARAMS);
            }
        }
        else {
            emit sigNetworkError(ERROR_CODE_PARAMS);
        }
    }
    else {
        emit sigNetworkError(ERROR_CODE_SERVER_ABNORMAL);
    }
}

CSwitchItem *C7SettingParamsWidget::createItem(const QString &text,
                                               const ParamsSettingsType &type)
{
    CSwitchItem * item = new CSwitchItem(567*m_rScaleFactor,35*m_rScaleFactor,this);
    item->setText(text);
    m_mapItemToType.insert(item,type);
    connect(item,&CSwitchItem::sigItemClicked,this,
            &C7SettingParamsWidget::onItemClicked);
    return  item;
}


CSwitchItem::CSwitchItem(int width, int height, C7BaseWidget *parent):
    C7BaseWidget (width,height,parent)
{
    initUi();
}

CSwitchItem::~CSwitchItem()
{

}

void CSwitchItem::setText(const QString &text)
{
    m_pText->setText(text);
}

void CSwitchItem::setChecked(bool check)
{
//    if(check != m_pSWitchBtn->isChecked()){
//    }
    m_pSWitchBtn->setChecked(check);
}

void CSwitchItem::initUi()
{
    m_pText = new QLabel(this);
    m_pText->setFont(m_font);

    m_pSWitchBtn = new TLSwitchBtn(this);
    m_pSWitchBtn->resize(50*m_rScaleFactor,25*m_rScaleFactor);
    m_pSWitchBtn->setNormalImage(m_strAppPath + "/skin/settings/btn_switch_close.png");
    m_pSWitchBtn->setPressImage(m_strAppPath + "/skin/settings/btn_switch_open.png");
    m_pSWitchBtn->setFgNormalImage(QPixmap(m_strAppPath + "/skin/settings/btn_switch.png"));
    m_pSWitchBtn->setFgChcekedImage(QPixmap(m_strAppPath + "/skin/settings/btn_switch.png"));
    connect(m_pSWitchBtn, &TLSwitchBtn::sigChecked, this, &CSwitchItem::sigItemClicked);

    m_pText->move(0,this->height()/2.-m_pText->sizeHint().height()/2.);
    m_pSWitchBtn->move(this->width()-m_pSWitchBtn->width(),this->height()/2.-m_pSWitchBtn->height()/2.);
}
