#ifndef TLUTILITYDEF_H
#define TLUTILITYDEF_H
#include <QString>

enum PathType{
    Config = 0,//%appdata%/配置的基础路径
    Profiles,//%appdata%/profiles 存放通用配置
    Cache,//%appdata%/cache存放解析文件缓存、下载文件缓存等
    Temp,//%temp% +/cbtemp临时文件存放
    HomePath,//
    Apppath,//app所在目录
    Desktop,//桌面
    Documents,//文档目录
};


#endif // TLUTILITYDEF_H
