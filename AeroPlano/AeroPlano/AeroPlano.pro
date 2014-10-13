#-------------------------------------------------
#
# Project created by QtCreator 2014-09-28T14:58:26
#
#-------------------------------------------------

QT       += core
QT       +=network
QT       -= gui

TARGET = AeroPlano
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_ts -lopencv_video -lopencv_videostab

SOURCES += main.cpp \
    TrataImage/trataimagecontroller.cpp \
    TrataImage/Controller/lbpcontroller.cpp \
    TrataImage/Controller/mrmrcontroller.cpp \
    TrataImage/Controller/waveletcontroller.cpp \
    TrataImage/Operacao/lbp.cpp \
    TrataImage/Operacao/mrmr.cpp \
    TrataImage/Operacao/wavelet.cpp \
    ImagemController/imagecontroller.cpp \
    ImagemController/imagem.cpp \
    TCP/tcpcontroller.cpp \
    TCP/tcpserver.cpp \
    TCP/client.cpp \
    IO/iocontroller.cpp \
    IO/read.cpp \
    IO/save.cpp \
    TrataImage/Controller/filtercontroller.cpp \
    TrataImage/Operacao/filter.cpp

HEADERS += \
    TrataImage/trataimagecontroller.h \
    TrataImage/Controller/lbpcontroller.h \
    TrataImage/Controller/mrmrcontroller.h \
    TrataImage/Controller/waveletcontroller.h \
    TrataImage/Operacao/lbp.h \
    TrataImage/Operacao/mrmr.h \
    TrataImage/Operacao/wavelet.h \
    ImagemController/imagecontroller.h \
    ImagemController/imagem.h \
    TCP/tcpcontroller.h \
    TCP/tcpserver.h \
    TCP/client.h \
    IO/iocontroller.h \
    IO/read.h \
    IO/save.h \
    TrataImage/Controller/filtercontroller.h \
    TrataImage/Operacao/filter.h
