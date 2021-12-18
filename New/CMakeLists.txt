cmake_minimum_required(VERSION 3.5)

project(ToDoList_Calendar_QtCreator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Network Sql Xml Qml REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network Sql Xml Qml REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        window.cpp window.h
        userdialog.cpp userdialog.h
        networkmanager.cpp networkmanager.h
        editwindow.cpp editwindow.h
        todoapps.cpp todoapps.h
        resources.qrc
        window.ui
        todoapps.ui
        style_Generic.css
	event.cpp event.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ToDoList_Calendar_QtCreator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ToDoList_Calendar_QtCreator APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ToDoList_Calendar_QtCreator SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ToDoList_Calendar_QtCreator
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ToDoList_Calendar_QtCreator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                                          Qt${QT_VERSION_MAJOR}::Network
                                                          Qt${QT_VERSION_MAJOR}::Sql
                                                          Qt${QT_VERSION_MAJOR}::Xml
                                                          Qt${QT_VERSION_MAJOR}::Qml)

set_target_properties(ToDoList_Calendar_QtCreator PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ToDoList_Calendar_QtCreator)
endif()
