cmake_minimum_required(VERSION 2.6)

project(PROCEDURAL)

set(CMAKE_MODULE_PATH 
  "${PROCEDURAL_SOURCE_DIR}/CMake"
  "${PROCEDURAL_SOURCE_DIR}/CMake/Utils"
  "${PROCEDURAL_SOURCE_DIR}/CMake/Packages"
)

set(PROCEDURAL_VERSION_DASH_SEPARATED "0.2")

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

find_package(Doxygen)

option(PROCEDURAL_BUILD_SAMPLES "Build OgreProcedural samples." TRUE)
option(PROCEDURAL_BUILD_TESTS "Build OgreProcedural test cases." FALSE)
#option(PROCEDURAL_INSTALL_DOCS "Install documentation." FALSE)
#always install docs when they are built ...

# Set docs target directory
if (WIN32 OR APPLE)
  set(PROCEDURAL_DOCS_PATH "docs")
elseif (UNIX)
  set(PROCEDURAL_DOCS_PATH "share/PROCEDURAL/docs")
endif ()

# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROCEDURAL_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROCEDURAL_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROCEDURAL_BINARY_DIR}/bin")

set(PROCEDURAL_TEMPLATES_DIR "${PROCEDURAL_SOURCE_DIR}/CMake/Templates")
set(PROCEDURAL_WORK_DIR ${PROCEDURAL_BINARY_DIR})

# Configure version file for use by scripts
configure_file("${PROCEDURAL_TEMPLATES_DIR}/version.txt.in" "${PROCEDURAL_BINARY_DIR}/version.txt" @ONLY)

include_directories("${PROCEDURAL_SOURCE_DIR}/library/include")

add_subdirectory(library)

# definitions for samples
set(PROCEDURAL_LIBRARIES OgreProcedural) 

# CMake macros
include(ProceduralMacros)
include(ProceduralConfigTargets)

if (PROCEDURAL_BUILD_SAMPLES)	
	add_subdirectory(samples)
endif()

if (PROCEDURAL_BUILD_TESTS)
	add_subdirectory(tests)
endif()

# Install documentation
add_subdirectory(docs)

# Install CMake modules
add_subdirectory(CMake)


# and CPack
# cpack
set(CPACK_PACKAGE_DESCRIPTION "OgreProcedual")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ogre-Procedural is an Ogre3D based lib enabling developpers to quickly create geometric primitives, such as cubes, spheres, cylinders...")
set(CPACK_PACKAGE_NAME "ogre-procedual")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "ois ogre")
set(CPACK_PACKAGE_CONTACT "support@rigsofrods.com")
set(CPACK_PACKAGE_VENDOR "support@rigsofrods.com")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/MIT.txt")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
# TODO: add the version variable at the top in here
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "2")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "OgreProcedual-${PROCEDURAL_VERSION_DASH_SEPARATED}")

SET(CPACK_GENERATOR ZIP)
IF(UNIX)
  SET(CPACK_GENERATOR ${CPACK_GENERATOR};STGZ;TGZ)
ENDIF(UNIX)
IF(LINUX)
SET(CPACK_GENERATOR ${CPACK_GENERATOR};DEB;RPM)
ENDIF(LINUX)
IF(MSVC)
  SET(CPACK_GENERATOR ${CPACK_GENERATOR};NSIS)
ENDIF(MSVC)
IF(APPLE)
  SET(CPACK_GENERATOR ${CPACK_GENERATOR};PackageMaker)
ENDIF(APPLE)
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${PROCEDURAL_VERSION_DASH_SEPARATED}-${CMAKE_SYSTEM_PROCESSOR}")

# some NSIS stuff
IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  #SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  #SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} OgreProcedual")
  SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\http://www.ogre3d.org/tikiwiki/Ogre+Procedural+Geometry+Library")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\http://www.ogre3d.org/tikiwiki/Ogre+Procedural+Geometry+Library")
  SET(CPACK_NSIS_CONTACT "thomas@rigsofrods.com")
  SET(CPACK_NSIS_MODIFY_PATH ON)
ELSE(WIN32 AND NOT UNIX)
  #SET(CPACK_STRIP_FILES "bin/MyExecutable")
  SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)
#SET(CPACK_PACKAGE_EXECUTABLES "MyExecutable" "My Executable")
INCLUDE(CPack)

# other doc files
set(DOC_FILES Contributors.txt MIT.txt ReadMe.txt)
install(FILES ${DOC_FILES} DESTINATION ${PROCEDURAL_DOCS_PATH})