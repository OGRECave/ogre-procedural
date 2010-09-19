cmake_minimum_required(VERSION 2.6)

project(Sample_Primitives)

set( HDRS
	include/Main.h
	)

set( SRCS
		src/Main.cpp
	)

if (WIN32)
  list(APPEND SRCS ${OGRE_DIR}/Samples/browser/src/FileSystemLayerImpl_WIN32.cpp)
elseif (OGRE_BUILD_PLATFORM_IPHONE)
  list(APPEND SRCS ${OGRE_DIR}/Samples/browser/src/FileSystemLayerImpl_iPhone.mm)
elseif (APPLE)
  list(APPEND SRCS ${OGRE_DIR}/Samples/browser/src/FileSystemLayerImpl_OSX.cpp)
elseif (UNIX)
  list(APPEND SRCS ${OGRE_DIR}/Samples/browser/src/FileSystemLayerImpl_Unix.cpp)
else ()
  list(APPEND SRCS ${OGRE_DIR}/Samples/browser/src/FileSystemLayerImpl_Default.cpp)
endif ()

include_directories( ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
	../../library/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(Sample_Primitives WIN32 ${HDRS} ${SRCS})

set_target_properties(Sample_Primitives PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(Sample_Primitives ${OGRE_LIBRARIES} ${OIS_LIBRARIES} OgreProcedural)

procedural_create_vcproj_userfile(Sample_Primitives)