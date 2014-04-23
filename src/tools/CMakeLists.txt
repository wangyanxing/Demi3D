#
# Copyright (c) 2013-2014 the Demi3D project.
#
#if (WIN32)
if (FALSE)
add_subdirectory(OgreConverter)
endif()

if (NOT DEMI_BUILD_PLATFORM_APPLE_IOS)
add_subdirectory(HonViewer)
endif()