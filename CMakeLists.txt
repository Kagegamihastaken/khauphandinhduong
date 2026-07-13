cmake_minimum_required(VERSION 3.25)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0069 NEW)
project(khauphandinhduong VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

#program setting
set(DEVELOPMENT_BUILD ON)
set(LOGGING_ENABLE OFF)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build Static Libraries" FORCE)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
include(FetchContent)
#dependency
FetchContent_Declare(cpptrace
        GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
        GIT_TAG v1.0.2
        GIT_SHALLOW ON
        GIT_PROGRESS ON
        SYSTEM
)
FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 11.2.0
        GIT_SHALLOW ON
        GIT_PROGRESS ON
        SYSTEM
)
FetchContent_Declare(Boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-1.88.0
        GIT_SHALLOW ON
        GIT_PROGRESS ON
        SYSTEM
)
message("Making dependencies available...")
FetchContent_MakeAvailable(cpptrace fmt)

set(BOOST_INCLUDE_LIBRARIES type_index unordered)
set(BOOST_ENABLE_CMAKE ON)
FetchContent_MakeAvailable(Boost)
#source project
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config/config.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/config/config.h
)
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/sources.cpp"
)
#header project
file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.hpp"
)

add_executable(${CMAKE_PROJECT_NAME} ${MY_SOURCES} ${MY_HEADERS})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -ffunction-sections -fdata-sections)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -Wl,--gc-sections)
    if(NOT DEVELOPMENT_BUILD)
        message(STATUS "Production Build detected. Applying -O3.")
        target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:-O3>)
    else()
        message(STATUS "Development Build detected. Applying -O2.")
        target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:-O2>)
    endif()
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        Qt6::Widgets cpptrace::cpptrace fmt::fmt Boost::type_index
        Boost::unordered)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/headers"
        "${CMAKE_CURRENT_BINARY_DIR}/config"
)

# Link platform-specific system libraries
if(WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE winmm gdi32 user32 shell32 ole32 uuid)
    if(MINGW)
        target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -static -static-libgcc -static-libstdc++ -flto)
        if (NOT DEVELOPMENT_BUILD)
            target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -Wl,-subsystem,windows)
        endif()
    endif()
endif()