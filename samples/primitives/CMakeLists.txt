cmake_minimum_required(VERSION 2.6)

project(Sample_Primitives)

set( HDRS
	include/Main.h
	)

set( SRCS
		src/Main.cpp
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
	../../library/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(Sample_Primitives WIN32 ${HDRS} ${SRCS})

set_target_properties(Sample_Primitives PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(Sample_Primitives ${OGRE_LIBRARIES} ${OIS_LIBRARIES} OgreProcedural)

# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command( TARGET Sample_Primitives PRE_BUILD
		COMMAND if not exist .\\bin mkdir .\\bin )
	add_custom_command( TARGET Sample_Primitives POST_BUILD
		COMMAND copy \"$(TargetPath)\" .\\bin )
endif(WIN32 AND NOT MINGW)

# set the executable output path for Unix and MinGW instead of post-build copy
if(MINGW OR UNIX)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
endif(MINGW OR UNIX)

# if win32, copy the Ogre DLLs and the media over
if(WIN32)
	install(TARGETS Sample_Primitives
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
