#Set minimum Cmake version
cmake_minimum_required(VERSION 3.12.1)

#Set compiler flags
include(ProcessorCount)
ProcessorCount(N)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_AUTOGEN_PARALLEL ${N})
set (CMAKE_BUILD_PARALLEL_LEVEL ${N})

# Find protobuf
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/Environment/Libraries/Protobuf/protobuf-3.11.3/cmake")
    option(protobuf_BUILD_TESTS "Build tests" OFF)
    option(protobuf_BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
    option(protobuf_WITH_ZLIB "Build with zlib support" OFF)
    option(protobuf_MSVC_STATIC_RUNTIME "Link static runtime libraries" OFF)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Environment/Libraries/Protobuf/protobuf-3.11.3/cmake")
endif()

#Load websocket driver
add_subdirectory(Libraries/IXWebSocket)

#Load zeroconf driver
add_subdirectory(Libraries/zeroconf)

#Load The Pololu USB Library
set (BUILD_SHARED_LIBS FALSE)
add_subdirectory(Libraries/libusbp)

# Enable ccache
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

#Set the name of this project
project(DAQ-Driver)
option(BUILD_DEBUGGER "Defines if the debugger should also be build" OFF)

#Load the subprojects for this project
add_subdirectory(Driver)

# Generate nicer visual studio SLN
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set (PREDEFINED_TARGETS_FOLDER "CustomTargets")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "daq-driver")

if(BUILD_DEBUGGER)
    find_package(Qt5 REQUIRED Core Widgets)
    add_subdirectory(Debugger)
endif()

