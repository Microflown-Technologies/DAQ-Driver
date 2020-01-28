#Set minimum Cmake version
cmake_minimum_required(VERSION 3.13.4)

#Set compiler flags
include(ProcessorCount)
ProcessorCount(N)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_AUTOGEN_PARALLEL ${N})
set (CMAKE_BUILD_PARALLEL_LEVEL ${N})

# Enable CMAKE
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

#Set the name of this project
project(DAQ-Driver)

#Load the subprojects for this project
add_subdirectory(Driver)
add_subdirectory(Debugger)


