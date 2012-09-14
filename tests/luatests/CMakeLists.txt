find_package(Lua52)

set( HDRS
		../../samples/common/include/BaseApplication.h
		include/LuaTests.h
	)

set( SRCS
		../../samples/common/src/BaseApplication.cpp
		"${CMAKE_CURRENT_SOURCE_DIR}/procedural_wrap.cxx"		
		src/LuaTests.cpp
	)

include_directories( ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	../../samples/common/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${LUA_INCLUDE_DIR}
)

add_custom_command(OUTPUT procedural_wrap.cxx
                     COMMAND ${SWIG_DIR}/swig.exe ARGS -c++ -lua -v -I"${OGRE_INCLUDE_DIR}" -I"${CMAKE_CURRENT_SOURCE_DIR}/../../library/include" procedural.i 1>out.log 2>err.log
                     WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

procedural_add_executable(LuaTests WIN32 ${HDRS} ${SRCS})

set_target_properties(LuaTests PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(LuaTests ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${OgreProcedural_LIBRARIES} ${LUA_LIBRARIES})

procedural_create_vcproj_userfile(LuaTests)