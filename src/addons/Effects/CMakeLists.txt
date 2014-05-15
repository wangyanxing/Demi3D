#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DiFx)

file(GLOB DI_FX_HEADERS "*.h")
file(GLOB DI_FX_SRCS "*.cpp")

# source files
DI_ADD_LIBRARY(DiFx ${DI_FX_HEADERS} ${DI_FX_SRCS} )
DI_CONFIG_CORE_LIB(DiFx "${CMAKE_CURRENT_SOURCE_DIR}/FxPch.h")

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

target_link_libraries(DiFx DiMisc DiGfx)

if(NOT ${DI_FX_PATH} STREQUAL "")
  set_target_properties(DiFx PROPERTIES
    COMPILE_FLAGS "-F${DI_FX_PATH}"
    LINK_FLAGS "-F${DI_FX_PATH} -framework IOKit -framework Cocoa -framework Carbon -framework CoreVideo"
  )
endif()

if (NOT DEMI_STATIC)
  set_target_properties(DiFx PROPERTIES
    COMPILE_DEFINITIONS DI_FX_EXPORT
  )
endif ()

set_property(TARGET DiFx PROPERTY FOLDER "Addons")

# specify a precompiled header to use
use_precompiled_header(DiFx
  "${CMAKE_CURRENT_SOURCE_DIR}/FxPch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/FxPch.cpp"
)