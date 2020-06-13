cmake_minimum_required (VERSION 2.6)
project (ppe)
include(FindPkgConfig)

option(DEBUG_MODE "Build for debugging" ON)

# Source files definition
set(SRC_ppe
	src/data/Picross.cpp
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
set(wxWidgets_USE_LIBS core base)
find_package(wxWidgets REQUIRED)
pkg_search_module(PROTOBUF REQUIRED protobuf)
pkg_search_module(QRENCODE REQUIRED libqrencode)

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
	${QRENCODE_LIBRARIES}
)

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/ppe DESTINATION bin)
