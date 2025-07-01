cmake_minimum_required(VERSION 3.10.0)
project(altBase VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)

add_executable(altBase 
    main.cpp 
    gui/CentralWidget.h 
    gui/CentralWidget.cpp
    gui/CollapsibleTableModel.h
    gui/CollapsibleTableModel.cpp
)

target_link_libraries(altBase PUBLIC Qt5::Widgets)
target_link_libraries(altBase PUBLIC Qt5::Core)
target_link_libraries(altBase PUBLIC Qt5::Gui)
target_link_libraries(altBase PUBLIC ${PROJECT_SOURCE_DIR}/lib/libAltBranches.so)

