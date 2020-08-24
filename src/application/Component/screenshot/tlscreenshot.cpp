#include "tlscreenshot.h"

#include <QDir>
#include <QDebug>
#include <QTimer>
#include <QPainter>
#include <QDateTime>
#include <QGraphicsSceneMouseEvent>
#include <Windows.h>
#include <atlimage.h>
#include <QGraphicsView>
#include <QStandardPaths>
#include <QThread>
#include "button/graphics/tlgraphicspushbutton.h"

#define MIN_WIDTH 5
#define MIN_HEIGHT 5

#define BTN_TYPE "BtnType"

TLScreenshot::TLScreenshot(QSize size, QGraphicsObject *parent, QGraphicsView *parentView)
    : QGraphicsObject(parent)
{
    m_size = size;
    m_pParentView = parentView;
    initUi();
    connect(this, &TLScreenshot::sigBtnPress, this, &TLScreenshot::onBtnClick);
}

TLScreenshot::~TLScreenshot()
{

}

void TLScreenshot::cut(bool bSelectArea)
{
    m_bSelectArea = bSelectArea;
    if (m_bSelectArea) {
        m_bGestureMode = true;
        m_bTouchMode = false;
    }

    update();
}

void TLScreenshot::setShadowColor(const QColor &color)
{
    m_shadowColor = color;
}

void TLScreenshot::setSelectColor(const QColor &color)
{
    m_selectColor = color;
}

void TLScreenshot::onGetSavePath(const QString &savePath)
{
    execScreenShot(true, savePath);
}

void TLScreenshot::initUi()
{
//    m_pScreenshotPixmap = new TLScreenshotPixmap(QPixmap(), this);
//    m_pScreenshotPixmap->setZValue(this->zValue() + 1);

    m_pSaveButton = createGraphicsPushButton(QString(":/screenshot/image/icon_save_normal.png"),
                                             QString(":/screenshot/image/icon_save_press.png"));
    m_pSaveButton->setZValue(this->zValue() + 3);
    m_pSaveButton->setProperty(BTN_TYPE, BtnType_Save);

    m_pExitButton = createGraphicsPushButton(QString(":/screenshot/image/icon_close_normal.png"),
                                             QString(":/screenshot/image/icon_close_press.png"));
    m_pExitButton->setZValue(this->zValue() + 3);
    m_pExitButton->setProperty(BTN_TYPE, BtnType_Exit);


    m_pConfirmButton = createGraphicsPushButton(QString(":/screenshot/image/icon_finish_normal.png"),
                                                QString(":/screenshot/image/icon_finish_press.png"));
    m_pConfirmButton->setZValue(this->zValue() + 3);
    m_pConfirmButton->setProperty(BTN_TYPE, BtnType_Confirm);

    setBtnVisible(false);
}

QRectF TLScreenshot::boundingRect() const
{
    return QRectF(0, 0, m_size.width(), m_size.height());
}

void TLScreenshot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug() << "hongcs" << __FUNCTION__ << "##################################"<< event->scenePos();
    if (m_bTouchMode)
    {
        return;
    }

    m_bShoting = true;

    dotPress(0, event->scenePos());
    update();
}

void TLScreenshot::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug() << "hongcs" << __FUNCTION__ << "---------------------------------------------------------"<< event->scenePos();
    m_bShoting = true;

    dotMove(0, event->scenePos());
    update();
}

void TLScreenshot::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug() << "hongcs" << __FUNCTION__ << "***********************************************************" << event->scenePos();
    m_bShoting = true;
    dotRelease(0, event->scenePos());
    update();
}

void TLScreenshot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    pen.setWidth(1);
    pen.setStyle(Qt::DashDotLine);
    painter->setPen(pen);

    QRectF targetRect;
    if (m_bGestureMode) {
        targetRect = m_selectRect;
    } else {
        targetRect = m_realtimeRect;
    }

    QPainter::CompositionMode mode = painter->compositionMode();
    painter->setCompositionMode(QPainter::CompositionMode_DestinationAtop);
    this->drawBorderRect(painter, targetRect, m_shadowColor);
    painter->setCompositionMode(mode);
    painter->fillRect(targetRect, m_selectColor);
    painter->drawRect(targetRect);
}

bool TLScreenshot::sceneEvent(QEvent *event)
{
    switch(event->type()){
    case QEvent::TouchEnd:
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
        m_bTouchMode = true;
        if(touchEvent(static_cast<QTouchEvent*>(event))){
            return true;
        }
    default:
        return QGraphicsItem::sceneEvent(event);
    }
}

