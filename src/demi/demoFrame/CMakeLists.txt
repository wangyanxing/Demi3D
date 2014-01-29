#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DemoFramework)

include(PrecompiledHeader)

file(GLOB DI_DEMO_FRAME_HEADERS "*.h")
file(GLOB DI_DEMO_FRAME_SRCS "*.cpp")

  
if (WIN32)
set (DI_DEMO_PLATFORM_SPEC ""
  )
  set(DI_DEMO_PLATFORM_LIBS "")
elseif (APPLE)
set (DI_DEMO_PLATFORM_SPEC ""
  )
  set(DI_DEMO_PLATFORM_LIBS ${Carbon_LIBRARY_FWK} ${Cocoa_LIBRARY_FWK})
endif()

# source files
DI_ADD_LIBRARY(DemoFramework ${DI_DEMO_FRAME_HEADERS} ${DI_DEMO_FRAME_SRCS} )
DI_CONFIG_CORE_LIB(DemoFramework "${CMAKE_CURRENT_SOURCE_DIR}/DemoPch.h")

include_directories(${OIS_INCLUDE_DIRS})
target_link_libraries(DemoFramework DiMisc DiGfx ${OIS_LIBRARIES} ${DI_DEMO_PLATFORM_LIBS})

if (NOT DEMI_STATIC)
  set_target_properties(DemoFramework PROPERTIES
    COMPILE_DEFINITIONS DI_DEMO_EXPORT
  )
endif ()

set_property(TARGET DemoFramework PROPERTY FOLDER "Core")

# specify a precompiled header to use
use_precompiled_header(DemoFramework 
  "${CMAKE_CURRENT_SOURCE_DIR}/DemoPch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/DemoPch.cpp"
)