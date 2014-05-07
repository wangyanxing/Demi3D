#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(ConfigGen)

file(GLOB _HEADERS "*.xml")
file(GLOB _SRCS "*.cpp")

DI_ADD_EXECUTABLE(ConfigGen ${_HEADERS} ${_SRCS})
include_directories(${OGRE_INCLUDE_DIRS} ${DEMI_SOURCE_DIR}/src/demi)

if (NOT APPLE)
	set_property(TARGET ConfigGen APPEND PROPERTY DEBUG_POSTFIX "_d")
endif ()

string(REGEX REPLACE "/Zm[0-9]+ *" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm500" CACHE STRING "" FORCE)
endif()

set_property(TARGET ConfigGen PROPERTY FOLDER "Tools")

target_link_libraries(ConfigGen DiMisc)
