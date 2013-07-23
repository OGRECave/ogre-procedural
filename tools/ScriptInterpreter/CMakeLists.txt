find_package(Lua REQUIRED)

if (NOT DEFINED SWIG_DIR)
	message(SEND_ERROR "Please define a variable SWIG_DIR, pointing to SWIG's root directory")
endif()

set( HDRS
		../../samples/common/include/BaseApplication.h
		include/ScriptInterpreter.h
	)

set( SRCS
		../../samples/common/src/BaseApplication.cpp
		src/ScriptInterpreter.cpp
	)
set( SRCS2
		"${CMAKE_CURRENT_SOURCE_DIR}/procedural_wrap.cxx"
	)
	

set( EXTRA
	procedural.i
	ogre.i
	)

include_directories( ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	../../samples/common/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${LUA_INCLUDE_DIR}
)

add_custom_command(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/procedural_wrap.cxx"
                     COMMAND ${SWIG_DIR}/swig ARGS -c++ -lua -v -I"${OGRE_INCLUDE_DIR}" -I"${CMAKE_CURRENT_SOURCE_DIR}/../../library/include" -I"${CMAKE_CURRENT_SOURCE_DIR}/include" procedural.i
                     WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
					 DEPENDS procedural.i ogre.i)

procedural_add_executable(ScriptInterpreter ${HDRS} ${SRCS} ${EXTRA} SEPARATE ${SRCS2})

set_target_properties(ScriptInterpreter PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(ScriptInterpreter ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${OgreProcedural_LIBRARIES} ${LUA_LIBRARIES})

if ((${OGRE_VERSION} VERSION_EQUAL "1.9.0") OR (${OGRE_VERSION} VERSION_GREATER "1.9.0"))
	include_directories(${OGRE_Overlay_INCLUDE_DIR})
	target_link_libraries(ScriptInterpreter ${OGRE_Overlay_LIBRARIES})
endif()

procedural_create_vcproj_userfile(ScriptInterpreter)