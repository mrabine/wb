# - Try to find join
# Once done this will define
#  JOIN_FOUND - System has join
#  JOIN_INCLUDE_DIRS - The join include directories
#  JOIN_LIBRARY_DIRS - Link directories for join libraries
#  JOIN_LIBRARIES - The libraries needed to use join

find_package(PkgConfig QUIET)
pkg_check_modules(PC_JOIN QUIET libjoin)

find_library(JOIN_LIBRARY join PATH_SUFFIXES build/join NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_CMAKE_FIND_ROOT_PATH)
find_library(JOIN_LIBRARY join HINTS ${PC_JOIN_LIBDIR} ${PC_JOIN_LIBRARY_DIRS})
mark_as_advanced(JOIN_LIBRARY)

find_path(JOIN_CORE_INCLUDE_DIR join/error.hpp PATH_SUFFIXES join/core/include NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_CMAKE_FIND_ROOT_PATH)
find_path(JOIN_CORE_INCLUDE_DIR join/error.hpp HINTS ${PC_JOIN_INCLUDEDIR} ${PC_JOIN_INCLUDE_DIRS})
mark_as_advanced(JOIN_CORE_INCLUDE_DIR)

find_path(JOIN_CRYPTO_INCLUDE_DIR join/openssl.hpp PATH_SUFFIXES join/crypto/include NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_CMAKE_FIND_ROOT_PATH)
find_path(JOIN_CRYPTO_INCLUDE_DIR join/openssl.hpp HINTS ${PC_JOIN_INCLUDEDIR} ${PC_JOIN_INCLUDE_DIRS})
mark_as_advanced(JOIN_CRYPTO_INCLUDE_DIR)

find_path(JOIN_NETWORK_INCLUDE_DIR join/socket.hpp PATH_SUFFIXES join/network/include NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_CMAKE_FIND_ROOT_PATH)
find_path(JOIN_NETWORK_INCLUDE_DIR join/socket.hpp HINTS ${PC_JOIN_INCLUDEDIR} ${PC_JOIN_INCLUDE_DIRS})
mark_as_advanced(JOIN_NETWORK_INCLUDE_DIR)

find_path(JOIN_SAX_INCLUDE_DIR join/sax.hpp PATH_SUFFIXES join/sax/include NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_CMAKE_FIND_ROOT_PATH)
find_path(JOIN_SAX_INCLUDE_DIR join/sax.hpp HINTS ${PC_JOIN_INCLUDEDIR} ${PC_JOIN_INCLUDE_DIRS})
mark_as_advanced(JOIN_SAX_INCLUDE_DIR)

find_path(JOIN_THREAD_INCLUDE_DIR join/thread.hpp PATH_SUFFIXES join/thread/include NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_CMAKE_FIND_ROOT_PATH)
find_path(JOIN_THREAD_INCLUDE_DIR join/thread.hpp HINTS ${PC_JOIN_INCLUDEDIR} ${PC_JOIN_INCLUDE_DIRS})
mark_as_advanced(JOIN_THREAD_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Join
    REQUIRED_VARS    JOIN_LIBRARY
                     JOIN_CORE_INCLUDE_DIR
                     JOIN_CRYPTO_INCLUDE_DIR
                     JOIN_NETWORK_INCLUDE_DIR
                     JOIN_SAX_INCLUDE_DIR
                     JOIN_THREAD_INCLUDE_DIR
)

if(JOIN_FOUND)
    set(JOIN_LIBRARIES
        ${JOIN_LIBRARY}
    )
    set(JOIN_INCLUDE_DIRS
        ${JOIN_CORE_INCLUDE_DIR}
        ${JOIN_CRYPTO_INCLUDE_DIR}
        ${JOIN_NETWORK_INCLUDE_DIR}
        ${JOIN_SAX_INCLUDE_DIR}
        ${JOIN_THREAD_INCLUDE_DIR}
    )
    list(REMOVE_DUPLICATES JOIN_INCLUDE_DIRS)
endif()
