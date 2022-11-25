cmake_minimum_required(VERSION 2.6)

project(libcmime)

# include modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include(${CMAKE_MODULE_PATH}/GNUInstallDirs.cmake)

# check for build.properties
include("${CMAKE_SOURCE_DIR}/build.properties" OPTIONAL)

# check prefix
if(PREFIX)
    set(CMAKE_INSTALL_PREFIX ${PREFIX})
endif(PREFIX)

# check out current version
set(THREE_PART_VERSION_REGEX "[0-9]+\\.[0-9]+\\.[0-9]+")
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/VERSION CMIME_VERSION)

if(${CMIME_VERSION} MATCHES ${THREE_PART_VERSION_REGEX})
    string(REGEX REPLACE "^([0-9]+)\\.[0-9]+\\.[0-9]+" "\\1" CMIME_MAJOR_VERSION ${CMIME_VERSION})
    string(REGEX REPLACE "^[0-9]+\\.([0-9])+\\.[0-9]+" "\\1" CMIME_MINOR_VERSION ${CMIME_VERSION})
    string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+)" "\\1" CMIME_MICRO_VERSION ${CMIME_VERSION})
else(${CMIME_VERSION} MATCHES ${THREE_PART_VERSION_REGEX})
    message(FATAL_ERROR "Problem parsing version string, I can't parse it properly.")
endif(${CMIME_VERSION} MATCHES ${THREE_PART_VERSION_REGEX})

math(EXPR version_number "${CMIME_MAJOR_VERSION} * 1000000 + ${CMIME_MINOR_VERSION} * 1000 + ${CMIME_MICRO_VERSION}" )
set(CMIME_VERSION_NUMBER ${version_number})
set(CMIME_SO_VERSION "${CMIME_MAJOR_VERSION}.${CMIME_MINOR_VERSION}")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# check for enabled debugging
if(ENABLE_DEBUG)
    set(CMAKE_VERBOSE_MAKEFILE TRUE)
    add_definitions(-DDEBUG -g -O0 -Wall)
    ENABLE_TESTING()
    file(COPY "${CMAKE_SOURCE_DIR}/test/samples" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/test")
endif(ENABLE_DEBUG)

# check for doxygen
if(ENABLE_DOC)
    include(${CMAKE_MODULE_PATH}/FindDoxygen.cmake)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    if(NOT DOXYGEN_VERSION GREATER "1\\.8")
        message(FATAL_ERROR "Doxygen >= 1.8 is needed to build the documentation.")
    endif()
    
    if(NOT DOXYGEN_DOT_FOUND)
        message(FATAL_ERROR "Graphviz is needed to build the documentation.")
    endif()


    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cmake)
    set(doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    configure_file(${doxyfile_in} ${doxyfile} @ONLY)
    
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    install(DIRECTORY 
        ${CMAKE_CURRENT_BINARY_DIR}/html 
        DESTINATION share/doc/libcmime-${CMIME_MAJOR_VERSION}.${CMIME_MINOR_VERSION}.${CMIME_MICRO_VERSION})
endif(ENABLE_DOC)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

find_program(FILE_EXECUTABLE NAME file PATHS ENV PATH)
if(NOT FILE_EXECUTABLE)
    message(FATAL_ERROR "Could not find file executable")
endif(NOT FILE_EXECUTABLE)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cmime_config.h.cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cmime_config.h
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/libcmime.pc.cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/libcmime.pc
    @ONLY
)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/libcmime.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

set(ARCHIVE_NAME "${CMAKE_PROJECT_NAME}-${CMIME_VERSION}")
set(CPACK_PACKAGE_NAME "libcmime")
set(CPACK_PACKAGE_VENDOR "Axel Steiner <ast@treibsand.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libcmime - A C mime library")
set(CPACK_PACKAGE_VERSION ${CMIME_VERSION})

set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CMIME_VERSION}")
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES
"\\\\.git"
"build"
"build\\\\.properties"
"doc/html/*"
"debian"
"libcmime\\\\.pc$"
"src/lex\\\\.yy\\\\.c"
"src/cmime_config\\\\.h$"
"src/cmime_parser\\\\.tab*"
)

include(CPack)
