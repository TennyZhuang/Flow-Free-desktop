#-------------------------------------------------
#
# Project created by QtCreator 2015-08-22T02:36:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Flow-Free-desktop
TEMPLATE = app

CONFIG += C++11

SOURCES += src/main.cpp\
    src/flowfree.cpp \
    src/gamescene.cpp \
    src/level.cpp \
    src/path.cpp \
    src/gamepoint.cpp \
    src/gamemodel.cpp \
    src/focus.cpp

HEADERS  += \
    src/common.h \
    src/flowfree.h \
    src/gamescene.h \
    src/level.h \
    src/path.h \
    src/gamepoint.h \
    src/gamemodel.h \
    src/focus.h

FORMS    += src/flowfree.ui

RESOURCES += \
    res/resource.qrc

DISTFILES +=
