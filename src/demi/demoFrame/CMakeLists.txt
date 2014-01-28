#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DemoFramework)

include(PrecompiledHeader)

file(GLOB DI_DEMO_FRAME_HEADERS "*.h")
file(GLOB DI_DEMO_FRAME_SRCS "*.cpp")

# source files
DI_ADD_LIBRARY(DemoFramework ${DI_DEMO_FRAME_HEADERS} ${DI_DEMO_FRAME_SRCS} )

include_directories(${OIS_INCLUDE_DIRS})
target_link_libraries(DemoFramework DiMisc DiGfx ${OIS_LIBRARIES})

if (NOT DEMI_STATIC)
  set_target_properties(DemoFramework PROPERTIES
    COMPILE_DEFINITIONS DI_DEMO_EXPORT
  )
endif ()

set_property(TARGET DemoFramework PROPERTY FOLDER "Core")

# specify a precompiled header to use
use_precompiled_header(DemoFramework "DemoPch.h")