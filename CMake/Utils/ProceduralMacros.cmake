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

macro (prepare_demo_dirs)
	find_package(OIS REQUIRED)

if(NOT OIS_FOUND)
	message(SEND_ERROR "Failed to find OIS.")
endif()
  
# if win32, copy the Ogre DLLs over
if(WIN32)
	copy_release(OgreMain.dll)	
	copy_debug(OgreMain_d.dll)
	copy_release(OgreRTShaderSystem.dll)	
	copy_debug(OgreRTShaderSystem_d.dll)
	
	copy_release(RenderSystem_Direct3D9.dll)	
	copy_debug(RenderSystem_Direct3D9_d.dll)
		
	copy_release(RenderSystem_GL.dll)
	copy_debug(RenderSystem_GL_d.dll)
	
	copy_release(libOIS.dll)
	copy_debug(libOIS_d.dll)	
	copy_release(OIS.dll)
	copy_debug(OIS_d.dll)		
	
	#MingW doesn't want to run samples if boost dlls are not present (but MSVC is ok with that..)
	FILE(GLOB BOOST_DEB_DLLS "${OGRE_PLUGIN_DIR_DBG}/*boost*.dll")
	foreach(DLL ${BOOST_DEB_DLLS})
		GET_FILENAME_COMPONENT(SHORT_DLL ${DLL} NAME)
		copy_debug(${SHORT_DLL})
	endforeach(DLL)
	FILE(GLOB BOOST_REL_DLLS "${OGRE_PLUGIN_DIR_REL}/*boost*.dll")
	foreach(DLL ${BOOST_REL_DLLS})
		GET_FILENAME_COMPONENT(SHORT_DLL ${DLL} NAME)
		copy_release(${SHORT_DLL})
	endforeach(DLL)
		
	set(OgreProcedural_PLUGIN_DIR_REL ".")
	set(OgreProcedural_PLUGIN_DIR_DBG ".")

	set(OGRE_PLUGIN_DIR_REL ".")
	set(OGRE_PLUGIN_DIR_DBG ".")
	
	
endif(WIN32)

# if Linux, copy the Ogre configuration files over
if(UNIX AND NOT APPLE)

	set(OgreProcedural_PLUGIN_DIR_REL ".")
	set(OgreProcedural_PLUGIN_DIR_DBG ".")
	set(OgreProcedural_COMMENT_RENDERSYSTEM_D3D9 "#")
	
	#comes from findOgre.cmake (don't know why it doesn't work there...)
	get_filename_component(OGRE_PLUGIN_DIR_REL ${OGRE_RenderSystem_GL_LIBRARY_REL} PATH)
	get_filename_component(OGRE_PLUGIN_DIR_DBG ${OGRE_RenderSystem_GL_LIBRARY_DBG} PATH)		

endif(UNIX AND NOT APPLE)

set(OgreProcedural_MEDIA_DIR "${OgreProcedural_SOURCE_DIR}/samples/media")

configure_release_dir(resources.cfg ${OgreProcedural_TEMPLATES_DIR})
configure_debug_dir(resources_d.cfg ${OgreProcedural_TEMPLATES_DIR})

configure_release_dir(plugins.cfg ${OgreProcedural_TEMPLATES_DIR})
configure_debug_dir(plugins_d.cfg ${OgreProcedural_TEMPLATES_DIR})
endmacro()