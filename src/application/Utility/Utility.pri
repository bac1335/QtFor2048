##
#通用模块，用于管理所有工具类
#
##

QT += network multimedia

INCLUDEPATH += $$PWD $$PWD/header
DEPENDPATH += $$PWD

SOURCES += \
    $$PWD/hardwareinfo.cpp \
    $$PWD/tlutility.cpp \
    $$PWD/source/tlpaths.cpp \
    $$PWD/source/tlsettings.cpp \
    $$PWD/source/tllinguist.cpp

HEADERS += \
    $$PWD/hardwareinfo.h \
    $$PWD/tlutility.h \
    $$PWD/header/tlpaths.h \
    $$PWD/header/tlsettings.h \
    $$PWD/header/tlutilitydef.h \
    $$PWD/header/tllinguist.h
