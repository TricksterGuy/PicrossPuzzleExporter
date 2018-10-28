cmake_minimum_required (VERSION 2.6)
project (ppe)

set(CPACK_BINARY_DEB "ON")
set(CPACK_BINARY_RPM "OFF")
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "PPE")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "ppe 1.0.0")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CPACK_DEBIAN_PACKAGE_NAME "ppe")
set(CPACK_DEBIAN_PACKAGE_VERSION "1.0")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Brandon Whitehead <brandon.whitehead@gatech.edu>")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "ppe")

set(CPACK_PACKAGE_FILE_NAME "ppe-1.0.0-i386")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")

include(CPack)
include(FindPkgConfig)

add_definitions(-DLINUX)

option(DEBUG_MODE "Build for debugging" ON)
option(TEST_MODE "Build for testing" OFF)

# Source files definition
set(SRC_ppe
	src/data/Picross.cpp
	src/data/PicrossLayer.cpp
	src/data/PicrossBW.cpp
	src/data/PicrossGray.cpp
	src/data/PicrossRGB.cpp
	src/data/PicrossRBY.cpp
	src/data/PicrossValidator.cpp

    src/gen/Picross.pb.cc
	src/gen/PicrossGUI.cpp

	src/gui/PicrossApp.cpp
	src/gui/PicrossDataCanvas.cpp
	src/gui/PicrossFrame.cpp
	src/gui/QRCodePanel.cpp
	src/gui/QRCodeDialog.cpp

	src/util/cdecode.c
	src/util/cencode.c
	src/util/color.cpp
	src/util/cpercep.cpp
	src/util/dither.cpp
	src/util/histogram.cpp
	src/util/mediancut.cpp
	src/util/reductionhelper.cpp
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
set(wxWidgets_USE_LIBS xrc xml html adv gl net core base)
find_package(wxWidgets REQUIRED)
find_package(GTK2 REQUIRED gtk)
pkg_search_module(PROTOBUF REQUIRED protobuf)
pkg_search_module(PROTOBUF REQUIRED qrencode)

IF(DEBUG_MODE)
    set(CMAKE_CXX_FLAGS "-g -Wall -std=c++0x -DwxUSE_GRAPHICS_CONTEXT=1")
ELSE()
    set(CMAKE_CXX_FLAGS "-O3 -s -Wall -std=c++0x -DwxUSE_GRAPHICS_CONTEXT=1")
ENDIF(DEBUG_MODE)

set(CMAKE_SKIP_RPATH TRUE)
include(${wxWidgets_USE_FILE})

include_directories(${ppe_SOURCE_DIR}/src/data)
include_directories(${ppe_SOURCE_DIR}/src/gen)
include_directories(${ppe_SOURCE_DIR}/src/gui)
include_directories(${ppe_SOURCE_DIR}/src/util)
include_directories(${GTK2_INCLUDE_DIRS})
include_directories(${PROTOBUF_INCLUDEDIR})
include_directories(${QRENCODE_INCLUDEDIR})

add_executable(
	ppe
	${SRC_ppe}
)

target_link_libraries(
	ppe
	${wxWidgets_LIBRARIES}
	${GTK2_LIBRARIES}
	${PROTOBUF_LIBRARIES}
	qrencode
)

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/ppe DESTINATION bin)
