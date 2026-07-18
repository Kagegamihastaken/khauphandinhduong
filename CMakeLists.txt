cmake_minimum_required(VERSION 3.25)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0069 NEW)
project(khauphandinhduong VERSION 0.4)

add_compile_definitions(__USE_MINGW_ANSI_STDIO=1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

#program setting
set(DEVELOPMENT_BUILD ON)
set(LOGGING_ENABLE OFF)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build Static Libraries" FORCE)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
include(FetchContent)

set(FETCHCONTENT_QUIET OFF CACHE BOOL "Show download progress" FORCE)

#dependency project
FetchContent_Declare(physfs
        GIT_REPOSITORY https://github.com/icculus/physfs.git
        GIT_TAG release-3.2.0
        GIT_SHALLOW ON
        GIT_PROGRESS ON
        SYSTEM
)
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
FetchContent_Declare(highs
        GIT_REPOSITORY https://github.com/ERGO-Code/HiGHS.git
        GIT_TAG        v1.15.1 # Or replace with 'master' / desired version release tag
        GIT_SHALLOW ON
        GIT_PROGRESS ON
        SYSTEM
)
FetchContent_Declare(JSON
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG  v3.12.0
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
FetchContent_Declare(magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_TAG v0.9.8
        GIT_SHALLOW ON
        GIT_PROGRESS ON
        SYSTEM
)
set(FAST_BUILD ON CACHE INTERNAL "")

set(BOOST_INCLUDE_LIBRARIES type_index unordered)
set(BOOST_ENABLE_CMAKE ON)
FetchContent_MakeAvailable(physfs cpptrace fmt highs JSON Boost magic_enum)

#source project
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config/config.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/config/config.h
)
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/sources.cpp"
)
file(GLOB_RECURSE MY_UIS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui"
)
qt6_wrap_ui(GENERATED_UI_HEADERS ${MY_UIS})
message(${GENERATED_UI_HEADERS})
#header project
file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.hpp"
)
add_executable(${CMAKE_PROJECT_NAME} ${MY_UIS} ${MY_SOURCES} ${MY_HEADERS} ${GENERATED_UI_HEADERS})

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
        Qt6::Widgets highs::highs cpptrace::cpptrace fmt::fmt
        Boost::type_index Boost::unordered physfs-static
        nlohmann_json::nlohmann_json magic_enum::magic_enum
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/headers"
        "${CMAKE_CURRENT_BINARY_DIR}/config"
        "${CMAKE_CURRENT_BINARY_DIR}"
)

# Link platform-specific system libraries
if(WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE winmm gdi32 user32 shell32 ole32 uuid dwmapi uxtheme setupapi version imm32)
    if(MINGW)
        target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -static -static-libgcc -static-libstdc++)
        if (NOT DEVELOPMENT_BUILD)
            target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -Wl,-subsystem,windows)
        endif()
    endif()
endif()