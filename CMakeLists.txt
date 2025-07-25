﻿cmake_minimum_required(VERSION 3.5)

project(Drawish VERSION 1.6 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets )
find_package(Qt6 REQUIRED COMPONENTS PrintSupport)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        area.h
        area.cpp
        borderright.h
        borderright.cpp
        borderbottom.h
        borderbottom.cpp
        geometric.h
        geometric.cpp
        angolo.h
        angolo.cpp
        selectionarea.h
        selectionarea.cpp
        shapearea.h
        shapearea.cpp
        curvelinearea.h
        curvelinearea.cpp
        zoomarea.h
        zoomarea.cpp
        dialogsize.h
        dialogsize.cpp
        dialogsize.ui
        dialogeffects.h
        dialogeffects.cpp
        dialogeffects.ui
        stretchdialog.h
        stretchdialog.cpp
        stretchdialog.ui
        stretchview.h
        stretchview.cpp
        dcolors.h
        dcolors.cpp
        dcolors.ui
        viewimage.h
        viewimage.cpp
        viewimage.ui
        savecam.h
        savecam.cpp
        richeditor.cpp
        richeditor.h
        richeditor.ui
        linedialog.h
        linedialog.cpp
        linedialog.ui
        figures.h
        figures.cpp
        figures.ui
        fileio.h
        fileio.cpp
        pickpalette.cpp
        pickpalette.h
        pickpalette.ui
        serialize.cpp
        serialize.h
        serialize.ui
        Drawish.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Drawish
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${QM_FILES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Drawish APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Drawish SHARED
            ${PROJECT_SOURCES}
            ${QM_FILES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Drawish
            ${PROJECT_SOURCES}
            ${QM_FILES}
        )
    endif()
endif()

target_link_libraries(Drawish PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Drawish PRIVATE Qt6::PrintSupport)

set_target_properties(Drawish PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Drawish
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Drawish)
endif()
