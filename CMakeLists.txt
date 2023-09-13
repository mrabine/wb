cmake_minimum_required(VERSION 3.1.0)
project(wb)

# Add custom CMake modules.
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Require C++20 support, prefer ISO C++ over GNU variants
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Strict warnings and warnings as errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-psabi")

file(STRINGS "${PROJECT_SOURCE_DIR}/VERSION" VERSION)
string(REGEX REPLACE "([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" VERSION_MAJOR ${VERSION})
string(REGEX REPLACE "[0-9]+\\.([0-9])+\\.[0-9]+.*" "\\1" VERSION_MINOR ${VERSION})
string(REGEX REPLACE "[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" VERSION_PATCH ${VERSION})

message(STATUS "${PROJECT_NAME} version: ${VERSION}")
configure_file("${PROJECT_SOURCE_DIR}/version.hpp.in" "${PROJECT_SOURCE_DIR}/src/version.hpp")

include(FetchContent)
FetchContent_Declare(
    join
    GIT_REPOSITORY    https://github.com/joinframework/join.git
    GIT_TAG           v1.1.3
    FIND_PACKAGE_ARGS Join
)
FetchContent_MakeAvailable(join)

include_directories(${JOIN_INCLUDE_DIRS})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} join pthread)

install(TARGETS ${PROJECT_NAME} DESTINATION /bin)
