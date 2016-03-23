#-------------------------------------------------
#
# Project created by QtCreator 2016-02-26T09:43:53
#
#-------------------------------------------------

PROJECT_ROOT=$$PWD/../../..
DEPENDENCIES=${PROJECT_ROOT}/depedencies

mytarget.target = .buildfile
mytarget.commands = make -C $$PWD/../../../ install
QMAKE_EXTRA_TARGETS += mytarget
PRE_TARGETDEPS += .buildfile

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

TARGET = editor
TEMPLATE = app

SOURCES += glwidget.cpp \
        main.cpp\
    controls.cpp \
    context_menus/context_menu_settings.cpp \
    context_menus/context_menu.cpp \
    context_menus/handler.cpp \
    widgets/objects_list/objects_settings.cpp \
    widgets/objects_list/objects_tree_factory.cpp \
    editor_window.cpp \
    system/object_manager.cpp \
    context_menus/factories/objects_cmenu_factory.cpp \
    context_menus/factories/scene_cmenu_factory.cpp \
    widgets/scene_list/scene_tree.cpp \
    widgets/objects_list/objects_tree.cpp

HEADERS  += glwidget.h \
    controls.h \
    context_menus/context_menu_settings.h \
    context_menus/context_menu.h \
    context_menus/handler.h \
    widgets/objects_list/objects_settings.h \
    widgets/objects_list/objects_tree_factory.h \
    editor_window.h \
    system/object_manager.h \
    context_menus/factories/objects _cmenu_factory.h \
    context_menus/factories/scene_cmenu_factory.h \
    context_menus/factories/objects_cmenu_factory.h \
    widgets/scene_list/scene_tree.h \
    widgets/objects_list/objects_tree.h

FORMS    += mainwindow.ui

# Geometry Lib
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../dependencies/lib/gm/release/ -lgm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../dependencies/lib/gm/debug/ -lgm
else:unix: LIBS += -L$$PWD/../../../dependencies/lib/gm/ -lgm
INCLUDEPATH += $$PWD/../../../dependencies/include/gm
DEPENDPATH += $$PWD/../../../dependencies/include/gm

# GLM
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../dependencies/lib/glm/release/ -lglm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../dependencies/lib/glm/debug/ -lglm
else:unix: LIBS += -L$$PWD/../../../dependencies/lib/glm/ -lglm
INCLUDEPATH += $$PWD/../../../dependencies/include/glm
DEPENDPATH += $$PWD/../../../dependencies/include/glm

# Ray
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../dependencies/lib/rc/release/ -lrc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../dependencies/lib/rc/debug/ -lrc
else:unix: LIBS += -L$$PWD/../../../dependencies/lib/rc/ -lrc
INCLUDEPATH += $$PWD/../../../dependencies/include/rc
DEPENDPATH += $$PWD/../../../dependencies/include/rc

# EXTERNALS
