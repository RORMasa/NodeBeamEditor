#-------------------------------------------------
#
# Project created by QtCreator 2015-04-24T12:17:17
#
#-------------------------------------------------

QT       += testlib
QT       += opengl
QT       += core gui
QT       += widgets

TARGET = tst_nodebeamtests
CONFIG   += console
CONFIG   -= app_bundle
DEFINES += TESTING


TEMPLATE = app

INCLUDEPATH += ../../lua/


SOURCES += tst_nodebeamtests.cpp \
           ../../NodeBeam.cpp \
           ../../NodeBeam_JBEAM.cpp \
           ../../NodeBeam_LUA.cpp

HEADERS += ../../NodeBeam.h \

DEFINES += SRCDIR=\\\"$$PWD/\\\"

LIBS += \

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../lua/ -llua52
    win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../lua/ -llua52

    INCLUDEPATH += $$PWD/../../lua
    DEPENDPATH += $$PWD/../../lua
