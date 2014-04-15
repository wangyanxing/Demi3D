#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DiScript)

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
	set(HEADER_FILES ${HEADER_FILES} PARENT_SCOPE)
	add_filtered_src("Source Files\\${filterPart}" "${CMAKE_CURRENT_SOURCE_DIR}/${relativePath}/*.c")
	set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
endfunction( add_filtered_std )


file(GLOB DI_HEADERS "*.h" "toluapp/*.h")
file(GLOB DI_SRCS "*.cpp" "toluapp/*.c")

add_filtered_std( "toluapp" )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/toluapp)
include_directories(${DEMI_EXTERNAL_SRC_PATH}/lua/src)
  
# source files
DI_ADD_LIBRARY(DiScript ${DI_HEADERS} ${DI_SRCS})
DI_CONFIG_CORE_LIB(DiScript "${CMAKE_CURRENT_SOURCE_DIR}/ScriptPch.h")

target_link_libraries(DiScript DiMisc DiGfx ${LUA_LIBRARIES})

if (NOT DEMI_STATIC)
  set_target_properties(DiScript PROPERTIES
    COMPILE_DEFINITIONS DI_SCRIPT_EXPORT
  )
endif ()

set_property(TARGET DiScript PROPERTY FOLDER "Core")