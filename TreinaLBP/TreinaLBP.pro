#-------------------------------------------------
#
# Project created by QtCreator 2014-10-26T11:48:36
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = TreinaLBP
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_ml -lopencv_contrib


SOURCES += main.cpp \
    treina.cpp

HEADERS += \
    treina.h
