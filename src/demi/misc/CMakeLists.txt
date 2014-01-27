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
  
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/pugixml)
  
# source files
DI_ADD_LIBRARY(DiMisc ${DI_MISC_HEADERS} ${DI_MISC_SRCS} ${DI_MISC_PUGIXML})

if (NOT DEMI_STATIC)
  set_target_properties(DiMisc PROPERTIES
    COMPILE_DEFINITIONS DI_MISC_EXPORT
  )
endif ()

# specify a precompiled header to use
use_precompiled_header(OgreMain 
  "${CMAKE_CURRENT_SOURCE_DIR}/MiscPch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/MiscPch.cpp"
)