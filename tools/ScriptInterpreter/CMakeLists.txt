find_package(Lua REQUIRED)
find_package(SWIG REQUIRED)

set( HDRS
		../../samples/common/include/BaseApplication.h
		include/ScriptInterpreter.h
	)

set( SRCS
		../../samples/common/src/BaseApplication.cpp
		src/ScriptInterpreter.cpp
	)
set( SRCS2
		"${OgreProcedural_BINARY_DIR}/procedural_wrap.cxx"
	)
	

set( EXTRA
	procedural.i
	ogre.i
	)

include_directories(
	${OGRE_INCLUDE_DIRS}
	../../samples/common/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${LUA_INCLUDE_DIR}
)

list(GET OGRE_INCLUDE_DIRS 0 OGRE_INCLUDE_DIR)
set(SWIG_ARGS -c++ -lua -v -DOGRE_DEPRECATED -I"${OGRE_INCLUDE_DIR}" -I"${CMAKE_CURRENT_SOURCE_DIR}/../../library/include" -I"${CMAKE_CURRENT_SOURCE_DIR}/include" -I"${OgreProcedural_BINARY_DIR}/include" -o "${OgreProcedural_BINARY_DIR}/procedural_wrap.cxx")
if(MSVC)
	set(SWIG_ARGS ${SWIG_ARGS} -Fmicrosoft)
endif()
if(FREETYPE_FOUND)
	set(SWIG_ARGS ${SWIG_ARGS} -DPROCEDURAL_USE_FREETYPE)
else()
	set(SWIG_ARGS ${SWIG_ARGS} -DPROCEDURAL_NO_FREETYPE)
endif()
add_custom_command(OUTPUT "${OgreProcedural_BINARY_DIR}/procedural_wrap.cxx"
                     COMMAND ${SWIG_EXECUTABLE} ARGS ${SWIG_ARGS} procedural.i
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
