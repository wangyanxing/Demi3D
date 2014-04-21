#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(MyGUI)

file(GLOB DI_MYGUI_HEADERS "*.h")
file(GLOB DI_MYGUI_SRCS "*.cpp")

if (WIN32)
set (DI_MYGUI_PLATFORM_SPEC ""
  )
  set(DI_MYGUI_PLATFORM_LIBS "")
elseif (APPLE)
set (DI_MYGUI_PLATFORM_SPEC ""
  )
  set(DI_MYGUI_PLATFORM_LIBS ${Carbon_LIBRARY_FWK} ${Cocoa_LIBRARY_FWK} ${IOKit_LIBRARY_FWK} ${CoreVideo_LIBRARY_FWK})
  #set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/K2GameApp.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
endif()

# source files
DI_ADD_LIBRARY(MyGUI ${DI_MYGUI_HEADERS} ${DI_MYGUI_SRCS} )
DI_CONFIG_CORE_LIB(MyGUI "${CMAKE_CURRENT_SOURCE_DIR}/MyGUI_Precompiled.h")

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

include_directories(${DEMI_EXTERNAL_SRC_PATH}/freetype/include)
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