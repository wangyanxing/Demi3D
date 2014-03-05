#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DiGfx)

include(PrecompiledHeader)

file(GLOB DI_GFX_HEADERS "*.h")
file(GLOB DI_GFX_SRCS "*.cpp")

set (DI_GFX_ADDON
  Addon/nv_dds.h
  Addon/nv_dds.cpp
  )
  
if (WIN32)
  set (DI_GFX_PLATFORM_SPEC
    Win32/Win32Window.h
    Win32/Win32Window.cpp
    )
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Win32)
elseif (APPLE)
  set (DI_GFX_PLATFORM_SPEC
    #OSX/OSXWindow.h
    #OSX/OSXWindow.cpp
    )
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/OSX)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Addon)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${DEMI_EXTERNAL_SRC_PATH}/STB)
include_directories(${DEMI_EXTERNAL_SRC_PATH}/zziplib)
  
# source files
DI_ADD_LIBRARY(DiGfx ${DI_GFX_HEADERS} ${DI_GFX_SRCS} ${DI_GFX_ADDON} ${DI_GFX_PLATFORM_SPEC})
DI_CONFIG_CORE_LIB(DiGfx "${CMAKE_CURRENT_SOURCE_DIR}/GfxPch.h")

target_link_libraries(DiGfx DiMisc ${STB_LIBRARIES} ${ZZIPLIB_LIBRARIES} ${ZLIB_LIBRARIES})

if (NOT DEMI_STATIC)
  set_target_properties(DiGfx PROPERTIES
    COMPILE_DEFINITIONS DI_GFX_EXPORT
  )
endif ()

set_property(TARGET DiGfx PROPERTY FOLDER "Core")

# specify a precompiled header to use
use_precompiled_header(DiGfx 
  "${CMAKE_CURRENT_SOURCE_DIR}/GfxPch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/GfxPch.cpp"
)