#if 1
bool TLScreenshot::touchEvent(QTouchEvent *event)
{
    bool ret = false;
    QList<QTouchEvent::TouchPoint> touchPoints = event->touchPoints();
    foreach (const QTouchEvent::TouchPoint& touchPoint, touchPoints) {
        int id = touchPoint.id();
        ret = (id > 0) ? true : false;
        switch (touchPoint.state()) {
        case Qt::TouchPointPressed:
            dotPress(id, touchPoint.scenePos());
            break;
        case Qt::TouchPointMoved:
            dotMove(id, touchPoint.scenePos());
            break;
        case Qt::TouchPointReleased://触发的设定很奇怪啊,和Pressed根本不配套.
            dotRelease(id, touchPoint.scenePos());
            break;
        default:
            break;
        }
    }
    return ret;
}
#endif

TLGraphicsPushButton *TLScreenshot::createGraphicsPushButton(const QString &normal, const QString &press)
{
    TLGraphicsPushButton *btn = new TLGraphicsPushButton(this);

    btn->setFixedSize(QSizeF(46 * m_rScaleFactor, 46 * m_rScaleFactor));
    btn->setNormalImage(normal);
    btn->setHoverImage(normal);
    btn->setPressImage(press);

    connect(btn, &TLGraphicsPushButton::sigClicked, this, &TLScreenshot::onBtnClick);

    return btn;
}

void TLScreenshot::setBtnVisible(bool bVisible)
{
    m_pSaveButton->setVisible(bVisible);
    m_pExitButton->setVisible(bVisible);
    m_pConfirmButton->setVisible(bVisible);
}

//画边框，选中以外的区域
void TLScreenshot::drawBorderRect(QPainter *painter, QRectF rect, QColor color)
{
    painter->fillRect(0, 0, rect.x(), m_size.height(), color);
    painter->fillRect(0, 0, m_size.width(), rect.y(), color);
    painter->fillRect(rect.x() + rect.width(), 0, m_size.width() - (rect.x() + rect.width()), m_size.height(), color);
    painter->fillRect(0, rect.y() + rect.height(), m_size.width(), m_size.height() - (rect.y() + rect.height()), color);
}

void TLScreenshot::dotPress(int id, QPointF pos)
{
    if (id > 1) //第三条线就不考虑了，id为0是第一指或者鼠标点，id为1代表第二指
    {
        return;
    } else if (id == 0) {
        m_previousPos_mouse = m_endPos_mouse = pos;
    }

    int touchBtn = getTouchBtn(pos);
    if (touchBtn > 0)
    {
        emit sigBtnPress((BtnType) touchBtn);
        return;
    }

//    adjustBtnPos();

//    m_bTouchToMousePressMode = (getTouchBtn(pos) > 0) ? true : false; //这个感觉没什么用，先屏蔽换
    m_touchMap.insert(id, pos);
    setBtnVisible(false);
    bool bContains = m_selectRect.contains(pos);
    m_bGestureMode =  bContains ? true : (id == 0 ? false : m_bGestureMode);
    if (bContains) {    //缩放平移操作
        m_pressPos = QPointF(pos - m_selectRect.topLeft());
    } else if (id == 0) {   //新建一个矩形的操作,一指以上的情况就不考虑新建矩形了
        m_pressPos = pos;
        m_realtimeRect = getRectF(m_pressPos, pos);
    }
}

void TLScreenshot::dotMove(int id, QPointF pos)
{
//    qDebug() << "hongcs" << __FUNCTION__ << "---------------------------------------------------------";
    if (id > 1) {
        return;
    }

    if (getTouchBtn(pos) > 0) {
        return;
    }
    initMoveOrRelease(id, pos);
    if (m_bGestureMode) {
        if (m_touchMap.count() == 1 && !m_bTouchMode) {
            m_selectRect.moveTo(pos - m_pressPos);
        } else {

        }

        adjustRectMoveAndScale(m_selectRect);
    } else if (id == 0) {
        m_realtimeRect = getRectF(m_pressPos, pos);
    }

    update();
}

void TLScreenshot::dotRelease(int id, QPointF pos)
{
    if (id > 1) {
        return;
    }

    if (getTouchBtn(pos) > 0) {
        return;
    }

    initMoveOrRelease(id, pos);
    m_bTouchMode = false;
    m_touchMap.remove(id);
    if (m_bGestureMode && m_touchMap.count() == 0) {
//        setBtnVisible(true);
//        adjustBtnPos();

    } else if (id == 0) {
        m_realtimeRect = getRectF(m_pressPos, pos);
        m_selectRect = m_realtimeRect;
        if (m_bTouchToMousePressMode) {
            return;
        }
    }
    adjustBtnPos();
    setBtnVisible(true);
}

