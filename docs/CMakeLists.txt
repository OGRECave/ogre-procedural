###############################################################
# Install documentation
# Build API docs, if doxygen is available
###############################################################

# Build and install API documentation if doxygen is available
if(DOXYGEN_FOUND)
	configure_file(${PROCEDURAL_TEMPLATES_DIR}/html.cfg.in ${PROCEDURAL_BINARY_DIR}/html.cfg)
	add_custom_target(doc COMMAND  ${DOXYGEN_EXECUTABLE} ${PROCEDURAL_BINARY_DIR}/html.cfg  WORKING_DIRECTORY ${PROCEDURAL_SOURCE_DIR}/docs COMMENT "Building documentation" VERBATIM)
	file(WRITE ${PROCEDURAL_SOURCE_DIR}/docs/api/temp.txt "This file should be deleted before installation. It is only here to force the creation of the api folder")
	file(REMOVE ${PROCEDURAL_SOURCE_DIR}/docs/api/temp.txt)
	if (PROCEDURAL_INSTALL_DOCS)
		install(DIRECTORY ${PROCEDURAL_BINARY_DIR}/docs/api DESTINATION ${PROCEDURAL_DOCS_PATH})
	endif()

	# cleanup $PROCEDURAL_DOCS_PATH on "make clean"
	set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${PROCEDURAL_DOCS_PATH})

	# add doxygen as dependency to doc-target
	get_target_property(DOC_TARGET doc TYPE)
	if(NOT DOC_TARGET)
		add_custom_target(doc)
	endif()
	#add_dependencies(doc doxygen)
endif()
