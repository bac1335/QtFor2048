##################################################
#@brief         qt自带的单例程序
#@date          2020-07-21
##################################################

QT += network

INCLUDEPATH += $$PWD/src/

HEADERS += \
    $$PWD/src/QtLockedFile \
    $$PWD/src/QtSingleApplication \
    $$PWD/src/qtlocalpeer.h \
    $$PWD/src/qtlockedfile.h \
    $$PWD/src/qtsingleapplication.h \
    $$PWD/src/qtsinglecoreapplication.h

SOURCES += \
    $$PWD/src/qtlocalpeer.cpp \
    $$PWD/src/qtlockedfile.cpp \
    $$PWD/src/qtlockedfile_unix.cpp \
    $$PWD/src/qtlockedfile_win.cpp \
    $$PWD/src/qtsingleapplication.cpp \
    $$PWD/src/qtsinglecoreapplication.cpp
