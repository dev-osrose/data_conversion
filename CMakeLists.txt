CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

INCLUDE(ProjectCMakeTools)

# Set up build directories
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

use_cxx17()

#Build 3rdparty dependencies if needed
#ADD_SUBDIRECTORY(3rdparty)


set(EXTERNAL_DEPS_ROOT ${CMAKE_SOURCE_DIR}/3rdparty)
set(SPDLOG_ROOT ${EXTERNAL_DEPS_ROOT}/spdlog/include)


#Build source code
set( PROJECT data_conv )
project( ${PROJECT} )



include_directories(
  ${SPDLOG_ROOT}
)

SET( HEADERS
    include/logconsole.h
    include/file.hpp
    include/filesystem.hpp 
    include/flatfile.hpp
    include/flatfilesystem.hpp 
    include/bufferedfile.hpp
    include/bufferedfilesystem.hpp 
    include/safedelete.hpp
    include/scopedpointer.hpp 
    include/stb.h
    include/stl.h
    include/ifo.h
    include/stbconverter.h)

SET( SOURCES
    src/main.cpp
    src/logconsole.cpp
    src/filesystem.cpp
    src/stb.cpp
    src/stl.cpp
    src/ifo.cpp
    src/stbconverter.cpp )
    
find_package( Threads REQUIRED )
add_executable(${PROJECT} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT})
