#
# Copyright (c) 2013-2014 the Demi3D project.
#

add_subdirectory(misc)
add_subdirectory(gfx)

if (WIN32)
add_subdirectory(drvD3D9)
endif()

if (DEMI_BUILD_RENDERSYSTEM_GLES2)
  if (NOT OPENGLES2_FOUND)
    MESSAGE(STATUS "Could not find dependency: GLES2 ")
	  MESSAGE(STATUS "Skipping GLES2 rendersystem build")
    set(DEMI_BUILD_RENDERSYSTEM_GLES2 0)
  else ()
	  add_subdirectory(drvGLES2)
  endif()
else()
  add_subdirectory(drvGL)
endif()

add_subdirectory(demoFrame)