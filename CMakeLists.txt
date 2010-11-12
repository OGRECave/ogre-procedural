cmake_minimum_required(VERSION 2.6)

project(PROCEDURAL)

set(CMAKE_MODULE_PATH 
  "${PROCEDURAL_SOURCE_DIR}/CMake"
  "${PROCEDURAL_SOURCE_DIR}/CMake/Utils"
  "${PROCEDURAL_SOURCE_DIR}/CMake/Packages"
)

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

set(CMAKE_DEBUG_POSTFIX "_d")

if (WIN32 OR APPLE)
  if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX
	  "${PROCEDURAL_BINARY_DIR}/sdk" CACHE PATH "PROCEDURAL install prefix" FORCE
    )
  endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
endif(WIN32 OR APPLE)

find_package(OGRE REQUIRED)

option(PROCEDURAL_BUILD_SAMPLES "Build OgreProcedural samples." TRUE)

# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROCEDURAL_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROCEDURAL_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROCEDURAL_BINARY_DIR}/bin")

set(PROCEDURAL_TEMPLATES_DIR "${PROCEDURAL_SOURCE_DIR}/CMake/Templates")
set(PROCEDURAL_WORK_DIR ${PROCEDURAL_BINARY_DIR})

add_subdirectory(library)

if (PROCEDURAL_BUILD_SAMPLES)	
	add_subdirectory(samples)
endif()
