#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(OgreConverter)

include(PrecompiledHeader)

file(GLOB _HEADERS "*.h")
file(GLOB _SRCS "*.cpp")

DI_ADD_EXECUTABLE(OgreConverter ${_HEADERS} ${_SRCS})
include_directories(${OGRE_INCLUDE_DIRS} ${DEMI_SOURCE_DIR}/src/demi)

if (NOT APPLE)
	set_property(TARGET OgreConverter APPEND PROPERTY DEBUG_POSTFIX "_d")
endif ()

string(REGEX REPLACE "/Zm[0-9]+ *" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm500" CACHE STRING "" FORCE)
endif()

set_property(TARGET OgreConverter PROPERTY FOLDER "Tools")

target_link_libraries(OgreConverter DiMisc DiGfx ${OGRE_LIBRARIES})

# specify a precompiled header to use
use_precompiled_header(OgreConverter "DiPch.h")

