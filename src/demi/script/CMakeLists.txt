#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DiScript)

include(PrecompiledHeader)

file(GLOB DI_HEADERS "*.h")
file(GLOB DI_SRCS "*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${DEMI_EXTERNAL_SRC_PATH}/lua/src)
include_directories(${DEMI_EXTERNAL_SRC_PATH}/lua/tolua++)
  
# source files
DI_ADD_LIBRARY(DiScript ${DI_HEADERS} ${DI_SRCS})
DI_CONFIG_CORE_LIB(DiScript "${CMAKE_CURRENT_SOURCE_DIR}/ScriptPch.h")

target_link_libraries(DiScript DiMisc DiGfx ${LUA_LIBRARIES})

if (NOT DEMI_STATIC)
  set_target_properties(DiScript PROPERTIES
    COMPILE_DEFINITIONS DI_SCRIPT_EXPORT
  )
endif ()

set_property(TARGET DiScript PROPERTY FOLDER "Core")

# specify a precompiled header to use
use_precompiled_header(DiScript 
  "${CMAKE_CURRENT_SOURCE_DIR}/ScriptPch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/ScriptPch.cpp"
)