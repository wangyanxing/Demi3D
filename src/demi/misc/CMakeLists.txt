#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(DiMisc)

include(PrecompiledHeader)

function( add_filtered_h filterName fileString )
	file(GLOB TMP_FILES ${fileString})
	set(HEADER_FILES ${HEADER_FILES} ${TMP_FILES} PARENT_SCOPE)
	source_group(${filterName} FILES ${TMP_FILES})
endfunction( add_filtered_h )

function( add_filtered_src filterName fileString )
	file(GLOB TMP_FILES ${fileString})
	set(SOURCE_FILES ${SOURCE_FILES} ${TMP_FILES} PARENT_SCOPE)
	source_group(${filterName} FILES ${TMP_FILES})
endfunction( add_filtered_src )

function( add_filtered_std relativePath )
	string(REPLACE "/" "\\" filterPart ${relativePath})
	add_filtered_h("Header Files\\${filterPart}" "${CMAKE_CURRENT_SOURCE_DIR}/${relativePath}/*.h")
	add_filtered_h("Header Files\\${filterPart}" "${CMAKE_CURRENT_SOURCE_DIR}/${relativePath}/*.inl")
	set(HEADER_FILES ${HEADER_FILES} PARENT_SCOPE)
	add_filtered_src("Source Files\\${filterPart}" "${CMAKE_CURRENT_SOURCE_DIR}/${relativePath}/*.cpp")
	set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
endfunction( add_filtered_std )


file(GLOB DI_MISC_HEADERS "*.h" "Math/Array/*.h" "Math/Simple/*.h" "Math/Simple/C/*.h")
file(GLOB DI_MISC_SRCS "*.cpp" "Math/Array/*.cpp" "Math/Simple/*.cpp" "Math/Simple/C/*.cpp")

if (DEMI_SIMD_SSE2)
	add_filtered_std( "Math/Array/SSE2/Single" )
endif()

if (DEMI_SIMD_NEON)
	add_filtered_std( "Math/Array/NEON/Single" )
endif()

if (NOT(DEMI_SIMD_SSE2 OR DEMI_SIMD_NEON))
	add_filtered_std( "Math/Array/C" )
endif ()

add_filtered_std("Math")
add_filtered_std("Math/Array")
add_filtered_std("Math/Simple")
add_filtered_std("Math/Simple/C")

set (DI_MISC_PUGIXML
  pugixml/pugiconfig.hpp
  pugixml/pugixml.hpp
  pugixml/pugixml.cpp
  )
  
if (WIN32)
set (DI_MISC_PLATFORM_SPEC
  win32/Win32Criticalsection.h
  win32/Win32Interlocked.h
  win32/Win32Event.h
  win32/Win32Function.h
  win32/Win32Function.cpp
  win32/Win32Thread.h
  win32/Win32Thread.cpp
  )
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/win32)
  set(DI_MISC_PLATFORM_LIBS "")
elseif (APPLE)
set (DI_MISC_PLATFORM_SPEC
  OSX/MacUtils.h
  OSX/MacUtils.cpp
  OSX/OSXFunction.h
  OSX/OSXFunction.cpp
  OSX/OSXPathLib.mm
  )
  
  if (DEMI_BUILD_PLATFORM_APPLE_IOS)
	  file(GLOB AFNetworking_ADDONS "AFNetworking/*.h" "AFNetworking/*.m" "JSONKit/*.h" "JSONKit/*.m")
	  file(GLOB JSONKit_ADDONS "JSONKit/*.h" "JSONKit/*.m")
	  add_filtered_std("AFNetworking")
	  add_filtered_std("JSONKit")
	  
	  set_property(SOURCE ${AFNetworking_ADDONS}
	    PROPERTY COMPILE_FLAGS "-fobjc-arc"
	    )
  	  set_property(SOURCE ${JSONKit_ADDONS}
  	    PROPERTY COMPILE_FLAGS "-fno-objc-arc"
  	    )
  endif()
  
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/OSX)
  set(DI_MISC_PLATFORM_LIBS ${Carbon_LIBRARY_FWK} ${Cocoa_LIBRARY_FWK})
endif()
  
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/pugixml)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/math)
include_directories(${DEMI_EXTERNAL_SRC_PATH}/LinearMath)
  
# source files
DI_ADD_LIBRARY(DiMisc ${DI_MISC_HEADERS} ${DI_MISC_SRCS} ${DI_MISC_PUGIXML} ${AFNetworking_ADDONS} 
						${JSONKit_ADDONS} ${DI_MISC_PLATFORM_SPEC} ${HEADER_FILES} ${SOURCE_FILES})
DI_CONFIG_CORE_LIB(DiMisc "${CMAKE_CURRENT_SOURCE_DIR}/MiscPch.h")

target_link_libraries(DiMisc ${DI_MISC_PLATFORM_LIBS} ${LinearMath_LIBRARIES})

set_property(TARGET DiMisc PROPERTY FOLDER "Core")

if (NOT DEMI_STATIC)
  set_target_properties(DiMisc PROPERTIES
	COMPILE_DEFINITIONS DI_MISC_EXPORT
  )
endif ()

# specify a precompiled header to use
use_precompiled_header(DiMisc 
  "${CMAKE_CURRENT_SOURCE_DIR}/MiscPch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/MiscPch.cpp"
)
