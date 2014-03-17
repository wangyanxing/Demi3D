#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DiK2)

file(GLOB DI_K2_HEADERS "*.h")
file(GLOB DI_K2_SRCS "*.cpp")


# source files
DI_ADD_LIBRARY(DiK2 ${DI_K2_HEADERS} ${DI_K2_SRCS} )
DI_CONFIG_CORE_LIB(DiK2 "${CMAKE_CURRENT_SOURCE_DIR}/K2Pch.h")

target_link_libraries(DiK2 DiMisc DiGfx)

if(NOT ${DEMI_FRAMEWORK_PATH} STREQUAL "")
  set_target_properties(DiK2 PROPERTIES
    COMPILE_FLAGS "-F${DEMI_FRAMEWORK_PATH}"
    LINK_FLAGS "-F${DEMI_FRAMEWORK_PATH} -framework IOKit -framework Cocoa -framework Carbon -framework OpenGL -framework CoreVideo"
  )
endif()

if (NOT DEMI_STATIC)
  set_target_properties(DiK2 PROPERTIES
    COMPILE_DEFINITIONS DI_K2_EXPORT
  )
endif ()

set_property(TARGET DiK2 PROPERTY FOLDER "Addons")

# specify a precompiled header to use
use_precompiled_header(DiDrvD3D9 
  "${CMAKE_CURRENT_SOURCE_DIR}/K2Pch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/K2Pch.cpp"
)