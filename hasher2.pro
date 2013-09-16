#-------------------------------------------------
#
# Project created by QtCreator 2013-09-08T22:45:39
#
#-------------------------------------------------

QT       += core gui widgets
TEMPLATE = app

TARGET = hasher2

CONFIG   += console
CONFIG   -= app_bundle

# You may have to add macx-clang-libc++ to the qt makespec (Qt < 5.1)
CONFIG   += c++11

# Required libraries
LIBS += -lsqlite3

# OSX brew-installed boost libraries
INCLUDEPATH += /usr/local/opt/boost/include/

# Files
SOURCES += main.cpp \
    filelistwindow.cpp \
    hashdatabase.cpp \
    scan.cpp \
    scanfile.cpp \
    sqlexception.cpp

HEADERS += \
    filelistwindow.h \
    hashdatabase.h \
    scan.h \
    scanfile.h \
    sqlexception.h

FORMS += \
    filelistwindow.ui

OTHER_FILES += \
    database_schema.txt

cache()
