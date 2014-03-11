#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(K2Converter)

include(PrecompiledHeader)

file(GLOB _HEADERS "*.h")
file(GLOB _SRCS "*.cpp")

DI_ADD_EXECUTABLE(K2Converter ${_HEADERS} ${_SRCS})
include_directories(${DEMI_SOURCE_DIR}/src/demi)

if (NOT APPLE)
	set_property(TARGET K2Converter APPEND PROPERTY DEBUG_POSTFIX "_d")
endif ()

string(REGEX REPLACE "/Zm[0-9]+ *" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm500" CACHE STRING "" FORCE)
endif()

set_property(TARGET K2Converter PROPERTY FOLDER "Tools")

target_link_libraries(K2Converter DiMisc DiGfx )