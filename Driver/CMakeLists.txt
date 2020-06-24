cmake_minimum_required(VERSION 3.0.0)

project(daq-driver VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find dependencies
find_package(Qt5 QUIET OPTIONAL_COMPONENTS Core SerialPort)
if(Qt5SerialPort_FOUND AND Qt5Core_FOUND)
    add_definitions(-DQT_IS_AVAILABLE)
    set(CMAKE_AUTOMOC ON)
endif()

# Find the Protobuf library
if (WIN32)
	find_package(protobuf QUIET REQUIRED)
else()
	find_package(Protobuf QUIET REQUIRED)
endif()

# Optimize CRC
add_definitions(-DCRCPP_BRANCHLESS -DCRCPP_USE_CPP11)

#Generate protobuff pb.h and pb.cc files, set define with all Protobuff Messages
file(GLOB_RECURSE PROTOBUF_DEFINITION_FILES "*.proto")
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOBUF_DEFINITION_FILES})
set_property(SOURCE ${PROTO_SRCS} ${PROTO_HDRS} PROPERTY SKIP_AUTOMOC ON)

# Find Headers
file(GLOB_RECURSE HEADER_FILES "*.h")

# Find Sources
file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.c" "*.cc")

#Order sources nicely for visual studio projects
file(GLOB_RECURSE DriverComponents_FILES "DriverComponents/*.h" "DriverComponents/*.cpp")
source_group(DriverComponents FILES ${DriverComponents_FILES})
file(GLOB_RECURSE MessageProcessing_FILES "MessageProcessing/*.h" "MessageProcessing/*.cpp")
source_group(MessageProcessing FILES ${MessageProcessing_FILES})
file(GLOB_RECURSE SerialConnector_FILES "SerialConnector/*.h" "SerialConnector/*.cpp")
source_group(SerialConnector FILES ${SerialConnector_FILES})
file(GLOB_RECURSE Threading_FILES "Threading/*.h" "Threading/*.cpp")
source_group(Discovery FILES ${Discovery_FILES})
file(GLOB_RECURSE Discovery_FILES "Discovery/*.h" "Discovery/*.cpp")
source_group(Threading FILES ${Threading_FILES})
source_group(ProtoBuf FILES ${PROTOBUF_DEFINITION_FILES} ${PROTO_HDRS} ${PROTO_SRCS})

# Create library from sources
add_library(${PROJECT_NAME} STATIC ${PROTOBUF_DEFINITION_FILES} ${PROTO_HDRS} ${PROTO_SRCS} ${HEADER_FILES} ${SOURCE_FILES} )
add_library(Voyager::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

#On windows, export all symbols
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Configure include directory
target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
)

# Link with dependencies
if(Qt5SerialPort_FOUND AND Qt5Core_FOUND)
        target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::SerialPort)
endif()
target_link_libraries(${PROJECT_NAME} protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
target_link_libraries(${PROJECT_NAME} ixwebsocket zeroconf)
