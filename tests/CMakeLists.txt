function(procedural_create_vcproj_userfile TARGETNAME)
  if (MSVC)
	if (${MSVC_VERSION} EQUAL 1600)
	   configure_file(
	  ${PROCEDURAL_TEMPLATES_DIR}/VisualStudioUserFile.vcxproj.user.in
	  ${CMAKE_CURRENT_BINARY_DIR}/${TARGETNAME}.vcxproj.user
	  @ONLY
	)
	else()
	  configure_file(
	  ${PROCEDURAL_TEMPLATES_DIR}/VisualStudioUserFile.vcproj.user.in
	  ${CMAKE_CURRENT_BINARY_DIR}/${TARGETNAME}.vcproj.user
	  @ONLY
	)
	endif()
  endif ()
 endfunction(procedural_create_vcproj_userfile)
  
macro(copy_release_dir INPUT INPUTDIR)
  if (EXISTS ${INPUTDIR}/${INPUT})
    if (MINGW OR NMAKE)
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INPUT} COPYONLY)
	else ()
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/release/${INPUT} COPYONLY)
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/relwithdebinfo/${INPUT} COPYONLY)
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/minsizerel/${INPUT} COPYONLY)
	endif ()
  endif ()
endmacro ()

macro(copy_release INPUT)
	copy_release_dir(${INPUT} ${OGRE_PLUGIN_DIR_REL})
endmacro()

macro(configure_release_dir INPUT INPUTDIR)
  if (EXISTS ${INPUTDIR}/${INPUT})
    if (MINGW OR NMAKE OR UNIX)
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INPUT} @ONLY)
	else ()
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/release/${INPUT} @ONLY)
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/relwithdebinfo/${INPUT} @ONLY)
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/minsizerel/${INPUT} @ONLY)
	endif ()
  endif ()
endmacro ()

macro(copy_release_debug INPUT INPUTDIR)
  if (EXISTS ${INPUTDIR}/${INPUT})
    if (MINGW OR NMAKE OR UNIX)
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INPUT} COPYONLY)
	else ()
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/release/${INPUT} COPYONLY)
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/relwithdebinfo/${INPUT} COPYONLY)
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/minsizerel/${INPUT} COPYONLY)
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug/${INPUT} COPYONLY)
	endif ()
 endif ()
endmacro ()

macro(copy_debug_dir INPUT INPUTDIR)
  if (EXISTS ${INPUTDIR}/${INPUT})
    if (MINGW OR NMAKE)
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INPUT} COPYONLY)
	else ()
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug/${INPUT} COPYONLY)
	endif ()
  endif ()
endmacro()

macro(configure_debug_dir INPUT INPUTDIR)
  if (EXISTS ${INPUTDIR}/${INPUT})
    if (MINGW OR NMAKE OR UNIX)
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INPUT} @ONLY)
	else ()
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug/${INPUT} @ONLY)
	endif ()
  endif ()
endmacro()

macro(copy_debug INPUT)
	copy_debug_dir(${INPUT} ${OGRE_PLUGIN_DIR_DBG})
endmacro()


find_package(OIS REQUIRED)

if(NOT OIS_FOUND)
	message(SEND_ERROR "Failed to find OIS.")
endif()

  
# if win32, copy the Ogre DLLs and media over
if(WIN32)
	copy_release(OgreMain.dll)	
	copy_debug(OgreMain_d.dll)
	
	copy_release(RenderSystem_Direct3D9.dll)	
	copy_debug(RenderSystem_Direct3D9_d.dll)
		
	copy_release(RenderSystem_GL.dll)
	copy_debug(RenderSystem_GL_d.dll)
		
	copy_release(OIS.dll)
	copy_debug(OIS_d.dll)	
	
	set(PROCEDURAL_PLUGIN_DIR_REL ".")
	set(PROCEDURAL_PLUGIN_DIR_DBG ".")

	set(OGRE_PLUGIN_DIR_REL ".")
	set(OGRE_PLUGIN_DIR_DBG ".")
	
endif(WIN32)

# if Linux, copy the Ogre configuration files over
if(UNIX AND NOT APPLE)

	set(PROCEDURAL_PLUGIN_DIR_REL ".")
	set(PROCEDURAL_PLUGIN_DIR_DBG ".")
	set(PROCEDURAL_COMMENT_RENDERSYSTEM_D3D9 "#")
	
	#comes from findOgre.cmake (don't know why it doesn't work there...)
	get_filename_component(OGRE_PLUGIN_DIR_REL ${OGRE_RenderSystem_GL_LIBRARY_REL} PATH)
	get_filename_component(OGRE_PLUGIN_DIR_DBG ${OGRE_RenderSystem_GL_LIBRARY_DBG} PATH)		

endif(UNIX AND NOT APPLE)

copy_release_debug(SdkTrays.zip ${PROCEDURAL_SOURCE_DIR}/samples/media)
copy_release_debug(old_fence.jpg ${PROCEDURAL_SOURCE_DIR}/samples/media)
copy_release_debug(red_brick.jpg ${PROCEDURAL_SOURCE_DIR}/samples/media)
copy_release_debug(Examples.material ${PROCEDURAL_SOURCE_DIR}/samples/media)

configure_release_dir(resources.cfg ${PROCEDURAL_TEMPLATES_DIR})
configure_debug_dir(resources_d.cfg ${PROCEDURAL_TEMPLATES_DIR})

configure_release_dir(plugins.cfg ${PROCEDURAL_TEMPLATES_DIR})
configure_debug_dir(plugins_d.cfg ${PROCEDURAL_TEMPLATES_DIR})

project(Unit_Tests)

set( HDRS
		../samples/common/include/BaseApplication.h
		include/Tests.h
	)

set( SRCS
		../samples/common/src/BaseApplication.cpp
		src/Tests.cpp
	)

include_directories( ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
	../library/include
	../samples/common/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(Unit_Tests WIN32 ${HDRS} ${SRCS})

set_target_properties(Unit_Tests PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(Unit_Tests ${OGRE_LIBRARIES} ${OIS_LIBRARIES} OgreProcedural)

procedural_create_vcproj_userfile(Unit_Tests)
