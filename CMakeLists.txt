cmake_minimum_required( VERSION 3.10 )

set(project hydrus)
project(${project})
cmake_policy(SET CMP0071 NEW)

list(APPEND LIBRARY_DIRS /usr/lib /usr/local /usr/local/lib)
message("\n")

# ---OpenCV---
find_package(OpenCV REQUIRED)
if (NOT OpenCV_FOUND)
    message(FATAL_ERROR "No OpenCV installation found, aborting")
endif()
list(APPEND OpenCV_LIBS opencv_highgui)
message("Found OpenCV ${OpenCV_VERSION}")
message("Includes:\t\t${OpenCV_INCLUDE_DIRS}")
message("Libraries:\t\t${OpenCV_LIBS}\n")
list(APPEND LIBRARIES ${OpenCV_LIBS})
list(APPEND INCLUDE_DIRS ${OpenCV_DIRS})
list(APPEND LIBRARY_DIRS /usr/local/lib/opencv4/3rdparty)

# ---Eigen---
find_package(Eigen3 REQUIRED)
if (NOT Eigen3_FOUND)
    message(FATAL_ERROR "No Eigen3 installation found, aborting")
endif()
message("Found Eigen ${Eigen3_VERSION}\n")

# ---Qt---
find_package(Qt5 COMPONENTS Core Gui OpenGL Test Sql Multimedia MultimediaWidgets REQUIRED)
if (NOT Qt5Core_FOUND)
    message(FATAL_ERROR "No Qt installation found, aborting")
endif()
set(Qt5_VERSION ${Qt5Core_VERSION})
set(Qt5_LIBS ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5OpenGL_LIBRARIES} ${Qt5Test_LIBRARIES} ${Qt5Sql_LIBRARIES} ${Qt5Multimedia_LIBRARIES} ${Qt5MultimediaWidgets_LIBRARIES})
set(Qt5_DIRS ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} ${Qt5TestLib_INCLUDE_DIRS} ${Qt5Sql_INCLUDE_DIRS} ${Qt5Multimedia_INCLUDE_DIRS} ${Qt5MultimediaWidgets_INCLUDE_DIRS})
message("Found Qt ${Qt5_VERSION}")
message("Includes:\t\t${Qt5_DIRS}")
message("Libraries:\t\t${Qt5_LIBS}\n")
list(APPEND LIBRARIES ${Qt5_LIBS})
list(APPEND INCLUDE_DIRS ${Qt5_DIRS})

# ---FPIC---
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# ^ Should replace:
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g")

# ---Build specific---
if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Release")
    message("Building with no optimization (Debug build)\n")

    # ---Debug mode---
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 --verbose")

    # ---Qt Debugging---
    if(CMAKE_COMPILER_IS_GNUCXX)
        set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -w -Wall -Wextra -Wstrict-aliasing -pedantic -fmax-errors=5 -Wunreachable-code -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option")
    endif()
else()
    message("Building with full optimization (Release build)\n")

    # ---Optimization---
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

# ---Qt---
get_target_property (QT_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
macro(qt_add_resources)
   qt5_add_resources(${ARGN})
endmacro()
macro(qt_wrap_cpp)
    qt5_wrap_cpp(${ARGN})
endmacro()
macro(qt_wrap_ui)
    qt5_wrap_ui(${ARGN})
endmacro()
set(CMAKE_AUTOMOC on)
list(APPEND INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
# get QT's directory
execute_process(
    COMMAND bash "-c" "qtchooser -print-env | grep QTLIBDIR | cut -d \"=\" -f2 |tr -d '\"' | tr -d '\\n'"
    COMMENT Fetching QT install directory
    OUTPUT_VARIABLE QT_SYSTEM_DIR
)
message("QT system dir: ${QT_SYSTEM_DIR}\n")

# ZLib
list(APPEND LIBRARIES z)
# OpenEXR
list(APPEND LIBRARIES IlmImf)
# Image parsing and processing
list(APPEND LIBRARIES png16 jpeg tiff)
# Intel
list(APPEND LIBRARIES tbb)
# Dynamic linking
list(APPEND LIBRARIES dl)

set(${project}_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/clientdb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hydrusanimation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hydrusimage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hydrusmedia.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hydrusthumbnailitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hydrusthumbnailview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hydrusvideo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imageutils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sha256.cpp
)
set(${project}_MOC
    ${CMAKE_CURRENT_SOURCE_DIR}/clientdb.h
    ${CMAKE_CURRENT_SOURCE_DIR}/hydrusanimation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/hydrusimage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/hydrusmedia.h
    ${CMAKE_CURRENT_SOURCE_DIR}/hydrusthumbnailitem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/hydrusthumbnailview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/hydrusvideo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imageutils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sha256.h
)

include_directories(${INCLUDE_DIRS})
link_directories(${LIBRARY_DIRS})
add_executable(${project} main.cpp ${${project}_SOURCES} ${${project}_MOC})
target_link_libraries(${project} ${LIBRARIES})
message("Building to ${CMAKE_CURRENT_BINARY_DIR}")
