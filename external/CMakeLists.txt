if (APPLE AND NOT ANDROID)
  cmake_minimum_required(VERSION 2.8.8)
else()
  cmake_minimum_required(VERSION 2.6)
endif ()

project (DemiExternal)

set(CMAKE_MODULE_PATH "${DemiExternal_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

if (WIN32)
  find_package(DirectX REQUIRED)
  add_definitions(-DWINVER=0x0500)
endif ()

if (APPLE AND DEMI_BUILD_PLATFORM_APPLE_IOS)
  # Set build variables
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
  set(XCODE_ATTRIBUTE_SDKROOT iphoneos)
  set(CMAKE_OSX_SYSROOT iphoneos)
  if(XCODE_VERSION VERSION_LESS 5.0)
    set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_32_BIT)")
  else()
    set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_INCLUDING_64_BIT)")
  endif()
  add_definitions(-DHAVE_UNISTD_H=1)
elseif (APPLE AND NOT DEMI_BUILD_PLATFORM_APPLE_IOS)
  # Set 10.7 as the base SDK by default
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

if (APPLE AND NOT DEMI_BUILD_PLATFORM_APPLE_IOS)
  include(CMakeForceCompiler)
  CMAKE_FORCE_C_COMPILER(clang GNU)
  CMAKE_FORCE_CXX_COMPILER(clang++ GNU)
  SET(CMAKE_SIZEOF_VOID_P 4)
  set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
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
if (APPLE AND DEMI_BUILD_PLATFORM_APPLE_IOS)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${DemiExternal_SOURCE_DIR}/lib_ios")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${DemiExternal_SOURCE_DIR}/lib_ios")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${DemiExternal_SOURCE_DIR}/bin_ios")
else()
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${DemiExternal_SOURCE_DIR}/lib")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${DemiExternal_SOURCE_DIR}/lib")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${DemiExternal_SOURCE_DIR}/bin")
endif()

add_subdirectory(src)