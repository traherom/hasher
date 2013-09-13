#-------------------------------------------------
#
# Project created by QtCreator 2013-09-08T22:45:39
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = hasher2
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lsqlite3

SOURCES += main.cpp \
    filelistwindow.cpp \
    hashdatabase.cpp

HEADERS += \
    filelistwindow.h \
    hashdatabase.h

FORMS += \
    filelistwindow.ui

OTHER_FILES += \
    database_schema.txt
