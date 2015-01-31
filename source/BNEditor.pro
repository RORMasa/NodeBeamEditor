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
    NodeBeam_LUA.cpp \
    blueprints.cpp \
    NodeBeam_JBEAM.cpp

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
    settings.h \
    blueprints.h \


FORMS    += mainwindow.ui \
    newprojectdialog.ui \
    Generators/ChassisGenerator.ui \
    inputdialog.ui \
    Generators/tracksgenerator.ui \
    beamdefaultsdialog.ui \
    settings.ui \
    blueprints.ui

RESOURCES += \
    icons.qrc

RC_FILE = BNE.rc

LIBS += \

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lua/ -llua52
    win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lua/ -llua52

    INCLUDEPATH += $$PWD/lua
    DEPENDPATH += $$PWD/lua
