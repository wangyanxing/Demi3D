#
# Copyright (c) 2013-2014 the Demi3D project.
#

add_subdirectory(misc)
add_subdirectory(gfx)

if (WIN32)
add_subdirectory(drvD3D9)
endif()

add_subdirectory(drvGL)
add_subdirectory(drvGLES2)
add_subdirectory(demoFrame)
