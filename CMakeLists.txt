cmake_minimum_required(VERSION 3.10.0)
project(ALBCT VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/structs)
link_directories(${PROJECT_SOURCE_DIR}/lib)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(ALBCT 
    main.cpp 
    gui/CentralWidget.h 
    gui/CentralWidget.cpp
    gui/TreeModel.h
    gui/TreeModel.cpp
)

target_link_libraries(ALBCT PUBLIC Qt5::Widgets)
target_link_libraries(ALBCT PUBLIC Qt5::Core)
target_link_libraries(ALBCT PUBLIC Qt5::Gui)
target_link_libraries(ALBCT PUBLIC libAltBranches.so)

