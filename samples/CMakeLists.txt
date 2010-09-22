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
  
# if win32, copy the Ogre DLLs and the media over
if(WIN32)

	if (MSVC)	
		install(DIRECTORY ${OGRE_MEDIA_DIR}
			DESTINATION .
		)
		set(PROCEDURAL_RUNTIME_INSTALL_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
		set(PROCEDURAL_RUNTIME_INSTALL_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo)
		set(PROCEDURAL_RUNTIME_INSTALL_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
	else()
		install(DIRECTORY ${OGRE_MEDIA_DIR}
			DESTINATION ../
		)
		set(PROCEDURAL_RUNTIME_INSTALL_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
		set(PROCEDURAL_RUNTIME_INSTALL_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
		set(PROCEDURAL_RUNTIME_INSTALL_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	endif()
		
	install(FILES ${OGRE_BINARY_REL}
		${OGRE_PLUGIN_DIR_REL}/RenderSystem_Direct3D9.dll
		${OGRE_PLUGIN_DIR_REL}/RenderSystem_GL.dll
		${OGRE_PLUGIN_DIR_REL}/Plugin_OctreeSceneManager.dll
		${OGRE_PLUGIN_DIR_REL}/cg.dll
		${OGRE_PLUGIN_DIR_REL}/OIS.dll
		${OGRE_PLUGIN_DIR_REL}/Plugin_CgProgramManager.dll
		${OGRE_PLUGIN_DIR_REL}/Plugin_ParticleFX.dll
		${OGRE_PLUGIN_DIR_REL}/Plugin_BSPSceneManager.dll
		${OGRE_PLUGIN_DIR_REL}/Plugin_CgProgramManager.dll
		${OGRE_PLUGIN_DIR_REL}/Plugin_PCZSceneManager.dll
		${OGRE_PLUGIN_DIR_REL}/Plugin_OctreeZone.dll
		${OGRE_PLUGIN_DIR_REL}/resources.cfg
		${OGRE_PLUGIN_DIR_REL}/plugins.cfg
		DESTINATION ${PROCEDURAL_RUNTIME_INSTALL_RELEASE}
		CONFIGURATIONS Release RelWithDebInfo
	)
	
	install(FILES ${OGRE_BINARY_REL}
		${OGRE_PLUGIN_DIR_REL}/RenderSystem_Direct3D9.dll
		${OGRE_PLUGIN_DIR_REL}/RenderSystem_GL.dll
		${OGRE_PLUGIN_DIR_REL}/Plugin_OctreeSceneManager.dll
		${OGRE_PLUGIN_DIR_REL}/cg.dll
		${OGRE_PLUGIN_DIR_REL}/OIS.dll
		${OGRE_PLUGIN_DIR_REL}/Plugin_CgProgramManager.dll
		${OGRE_PLUGIN_DIR_REL}/Plugin_ParticleFX.dll
		${OGRE_PLUGIN_DIR_REL}/Plugin_BSPSceneManager.dll
		${OGRE_PLUGIN_DIR_REL}/Plugin_CgProgramManager.dll
		${OGRE_PLUGIN_DIR_REL}/Plugin_PCZSceneManager.dll
		${OGRE_PLUGIN_DIR_REL}/Plugin_OctreeZone.dll
		${OGRE_PLUGIN_DIR_REL}/resources.cfg
		${OGRE_PLUGIN_DIR_REL}/plugins.cfg
		DESTINATION ${PROCEDURAL_RUNTIME_INSTALL_RELWITHDEBINFO}
		CONFIGURATIONS RelWithDebInfo
	)

	install(FILES ${OGRE_BINARY_REL}
		${OGRE_PLUGIN_DIR_DBG}/RenderSystem_Direct3D9_d.dll
		${OGRE_PLUGIN_DIR_DBG}/RenderSystem_GL_d.dll
		${OGRE_PLUGIN_DIR_DBG}/Plugin_OctreeSceneManager_d.dll
		${OGRE_PLUGIN_DIR_DBG}/cg.dll
		${OGRE_PLUGIN_DIR_DBG}/OIS_d.dll
		${OGRE_PLUGIN_DIR_DBG}/Plugin_CgProgramManager_d.dll
		${OGRE_PLUGIN_DIR_DBG}/Plugin_ParticleFX_d.dll
		${OGRE_PLUGIN_DIR_DBG}/Plugin_BSPSceneManager_d.dll
		${OGRE_PLUGIN_DIR_DBG}/Plugin_CgProgramManager_d.dll
		${OGRE_PLUGIN_DIR_DBG}/Plugin_PCZSceneManager_d.dll
		${OGRE_PLUGIN_DIR_DBG}/Plugin_OctreeZone_d.dll
		${OGRE_PLUGIN_DIR_DBG}/resources_d.cfg
		${OGRE_PLUGIN_DIR_DBG}/plugins_d.cfg
		DESTINATION ${PROCEDURAL_RUNTIME_INSTALL_DEBUG}
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


add_subdirectory(primitives)