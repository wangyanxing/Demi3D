#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DiK2)

file(GLOB DI_K2_HEADERS "*.h" "Fx/*.h")
file(GLOB DI_K2_SRCS "*.cpp" "Fx/*.cpp")


function( add_filtered_h filterName fileString )
  file(GLOB TMP_FILES ${fileString})
  set(HEADER_FILES ${HEADER_FILES} ${TMP_FILES} PARENT_SCOPE)
  source_group(${filterName} FILES ${TMP_FILES})
endfunction( add_filtered_h )

function( add_filtered_src filterName fileString )
  file(GLOB TMP_FILES ${fileString})
  set(SOURCE_FILES ${SOURCE_FILES} ${TMP_FILES} PARENT_SCOPE)
  source_group(${filterName} FILES ${TMP_FILES})
endfunction( add_filtered_src )

function( add_filtered_std relativePath )
  string(REPLACE "/" "\\" filterPart ${relativePath})
  add_filtered_h("Header Files\\${filterPart}" "${CMAKE_CURRENT_SOURCE_DIR}/${relativePath}/*.h")
  add_filtered_h("Header Files\\${filterPart}" "${CMAKE_CURRENT_SOURCE_DIR}/${relativePath}/*.inl")
  set(HEADER_FILES ${HEADER_FILES} PARENT_SCOPE)
  add_filtered_src("Source Files\\${filterPart}" "${CMAKE_CURRENT_SOURCE_DIR}/${relativePath}/*.cpp")
  set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
endfunction( add_filtered_std )

if (WIN32)
set (DI_K2_PLATFORM_SPEC ""
  )
  set(DI_K2_PLATFORM_LIBS "")
elseif (APPLE)
set (DI_K2_PLATFORM_SPEC ""
  )
  set(DI_K2_PLATFORM_LIBS ${Carbon_LIBRARY_FWK} ${Cocoa_LIBRARY_FWK} ${IOKit_LIBRARY_FWK} ${CoreVideo_LIBRARY_FWK})
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/K2GameApp.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
endif()

add_filtered_std( "Fx" )

# source files
DI_ADD_LIBRARY(DiK2 ${DI_K2_HEADERS} ${DI_K2_SRCS} )
DI_CONFIG_CORE_LIB(DiK2 "${CMAKE_CURRENT_SOURCE_DIR}/K2Pch.h")

if (DEMI_STATIC)
	if (APPLE AND NOT DEMI_BUILD_PLATFORM_APPLE_IOS)
		include_directories(${DEMI_SOURCE_DIR}/src/demi/drvGL)
	elseif (APPLE AND DEMI_BUILD_PLATFORM_APPLE_IOS)
		include_directories(${DEMI_SOURCE_DIR}/src/demi/drvGLES2)
	elseif (WIN32)
		include_directories(${DEMI_SOURCE_DIR}/src/demi/drvD3D9)
		include_directories(${DEMI_SOURCE_DIR}/src/demi/drvGLES2)
		include_directories(${DEMI_SOURCE_DIR}/src/demi/drvGL)
	endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Fx)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${OIS_INCLUDE_DIRS})
target_link_libraries(DiK2 DiMisc DiGfx ${OIS_LIBRARIES} ${DI_K2_PLATFORM_LIBS})

if(NOT ${DI_K2_PATH} STREQUAL "")
  set_target_properties(DiK2 PROPERTIES
    COMPILE_FLAGS "-F${DI_K2_PATH}"
    LINK_FLAGS "-F${DI_K2_PATH} -framework IOKit -framework Cocoa -framework Carbon -framework CoreVideo"
  )
endif()

if (NOT DEMI_STATIC)
  set_target_properties(DiK2 PROPERTIES
    COMPILE_DEFINITIONS DI_K2_EXPORT
  )
endif ()

set_property(TARGET DiK2 PROPERTY FOLDER "Addons")

# specify a precompiled header to use
use_precompiled_header(DiK2 
  "${CMAKE_CURRENT_SOURCE_DIR}/K2Pch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/K2Pch.cpp"
)