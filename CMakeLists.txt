CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

INCLUDE(ProjectCMakeTools)

# Set up build directories
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

use_cxx14()

#Build source code
set( PROJECT data_conv )
project( ${PROJECT} )

SET( HEADERS
    include/file.hpp
    include/filesystem.hpp 
    include/flatfile.hpp
    include/flatfilesystem.hpp 
    include/safedelete.hpp
    include/stb.h )

SET( SOURCES
    src/main.cpp
    src/stb.cpp
    src/filesystem.cpp )
    
add_executable(${PROJECT} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
