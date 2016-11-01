#-------------------------------------------------
#
# Project created by QtCreator 2016-10-27T10:02:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt-corrector
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    datasetobject.cpp \
    correctorgraphicsscene.cpp

HEADERS  += mainwindow.h \
    datasetobject.h \
    correctorgraphicsscene.h

FORMS    += mainwindow.ui
CONFIG += c++11
