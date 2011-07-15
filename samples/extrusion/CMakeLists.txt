cmake_minimum_required(VERSION 2.6)

project(Sample_Extrusion)

set( HDRS
		../common/include/BaseApplication.h
		include/Extrusion.h
	)

set( SRCS
		../common/src/BaseApplication.cpp
		src/Extrusion.cpp
	)

include_directories( ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
	../common/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(Sample_Extrusion WIN32 ${HDRS} ${SRCS})

set_target_properties(Sample_Extrusion PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(Sample_Extrusion ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${PROCEDURAL_LIBRARIES})

procedural_create_vcproj_userfile(Sample_Extrusion)