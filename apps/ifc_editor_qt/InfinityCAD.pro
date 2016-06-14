#-------------------------------------------------
#
# Project created by QtCreator 2016-02-26T09:43:53
#
#-------------------------------------------------

PROJECT_ROOT=$$PWD/../../
DEPENDENCIES=${PROJECT_ROOT}/depedencies

mytarget.target = .buildfile
mytarget.commands = make -C $$PWD/../../ install
QMAKE_EXTRA_TARGETS += mytarget
PRE_TARGETDEPS += .buildfile

QT       += core gui opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11 silent
QMAKE_CXXFLAGS += -std=c++11 -g

TARGET = InfinityCAD
TEMPLATE = app

SRC_DIRS=$$system(find ./src -type d 2>/dev/null)
INC_DIRS=$$system(find ./include -type d 2>/dev/null)

for(dir, SRC_DIRS): exists($$dir/*.cpp) {
    #SOURCES += $$dir/*.cpp
}

for(dir, INC_DIRS): exists($$dir/*.h) {
    #HEADERS += $$dir/*.h
}
INCLUDEPATH += ./include
INCLUDEPATH += ./src


SOURCES += src/glwidget.cpp \
        src/main.cpp\
    src/controls.cpp \
    src/context_menus/context_menu.cpp \
    src/context_menus/handler.cpp \
    src/editor_window.cpp \
    src/system/object_manager.cpp \
    src/widgets/scene_list/scene_tree.cpp \
    src/widgets/scene_list/context_menus/scene_cmenu_factory.cpp \
    src/widgets/scene_list/context_menus/scene_cmenu_default.cpp \
    src/widgets/scene_list/context_menus/scene_cmenu_point.cpp \
    src/widgets/scene_list/entities/item_id.cpp \
    src/widgets/scene_list/entities/item.cpp \
    src/widgets/scene_list/entities/item_id_factory.cpp \
    src/widgets/scene_list/context_menus/scene_cmenu_points.cpp \
    src/widgets/scene_list/context_menus/scene_cmenu_point_bezier.cpp \
    src/widgets/scene_list/context_menus/scene_cmenu_points_bezier.cpp \
    src/context_menus/scene_context_menu.cpp \
    src/context_menus/scene_cm_handler.cpp \
    src/widgets/scene_list/context_menus/scene_cmenu_bezier.cpp \
    src/system/system_binding/bspline_binding.cpp \
    src/widgets/scene_list/context_menus/scene_cmenu_handlers.cpp \
    src/system/ifc_types.cpp \
    src/widgets/scene_list/context_menus/scm_bspline.cpp \
    src/system/system_binding/bspline_interp_binding.cpp \
    src/objects_dialog.cpp \
    src/dialogs/surface_settings_dialog.cpp \
    src/dialogs/surface_c0_dialog.cpp \
    src/widgets/scene_list/context_menus/scm_surfacec0.cpp \
    src/system/serialization/serialization_scene.cpp \
    src/system/serialization/deserialization_scene.cpp \
    src/widgets/scene_list/context_menus/scm_surfaces.cpp \
    src/plot/qcustomplot.cpp \
    src/dialogs/intersectiondialog.cpp \
    src/plot/plot_style.cpp

HEADERS  += build/ui/ui_mainwindow.h \
    src/context_menus/context_menu.h \
    src/context_menus/handler.h \
    src/context_menus/scene_cm_handler.h \
    src/context_menus/scene_context_menu.h \
    src/system/system_binding/bspline_binding.h \
    src/system/object_manager.h \
    src/system/ifc_types.h \
    src/widgets/scene_list/context_menus/scene_cmenu_bezier.h \
    src/widgets/scene_list/context_menus/scene_cmenu_default.h \
    src/widgets/scene_list/context_menus/scene_cmenu_factory.h \
    src/widgets/scene_list/context_menus/scene_cmenu_point.h \
    src/widgets/scene_list/context_menus/scene_cmenu_point_bezier.h \
    src/widgets/scene_list/context_menus/scene_cmenu_points.h \
    src/widgets/scene_list/context_menus/scene_cmenu_points_bezier.h \
    src/widgets/scene_list/entities/item.h \
    src/widgets/scene_list/entities/item_id.h \
    src/widgets/scene_list/entities/item_id_factory.h \
    src/widgets/scene_list/scene_tree.h \
    src/controls.h \
    src/editor_window.h \
    src/glwidget.h \
    src/widgets/scene_list/context_menus/scene_cmenu_handlers.h \
    build/ui/ui_mainwindow.h \
    src/widgets/scene_list/context_menus/scm_bspline.h \
    src/system/system_binding/bspline_interp_binding.h \
    src/objects_dialog.h \
    src/dialogs/surface_settings_dialog.h \
    src/dialogs/surface_c0_dialog.h \
    src/widgets/scene_list/context_menus/scm_surfacec0.h \
    src/system/serialization/serialization_scene.h \
    src/system/serialization/deserialization_scene.h \
    src/widgets/scene_list/context_menus/scm_surfaces.h \
    src/plot/qcustomplot.h \
    src/dialogs/intersectiondialog.h \
    src/plot/plot_style.h

##############################

#system(mkdir -p build)

DESTDIR = build
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui


##############################

FORMS    += mainwindow.ui \
    objects_dialog.ui \
    src/dialogs/surface_settings_dialog.ui \
    src/dialogs/surface_c0_dialog.ui \
    src/dialogs/intersectiondialog.ui

RESOURCES += resources/icons.qrc \
    qdarkstyle/style.qrc
#############################################################################

# InfinityCAD
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../dependencies/lib/ifc/release/ -lifc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../dependencies/lib/ifc/debug/ -lifc
else:unix: LIBS += -L$$PWD/../../dependencies/lib/ifc/ -lifc
INCLUDEPATH += $$PWD/../../dependencies/include/ifc
DEPENDPATH += $$PWD/../../dependencies/include/ifc
LIBS += -Wl,-rpath $$PWD/../../dependencies/lib/ifc/

# ifcgpu
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../dependencies/lib/ifcgpu/release/ -lifcgpu
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../dependencies/lib/ifcgpu/debug/ -lifcgpu
else:unix: LIBS += -L$$PWD/../../dependencies/lib/ifcgpu/ -lifcgpu
INCLUDEPATH += $$PWD/../../dependencies/include/ifcgpu
DEPENDPATH += $$PWD/../../dependencies/include/ifcgpu
LIBS += -Wl,-rpath $$PWD/../../dependencies/lib/ifcgpu/

# GLM
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../dependencies/lib/glm/release/ -lglm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../dependencies/lib/glm/debug/ -lglm
else:unix: LIBS += -L$$PWD/../../dependencies/lib/glm/ -lglm
INCLUDEPATH += $$PWD/../../dependencies/include/glm
DEPENDPATH += $$PWD/../../dependencies/include/glm

# Ray
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../dependencies/lib/rc/release/ -lrc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../dependencies/lib/rc/debug/ -lrc
else:unix: LIBS += -L$$PWD/../../dependencies/lib/rc/ -lrc
INCLUDEPATH += $$PWD/../../dependencies/include/rc
DEPENDPATH += $$PWD/../../dependencies/include/rc

# Math
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../dependencies/lib/math/release/ -lmath
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../dependencies/lib/math/debug/ -lmath
else:unix: LIBS += -L$$PWD/../../dependencies/lib/math/ -lmath
INCLUDEPATH += $$PWD/../../dependencies/include/math
DEPENDPATH += $$PWD/../../dependencies/include/math

# Strings
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../dependencies/lib/strings/release/ -lstrings
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../dependencies/lib/strings/debug/ -lstrings
else:unix: LIBS += -L$$PWD/../../dependencies/lib/strings/ -lstrings
INCLUDEPATH += $$PWD/../../dependencies/include/strings
DEPENDPATH += $$PWD/../../dependencies/include/strings
LIBS += -Wl,-rpath $$PWD/../../dependencies/lib/strings/

# Parallel
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../dependencies/lib/parallel/release/ -lparallel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../dependencies/lib/parallel/debug/ -lparallel
else:unix: LIBS += -L$$PWD/../../dependencies/lib/parallel/ -lparallel
INCLUDEPATH += $$PWD/../../dependencies/include/parallel
DEPENDPATH += $$PWD/../../dependencies/include/parallel

# PSO
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../dependencies/lib/pso/release/ -lpso
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../dependencies/lib/pso/debug/ -lpso
else:unix: LIBS += -L$$PWD/../../dependencies/lib/pso/ -lpso
INCLUDEPATH += $$PWD/../../dependencies/include/pso
DEPENDPATH += $$PWD/../../dependencies/include/pso

# Time
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../dependencies/lib/time/release/ -ltime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../dependencies/lib/time/debug/ -ltime
else:unix: LIBS += -L$$PWD/../../dependencies/lib/time/ -ltime
INCLUDEPATH += $$PWD/../../dependencies/include/time
DEPENDPATH += $$PWD/../../dependencies/include/time

# Uncomment
#LIBS += -L /opt/cuda/lib64
#LIBS += -Wl,-rpath /opt/cuda/lib64
LIBS += -L /usr/local/cuda-7.0/lib64
LIBS += -lcudart -lcuda -lcudadevrt

# EXTERNALS
