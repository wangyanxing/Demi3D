#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DiDrvGLES2)

include(PrecompiledHeader)

file(GLOB DI_DRV_GLES2_HEADERS "*.h")
file(GLOB DI_DRV_GLES2_SRCS "*.cpp")
file(GLOB GLESW_HEADERS "GLES2/*.h")
file(GLOB KHR_HEADERS "KHR/*.h")

if (WIN32)
  file(GLOB DI_GLES2_PLATFORM_SPEC "EGL/WIN32/*.h" "EGL/*.h" "EGL/WIN32/*.cpp" "EGL/*.cpp")
  set (DI_GLES2_PLATFORM_LIBS)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/EGL/Win32)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/EGL)
elseif (APPLE)
  file(GLOB DI_GLES2_PLATFORM_SPEC "EAGL/*.h" "EAGL/*.mm")
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/EAGL)
  link_directories(${OPENGLES2_LIBRARY_PATH})
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
				    ${CMAKE_CURRENT_SOURCE_DIR}/GLES2
					${OPENGLES2_INCLUDE_DIR} ${EGL_INCLUDE_DIR}
					${DEMI_DEPENDENCIES_DIR}/include
					${iOSSDK_INCLUDE_DIR})

if (DEMI_BUILD_PLATFORM_APPLE_IOS)
    set_source_files_properties(gles2w.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
endif()

# source files
DI_ADD_LIBRARY(DiDrvGLES2 ${DI_DRV_GLES2_HEADERS} ${DI_DRV_GLES2_SRCS} ${DI_GLES2_PLATFORM_SPEC} ${GLESW_HEADERS} ${KHR_HEADERS})
DI_CONFIG_CORE_LIB(DiDrvGLES2 "${CMAKE_CURRENT_SOURCE_DIR}/DrvGLES2Pch.h")

target_link_libraries(DiDrvGLES2 DiMisc DiGfx ${OPENGL_LIBRARIES} ${DI_GLES2_PLATFORM_LIBS} ${OPENGLES2_LIBRARIES} ${EGL_LIBRARIES})

if (NOT DEMI_STATIC)
  set_target_properties(DiDrvGLES2 PROPERTIES
    COMPILE_DEFINITIONS DI_GLES2_EXPORT
  )
endif ()

set_property(TARGET DiDrvGLES2 PROPERTY FOLDER "Core")

# specify a precompiled header to use
use_precompiled_header(DiDrvGLES2 
  "${CMAKE_CURRENT_SOURCE_DIR}/DrvGLES2Pch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/DrvGLES2Pch.cpp"
)