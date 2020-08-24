#-------------------------------------------------
#
# Project created by QtCreator 2020-08-24T08:57:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include($$PWD/qtsingleapplication/qtsingleapplication.pri)
include($$PWD/Component/Component.pri)
include($$PWD/Utility/Utility.pri)

RC_FILE += $$PWD/logo.rc

DESTDIR += $$PWD/../bin
OBJECTS_DIR += $$PWD/../dir

TARGET = 2048
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += manager ui

CONFIG += c++11

SOURCES += \
        main.cpp \
        manager/q2048numbermanager.cpp \
        ui/q2048basewidget.cpp \
        ui/q2048boxitem.cpp \
        ui/q2048widget.cpp \
        ui/uimanager.cpp

HEADERS += \
        manager/q2048managebase.h \
        manager/q2048numbermanager.h \
        ui/q2048basewidget.h \
        ui/q2048boxitem.h \
        ui/q2048widget.h \
        ui/uimanager.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
