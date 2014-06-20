#-------------------------------------------------
#
# Project created by QtCreator 2014-06-09T18:46:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gxinMine
TEMPLATE = app

#        RC_ICONS = images/mine.ico

SOURCES += main.cpp\
        mainwindow.cpp \
    block.cpp \
    gamewidget.cpp \
    leveldialog.cpp

HEADERS  += mainwindow.h \
    block.h \
    gamewidget.h \
    leveldialog.h

RESOURCES += \
    data.qrc
