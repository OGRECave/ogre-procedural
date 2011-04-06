macro(_FIND_PROCEDURAL_LIBRARY _var)
  find_library(${_var}
     NAMES 
        ${ARGN}
     PATHS
        ${PROCEDURAL_ROOT}
     PATH_SUFFIXES lib
  )
  mark_as_advanced(${_var})
endmacro()

macro(_PROCEDURAL_APPEND_LIBRARIES _list _release)
   set(_debug ${_release}_DEBUG)
   if(${_debug})
      set(${_list} ${${_list}} optimized ${${_release}} debug ${${_debug}})
   else()
      set(${_list} ${${_list}} ${${_release}})
   endif()
endmacro()

find_path(PROCEDURAL_INCLUDE_DIR NAMES Procedural.h
  PATHS
    ${PROCEDURAL_ROOT}/include
    ${PROCEDURAL_ROOT}/src
)

# Find the libraries

_FIND_PROCEDURAL_LIBRARY(PROCEDURAL_LIBRARY        OgreProcedural)
_FIND_PROCEDURAL_LIBRARY(PROCEDURAL_LIBRARY_DEBUG  OgreProcedural_d)


# handle the QUIETLY and REQUIRED arguments and set PROCEDURAL_FOUND to TRUE if 
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Procedural DEFAULT_MSG
    PROCEDURAL_LIBRARY PROCEDURAL_INCLUDE_DIR)

set(PROCEDURAL_INCLUDE_DIRS ${PROCEDURAL_INCLUDE_DIR})
if(PROCEDURAL_FOUND)
   _PROCEDURAL_APPEND_LIBRARIES(PROCEDURAL_LIBRARIES PROCEDURAL_LIBRARY)
endif()
