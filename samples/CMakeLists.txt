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

macro(copy_debug_dir INPUT INPUTDIR)
  if (EXISTS ${INPUTDIR}/${INPUT})
    if (MINGW OR NMAKE)
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INPUT} COPYONLY)
	else ()
      configure_file(${INPUTDIR}/${INPUT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug/${INPUT} COPYONLY)
	endif ()
  endif ()
endmacro()

macro(copy_debug INPUT)
	copy_debug_dir(${INPUT} ${OGRE_PLUGIN_DIR_DBG})
endmacro()
  
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
	
	copy_release_dir(resources.cfg ${PROCEDURAL_TEMPLATES_DIR})
	copy_debug_dir(resources_d.cfg ${PROCEDURAL_TEMPLATES_DIR})
	
	copy_release_dir(plugins.cfg ${PROCEDURAL_TEMPLATES_DIR})
	copy_debug_dir(plugins_d.cfg ${PROCEDURAL_TEMPLATES_DIR})
	
	copy_release_dir(SdkTrays.zip ${PROCEDURAL_SOURCE_DIR}/samples/media)
	copy_debug_dir(SdkTrays.zip ${PROCEDURAL_SOURCE_DIR}/samples/media)
	
	copy_release_dir(old_fence.jpg ${PROCEDURAL_SOURCE_DIR}/samples/media)
	copy_debug_dir(old_fence.jpg ${PROCEDURAL_SOURCE_DIR}/samples/media)
	
	copy_release_dir(red_brick.jpg ${PROCEDURAL_SOURCE_DIR}/samples/media)
	copy_debug_dir(red_brick.jpg ${PROCEDURAL_SOURCE_DIR}/samples/media)
	
	copy_release_dir(Examples.material ${PROCEDURAL_SOURCE_DIR}/samples/media)
	copy_debug_dir(Examples.material ${PROCEDURAL_SOURCE_DIR}/samples/media)
	
	
	#configure_file(${PROCEDURAL_TEMPLATES_DIR}/plugins.cfg.in ${CMAKE_CURRENT_BINARY_DIR}/plugins.cfg COPYONLY)
	#configure_file(${PROCEDURAL_TEMPLATES_DIR}/plugins_d.cfg.in ${CMAKE_CURRENT_BINARY_DIR}/plugins_d.cfg COPYONLY)
	
endif(WIN32)

# if Linux, copy the Ogre configuration files over
if(UNIX AND NOT APPLE)
	configure_file(/usr/local/share/OGRE/plugins.cfg bin COPYONLY)
	configure_file(/usr/local/share/OGRE/resources.cfg bin COPYONLY)	
endif(UNIX AND NOT APPLE)


add_subdirectory(primitives)