int TLScreenshot::getTouchBtn(QPointF pos)
{
    QRectF rect = QRectF(m_pSaveButton->x(), m_pSaveButton->y(),
                       m_pSaveButton->width(), m_pSaveButton->height());
    if (rect.contains(pos))
    {
        return BtnType_Save;
    }

    rect = QRectF(m_pExitButton->x(), m_pExitButton->y(),
                 m_pExitButton->width(), m_pExitButton->height());
    if (rect.contains(pos))
    {
        return BtnType_Exit;
    }

    rect = QRectF(m_pConfirmButton->x(), m_pConfirmButton->y(),
                 m_pConfirmButton->width(), m_pConfirmButton->height());
    if (rect.contains(pos))
    {
        return BtnType_Confirm;
    }

    return -1;
}

void TLScreenshot::adjustBtnPos()
{
    int xPos = -1, yPos = -1;
    if (m_selectRect.width() < m_pSaveButton->width() * 3) {
        xPos = m_selectRect.x();
        yPos = (m_selectRect.y() >= (m_size.height() - m_selectRect.height() - m_pSaveButton->height())) ?
                (m_selectRect.y() - m_pSaveButton->height()) : (m_selectRect.y() + m_selectRect.height());
    } else if (m_selectRect.height() < m_pSaveButton->height()) {
        yPos = (m_selectRect.y() > (m_size.height() - m_pSaveButton->height())) ?
               (m_selectRect.y() - m_pSaveButton->height()) : (m_selectRect.y() + m_selectRect.height());
    }

    xPos = (xPos < 0) ? (m_selectRect.x() + m_selectRect.width() - m_pSaveButton->width() * 3) : xPos;
    yPos = (yPos < 0) ? (m_selectRect.y() + m_selectRect.height() - m_pSaveButton->height()) : yPos;

    m_pSaveButton->setPos(xPos, yPos);
    m_pConfirmButton->setPos(xPos + m_pSaveButton->width(), yPos);
    m_pExitButton->setPos(m_pConfirmButton->x() + m_pConfirmButton->width(), yPos);
}

void TLScreenshot::initMoveOrRelease(int id, QPointF pos)
{
    if (id > 1) {
        return;
    } else if (id == 0) {
        m_previousPos_mouse = m_endPos_mouse;
        m_endPos_mouse = pos;
    }
}

//由两点确定一个矩形
QRectF TLScreenshot::getRectF(QPointF pointO, QPointF pointT)
{
    int width = qAbs(pointO.x() - pointT.x());
    int height = qAbs(pointO.y() - pointT.y());
    QRectF rectF = QRectF((pointO.x() < pointT.x() ? pointO.x() : pointT.x()),
                          (pointO.y() < pointT.y() ? pointO.y() : pointT.y()),
                           width > MIN_WIDTH ? width: MIN_WIDTH,
                           height > MIN_HEIGHT ? height : MIN_HEIGHT);
    return rectF;
}

void TLScreenshot::adjustRectMoveAndScale(QRectF &rect)
{
    qreal rectWidth = rect.width();
    qreal rectHeight = rect.height();
    qreal screenWidth = m_size.width();
    qreal screenHeight = m_size.height();

    if ((rectWidth > screenWidth || rectHeight > screenHeight))
    {
        qreal scaleFactor = ((screenWidth / rectWidth) < (screenHeight / rectHeight)) ? (screenWidth / rectWidth)
                                                                                    : (screenHeight / rectHeight);

        rect.setWidth(rectWidth * scaleFactor);
        rect.setHeight(rectHeight * scaleFactor);
    }

    if (rect.x() < 0) {
        rect.moveLeft(0);
    } else if ((rect.x() + rect.width()) > screenWidth) {
        rect.moveLeft(rect.x() - (rect.x() + rect.width() - screenWidth));
    }

    if (rect.y() < 0) {
        rect.moveTop(0);
    } else if ((rect.y() + rect.height()) > screenHeight) {
        rect.moveTop(rect.y() - (rect.y() + rect.height() - screenHeight));
    }

}

