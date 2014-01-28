#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DiDrvGL)

include(PrecompiledHeader)

file(GLOB DI_DRV_GL_HEADERS "*.h")
file(GLOB DI_DRV_GL_SRCS "*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/GL)

# source files
DI_ADD_LIBRARY(DiDrvGL ${DI_DRV_GL_HEADERS} ${DI_DRV_GL_SRCS} )

target_link_libraries(DiDrvGL DiMisc DiGfx ${OPENGL_LIBRARIES})

if (NOT DEMI_STATIC)
  set_target_properties(DiDrvGL PROPERTIES
    COMPILE_DEFINITIONS DI_GL_EXPORT
  )
endif ()

set_property(TARGET DiDrvGL PROPERTY FOLDER "Core")

# specify a precompiled header to use
use_precompiled_header(DiDrvGL "DrvGLPch.h")