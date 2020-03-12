cmake_minimum_required(VERSION 3.5)

project(Debugger LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_definitions(-DQT_IS_AVAILABLE)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets Charts Multimedia REQUIRED)

# Find Headers
file(GLOB_RECURSE HEADER_FILES "*.h")

# Find Sources
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

# Find UI files
file(GLOB_RECURSE UI_FILES "*.ui")

# Create library from sources
add_executable(${PROJECT_NAME} ${UI_FILES} ${HEADER_FILES} ${SOURCE_FILES} )

target_link_libraries(Debugger daq-driver)
target_link_libraries(Debugger Qt5::Widgets Qt5::Charts Qt5::Multimedia)
