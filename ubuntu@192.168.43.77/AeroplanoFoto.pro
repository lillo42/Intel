#-------------------------------------------------
#
# Project created by QtCreator 2014-10-08T18:48:17
#
#-------------------------------------------------

QT       += core network gui

TARGET = AeroplanoFoto
CONFIG   += console
CONFIG   -= app_bundle

LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc
TEMPLATE = app


SOURCES += main.cpp \
    tcpclient.cpp \
    camera.cpp \
    gps.cpp

HEADERS += \
    tcpclient.h \
    camera.h \
    gps.h
