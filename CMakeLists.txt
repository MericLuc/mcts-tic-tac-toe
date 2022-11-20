cmake_minimum_required (VERSION 3.10)
project (mcts-tic-tac-toe LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC             ON)
set(CMAKE_AUTOMOC             ON)
set(CMAKE_AUTORCC             ON)
set(ECV_BRIEF "Monte Carlo Tree Search : Tic Tac Toe application")


if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "This application requires an out of source build.
        Please create a separate build directory")
endif()

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets Svg)
find_package(Qt5 REQUIRED COMPONENTS Widgets Svg)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.ui src/*.hpp src/*.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(3rd/mcts EXCLUDE_FROM_ALL)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE mcts Qt5::Widgets Qt5::Svg)

target_compile_options    (${PROJECT_NAME} PRIVATE -O0 -Werror -Wall -Wextra -pedantic)
target_compile_features   (${PROJECT_NAME} PRIVATE cxx_std_17)
