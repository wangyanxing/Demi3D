#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(Arena)

file(GLOB ARENA_HEADERS "*.h" "BehaviorTree/*.h")
file(GLOB ARENA_SRCS "*.cpp" "BehaviorTree/*.cpp")

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
set (ARENA_PLATFORM_SPEC ""
  )
  set(ARENA_PLATFORM_LIBS "")
elseif (APPLE)
set (ARENA_PLATFORM_SPEC ""
  )
  set(ARENA_PLATFORM_LIBS ${Carbon_LIBRARY_FWK} ${Cocoa_LIBRARY_FWK} ${IOKit_LIBRARY_FWK} ${CoreVideo_LIBRARY_FWK})
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/ArenaGameApp.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
endif()

add_filtered_std( "BehaviorTree" )

# source files
DI_ADD_EXECUTABLE(Arena ${ARENA_HEADERS} ${ARENA_SRCS})

include_directories(${OIS_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} ${DEMI_SOURCE_DIR}/src/demi 
	${DEMI_SOURCE_DIR}/src/demi/demoFrame 
	${DEMI_SOURCE_DIR}/src/addons/K2 
	${DEMI_SOURCE_DIR}/src/addons/K2/Fx
	${DEMI_SOURCE_DIR}/src/demi/script)

if (NOT APPLE)
	set_property(TARGET Arena APPEND PROPERTY DEBUG_POSTFIX "_d")
endif ()

set_property(TARGET Arena PROPERTY FOLDER "Games")

target_link_libraries(Arena DiMisc DiGfx DiK2 DiScript ${OIS_LIBRARIES})

DI_CONFIG_CORE_LIB(Arena "${CMAKE_CURRENT_SOURCE_DIR}/ArenaPch.h")

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

include_directories(${OIS_INCLUDE_DIRS})
include_directories(${DEMI_EXTERNAL_SRC_PATH}/lua/src)
include_directories(${DEMI_EXTERNAL_SRC_PATH}/lua/tolua++)

target_link_libraries(Arena DiMisc DiGfx ${OIS_LIBRARIES} ${ARENA_PLATFORM_LIBS})

if(NOT ${ARENA_PATH} STREQUAL "")
  set_target_properties(Arena PROPERTIES
    COMPILE_FLAGS "-F${ARENA_PATH}"
    LINK_FLAGS "-F${ARENA_PATH} -framework IOKit -framework Cocoa -framework Carbon -framework CoreVideo"
  )
endif()

if (NOT DEMI_STATIC)
  set_target_properties(Arena PROPERTIES
    COMPILE_DEFINITIONS ARENA_EXPORT
  )
endif ()

# Copy hon resources
if (APPLE AND DEMI_BUILD_PLATFORM_APPLE_IOS)
add_custom_command(TARGET Arena POST_BUILD
COMMAND ditto ${DEMI_SOURCE_DIR}/media_ios ${DEMI_SAMPLE_CONTENTS_PATH}/media_ios
)
endif()

# specify a precompiled header to use
use_precompiled_header(Arena 
  "${CMAKE_CURRENT_SOURCE_DIR}/ArenaPch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/ArenaPch.cpp"
)