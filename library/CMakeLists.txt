cmake_minimum_required(VERSION 2.6)

project(OgreProcedural)

set( HDRS
	include/ProceduralPlatform.h
	include/Procedural.h
	include/ProceduralMeshGenerator.h
	include/ProceduralBoxGenerator.h
	include/ProceduralCapsuleGenerator.h
	include/ProceduralConeGenerator.h
	include/ProceduralCylinderGenerator.h
	include/ProceduralIcoSphereGenerator.h
	include/ProceduralPlaneGenerator.h
	include/ProceduralSphereGenerator.h
	include/ProceduralRoot.h
	include/ProceduralRoundedBoxGenerator.h
	include/ProceduralTorusGenerator.h
	include/ProceduralTorusKnotGenerator.h
	include/ProceduralTubeGenerator.h
	include/ProceduralUtils.h
	)

set( SRCS
		src/ProceduralBoxGenerator.cpp
		src/ProceduralCapsuleGenerator.cpp
		src/ProceduralConeGenerator.cpp
		src/ProceduralCylinderGenerator.cpp
		src/ProceduralIcoSphereGenerator.cpp
		src/ProceduralPlaneGenerator.cpp
		src/ProceduralRoot.cpp
		src/ProceduralRoundedBoxGenerator.cpp
		src/ProceduralSphereGenerator.cpp
		src/ProceduralTorusGenerator.cpp
		src/ProceduralTorusKnotGenerator.cpp
		src/ProceduralTubeGenerator.cpp
	)

include_directories( ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
	./include
)
add_definitions(-DPROCEDURAL_NONCLIENT_BUILD)

add_library(OgreProcedural SHARED ${HDRS} ${SRCS})

set_target_properties(OgreProcedural PROPERTIES DEBUG_POSTFIX _d)

set_target_properties(OgreProcedural PROPERTIES PREFIX "")

target_link_libraries(OgreProcedural ${OGRE_LIBRARIES} ${OIS_LIBRARIES})

install(TARGETS OgreProcedural 
		BUNDLE DESTINATION "bin"
		RUNTIME DESTINATION "bin"
		LIBRARY DESTINATION "lib"		
		ARCHIVE DESTINATION "lib"		
		FRAMEWORK DESTINATION "lib")
		
install(FILES ${HDRS} DESTINATION include)



#set_target_properties(OgreProcedural PROPERTIES
# 	ARCHIVE_OUTPUT_DIRECTORY ${PROCEDURAL_BINARY_DIR}/lib
#    LIBRARY_OUTPUT_DIRECTORY ${PROCEDURAL_BINARY_DIR}/lib
#    RUNTIME_OUTPUT_DIRECTORY ${PROCEDURAL_BINARY_DIR}/bin
#  )
#install(TARGETS OgreProcedural
	#BUNDLE DESTINATION ${PROCEDURAL_BINARY_DIR}/bin CONFIGURATIONS All
	#RUNTIME DESTINATION ${PROCEDURAL_BINARY_DIR}/bin CONFIGURATIONS All
	#LIBRARY DESTINATION ${PROCEDURAL_BINARY_DIR}/bin CONFIGURATIONS All
	#ARCHIVE DESTINATION ${PROCEDURAL_BINARY_DIR}/bin CONFIGURATIONS All
	#FRAMEWORK DESTINATION ${PROCEDURAL_BINARY_DIR}/bin CONFIGURATIONS All
	#)
