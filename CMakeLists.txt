#
# Copyright (c) 2013-2014 the Demi3D project.
#

# Set minimum version
cmake_minimum_required (VERSION 2.8.3)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

if(WIN32)
  # This seems to break Xcode projects so definitely don't enable on Apple builds
  set(CMAKE_USE_RELATIVE_PATHS true)
  set(CMAKE_SUPPRESS_REGENERATION true)
endif()

if (APPLE)
  include(CMakeForceCompiler)
  CMAKE_FORCE_C_COMPILER(clang GNU)
  CMAKE_FORCE_CXX_COMPILER(clang++ GNU)
  SET(CMAKE_SIZEOF_VOID_P 4)
  set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
endif ()

if (WIN32)
  add_definitions(-DWINVER=0x0500)
endif ()

# Set project name
project (DEMI)

# Include necessary submodules
set(CMAKE_MODULE_PATH "${DEMI_SOURCE_DIR}/cmake")

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()


# determine if we are compiling for a 32 or 64bit plartform
include(CheckTypeSize)
CHECK_TYPE_SIZE("void*" DEMI_PTR_SIZE BUILTIN_TYPES_ONLY)

if (DEMI_PTR_SIZE EQUAL 8)
  set(DEMI_PLATFORM_X64 TRUE)
else ()
  set(DEMI_PLATFORM_X64 FALSE)
endif ()

if (NOT APPLE)
  # Create debug libraries with _d postfix
  set(CMAKE_DEBUG_POSTFIX "_d")
endif ()

include(CheckCXXCompilerFlag)
include(FindFrameworks)
include(MacroLogFeature)
include(Depends)
include(AddTargets)
include(AddSamples)

# Set compiler specific build flags
if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
  CHECK_CXX_COMPILER_FLAG(-msse DEMI_GCC_HAS_SSE)
  if (DEMI_GCC_HAS_SSE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse")
  endif ()
  # This is a set of sensible warnings that provide meaningful output
  set(DEMI_WARNING_FLAGS "-Wno-deprecated -Wall -Wctor-dtor-privacy -Winit-self -Wno-overloaded-virtual -Wcast-qual -Wwrite-strings -Wextra -Wno-unused-parameter -pedantic -Wshadow -Wno-missing-field-initializers -Wno-long-long")
  if (NOT APPLE)
      set(DEMI_WARNING_FLAGS "${DEMI_WARNING_FLAGS} -Wno-unused-but-set-parameter")
  endif ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEMI_WARNING_FLAGS}")
endif ()

if (MSVC)
  if (CMAKE_BUILD_TOOL STREQUAL "nmake")
    # set variable to state that we are using nmake makefiles
	set(NMAKE TRUE)
  endif ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast")
  # Enable intrinsics on MSVC in debug mode
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Oi")
  if (CMAKE_CL_64)
    # Visual Studio bails out on debug builds in 64bit mode unless
	# this flag is set...
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /bigobj")
  endif ()
endif ()

if (MINGW)
  add_definitions(-D_WIN32_WINNT=0x0500)
  # set architecture to i686, since otherwise some versions of MinGW can't link 
  # the atomic primitives
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=i686")
  # disable this optimisation because it breaks release builds (reason unknown)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-tree-slp-vectorize")
  
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-keep-inline-dllexport")
  
endif ()

if(CMAKE_COMPILER_IS_GNUCXX AND NOT APPLE)
    add_definitions(-std=gnu++0x)
elseif (APPLE)
    add_definitions("-std=c++11")
endif()

