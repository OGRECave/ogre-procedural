cmake_minimum_required(VERSION 2.6)

project(Sample_OSM)

set( HDRS
		../common/include/BaseApplication.h
		OSM.h
	)

set( SRCS
		../common/src/BaseApplication.cpp
		OSM.cpp
	)

include_directories( ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
	../common/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(Sample_OSM WIN32 ${HDRS} ${SRCS})

set_target_properties(Sample_OSM PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(Sample_OSM ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${PROCEDURAL_LIBRARIES})

procedural_create_vcproj_userfile(Sample_OSM)

