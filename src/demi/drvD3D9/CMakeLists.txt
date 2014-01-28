#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DiDrvD3D9)

include(PrecompiledHeader)

file(GLOB DI_DRV_D3D9_HEADERS "*.h")
file(GLOB DI_DRV_D3D9_SRCS "*.cpp")

include_directories(${DirectX_INCLUDE_DIR})
link_directories(${DirectX_LIBRARY_DIR})

# source files
DI_ADD_LIBRARY(DiDrvD3D9 ${DI_DRV_D3D9_HEADERS} ${DI_DRV_D3D9_SRCS} )

target_link_libraries(DiDrvD3D9 
  DiMisc DiGfx
  ${DirectX_LIBRARIES})

if (NOT DEMI_STATIC)
  set_target_properties(DiDrvD3D9 PROPERTIES
    COMPILE_DEFINITIONS DI_D3D9_EXPORT
  )
endif ()

set_property(TARGET DiDrvD3D9 PROPERTY FOLDER "Core")

# specify a precompiled header to use
use_precompiled_header(DiDrvD3D9 "DrvD3D9Pch.h")