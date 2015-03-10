#-------------------------------------------------
#
# Project created by QtCreator 2013-06-20T23:05:04
#
#-------------------------------------------------

QT       += core
QT       -= gui

include(qtsingleapplication/qtsinglecoreapplication.pri)
include(qtvlc/qtvlc.pri)

TARGET = AirPlay
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += bindings/qt4
LIBS +=  -lshairplay

QMAKE_RPATHDIR += /home/aFei/app/shairplay/lib

SOURCES += main.cpp \
    bindings/qt4/raopservice.cpp \
    bindings/qt4/raopcallbackhandler.cpp \
    bindings/qt4/dnssdservice.cpp \
    AirPlay.cpp \
    VlcMemData.cpp \
    VlcDevice.cpp \
    MediaDevice.cpp \
    VlcCallBack.cpp

HEADERS += \
    bindings/qt4/raopservice.h \
    bindings/qt4/raopcallbacks.h \
    bindings/qt4/raopcallbackhandler.h \
    bindings/qt4/dnssdservice.h \
    AirPlay.h \
    VlcMemData.h \
    VlcDevice.h \
    MediaDevice.h \
    VlcCallBack.h

OTHER_FILES +=
