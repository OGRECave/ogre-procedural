cmake_minimum_required (VERSION 2.6)
project(OGREPROCEDURAL)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}	
	"$ENV{OGRE_HOME}/CMake"
	)
	
find_package(OGRE)
find_package(OIS)

MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${Sources}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledBinary}\" /FI\"${PrecompiledBinary}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")  
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)

set( HEADER_FILES
	include/SphereGenerator.h
	include/Common.h
	include/Main.h
	include/Precompiled.h
	)
	
set( SOURCE_FILES
		src/Main.cpp
		src/SphereGenerator.cpp
	)
	
ADD_MSVC_PRECOMPILED_HEADER("Precompiled.h" "src/Precompiled.cpp" SOURCE_FILES)
	
INCLUDE_DIRECTORIES("${OGREPROCEDURAL_SOURCE_DIR}/include"
					${OGRE_INCLUDE_DIRS}
					${OIS_INCLUDE_DIRS}
					"$ENV{OGRE_HOME}/boost_1_42")
					
LINK_DIRECTORIES("$ENV{OGRE_HOME}/boost_1_42/lib")

LINK_LIBRARIES(${OGRE_LIBRARIES} ${OIS_LIBRARIES})
					
ADD_EXECUTABLE(OGREPROCEDURAL WIN32 ${SOURCE_FILES} ${HEADER_FILES})