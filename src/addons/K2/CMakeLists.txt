#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DiK2)

file(GLOB DI_K2_HEADERS "*.h")
file(GLOB DI_K2_SRCS "*.cpp")

if (WIN32)
set (DI_K2_PLATFORM_SPEC ""
  )
  set(DI_K2_PLATFORM_LIBS "")
elseif (APPLE)
set (DI_K2_PLATFORM_SPEC ""
  )
  set(DI_K2_PLATFORM_LIBS ${Carbon_LIBRARY_FWK} ${Cocoa_LIBRARY_FWK} ${IOKit_LIBRARY_FWK} ${CoreVideo_LIBRARY_FWK})
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/K2GameApp.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
endif()

# source files
DI_ADD_LIBRARY(DiK2 ${DI_K2_HEADERS} ${DI_K2_SRCS} )
DI_CONFIG_CORE_LIB(DiK2 "${CMAKE_CURRENT_SOURCE_DIR}/K2Pch.h")

include_directories(${OIS_INCLUDE_DIRS})
target_link_libraries(DiK2 DiMisc DiGfx ${OIS_LIBRARIES} ${DI_K2_PLATFORM_LIBS})

if(NOT ${DI_K2_PATH} STREQUAL "")
  set_target_properties(DiK2 PROPERTIES
    COMPILE_FLAGS "-F${DI_K2_PATH}"
    LINK_FLAGS "-F${DI_K2_PATH} -framework IOKit -framework Cocoa -framework Carbon -framework OpenGL -framework CoreVideo"
  )
endif()

if (NOT DEMI_STATIC)
  set_target_properties(DiK2 PROPERTIES
    COMPILE_DEFINITIONS DI_K2_EXPORT
  )
endif ()

set_property(TARGET DiK2 PROPERTY FOLDER "Addons")

# specify a precompiled header to use
use_precompiled_header(DiK2 
  "${CMAKE_CURRENT_SOURCE_DIR}/K2Pch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/K2Pch.cpp"
)