if ((CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX) AND NOT MINGW)
  # Test for GCC visibility
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag(-fvisibility=hidden DEMI_GCC_VISIBILITY)
  if (DEMI_GCC_VISIBILITY)
    # determine gcc version
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion 
      OUTPUT_VARIABLE DEMI_GCC_VERSION)
    message(STATUS "Detected g++ ${DEMI_GCC_VERSION}")
    message(STATUS "Enabling GCC visibility flags")
    set(DEMI_GCC_VISIBILITY_FLAGS "-DDEMI_GCC_VISIBILITY -fvisibility=hidden")
    set(XCODE_ATTRIBUTE_GCC_SYMBOLS_PRIVATE_EXTERN "YES")
  
    # check if we can safely add -fvisibility-inlines-hidden
    string(TOLOWER "${CMAKE_BUILD_TYPE}" DEMI_BUILD_TYPE)
    if (DEMI_BUILD_TYPE STREQUAL "debug" AND DEMI_GCC_VERSION VERSION_LESS "4.2")
      message(STATUS "Skipping -fvisibility-inlines-hidden due to possible bug in g++ < 4.2")
    else ()
      if (APPLE AND NOT DEMI_BUILD_PLATFORM_APPLE_IOS)
        message(STATUS "Skipping -fvisibility-inlines-hidden due to linker issues")
        set(XCODE_ATTRIBUTE_GCC_INLINES_ARE_PRIVATE_EXTERN[arch=x86_64] "YES")
      else()
        set(DEMI_GCC_VISIBILITY_FLAGS "${DEMI_GCC_VISIBILITY_FLAGS} -fvisibility-inlines-hidden")
        set(XCODE_ATTRIBUTE_GCC_INLINES_ARE_PRIVATE_EXTERN "YES")
      endif()
    endif ()
  endif (DEMI_GCC_VISIBILITY)
  
  # Fix x64 issues on Linux
  if(DEMI_PLATFORM_X64 AND NOT APPLE)
    add_definitions(-fPIC)
  endif()  
endif ((CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX) AND NOT MINGW)

# determine system endianess
if (MSVC)
  # This doesn't work on VS 2010
  # MSVC only builds for intel anyway
  set(DEMI_TEST_BIG_ENDIAN FALSE)
else()
  include(TestBigEndian)
  test_big_endian(DEMI_TEST_BIG_ENDIAN)
endif()

# Add core lib include path
include_directories("${DEMI_SOURCE_DIR}/src/demi/misc")
include_directories("${DEMI_SOURCE_DIR}/src/demi/gfx")

# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${DEMI_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${DEMI_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${DEMI_SOURCE_DIR}/bin")


# Set up iOS overrides.
if (DEMI_BUILD_PLATFORM_APPLE_IOS)
  # Set build variables
  set(CMAKE_OSX_SYSROOT iphoneos)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "")
  set(CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework CoreGraphics -framework QuartzCore -framework UIKit")
  set(XCODE_ATTRIBUTE_SDKROOT iphoneos)
  set(XCODE_ATTRIBUTE_GCC_UNROLL_LOOPS "YES")
  set(XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
  set(XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")
  set(DEMI_STATIC TRUE CACHE BOOL "Forcing static build for iOS" FORCE)
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourcompany.\${PRODUCT_NAME:rfc1034identifier}")
  add_definitions(-mno-thumb)

  set(CMAKE_OSX_ARCHITECTURES ${ARCHS_STANDARD_32_BIT})
  # armv7 only
  #set(CMAKE_OSX_ARCHITECTURES ${ARCHS_UNIVERSAL_IPHONE_OS})

  remove_definitions(-msse)
elseif (APPLE AND NOT DEMI_BUILD_PLATFORM_APPLE_IOS)

  # Set 10.5 as the base SDK by default
  set(XCODE_ATTRIBUTE_SDKROOT macosx)
  set(CMAKE_OSX_SYSROOT macosx)
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.5)

  if (NOT CMAKE_OSX_ARCHITECTURES)
    set(CMAKE_OSX_ARCHITECTURES ${ARCHS_STANDARD_32_64_BIT})
  endif()

endif ()

# Building configuration
include(BuildConfig)

add_subdirectory(src)

# Solution Folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
