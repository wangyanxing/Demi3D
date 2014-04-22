#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(MyGUI)

file(GLOB DI_MYGUI_HEADERS "*.h" "Extends/*.h")
file(GLOB DI_MYGUI_SRCS "*.cpp" "Extends/*.cpp")


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
	add_filtered_src("Source Files\\${filterPart}" "${CMAKE_CURRENT_SOURCE_DIR}/${relativePath}/*.cpp")
	set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
endfunction( add_filtered_std )

add_filtered_std("Extends")

if (WIN32)
set (DI_MYGUI_PLATFORM_SPEC ""
  )
  set(DI_MYGUI_PLATFORM_LIBS "")
elseif (APPLE)
set (DI_MYGUI_PLATFORM_SPEC ""
  )
  set(DI_MYGUI_PLATFORM_LIBS ${Carbon_LIBRARY_FWK} ${Cocoa_LIBRARY_FWK} ${IOKit_LIBRARY_FWK} ${CoreVideo_LIBRARY_FWK})
endif()

# source files
DI_ADD_LIBRARY(MyGUI ${DI_MYGUI_HEADERS} ${DI_MYGUI_SRCS} )
DI_CONFIG_CORE_LIB(MyGUI "${CMAKE_CURRENT_SOURCE_DIR}/MyGUI_Precompiled.h")

include_directories(${DEMI_EXTERNAL_SRC_PATH}/freetype/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Extends)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MyGUI DiMisc DiGfx ${DI_MYGUI_PLATFORM_LIBS} ${FREETYPE_LIBRARIES})

if(NOT ${DI_MYGUI_PATH} STREQUAL "")
  set_target_properties(MyGUI PROPERTIES
    COMPILE_FLAGS "-F${DI_MYGUI_PATH}"
    LINK_FLAGS "-F${DI_MYGUI_PATH} -framework IOKit -framework Cocoa -framework Carbon -framework CoreVideo"
  )
endif()

if (NOT DEMI_STATIC)
  set_target_properties(MyGUI PROPERTIES
    COMPILE_DEFINITIONS DI_MYGUI_EXPORT
  )
else()
  add_definitions(-DMYGUI_STATIC)
endif ()

add_definitions(-DMYGUI_USE_FREETYPE)
add_definitions(-DMYGUI_BUILD)

if (MSVC)
  # disable: "no suitable definition provided for explicit template instantiation request"
  # Occurs for no justifiable reason on all #includes of Singleton
  add_definitions(/wd4661)
endif ()

set_property(TARGET MyGUI PROPERTY FOLDER "Addons")

# specify a precompiled header to use
use_precompiled_header(MyGUI 
  "${CMAKE_CURRENT_SOURCE_DIR}/MyGUI_Precompiled.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/MyGUI_Precompiled.cpp"
)