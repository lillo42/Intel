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

#LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_ml -lopencv_contrib
LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_ts -lopencv_video -lopencv_videostab

SOURCES += main.cpp \
    treina.cpp

HEADERS += \
    treina.h
