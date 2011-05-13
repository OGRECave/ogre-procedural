if(WIN32 OR APPLE)
	set(PROCEDURAL_CMAKE_DIR "CMake")
else()
	set(PROCEDURAL_CMAKE_DIR "${PROCEDURAL_LIB_DIRECTORY}/PROCEDURAL/cmake")
endif()

set(INST_FILES
  Packages/FindOIS.cmake
  Packages/FindOGRE.cmake
  Packages/FindPROCEDURAL.cmake
  Utils/FindPkgMacros.cmake  
  Utils/PreprocessorUtils.cmake
  Utils/ProceduralMacros.cmake
) 
if (WIN32)
  set(INST_FILES ${INST_FILES}
    Templates/VisualStudioUserFile.vcproj.user.in
    Templates/VisualStudioUserFile.vcxproj.user.in
  )
endif ()

if (APPLE)
  set(INST_FILES ${INST_FILES}
    Packages/FindIOKit.cmake
  )
  set(STATIC_INST_FILES ${STATIC_INST_FILES}
    Packages/FindCarbon.cmake
    Packages/FindCocoa.cmake
    Packages/FindiPhoneSDK.cmake
  )
endif ()

install(FILES ${INST_FILES} DESTINATION ${PROCEDURAL_CMAKE_DIR})
