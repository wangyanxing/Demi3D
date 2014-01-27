#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DiMisc)

include(PrecompiledHeader)

file(GLOB DI_MISC_HEARDERS "*.h")
file(GLOB DI_MISC_SRCS "*.cpp")

# source files
add_library(${DI_MISC_HEADERS} ${DI_MISC_SRCS})

# specify a precompiled header to use
use_precompiled_header(OgreMain 
  "${CMAKE_CURRENT_SOURCE_DIR}/MiscPch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/MiscPch.cpp"
)