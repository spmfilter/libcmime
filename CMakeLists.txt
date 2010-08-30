cmake_minimum_required(VERSION 2.6)

project(libcmime)

# check for build.properties
include("${CMAKE_SOURCE_DIR}/build.properties" OPTIONAL)

# check prefix
if(PREFIX)
	SET(CMAKE_INSTALL_PREFIX ${PREFIX})
endif(PREFIX)

# check out current version
set(THREE_PART_VERSION_REGEX "[0-9]+\\.[0-9]+\\.[0-9]+")
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/VERSION CMIME_VERSION)

# check for enabled debugging
if(ENABLE_DEBUG)
        set(CMAKE_VERBOSE_MAKEFILE TRUE)
    add_definitions(-DDEBUG -g -O0 -Wall)
endif(ENABLE_DEBUG)

add_subdirectory(src)
add_subdirectory(test)

set(ARCHIVE_NAME "${CMAKE_PROJECT_NAME}-${CMIME_VERSION}")
set(CPACK_PACKAGE_NAME "libcmime")
set(CPACK_PACKAGE_VENDOR "Axel Steiner")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libcmime - A C mime library")
set(CPACK_PACKAGE_VERSION ${CMIME_VERSION})
set(CPACK_COMPONENTS_ALL applications libraries headers)
