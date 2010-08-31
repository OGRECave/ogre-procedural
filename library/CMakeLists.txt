cmake_minimum_required(VERSION 2.6)

project(OgreProcedural)

set( HDRS
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

add_library(OgreProcedural ${HDRS} ${SRCS})

set_target_properties(OgreProcedural PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(OgreProcedural ${OGRE_LIBRARIES} ${OIS_LIBRARIES})