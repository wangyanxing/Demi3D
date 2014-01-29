#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DiMisc)

include(PrecompiledHeader)

file(GLOB DI_MISC_HEADERS "*.h")
file(GLOB DI_MISC_SRCS "*.cpp")

set (DI_MISC_PUGIXML
  pugixml/pugiconfig.hpp
  pugixml/pugixml.hpp
  pugixml/pugixml.cpp
  )
  
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/pugixml)
  
# source files
DI_ADD_LIBRARY(DiMisc ${DI_MISC_HEADERS} ${DI_MISC_SRCS} ${DI_MISC_PUGIXML})

if(WIN32 AND MSVC10 AND CMAKE_CL_64)
  set_target_properties(DiMisc PROPERTIES 
                        LINK_FLAGS "/INCREMENTAL:NO"
                      )
endif()

if (APPLE)
  if (DEMI_BUILD_PLATFORM_APPLE_IOS)
    set_target_properties(DiMisc PROPERTIES INSTALL_NAME_DIR "OGRE")
  else ()
    set_target_properties(DiMisc PROPERTIES FRAMEWORK TRUE)
    # Set the INSTALL_PATH so that Frameworks can be local
    set_target_properties(DiMisc
       PROPERTIES BUILD_WITH_INSTALL_RPATH 1
       INSTALL_NAME_DIR "@executable_path/../Frameworks"
    )
    set_target_properties(DiMisc PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")
	set_target_properties(DiMisc PROPERTIES RESOURCE "${RESOURCE_FILES}")
	set_source_files_properties("${RESOURCE_FILES}" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_target_properties(DiMisc PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")
    set_target_properties(DiMisc PROPERTIES XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${DEMI_SOURCE_DIR}/src/demi/misc/MiscPch.h")
    set_target_properties(DiMisc PROPERTIES XCODE_ATTRIBUTE_GCC_UNROLL_LOOPS "YES")
  endif ()

  # Framework is called 'DEMI'
  set_target_properties(DiMisc PROPERTIES	OUTPUT_NAME DEMI)
endif ()
target_link_libraries(DiMisc ${LIBRARIES})
if (MINGW)
  # may need winsock htons functions for FreeImage
  target_link_libraries(DiMisc ws2_32)
endif ()

if (NOT DEMI_STATIC)
  set_target_properties(DiMisc PROPERTIES
    COMPILE_DEFINITIONS DI_MISC_EXPORT
  )
endif ()

set_property(TARGET DiMisc PROPERTY FOLDER "Core")

# specify a precompiled header to use
use_precompiled_header(DiMisc 
  "${CMAKE_CURRENT_SOURCE_DIR}/MiscPch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/MiscPch.cpp"
)
