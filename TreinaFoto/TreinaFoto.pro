#-------------------------------------------------
#
# Project created by QtCreator 2014-10-21T23:02:38
#
#-------------------------------------------------

QT       += core

#QT       -= gui

TARGET = TreinaFoto
CONFIG   += console
CONFIG   -= app_bundle

LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_ml -lopencv_contrib

TEMPLATE = app


SOURCES += main.cpp \
    busca.cpp

HEADERS += \
    busca.h
