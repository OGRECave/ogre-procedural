cmake_minimum_required(VERSION 2.6)

project(Sample_SVG)

set( HDRS
		../common/include/BaseApplication.h
		include/Svg.h
	)

set( SRCS
		../common/src/BaseApplication.cpp
		src/Svg.cpp
	)

include_directories( ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	../common/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(Sample_SVG WIN32 ${HDRS} ${SRCS})

set_target_properties(Sample_SVG PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(Sample_SVG ${OgreProcedural_LIBRARIES} ${OGRE_LIBRARIES} ${OIS_LIBRARIES})

procedural_create_vcproj_userfile(Sample_SVG)