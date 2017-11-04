project(OgreProceduralIllustrations)

set( HDRS
		${CMAKE_SOURCE_DIR}/library/include/ProceduralStableHeaders.h
		include/Illustrations.h
	)

set( SRCS
		${CMAKE_SOURCE_DIR}/library/src/ProceduralPrecompiledHeaders.cpp
		src/Illustrations.cpp
	)

include_directories(
	${OGRE_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(FREETYPE_FOUND)
  if(OgreProcedural_STATIC)
    target_link_libraries(OgreProcedural ${FREETYPE_LIBRARIES})
  endif()
endif ()

procedural_add_executable(OgreProceduralIllustrations WIN32 ${HDRS} ${SRCS})

file(MAKE_DIRECTORY ${OgreProcedural_ILLUSTRATIONS_PATH})

set_target_properties(OgreProceduralIllustrations PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(OgreProceduralIllustrations ${OGRE_LIBRARIES} ${OgreProcedural_LIBRARIES})

procedural_create_vcproj_userfile(OgreProceduralIllustrations)

# specify a precompiled header to use
use_precompiled_header(OgreProceduralIllustrations 
  "${CMAKE_SOURCE_DIR}/library/include/ProceduralStableHeaders.h"
  "${CMAKE_SOURCE_DIR}/library/src/ProceduralPrecompiledHeaders.cpp"
)

add_custom_command(TARGET OgreProceduralIllustrations POST_BUILD 
	COMMAND OgreProceduralIllustrations ARGS "${OgreProcedural_ILLUSTRATIONS_PATH}" WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
