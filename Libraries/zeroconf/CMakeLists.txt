#Set the name of this project
project(zeroconf)

# Find Headers
file(GLOB_RECURSE HEADER_FILES src/*.hpp)

# Create library from sources
add_library(${PROJECT_NAME} INTERFACE)

# Configure include directory
target_include_directories( ${PROJECT_NAME}
    INTERFACE ${PROJECT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}
)
