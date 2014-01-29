#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DiMisc)

include(PrecompiledHeader)

file(GLOB DI_MISC_HEADERS "*.h")
file(GLOB DI_MISC_SRCS "*.cpp")

set (DI_MISC_PUGIXML
  pugixml/pugiconfig.hpp
  pugixml/pugixml.hpp
  pugixml/pugixml.cpp
  )
  
if (WIN32)
set (DI_MISC_PLATFORM_SPEC
  win32/Win32Criticalsection.h
  win32/Win32Interlocked.h
  win32/Win32Event.h
  win32/Win32Function.h
  win32/Win32Function.cpp
  win32/Win32Thread.h
  win32/Win32Thread.cpp
  )
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/win32)
  set(DI_MISC_PLATFORM_LIBS "")
elseif (APPLE)
set (DI_MISC_PLATFORM_SPEC
  OSX/MacUtils.h
  OSX/MacUtils.cpp
  OSX/OSXFunction.h
  OSX/OSXFunction.cpp
  )
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/OSX)
  set(DI_MISC_PLATFORM_LIBS ${Carbon_LIBRARY_FWK} ${Cocoa_LIBRARY_FWK})
endif()
  
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/pugixml)
  
# source files
DI_ADD_LIBRARY(DiMisc ${DI_MISC_HEADERS} ${DI_MISC_SRCS} ${DI_MISC_PUGIXML} ${DI_MISC_PLATFORM_SPEC})
DI_CONFIG_CORE_LIB(DiMisc "${CMAKE_CURRENT_SOURCE_DIR}/MiscPch.h")

target_link_libraries(DiMisc ${DI_MISC_PLATFORM_LIBS})

set_property(TARGET DiMisc PROPERTY FOLDER "Core")

if (NOT DEMI_STATIC)
  set_target_properties(DiMisc PROPERTIES
	COMPILE_DEFINITIONS DI_MISC_EXPORT
  )
endif ()

# specify a precompiled header to use
use_precompiled_header(DiMisc 
  "${CMAKE_CURRENT_SOURCE_DIR}/MiscPch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/MiscPch.cpp"
)
