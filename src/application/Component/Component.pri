##
#通用模块，用于管理所有通用的UI控件，如按钮
##

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

INCLUDEPATH += button/widget
INCLUDEPATH += base
INCLUDEPATH += base/widget
INCLUDEPATH += $$PWD/waitting/
INCLUDEPATH += maskwidget keyboard lineedit messagebox maskwidget

QT += multimedia

LIBS += User32.lib

SOURCES += \
    $$PWD/base/tlbuttoninterface.cpp \
    $$PWD/base/tlcontrolinterface.cpp \
    $$PWD/button/widget/tllabelbutton.cpp \
    $$PWD/button/widget/tlloadingbtn.cpp \
    $$PWD/button/widget/tltogglebutton.cpp \
    $$PWD/lineedit/tllineedit.cpp \
    $$PWD/keyboard/tlvirtualkeyboardbtn.cpp \
    $$PWD/keyboard/tlvirtualkeyboardtray.cpp \
    $$PWD/keyboard/tlvirtualkeyboard.cpp \
#    $$PWD/messagebox/tlmessagebox.cpp \
    $$PWD/maskwidget/tlmaskwidget.cpp \
    $$PWD/language/tllanguage.cpp \
    $$PWD/notice/tlnotice.cpp \
    $$PWD/screenshot/tlscreenshot.cpp \
    $$PWD/base/widget/tlbasecontrol.cpp \
    $$PWD/button/widget/tlcheckbtn.cpp \
    $$PWD/button/widget/tlpushbutton.cpp \
    $$PWD/button/widget/tlswitchbtn.cpp \
    $$PWD/button/widget/tlthressstatebutton.cpp \
    $$PWD/base/graphics/tlgraphicsbasecontrol.cpp \
    $$PWD/base/graphics/tlgraphicsbuttoninterface.cpp \
    $$PWD/button/graphics/tlgraphicspushbutton.cpp \
    $$PWD/waitting/tlloading.cpp \
#    $$PWD/waitting/tlwaitwidget.cpp

HEADERS += \
    $$PWD/base/tlbuttoninterface.h \
    $$PWD/base/tlcontrolinterface.h \
    $$PWD/button/widget/tllabelbutton.h \
    $$PWD/button/widget/tlloadingbtn.h \
    $$PWD/button/widget/tltogglebutton.h \
    $$PWD/lineedit/tllineedit.h \
    $$PWD/keyboard/tlvirtualkeyboardbtn.h \
    $$PWD/keyboard/tlvirtualkeyboardtray.h \
    $$PWD/keyboard/tlvirtualkeyboard.h \
#    $$PWD/messagebox/tlmessagebox.h \
    $$PWD/maskwidget/tlmaskwidget.h \
    $$PWD/language/tllanguage.h \
    $$PWD/notice/tlnotice.h \
    $$PWD/screenshot/tlscreenshot.h \
    $$PWD/base/widget/tlbasecontrol.h \
    $$PWD/button/widget/tlcheckbtn.h \
    $$PWD/button/widget/tlpushbutton.h \
    $$PWD/button/widget/tlswitchbtn.h \
    $$PWD/button/widget/tlthressstatebutton.h \
    $$PWD/base/graphics/tlgraphicsbasecontrol.h \
    $$PWD/base/graphics/tlgraphicsbuttoninterface.h \
    $$PWD/button/graphics/tlgraphicspushbutton.h \
    $$PWD/waitting/tlloading.h \
#    $$PWD/waitting/tlwaitwidget.h


RESOURCES += \
    $$PWD/sound.qrc \
    $$PWD/image.qrc

INCLUDEPATH += $$PWD/button/widget $$PWD/lineedit $$PWD/base $$PWD/base/widget $$PWD/maskwidget/
