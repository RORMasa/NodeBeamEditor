#-------------------------------------------------
#
# Project created by QtCreator 2013-08-11T20:03:52
#
#-------------------------------------------------

QT       += core gui webkitwidgets
QT       += opengl
#QT       += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BNEditor
TEMPLATE = app

DEFINES *= QT_USE_QSTRINGBUILDER

#CONFIG += RELEASE
#CONFIG += build_all
#CONFIG += console

INCLUDEPATH += . \
    Generators \

SOURCES += main.cpp\
        mainwindow.cpp \
        glwidget.cpp \
        glwidgetOrtho.cpp \
        newprojectdialog.cpp \
        inputdialog.cpp \
        beamdefaultsdialog.cpp \
        settings.cpp \
        NodeBeam.cpp \
        NodeBeam_LUA.cpp \
        NodeBeam_JBEAM.cpp \
        blueprints.cpp \
        colladadae.cpp \
        htmlinputdialog.cpp \
    jbeamtextbox.cpp \
    processbar.cpp


HEADERS  += mainwindow.h \
    glwidget.h \
    glwidgetOrtho.h \
    newprojectdialog.h \
    inputdialog.h \
    beamdefaultsdialog.h \
    settings.h \
    NodeBeam.h \
    blueprints.h \
    colladadae.h \
    htmlinputdialog.h \
    jbeamtextbox.h \
    processbar.h


FORMS    += mainwindow.ui \
    newprojectdialog.ui \
    inputdialog.ui \
    beamdefaultsdialog.ui \
    settings.ui \
    blueprints.ui \
    htmlinputdialog.ui \
    processbar.ui

RESOURCES += \
    icons.qrc

RC_FILE = BNE.rc

LIBS += \

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lua/ -llua52
    win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lua/ -llua52

    INCLUDEPATH += $$PWD/lua
    DEPENDPATH += $$PWD/lua
