#
# Copyright (c) 2013-2014 the Demi3D project.
#

cmake_minimum_required (VERSION 2.8.6)

project (DemiExternal)

set(CMAKE_MODULE_PATH "${DemiExternal_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

if (WIN32)
  find_package(DirectX REQUIRED)
  add_definitions(-DWINVER=0x0500)
endif ()

if (APPLE)
  # Set 10.5 as the base SDK by default
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)

  if (NOT CMAKE_OSX_ARCHITECTURES)
    set(CMAKE_OSX_ARCHITECTURES ${ARCHS_STANDARD_32_64_BIT})
  endif()
endif ()

if (NOT APPLE)
  # Create debug libraries with _d postfix
  set(CMAKE_DEBUG_POSTFIX "_d")
endif ()

include(CheckTypeSize)
CHECK_TYPE_SIZE("void*" DI_EXTERNAL_PTR_SIZE BUILTIN_TYPES_ONLY)
if (DI_EXTERNAL_PTR_SIZE EQUAL 8)
  set(DI_EXTERNAL_PLATFORM_X64 TRUE)
else ()
  set(DI_EXTERNAL_PLATFORM_X64 FALSE)
endif ()

# Add -fPIC flag to x64 GCC builds
if (CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
  if (DI_EXTERNAL_PLATFORM_X64 AND NOT APPLE)
    add_definitions(-fPIC)
  endif ()
endif ()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: Debug, Release" FORCE)
endif ()

# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${DemiExternal_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${DemiExternal_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${DemiExternal_SOURCE_DIR}/bin")

add_subdirectory(src)