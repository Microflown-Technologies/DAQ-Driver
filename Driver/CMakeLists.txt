cmake_minimum_required(VERSION 3.0.0)

project(daq-driver VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Find dependencies
find_package(Qt5 REQUIRED COMPONENTS Core SerialPort)

# Find the Protobuf library
find_package(Protobuf REQUIRED)


# Optimize CRC
add_definitions(-DCRCPP_BRANCHLESS -DCRCPP_USE_CPP11 -DQT_IS_AVAILABLE)

#Generate protobuff pb.h and pb.cc files, set define with all Protobuff Messages
file(GLOB_RECURSE PROTOBUF_DEFINITION_FILES "*.proto")
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOBUF_DEFINITION_FILES})

# Find Headers
file(GLOB_RECURSE HEADER_FILES "*.h")

# Find Sources
file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.c")

# Create library from sources
add_library(${PROJECT_NAME} SHARED ${PROTOBUF_DEFINITION_FILES} ${PROTO_HDRS} ${PROTO_SRCS} ${HEADER_FILES} ${SOURCE_FILES} )
add_library(Voyager::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Configure include directory
target_include_directories( ${PROJECT_NAME}
    SYSTEM PUBLIC ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
)

# Link with dependencies
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::SerialPort)
target_link_libraries(${PROJECT_NAME} protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
