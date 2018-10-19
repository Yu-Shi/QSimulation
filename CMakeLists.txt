cmake_minimum_required(VERSION 3.1.0)

project(QSimulation)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


# Find the QtWidgets library
find_package(Qt5Widgets)
#find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)

# Populate a CMake variable with the sources
set(QSimulation_SRCS
    mainwindow.cpp mainwindow.h dialognew.h pipebutton.h
    main.cpp dialognew.cpp paintarea.cpp pipebutton.cpp utili.cpp dialoganneal.cpp
        )

# Tell CMake to create the Sudoku executable
add_executable(QSimulation WIN32 ${QSimulation_SRCS})
# Use the Widgets module from Qt 5
target_link_libraries(QSimulation Qt5::Widgets Qt5::Gui Qt5::Core)