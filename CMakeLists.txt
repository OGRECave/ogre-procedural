cmake_minimum_required(VERSION 2.6)

project(PROCEDURAL)

set(OGRE_DIR $ENV{OGRE_HOME})
STRING(REGEX REPLACE "\\\\" "/" OGRE_DIR ${OGRE_DIR}) 

if(WIN32)
	set(CMAKE_MODULE_PATH "${OGRE_DIR}/CMake/;${CMAKE_MODULE_PATH}")
	set(OGRE_SAMPLES_INCLUDEPATH
		$ENV{OGRE_HOME}/Samples/common/include/
		$ENV{OGRE_HOME}/Samples/browser/include/
	)
endif(WIN32)

if(UNIX)
	set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	set(OGRE_SAMPLES_INCLUDEPATH
		/usr/local/share/OGRE/samples/Common/include/
	)
endif(UNIX)

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

set(CMAKE_DEBUG_POSTFIX "_d")

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}")

find_package(OGRE REQUIRED)

if(NOT "${OGRE_VERSION_NAME}" STREQUAL "Cthugha")
  message(SEND_ERROR "You need Ogre 1.7 Cthugha to build this.")
endif()

find_package(OIS REQUIRED)

if(NOT OIS_FOUND)
    message(SEND_ERROR "Failed to find OIS.")
endif()

# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROCEDURAL_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROCEDURAL_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROCEDURAL_BINARY_DIR}/bin")

set(PROCEDURAL_TEMPLATES_DIR "${PROCEDURAL_SOURCE_DIR}/CMake/Templates")
set(PROCEDURAL_WORK_DIR ${PROCEDURAL_BINARY_DIR})

add_subdirectory(library)

add_subdirectory(samples)
