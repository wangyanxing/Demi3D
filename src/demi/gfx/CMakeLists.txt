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
  Addon/targa.h
  Addon/targa.cpp
  )
  
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Addon)
  
# source files
DI_ADD_LIBRARY(DiGfx ${DI_GFX_HEADERS} ${DI_GFX_SRCS} ${DI_GFX_ADDON})

target_link_libraries(DiGfx DiMisc)

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