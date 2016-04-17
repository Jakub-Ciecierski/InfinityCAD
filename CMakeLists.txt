cmake_minimum_required(VERSION 3.3)

set(PROJECT_NAME "ic")

set(APPS_PATH "apps")
set(LIBS_PATH "libs")

project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -MMD")

include_directories(/home/jakub/programming/ic/apps/ifc_editor_qt/src/)
include_directories(/home/jakub/ide/qt/5.5/gcc_64/include)
include_directories(/home/jakub/ide/qt/5.5/gcc_64/include/QtWidgets)
include_directories(/home/jakub/ide/qt/5.5/gcc_64/include/QtCore)
include_directories(/home/jakub/ide/qt/5.5/gcc_64/include/QtOpenGL/)
include_directories(/home/jakub/ide/qt/5.5/gcc_64/include/QtGui)
include_directories(/home/jakub/programming/ic/apps/ifc_editor_qt)

#############################################################################

MACRO(FIND_INCLUDE_DIR return_list regex)
    FILE(GLOB_RECURSE new_list ${regex})
    message("${new_list}")
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        message("")
        message("---------------------------------------------------")
        message("${file_path}")
        string(REGEX MATCH ^.*/include/ dir ${file_path})
        message("${dir}")
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

MACRO(FIND_SRC_DIR return_list regex)
    FILE(GLOB_RECURSE new_list ${regex})
    message("${new_list}")
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        message("")
        message("---------------------------------------------------")
        message("${file_path}")
        string(REGEX MATCH ^.*/src/ dir ${file_path})
        message("${dir}")
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

#############################################################################

FIND_INCLUDE_DIR(APPS_INC_DIRS ${APPS_PATH}/*/include/*.h)
FIND_SRC_DIR(APPS_SRC_DIRS ${APPS_PATH}/*/src/*.cpp)

FIND_INCLUDE_DIR(APPS_TEST_INC_DIRS ${APPS_PATH}/*/test/include/*.h)
FIND_SRC_DIR(APPS_TEST_SRC_DIRS ${APPS_PATH}/*/src/test/*.cpp)

FIND_INCLUDE_DIR(LIBS_INC_DIRS ${LIBS_PATH}/*/include/*.h)
FIND_INCLUDE_DIR(LIBS_INC_DIRS_HPP ${LIBS_PATH}/*/include/*.hpp)
FIND_SRC_DIR(LIBS_SRC_DIRS ${LIBS_PATH}/*/src/*.cpp)

FIND_INCLUDE_DIR(LIBS_TEST_INC_DIRS ${LIBS_PATH}/*/test/include/*.h)
FIND_SRC_DIR(LIBS_TEST_SRC_DIRS ${LIBS_PATH}/*/test/src/*.cpp)

include_directories(${APPS_INC_DIRS})
include_directories(${APPS_SRC_DIRS})
include_directories(${APPS_TEST_INC_DIRS})
include_directories(${APPS_TEST_SRC_DIRS})

include_directories(${LIBS_INC_DIRS})
include_directories(${LIBS_INC_DIRS_HPP})
include_directories(${LIBS_SRC_DIRS})
include_directories(${LIBS_TEST_INC_DIRS})
include_directories(${LIBS_TEST_SRC_DIRS})

include_directories(libs/googletest)
include_directories(libs/googletest/include)

file(GLOB_RECURSE FILES
        ${APPS_PATH}/*.cpp
        ${APPS_PATH}/*.h
        ${LIBS_PATH}/*.cpp
        ${LIBS_PATH}/*.h
        ${LIBS_PATH}/*.hpp
        ${LIBS_PATH}/googletest/include/*.h
)

set(SPAM libs/infinity_cad/src/infinity_cad/rendering/render_objects/curves/bspline.cpp libs/infinity_cad/include/infinity_cad/rendering/render_objects/curves/bspline.h libs/infinity_cad/src/infinity_cad/rendering/render_objects/curves/spline.cpp libs/infinity_cad/include/infinity_cad/rendering/render_objects/curves/spline.h libs/infinity_cad/src/infinity_cad/geometry/polynomials/bspline_basis.cpp libs/infinity_cad/include/infinity_cad/geometry/polynomials/bspline_basis.h libs/infinity_cad/test/src/bspline_tests/bspline_test.cpp libs/infinity_cad/src/infinity_cad/rendering/render_objects/curves/bezier_spline_c2.cpp libs/infinity_cad/include/infinity_cad/rendering/render_objects/curves/bezier_spline_c2.h libs/infinity_cad/src/infinity_cad/math/math.cpp libs/infinity_cad/include/infinity_cad/math/math.h)

set(CMAKE_PREFIX_PATH $ENV{HOME}/ide/qt/5.5/gcc_64/)

find_package(Qt5Widgets  REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Gui REQUIRED)

qt5_wrap_ui(UI_HEADERS /home/jakub/programming/infinityCAD/apps/ifc_editor_qt/mainwindow.ui)

add_executable(${PROJECT_NAME} ${SPAM} ${FILES} ${UI_HEADERS})

target_link_libraries(${PROJECT_NAME} gtest gtest_main ${Qt5Core_QTMAIN_LIBRARIES}
        Qt5Widgets Qt5Core Qt5OpenGL Qt5Gui)