QString TLScreenshot::screenShot(QRectF rect, bool bSave,QString dirPath)
{
    if (rect.size().isEmpty())
    {
        return QString("");
    } else{
        //这个是为了去掉截图的框
        rect.setX(rect.x() + 1);
        rect.setY(rect.y() + 1);
    }
    QString savePath, tempSavePath,pixName,tempPixName;

    if (dirPath.isEmpty()) { //直接点击确认按钮时 dirPath为空

        pixName = QString("image-") + QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        tempPixName = pixName + QString("_temp.png");
        pixName += QString(".png");
        savePath = QDir::tempPath() + QDir::separator() + pixName;
        tempSavePath = QDir::tempPath() + QDir::separator() + tempPixName;

    } else { //保存时
        QFileInfo fileInfo(dirPath);
        savePath = dirPath;
        tempSavePath = dirPath.remove(".png") + QString("_temp.png");
        pixName = fileInfo.fileName();
    }
      QThread::msleep(200); //延时保存，避免路径选择弹窗水印残留
#ifdef Q_OS_WIN
    int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    // 获取窗口的设备上下文（Device Contexts）
    HDC hdcWindow = GetDC(GetDesktopWindow());  // 要截图的窗口句柄，为空则全屏
    // 获取设备相关信息的尺寸大小
    int nBitPerPixel = GetDeviceCaps(hdcWindow, BITSPIXEL);
    CImage image;
    image.Create(width, height, nBitPerPixel);
    BitBlt(image.GetDC(), // 保存到的目标 图片对象 上下文
           0, 0,     // 起始 x, y 坐标
           width, height,  // 截图宽高
           hdcWindow,      // 截取对象的 上下文句柄
           m_pParentView->x(), m_pParentView->y(),           // 指定源矩形区域左上角的 X, y 逻辑坐标
           SRCCOPY
           );
    // 释放 DC句柄
    ReleaseDC(NULL, hdcWindow);
    // 释放图片上下文
    image.ReleaseDC();
    WCHAR wfile[256];
    memset(wfile,0, sizeof(wfile));
    tempSavePath.toWCharArray(wfile);
    HRESULT result = image.Save(wfile, Gdiplus::ImageFormatPNG);
    QImage shot = cutImage(rect, QImage(tempSavePath));
    if (bSave)
    {  
        bool bResult = shot.save(savePath, "png");
        emit sigSaveImageFinished();
    } else {
        emit sigScreenshotFinished(shot, pixName);
    }

    QFile::remove(tempSavePath);
    Q_ASSERT(result == S_OK);
#endif

    return savePath;
}

QImage TLScreenshot::cutImage(const QRectF &rect, const QImage &image)
{
    QImage sceneShot(rect.size().toSize(), QImage::Format_RGB32);
    QPainter painter;
    painter.begin(&sceneShot);
    painter.drawImage(-rect.x(), -rect.y(), image);
    painter.end();
    return sceneShot;
}

//重新截图
void TLScreenshot::rescreenShot()
{
    m_selectRect = m_realtimeRect = QRectF();
    setBtnVisible(false);
    update();
}

void TLScreenshot::exitScreenshot()
{
    rescreenShot();
    this->hide();
}

void TLScreenshot::execScreenShot(bool bSave, const QString &savePath)
{
    setBtnVisible(false);

    QTimer::singleShot(10, this, [=]{
        this->screenShot(m_selectRect, bSave, savePath);
        exitScreenshot();
    });
}

void TLScreenshot::onBtnClick()
{
    TLGraphicsPushButton* pBtn = qobject_cast<TLGraphicsPushButton *>(sender());
    if (pBtn != nullptr)
    {
        BtnType btnType = (BtnType) pBtn->property(BTN_TYPE).toInt();
        switch (btnType) {
            case BtnType_Save:
            {
                qDebug() << "hongcs" << __FUNCTION__ << "save";
                emit sigSaveRequest();
                break;
            }
            case BtnType_Exit:
            {
                qDebug() << "hongcs" << __FUNCTION__ << "exit";
                exitScreenshot();
                emit sigExitScreenshot();
                break;
            }
            case BtnType_Confirm:
            {
                qDebug() << "hongcs" << __FUNCTION__ << "confirm";
                execScreenShot(false, QString(""));
                break;
            }
        }
    }
}

TLScreenshotPixmap::TLScreenshotPixmap(const QPixmap &pixmap, QGraphicsObject *parent)
    : QGraphicsObject(parent), m_pixmap(pixmap)
{

}

QRectF TLScreenshotPixmap::boundingRect() const
{
    return QRectF(-0.5, -0.5, m_pixmap.width() + 0.5, m_pixmap.height() + 0.5);
}

void TLScreenshotPixmap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0, 0, m_pixmap);
}

void TLScreenshotPixmap::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        m_bRightButtonPress = true;
    } else if (event->button() == Qt::LeftButton) {
        m_dragMode = Press_Mode;
    }

    update();
}

void TLScreenshotPixmap::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dragMode == Press_Mode || m_dragMode == NoDrag_Mode) {
        if (qAbs(event->buttonDownScenePos(Qt::LeftButton).y() - event->scenePos().y()) > 7
            || qAbs(event->buttonDownScenePos(Qt::LeftButton).x() - event->scenePos().x()) > 7) {
            m_dragMode = Drag_Mode;
        }
    }  else if (m_dragMode == Drag_Mode) {
        if (m_bMoveable) {
            this->setPos(event->scenePos() - event->buttonDownPos(Qt::LeftButton));
        }
    }
}

void TLScreenshotPixmap::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if ((m_dragMode == Drag_Mode) && m_bMoveable) {
        this->setPos(event->scenePos() - event->buttonDownPos(Qt::LeftButton));
    }
    m_dragMode = NoDrag_Mode;
    update();
}
