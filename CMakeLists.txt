cmake_minimum_required(VERSION 4.2)
project(khauphandinhduong VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build Static Libraries" FORCE)
#dependency

add_executable(khauphandinhduong main.cpp)
target_link_libraries(khauphandinhduong PRIVATE Qt6::Widgets)
