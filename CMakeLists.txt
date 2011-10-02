cmake_minimum_required(VERSION 2.6)

project(OgreProcedural)

set(CMAKE_MODULE_PATH 
  "${OgreProcedural_SOURCE_DIR}/CMake"
  "${OgreProcedural_SOURCE_DIR}/CMake/Utils"
  "${OgreProcedural_SOURCE_DIR}/CMake/Packages"
)

set(OgreProcedural_VERSION_DASH_SEPARATED "0.2")

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

set(CMAKE_DEBUG_POSTFIX "_d")

if (WIN32 OR APPLE)
  if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX
	  "${OgreProcedural_BINARY_DIR}/sdk" CACHE PATH "PROCEDURAL install prefix" FORCE
    )
  endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
endif(WIN32 OR APPLE)

find_package(OGRE REQUIRED)

find_package(Boost)

if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif ()

find_package(Doxygen)

option(OgreProcedural_BUILD_SAMPLES "Build OgreProcedural samples." TRUE)
option(OgreProcedural_BUILD_TESTS "Build OgreProcedural test cases." FALSE)
option(OgreProcedural_INSTALL_DOCS "Install documentation." FALSE)
option(OgreProcedural_STATIC "Static build" TRUE)
option(OgreProcedural_ILLUSTRATIONS "Build illustrations for manual" FALSE)

# Set docs target directory
if (WIN32 OR APPLE)
  set(OgreProcedural_DOCS_PATH "docs")
elseif (UNIX)
  set(OgreProcedural_DOCS_PATH "share/OgreProcedural/docs")
endif ()

# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OgreProcedural_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OgreProcedural_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OgreProcedural_BINARY_DIR}/bin")

set(OgreProcedural_TEMPLATES_DIR "${OgreProcedural_SOURCE_DIR}/CMake/Templates")
set(OgreProcedural_WORK_DIR ${OgreProcedural_BINARY_DIR})
set(OgreProcedural_LIB_DIRECTORY "lib${LIB_SUFFIX}" CACHE STRING "Install path for libraries, e.g. 'lib64' on some 64-bit Linux distros.")

# Configure version file for use by scripts
configure_file("${OgreProcedural_TEMPLATES_DIR}/version.txt.in" "${OgreProcedural_BINARY_DIR}/version.txt" @ONLY)

include_directories("${OgreProcedural_SOURCE_DIR}/library/include")

add_subdirectory(library)

# definitions for samples
set(OgreProcedural_LIBRARIES OgreProcedural) 

# CMake macros
include(ProceduralMacros)
include(ProceduralConfigTargets)

if (OgreProcedural_BUILD_SAMPLES)	
	add_subdirectory(samples)
endif()

if (OgreProcedural_BUILD_TESTS)
	add_subdirectory(tests)
endif()

# documentation
add_subdirectory(docs)

# Install CMake modules
add_subdirectory(CMake)


# and CPack
# cpack
set(CPACK_PACKAGE_DESCRIPTION "OgreProcedural")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ogre-Procedural is an Ogre3D based lib enabling developpers to quickly create geometric primitives, such as cubes, spheres, cylinders...")
set(CPACK_PACKAGE_NAME "ogre-procedural")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "ois ogre")
set(CPACK_PACKAGE_CONTACT "support@rigsofrods.com")
set(CPACK_PACKAGE_VENDOR "support@rigsofrods.com")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/MIT.txt")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
# TODO: add the version variable at the top in here
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "2")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "OgreProcedural-${OgreProcedural_VERSION_DASH_SEPARATED}")

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
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${OgreProcedural_VERSION_DASH_SEPARATED}-${CMAKE_SYSTEM_PROCESSOR}")

# some NSIS stuff
IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  #SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  #SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} OgreProcedural")
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

if (OgreProcedural_INSTALL_DOCS)
# other doc files
set(DOC_FILES Contributors.txt MIT.txt ReadMe.txt)
install(FILES ${DOC_FILES} DESTINATION ${OgreProcedural_DOCS_PATH})
endif()