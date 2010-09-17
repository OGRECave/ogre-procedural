# if win32, copy the Ogre DLLs and the media over
if(WIN32)

	install(DIRECTORY ${OGRE_MEDIA_DIR}
		DESTINATION ../
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
		DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		CONFIGURATIONS Release RelWithDebInfo
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
		DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
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