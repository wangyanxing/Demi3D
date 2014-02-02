#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DiDrvGL)

include(PrecompiledHeader)

file(GLOB DI_DRV_GL_HEADERS "*.h")
file(GLOB DI_DRV_GL_SRCS "*.cpp")

if (WIN32)
  set (DI_GL_PLATFORM_SPEC
    Win32/Win32GLUtil.h
    Win32/Win32GLUtil.cpp
    Win32/Win32GLContext.h
    Win32/Win32GLContext.cpp
    )
  set (DI_GL_PLATFORM_LIBS)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Win32)
elseif (APPLE)
  set (DI_GL_PLATFORM_SPEC
    OSX/OSXGLUtil.h
    OSX/OSXGLUtil.mm
	OSX/OSXCocoaContext.h
	OSX/OSXCocoaContext.mm
    OSX/OSXCGLContext.h
    OSX/OSXCGLContext.cpp
    OSX/OSXWindow.h
    OSX/OSXWindow.mm
	OSX/OSXWindowDelegate.h
	OSX/OSXWindowDelegate.mm
	OSX/OSXWindowView.h
	OSX/OSXWindowView.mm
    )
  set (DI_GL_PLATFORM_LIBS ${Cocoa_LIBRARY_FWK})	
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/OSX)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
				    ${CMAKE_CURRENT_SOURCE_DIR}/GL)

# source files
DI_ADD_LIBRARY(DiDrvGL ${DI_DRV_GL_HEADERS} ${DI_DRV_GL_SRCS} ${DI_GL_PLATFORM_SPEC})
DI_CONFIG_CORE_LIB(DiDrvGL "${CMAKE_CURRENT_SOURCE_DIR}/DrvGLPch.h")

target_link_libraries(DiDrvGL DiMisc DiGfx ${OPENGL_LIBRARIES} ${DI_GL_PLATFORM_LIBS})

if (NOT DEMI_STATIC)
  set_target_properties(DiDrvGL PROPERTIES
    COMPILE_DEFINITIONS DI_GL_EXPORT
  )
endif ()

set_property(TARGET DiDrvGL PROPERTY FOLDER "Core")

# specify a precompiled header to use
use_precompiled_header(DiDrvGL 
  "${CMAKE_CURRENT_SOURCE_DIR}/DrvGLPch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/DrvGLPch.cpp"
)