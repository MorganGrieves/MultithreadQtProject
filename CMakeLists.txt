cmake_minimum_required(VERSION 3.5)

project(MultitheadQtProject LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file (GLOB_RECURSE SOURCES "*.cpp")
file (GLOB_RECURSE HEADERS "*.h")
file (GLOB_RECURSE UI "*.ui")
file (GLOB_RECURSE RESOURCE "*.qrc" "*.rc")


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)


add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${UI} ${RESOURCE})
target_link_libraries(MultitheadQtProject PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
