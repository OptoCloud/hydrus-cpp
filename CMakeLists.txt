cmake_minimum_required (VERSION 3.5)

set (project hydrus)
project (
    ${project}
    DESCRIPTION "Hydrus rewritten in C++"
    LANGUAGES CXX
)

option(VERBOSE "Verbose compiler output" OFF)

# --- Configure default build ---
set (default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message (STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set (CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    set_property (CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()
# --------------------------------

# --- Simplify build checking ---
math (EXPR VPTR_BITS "${CMAKE_SIZEOF_VOID_P}*8") # Convert 4/8 -> 32/64

if (APPLE)
    set (BUILD_OS "osx")
elseif (UNIX)
    set (BUILD_OS "linux")
elseif (WIN32)
    set (BUILD_OS "windows")
endif ()

string (TOLOWER ${CMAKE_CXX_COMPILER_ID} BUILD_COMPILER)

# Translate some stuff
if (${BUILD_COMPILER} STREQUAL "gnu")
  set (BUILD_COMPILER "gcc")
elseif (${BUILD_COMPILER} STREQUAL "intel")
  set (BUILD_COMPILER "icc")
endif ()
# --------------------------------

# --- Set global C++ CMake settings ---
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)
set (CMAKE_POLICY_DEFAULT_CMP0069 NEW)
set (CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
if (VERBOSE)
    set (CMAKE_VERBOSE_MAKEFILE ON)
endif ()
# -------------------------------

# --- Set compiler flags ---
# Debugging, and warnings
add_definitions (-DBUILDER_STATIC_DEFINE)
if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Release")
    add_definitions (-DDEBUG)
    set (BUILD_TYPE "dbg")

    if (MSVC)
        # Remove all previously declared flags before adding new ones
        string (REGEX REPLACE "[/-]W[0-4] ?" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

        # Verboseeeee
        #string (APPEND CMAKE_CXX_FLAGS " /Wall")
    else ()
        # Verboseeeee
        string (APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -pedantic -Wformat=2 -Wsign-conversion -Wundef -Wredundant-decls -Wwrite-strings -Wpointer-arith -Wcast-qual -Wmissing-include-dirs -Wcast-align -Wctor-dtor-privacy -Wdisabled-optimization -Winvalid-pch -Woverloaded-virtual -Wconversion -Wswitch-enum -Wno-ctor-dtor-privacy -Wno-format-nonliteral -Wnoexcept -Wno-dangling-else -Wno-unused-local-typedefs -Wdouble-promotion -Wtrampolines -Wzero-as-null-pointer-constant -Wuseless-cast -Wvector-operation-performance -Wsized-deallocation -Wshadow -Wshift-overflow=2 -Wnull-dereference -Wduplicated-cond -Wpessimizing-move -Wredundant-move")
    endif ()
else ()
    set (BUILD_TYPE "rel")
endif ()

# Optimization and compiler specific fixes
if (MSVC)
    # Remove all previously declared flags before adding new ones
    string (REGEX REPLACE "[/-][ML][DT]d? ?" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string (REGEX REPLACE "[/-]O[12bdgistxy] ?" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string (REGEX REPLACE "[/-]DEBUG:[a-zA-Z]+ ?" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

    # Enable max warnings (But dont spam the console)
    string (APPEND CMAKE_CXX_FLAGS " /DNOMINMAX /wo5054")

    if (${CMAKE_BUILD_TYPE} MATCHES "Release")
        string (APPEND CMAKE_CXX_FLAGS " /MD /DEBUG:NONE /O2 /Ob2")
    else ()
        string (APPEND CMAKE_CXX_FLAGS " /MDd /DEBUG:FULL /Od /Ob0")
    endif ()
else ()
    # 64-bit, and threading
    string (APPEND CMAKE_CXX_FLAGS " -m64 -pthread")

    if (${CMAKE_BUILD_TYPE} MATCHES "Release")
        string (APPEND CMAKE_CXX_FLAGS " -fstack-protector -O3 -g0")
        if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            string (APPEND CMAKE_CXX_FLAGS " -s")
        endif ()
    else ()
        string (APPEND CMAKE_CXX_FLAGS " -O0 -g3")
    endif ()
endif ()
# --------------------------

# -- Set global libraries --
if (WIN32)
    set (WIN_LIBS ws2_32 winmm Rpcrt4 Iphlpapi)
endif ()

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)

link_libraries (
    Threads::Threads
    ${CMAKE_DL_LIBS}
    ${WIN_LIBS}
)
# --------------------------

# - Find and configure Qt --
find_package (
    Qt5
    COMPONENTS
        Core
        Gui
        Test
        OpenGL
        Widgets
        Multimedia
        MultimediaWidgets
        Concurrent
        Network
        Sql
    REQUIRED
)
if (NOT Qt5Core_FOUND)
    message (FATAL_ERROR "No Qt installation found, aborting")
endif ()

set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

message ("Found Qt ${Qt5Core_VERSION}")
# --------------------------

# -------- Dependencies ---------
find_package (OpenCV REQUIRED CONFIG)
include_directories (${OpenCV_INCLUDE_DIRS})
# -------------------------------

if (LINUX)
	# ZLib
        list (APPEND LIBRARIES z)
	# OpenEXR
        list (APPEND LIBRARIES IlmImf)
	# Image parsing and processing
        list (APPEND LIBRARIES png16 jpeg tiff)
endif ()

set (${project}_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/clientdb.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/imageutils.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/hydrusthumbnailitem.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/hydrusthumbnailscene.cpp
)
set (${project}_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/clientdb.h
	${CMAKE_CURRENT_SOURCE_DIR}/hydrusid.h
	${CMAKE_CURRENT_SOURCE_DIR}/imageutils.h
	#${CMAKE_CURRENT_SOURCE_DIR}/hydrusthumbnailitem.h
	#${CMAKE_CURRENT_SOURCE_DIR}/hydrusthumbnailscene.h
)
set (${project}_RESOURCES
        #${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc
)

include_directories (
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Test_INCLUDE_DIRS}
    ${Qt5OpenGL_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Multimedia_INCLUDE_DIRS}
    ${Qt5MultimediaWidgets_INCLUDE_DIRS}
    ${Qt5Concurrent_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${Qt5Sql_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable (${project}
    main.cpp
    ${${project}_HEADERS}
    ${${project}_SOURCES}
    ${${project}_RESOURCES}
)

target_link_libraries (${project} PRIVATE
    ${Qt5Core_LIBRARIES}
    ${Qt5Gui_LIBRARIES}
    ${Qt5Test_LIBRARIES}
    ${Qt5OpenGL_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Multimedia_LIBRARIES}
    ${Qt5MultimediaWidgets_LIBRARIES}
    ${Qt5Concurrent_LIBRARIES}
    ${Qt5Network_LIBRARIES}
    ${Qt5Sql_LIBRARIES}
    ${OpenCV_LIBRARIES}
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property (TARGET ${project} PROPERTY WIN32_EXECUTABLE TRUE)
endif ()

