cmake_minimum_required(VERSION 3.5)
# Ignore warnings about rpath behavior on OS X
cmake_policy(SET CMP0042 NEW)
project (ppe)
include(FindPkgConfig)

# Require C++17 build
if(NOT CMAKE_CXX_FLAGS MATCHES "-std=(c|gnu)\\+\\+17")
    message(STATUS "This project requires C++17. Adding -std=c++17 to CXXFLAGS.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17" CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

# Additional debug flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -pg")

# Set default warning flags
set(PROJECT_WARNING_FLAGS "-Wall -Wno-deprecated -Wno-deprecated-declarations" CACHE STRING "Compiler warning flags to include")
mark_as_advanced(PROJECT_WARNING_FLAGS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PROJECT_WARNING_FLAGS}")

# Default to release build if not specified
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif(NOT CMAKE_BUILD_TYPE)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
endif(CMAKE_BUILD_TYPE MATCHES "Debug")

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

	src/util/color.cpp
	src/util/cpercep.cpp
	src/util/dither.cpp
	src/util/reductionhelper.cpp
)

set(wxWidgets_USE_LIBS core base)
find_package(wxWidgets REQUIRED)
pkg_search_module(PROTOBUF REQUIRED protobuf)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/gen/Picross.pb.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/gen/Picross.pb.h
                   COMMAND protoc --cpp_out=../src/gen Picross.proto
                   MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/gen/Picross.proto
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/gen)

set(CMAKE_SKIP_RPATH TRUE)
include(${wxWidgets_USE_FILE})

include_directories(${ppe_SOURCE_DIR}/src/data)
include_directories(${ppe_SOURCE_DIR}/src/gen)
include_directories(${ppe_SOURCE_DIR}/src/gui)
include_directories(${ppe_SOURCE_DIR}/src/util)
include_directories(${PROTOBUF_INCLUDEDIR})

add_executable(
	ppe
	${SRC_ppe}
)

target_link_libraries(
	ppe
	${wxWidgets_LIBRARIES}
	${GTK2_LIBRARIES}
	${PROTOBUF_LIBRARIES}
)

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/ppe DESTINATION bin)
