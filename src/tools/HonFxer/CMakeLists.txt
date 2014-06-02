#
# Copyright (c) 2013-2014 the Demi3D project.
#

project(HonFxer)

include(PrecompiledHeader)

file(GLOB SAMPLE_HEADERS "*.h" "UI/*.h" "Editor/*.h")
file(GLOB SAMPLE_SRCS "*.cpp" "UI/*.cpp" "Editor/*.cpp")

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

add_filtered_std("Editor")
add_filtered_std("UI")

if (WIN32)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Win32)
	add_filtered_std("Win32")
	set(RESOURCE_FILES
      Win32/resource.h
      Win32/HonFxer.rc
      Win32/winres.h
    )
elseif(APPLE)
	set(RESOURCE_FILES
      ${DEMI_SOURCE_DIR}/src/tools/HonFxer/honicon.icns
    )
endif()

DI_ADD_EXECUTABLE(HonFxer ${SAMPLE_HEADERS} ${SAMPLE_SRCS} ${RESOURCE_FILES})
include_directories(${OIS_INCLUDE_DIRS} ${DEMI_SOURCE_DIR}/src/demi ${DEMI_SOURCE_DIR}/src/demi/demoFrame ${DEMI_SOURCE_DIR}/src/addons/K2 ${DEMI_SOURCE_DIR}/src/demi/script)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Editor)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/UI)


if (NOT APPLE)
	set_property(TARGET HonFxer APPEND PROPERTY DEBUG_POSTFIX "_d")
else()
	set_property(TARGET HonFxer PROPERTY MACOSX_BUNDLE TRUE)
	set_property(TARGET HonFxer PROPERTY MACOSX_BUNDLE_ICON_FILE honicon)
	set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.demi3d.\${PRODUCT_NAME:rfc1034identifier}")

	add_custom_command(TARGET HonFxer POST_BUILD
  	    	COMMAND ditto ${DEMI_SOURCE_DIR}/src/tools/HonFxer/honicon.icns ${DEMI_SOURCE_DIR}/bin/$(CONFIGURATION)/${DEMI_BIN_NAME}.app/Contents/Resources/
  	    	)
endif ()

set_property(TARGET HonFxer PROPERTY FOLDER "Tools")

target_link_libraries(HonFxer DemoFramework DiMisc DiGfx DiK2 DiScript MyGUI ${OIS_LIBRARIES})

# specify a precompiled header to use
use_precompiled_header(HonFxer 
  "${CMAKE_CURRENT_SOURCE_DIR}/FxerPch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/FxerPch.cpp"
)