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
  set(DI_DEMO_PLATFORM_LIBS ${Carbon_LIBRARY_FWK} ${Cocoa_LIBRARY_FWK} ${IOKit_LIBRARY_FWK} ${CoreVideo_LIBRARY_FWK})
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
endif()

# source files
DI_ADD_LIBRARY(DemoFramework ${DI_DEMO_FRAME_HEADERS} ${DI_DEMO_FRAME_SRCS} )
DI_CONFIG_CORE_LIB(DemoFramework "${CMAKE_CURRENT_SOURCE_DIR}/DemoPch.h")

if (DEMI_STATIC)
	if (APPLE AND NOT DEMI_BUILD_PLATFORM_APPLE_IOS)
		include_directories(${DEMI_SOURCE_DIR}/src/demi/drvGL)
	elseif (APPLE AND DEMI_BUILD_PLATFORM_APPLE_IOS)
		include_directories(${DEMI_SOURCE_DIR}/src/demi/drvGLES2)
	elseif (WIN32)
		include_directories(${DEMI_SOURCE_DIR}/src/demi/drvD3D9)
		include_directories(${DEMI_SOURCE_DIR}/src/demi/drvGLES2)
		include_directories(${DEMI_SOURCE_DIR}/src/demi/drvGL)
	endif()
endif()

include_directories(${OIS_INCLUDE_DIRS})
include_directories(${DEMI_SOURCE_DIR}/src/demi/script)
target_link_libraries(DemoFramework DiMisc DiGfx DiScript ${OIS_LIBRARIES} ${DI_DEMO_PLATFORM_LIBS})

if(NOT ${DEMI_FRAMEWORK_PATH} STREQUAL "")
  set_target_properties(DemoFramework PROPERTIES
    COMPILE_FLAGS "-F${DEMI_FRAMEWORK_PATH}"
    LINK_FLAGS "-F${DEMI_FRAMEWORK_PATH} -framework IOKit -framework Cocoa -framework Carbon -framework CoreVideo"
  )
endif()

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