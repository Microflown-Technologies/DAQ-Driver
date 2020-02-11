#Set minimum Cmake version
cmake_minimum_required(VERSION 3.12.0)

#Set compiler flags
include(ProcessorCount)
ProcessorCount(N)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_AUTOGEN_PARALLEL ${N})
set (CMAKE_BUILD_PARALLEL_LEVEL ${N})

# Find protobuf
option(protobuf_BUILD_TESTS "Build tests" OFF)
option(protobuf_BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(protobuf_WITH_ZLIB "Build with zlib support" OFF)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Environment/Libraries/Protobuf/protobuf-3.11.3/cmake")

# Enable CMAKE
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

#Set the name of this project
project(DAQ-Driver)

#Load the subprojects for this project
add_subdirectory(Driver)

find_package(Qt5 COMPONENTS Widgets OPTIONAL)
if(Qt5Widgets_FOUND)
    add_subdirectory(Debugger)
endif()

