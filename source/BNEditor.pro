#-------------------------------------------------
#
# Project created by QtCreator 2013-08-11T20:03:52
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BNEditor
TEMPLATE = app

#CONFIG += RELEASE
#CONFIG += build_all

INCLUDEPATH += . \
    Generators \
    "F:\Program Files (x86)\Lua\5.1\include" \

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    NodeBeam.cpp \
    glwidgetOrtho.cpp \
    newprojectdialog.cpp \
    Generators/ChassisGenerator.cpp \
    inputdialog.cpp \
    Generators/tracksgenerator.cpp \
    jbeam.cpp \
    jbeam_object.cpp \
    beamdefaultsdialog.cpp \
    settings.cpp \
    NodeBeam_LUA.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    NodeBeam.h \
    glwidgetOrtho.h \
    newprojectdialog.h \
    Generators/ChassisGenerator.h \
    inputdialog.h \
    Generators/tracksgenerator.h \
    jbeam.h \
    jbeam_object.h \
    beamdefaultsdialog.h \
    settings.h


FORMS    += mainwindow.ui \
    newprojectdialog.ui \
    Generators/ChassisGenerator.ui \
    inputdialog.ui \
    Generators/tracksgenerator.ui \
    beamdefaultsdialog.ui \
    settings.ui

RESOURCES += \
    icons.qrc

LIBS += \
    -L"F:\Program Files (x86)\Lua\5.1\lib\lua5.1.lib" -llua51
