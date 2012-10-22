###############################################################
# Install documentation
# Build API docs, if doxygen is available
###############################################################

if(PYTHONINTERP_FOUND OR DOXYGEN_FOUND)
	add_subdirectory(manual/illustrations)
endif()

# Build and install API documentation if doxygen is available
if(DOXYGEN_FOUND)
	configure_file(${OgreProcedural_TEMPLATES_DIR}/html.cfg.in ${OgreProcedural_BINARY_DIR}/html.cfg)
	add_custom_target(doc
					COMMAND  ${DOXYGEN_EXECUTABLE} ${OgreProcedural_BINARY_DIR}/html.cfg
					WORKING_DIRECTORY ${OgreProcedural_SOURCE_DIR}/docs
					DEPENDS Illustrations
					COMMENT "Building documentation" VERBATIM)
	file(WRITE ${OgreProcedural_SOURCE_DIR}/docs/api/temp.txt "This file should be deleted before installation. It is only here to force the creation of the api folder")
	file(REMOVE ${OgreProcedural_SOURCE_DIR}/docs/api/temp.txt)
	if (OgreProcedural_INSTALL_DOCS)
		install(DIRECTORY ${OgreProcedural_BINARY_DIR}/docs/api DESTINATION ${OgreProcedural_DOCS_PATH})
	endif()

	# cleanup $OgreProcedural_DOCS_PATH on "make clean"
	set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${OgreProcedural_DOCS_PATH})

	# add doxygen as dependency to doc-target
	get_target_property(DOC_TARGET doc TYPE)
	if(NOT DOC_TARGET)
		add_custom_target(doc)
	endif()
endif()

if(PYTHONINTERP_FOUND)
	file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/manual/temp.txt "This file should be deleted before installation. It is only here to force the creation of the namual folder")
	file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/manual/temp.txt)
	configure_file(${OgreProcedural_TEMPLATES_DIR}/a2x.cfg.in ${OgreProcedural_BINARY_DIR}/a2x.cfg)
	add_custom_target(manual
					COMMAND ${PYTHON_EXECUTABLE} "${ASCIIDOC_DIR}/a2x.py" -L -v --format=chunked --conf-file="${OgreProcedural_BINARY_DIR}/a2x.cfg" --destination-dir="${CMAKE_CURRENT_BINARY_DIR}/manual" --resource="${OgreProcedural_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/manual/manual.txt"
					WORKING_DIRECTORY "${ASCIIDOC_DIR}"
					DEPENDS manual/manual.txt Illustrations
					COMMENT "Building manual")
	if (OgreProcedural_INSTALL_DOCS)
		install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/manual DESTINATION ${OgreProcedural_DOCS_PATH})
	endif()
endif()