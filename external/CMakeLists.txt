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
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.6)

  if (NOT CMAKE_OSX_ARCHITECTURES)
    set(CMAKE_OSX_ARCHITECTURES ${ARCHS_STANDARD_32_64_BIT})
  endif()
endif ()

# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${DemiExternal_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${DemiExternal_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${DemiExternal_SOURCE_DIR}/bin")

add_subdirectory(src)