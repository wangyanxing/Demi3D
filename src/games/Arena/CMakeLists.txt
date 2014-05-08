#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(Arena)

file(GLOB ARENA_HEADERS "*.h")
file(GLOB ARENA_SRCS "*.cpp")

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

# source files
DI_ADD_EXECUTABLE(Arena ${ARENA_HEADERS} ${ARENA_SRCS})

include_directories(${OIS_INCLUDE_DIRS} ${DEMI_SOURCE_DIR}/src/demi ${DEMI_SOURCE_DIR}/src/demi/demoFrame ${DEMI_SOURCE_DIR}/src/addons/K2 ${DEMI_SOURCE_DIR}/src/demi/script)

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

# specify a precompiled header to use
use_precompiled_header(Arena 
  "${CMAKE_CURRENT_SOURCE_DIR}/ArenaPch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/ArenaPch.cpp"
)