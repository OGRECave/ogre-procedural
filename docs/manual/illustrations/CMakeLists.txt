project(Illustrations)

set( HDRS
		../../../library/include/ProceduralStableHeaders.h
		include/Illustrations.h
	)

set( SRCS
		../../../library/src/ProceduralPrecompiledHeaders.cpp
		src/Illustrations.cpp
	)

include_directories( ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

procedural_add_executable(Illustrations WIN32 ${HDRS} ${SRCS})

set_target_properties(Illustrations PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(Illustrations ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${OgreProcedural_LIBRARIES})

procedural_create_vcproj_userfile(Illustrations)

# specify a precompiled header to use
use_precompiled_header(Illustrations 
  "../../../library/include/ProceduralStableHeaders.h"
  "../../../library/src/ProceduralPrecompiledHeaders.cpp"
)