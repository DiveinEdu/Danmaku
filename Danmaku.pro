#-------------------------------------------------
#
# Project created by QtCreator 2014-12-18T22:26:45
#
#-------------------------------------------------

QT       += core gui network opengl
linux {
    QT     += x11extras
    LIBS   +=  -lXext -lX11
}
win32 {
    RC_FILE = danmaku-icon.rc
}
macx {
    ICON = danmaku-icon.icns
    QMAKE_INFO_PLIST = Info.plist
    QMAKE_CFLAGS += -gdwarf-2
    QMAKE_CXXFLAGS += -gdwarf-2
    BUNDLEID = com.diveinedu.Danmaku

}
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Danmaku
TEMPLATE = app


SOURCES += main.cpp\
        watermarkwindow.cpp \
    bulletwidget.cpp \
    danmakuudpserver.cpp \
    danmakutray.cpp \
    subscribethread.cpp \
    settingdialog.cpp \
    danmakuengine.cpp

HEADERS  += watermarkwindow.h \
    bulletwidget.h \
    danmakuudpserver.h \
    danmakutray.h \
    singleton.h \
    defines.h \
    subscribethread.h \
    danmakuengine.h \
    settingdialog.h

RESOURCES += \
    resource.qrc

FORMS += \
    settingdialog.ui

OTHER_FILES += \
    Info.plist

OBJECTIVE_SOURCES +=

DISTFILES += \
    danmaku-icon.ico \
    danmaku-icon.rc
