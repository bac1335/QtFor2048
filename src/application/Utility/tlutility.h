#ifndef TLUTILITY_H
#define TLUTILITY_H

/*
 * @brief   :本类用于管理通用接口
 * @author  :Lynn
 * @date    :2018.11.01
 */

#include <QObject>
#include "header/tlutilitydef.h"
#include <QVariant>


class TLPaths;
class TLSettings;
class TLMaskWidget;

#define Uti TLUtility::GetInstance()


class TLUtility : public QObject
{
    Q_OBJECT
public:
    static TLUtility * GetInstance();
    static void Release();
    //返回路径
    QString path(PathType type);
    //返回配置文件路径
    QString path();
    //返回配置
    TLSettings *setting();

    //配置文件相关
    void initSettings(const QString & fileName); //使用settings前必须要先调用这个进行初始化，可以在总控制台进行调用
    void setValue(const QString &group, const QString &key, const QVariant &value);
    QVariant value(const QString &group, const QString &key, const QVariant &defaultValue = QVariant());
    bool containts(const QString &group, const QString &key);
    void remove(const QString &group, const QString &key);
    void sync();
    //获取桌面大小
    QSize getDesktopSize();
    QRect getDesktopRect();

    //获取缩放系数,参数为要标准的宽和高
    qreal getScaleFactor();

    //获取字体  默认是思源黑体Medium（SourceHanSansSC-Medium.otf）、18像素、非粗体
    QFont getFont(int pixSize = 18, bool bBold = false, const QString& fontFamily = u8"思源黑体 Medium");

    //计算字符串的宽度
    int calStringWidth(const QFont& font,const QString& str);

    //获取省略后的文本  默认是中间省略
    QString getElideText(const QString &srcText, QFont font,
                         int displayWidth, Qt::TextElideMode elideMode = Qt::ElideMiddle);

    //添加遮罩  默认透明度是80/255
    TLMaskWidget* addMask(QWidget *widget);

    //窗口显示，有mask就showmask 默认透明度是80/255
    TLMaskWidget* showMask(QWidget *widget);

    //复制文件夹
    bool copyDirectory(QString fromDir, QString toDir, bool bCoverIfFileExists);
    //清空文件夹
    void cleanDirectory(const QString dirName);
    //删除文件夹
    void removeDirectory(const QString dirName);

    //获取文件的md5码
    QString getFileMd5(const QString &filePath);
    //获取Windows CPU序列号
    QString getCpuID();
    //检测第三方软件是否运行  windows平台
    bool CheckAppRunningStatus(const QString & appName);
    //强制杀死第三方软件进程  windows平台
    void KillAppProcess(const QString & appName);
    //设置开机自启动
    void setAutoStart(bool isAutoStart);
    //获取本地IP地址
    QString getLocalIp();
    //获取本机子网掩码
    QString getLocalMask();
    //获取随机字符串
    QString getRandomString(int length);
    //检查程序的数字签名,如果没有数字签名，会自动退出程序
    void checkSelfWinDigitallySigned();

    QString getUuid();

    //生成圆角图片
    QPixmap createRadiusPixmap(QSize size, QPixmap pixmap);

    QString getDiskID();//获取硬盘id
    QString getMacAddr();//获取Mac地址


private:
    QString getCpuIdThroughCmd();   //直接通过命令行的方式来获取，可能在某些机器上获取不到
    QString getCpuIdThroughCode(); //通过win代码来获取
    void getcpuid(unsigned int CPUInfo[4], unsigned int InfoType);
    void getcpuidex(unsigned int CPUInfo[4], unsigned int InfoType, unsigned int ECXValue);
//    QString getIp();
    qreal scaleFactor();
    int getScreenIndex();

private:
    explicit TLUtility(QObject *parent = nullptr);
    static TLUtility * m_instance;
    TLSettings * m_pSettings = nullptr;
    QString m_ipAddr;
    QString m_ipMask; //子网掩码
    qreal m_rScaleFactor = 1.0;

};

#endif // TLUTILITY_H
