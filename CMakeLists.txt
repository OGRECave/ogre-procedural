cmake_minimum_required(VERSION 2.6)

project(OgreProcedural)

if(WIN32)
	set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
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

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")

find_package(OGRE REQUIRED)

if(NOT "${OGRE_VERSION_NAME}" STREQUAL "Cthugha")
  message(SEND_ERROR "You need Ogre 1.7 Cthugha to build this.")
endif()

find_package(OIS REQUIRED)

if(NOT OIS_FOUND)
    message(SEND_ERROR "Failed to find OIS.")
endif()



set( HDRS
	include/Procedural.h
	include/ProceduralMeshGenerator.h
	include/ProceduralBoxGenerator.h
	include/ProceduralConeGenerator.h
	include/ProceduralCylinderGenerator.h
	include/ProceduralPlaneGenerator.h
	include/ProceduralSphereGenerator.h
	include/ProceduralRoot.h
	include/ProceduralTorusGenerator.h
	include/ProceduralTubeGenerator.h
	include/ProceduralUtils.h
	include/Common.h
	include/Main.h
	include/Precompiled.h
	)

set( SRCS
		src/Main.cpp
		src/ProceduralBoxGenerator.cpp
		src/ProceduralConeGenerator.cpp
		src/ProceduralCylinderGenerator.cpp
		src/ProceduralPlaneGenerator.cpp
		src/ProceduralRoot.cpp
		src/ProceduralSphereGenerator.cpp
		src/ProceduralTorusGenerator.cpp
		src/ProceduralTubeGenerator.cpp
	)

if (WIN32)
  list(APPEND SRCS $ENV{OGRE_HOME}/Samples/browser/src/FileSystemLayerImpl_WIN32.cpp)
elseif (OGRE_BUILD_PLATFORM_IPHONE)
  list(APPEND SRCS $ENV{OGRE_HOME}/Samples/browser/src/FileSystemLayerImpl_iPhone.mm)
elseif (APPLE)
  list(APPEND SRCS $ENV{OGRE_HOME}/Samples/browser/src/FileSystemLayerImpl_OSX.cpp)
elseif (UNIX)
  list(APPEND SRCS $ENV{OGRE_HOME}/Samples/browser/src/FileSystemLayerImpl_Unix.cpp)
else ()
  list(APPEND SRCS $ENV{OGRE_HOME}/Samples/browser/src/FileSystemLayerImpl_Default.cpp)
endif ()

include_directories( ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
	./include
)

add_executable(OgreProcedural WIN32 ${HDRS} ${SRCS})

set_target_properties(OgreProcedural PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(OgreProcedural ${OGRE_LIBRARIES} ${OIS_LIBRARIES})

# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command( TARGET OgreProcedural PRE_BUILD
		COMMAND if not exist .\\dist\\bin mkdir .\\dist\\bin )
	add_custom_command( TARGET OgreProcedural POST_BUILD
		COMMAND copy \"$(TargetPath)\" .\\dist\\bin )
endif(WIN32 AND NOT MINGW)

# set the executable output path for Unix and MinGW instead of post-build copy
if(MINGW OR UNIX)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)

# if win32, copy the Ogre DLLs and the media over
if(WIN32)
	install(TARGETS OgreProcedural
		RUNTIME DESTINATION bin
		CONFIGURATIONS All)

	install(DIRECTORY $ENV{OGRE_HOME}/media
		DESTINATION ../
	)

	install(FILES $ENV{OGRE_HOME}/bin/RelWithDebInfo/OgreMain.dll
		$ENV{OGRE_HOME}/bin/RelWithDebInfo/RenderSystem_Direct3D9.dll
		$ENV{OGRE_HOME}/bin/RelWithDebInfo/RenderSystem_GL.dll
		$ENV{OGRE_HOME}/bin/RelWithDebInfo/Plugin_OctreeSceneManager.dll
		$ENV{OGRE_HOME}/bin/RelWithDebInfo/cg.dll
		$ENV{OGRE_HOME}/bin/RelWithDebInfo/OIS.dll
		$ENV{OGRE_HOME}/bin/RelWithDebInfo/Plugin_CgProgramManager.dll
		$ENV{OGRE_HOME}/bin/RelWithDebInfo/Plugin_ParticleFX.dll
		$ENV{OGRE_HOME}/bin/RelWithDebInfo/Plugin_BSPSceneManager.dll
		$ENV{OGRE_HOME}/bin/RelWithDebInfo/Plugin_CgProgramManager.dll
		$ENV{OGRE_HOME}/bin/RelWithDebInfo/Plugin_PCZSceneManager.dll
		$ENV{OGRE_HOME}/bin/RelWithDebInfo/Plugin_OctreeZone.dll
		$ENV{OGRE_HOME}/bin/RelWithDebInfo/resources.cfg
		$ENV{OGRE_HOME}/bin/RelWithDebInfo/plugins.cfg
		DESTINATION bin
		CONFIGURATIONS Release RelWithDebInfo
	)

	install(FILES $ENV{OGRE_HOME}/bin/debug/OgreMain_d.dll
		$ENV{OGRE_HOME}/bin/debug/RenderSystem_Direct3D9_d.dll
		$ENV{OGRE_HOME}/bin/debug/RenderSystem_GL_d.dll
		$ENV{OGRE_HOME}/bin/debug/Plugin_OctreeSceneManager_d.dll
		$ENV{OGRE_HOME}/bin/debug/cg.dll
		$ENV{OGRE_HOME}/bin/debug/OIS_d.dll
		$ENV{OGRE_HOME}/bin/debug/Plugin_CgProgramManager_d.dll
		$ENV{OGRE_HOME}/bin/debug/Plugin_ParticleFX_d.dll
		$ENV{OGRE_HOME}/bin/debug/Plugin_BSPSceneManager_d.dll
		$ENV{OGRE_HOME}/bin/debug/Plugin_CgProgramManager_d.dll
		$ENV{OGRE_HOME}/bin/debug/Plugin_PCZSceneManager_d.dll
		$ENV{OGRE_HOME}/bin/debug/Plugin_OctreeZone_d.dll
		$ENV{OGRE_HOME}/bin/debug/resources_d.cfg
		$ENV{OGRE_HOME}/bin/debug/plugins_d.cfg
		DESTINATION bin
		CONFIGURATIONS Debug
	)
endif(WIN32)

# if Linux, copy the Ogre configuration files over
if(UNIX AND NOT APPLE)
	install(FILES /usr/local/share/OGRE/plugins.cfg
		/usr/local/share/OGRE/resources.cfg
		DESTINATION bin
		CONFIGURATIONS Release RelWithDebInfo Debug
	)
endif(UNIX AND NOT APPLE)
