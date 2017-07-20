#-------------------------------------------------
#
# Project created by QtCreator 2017-07-20T18:41:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    arduino-serial-lib.cpp

HEADERS  += mainwindow.h \
    arduino-serial-lib.h

FORMS    += mainwindow.ui
