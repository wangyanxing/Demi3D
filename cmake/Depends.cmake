
include(FindPkgMacros)

if(NOT DEMI_BUILD_PLATFORM_APPLE_IOS)
# Find OpenGL
find_package(OpenGL)
macro_log_feature(OPENGL_FOUND "OpenGL" "Support for the OpenGL render system" "http://www.opengl.org/" FALSE "" "")
endif()

# Find DirectX
if(WIN32)
	find_package(DirectX)
	macro_log_feature(DirectX_FOUND "DirectX" "Support for the DirectX render system" "http://msdn.microsoft.com/en-us/directx/" FALSE "" "")
endif()

# Find OpenGL ES 2.x
find_package(OpenGLES2)
macro_log_feature(OPENGLES2_FOUND "OpenGL ES 2.x" "Support for the OpenGL ES 2.x render system" "http://www.khronos.org/opengles/" FALSE "" "")

if (NOT OPENGLES2_FOUND)
  MESSAGE(STATUS "Could not find dependency: GLES2 ")
else()
  MESSAGE(STATUS "Found dependency: GLES2 ")
endif()

set(EXTERNAL_HOME "${DEMI_SOURCE_DIR}/external")

if(DEMI_BUILD_PLATFORM_APPLE_IOS)
	set(EXTERNAL_LIB "${DEMI_SOURCE_DIR}/external/lib_ios")
else()
	set(EXTERNAL_LIB "${DEMI_SOURCE_DIR}/external/lib")
endif()

#-------------------------------------------------------------------
# Find OIS
# - Try to find OIS
# Once done, this will define
#
#  OIS_FOUND - system has OIS
#  OIS_INCLUDE_DIRS - the OIS include directories 
#  OIS_LIBRARIES - link these to use OIS
#  OIS_BINARY_REL / OIS_BINARY_DBG - DLL names (windows only)

findpkg_begin(OIS)
set(OIS_HOME "${EXTERNAL_HOME}/src/ois")

set(OIS_LIBRARY_NAMES OIS)
get_debug_names(OIS_LIBRARY_NAMES)

if (OIS_HOME)
  set(OIS_INC_SEARCH_PATH ${OIS_INC_SEARCH_PATH} ${OIS_HOME}/includes)
  set(OIS_LIB_SEARCH_PATH ${OIS_LIB_SEARCH_PATH} ${EXTERNAL_LIB})
endif()

find_path(OIS_INCLUDE_DIR NAMES OIS.h HINTS ${OIS_INC_SEARCH_PATH} ${OIS_PKGC_INCLUDE_DIRS} PATH_SUFFIXES OIS)
find_library(OIS_LIBRARY_REL NAMES ${OIS_LIBRARY_NAMES} HINTS ${OIS_LIB_SEARCH_PATH} ${OIS_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(OIS_LIBRARY_DBG NAMES ${OIS_LIBRARY_NAMES_DBG} HINTS ${OIS_LIB_SEARCH_PATH} ${OIS_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" debug)

make_library_set(OIS_LIBRARY)

if (WIN32)
	set(OIS_BIN_SEARCH_PATH ${EXTERNAL_HOME}/bin)
	find_file(OIS_BINARY_REL NAMES "OIS.dll" HINTS ${OIS_BIN_SEARCH_PATH}
	  PATH_SUFFIXES "" release relwithdebinfo minsizerel)
	find_file(OIS_BINARY_DBG NAMES "OIS_d.dll" HINTS ${OIS_BIN_SEARCH_PATH}
	  PATH_SUFFIXES "" debug )
endif()
mark_as_advanced(OIS_BINARY_REL OIS_BINARY_DBG)


findpkg_finish(OIS)

#-------------------------------------------------------------------
# Find STB
# - Try to find STB
# Once done, this will define
#
#  STB_FOUND - system has STB
#  STB_INCLUDE_DIRS - the STB include directories 
#  STB_LIBRARIES - link these to use STB

findpkg_begin(STB)
set(STB_HOME "${EXTERNAL_HOME}/src/STB")

set(STB_LIBRARY_NAMES STB)
get_debug_names(STB_LIBRARY_NAMES)

if (STB_HOME)
  set(STB_INC_SEARCH_PATH ${STB_INC_SEARCH_PATH} ${STB_HOME})
  set(STB_LIB_SEARCH_PATH ${STB_LIB_SEARCH_PATH} ${EXTERNAL_LIB})
endif()

find_path(STB_INCLUDE_DIR NAMES stb_image.h HINTS ${STB_INC_SEARCH_PATH} ${STB_PKGC_INCLUDE_DIRS} PATH_SUFFIXES STB)
find_library(STB_LIBRARY_REL NAMES ${STB_LIBRARY_NAMES} HINTS ${STB_LIB_SEARCH_PATH} ${STB_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(STB_LIBRARY_DBG NAMES ${STB_LIBRARY_NAMES_DBG} HINTS ${STB_LIB_SEARCH_PATH} ${STB_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" debug)

make_library_set(STB_LIBRARY)

findpkg_finish(STB)

#-------------------------------------------------------------------
# Find lua
# - Try to find lua
# Once done, this will define
#
#  LUA_FOUND - system has lua
#  LUA_INCLUDE_DIRS - the lua include directories 
#  LUA_LIBRARIES - link these to use lua

findpkg_begin(LUA)
set(LUA_HOME "${EXTERNAL_HOME}/src/lua")

set(LUA_LIBRARY_NAMES lua)
get_debug_names(LUA_LIBRARY_NAMES)

if (LUA_HOME)
  set(LUA_INC_SEARCH_PATH ${LUA_INC_SEARCH_PATH} ${LUA_HOME}/src)
  set(LUA_LIB_SEARCH_PATH ${LUA_LIB_SEARCH_PATH} ${EXTERNAL_LIB})
endif()

find_path(LUA_INCLUDE_DIR NAMES lua.h HINTS ${LUA_INC_SEARCH_PATH} ${LUA_PKGC_INCLUDE_DIRS} PATH_SUFFIXES lua)
find_library(LUA_LIBRARY_REL NAMES ${LUA_LIBRARY_NAMES} HINTS ${LUA_LIB_SEARCH_PATH} ${LUA_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(LUA_LIBRARY_DBG NAMES ${LUA_LIBRARY_NAMES_DBG} HINTS ${LUA_LIB_SEARCH_PATH} ${LUA_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" debug)

make_library_set(LUA_LIBRARY)

findpkg_finish(LUA)

#-------------------------------------------------------------------
# Find zlib
# - Try to find zlib
# Once done, this will define
#
#  ZLIB_FOUND - system has ZLIB
#  ZLIB_INCLUDE_DIRS - the ZLIB include directories 
#  ZLIB_LIBRARIES - link these to use ZLIB

findpkg_begin(ZLIB)
set(ZLIB_HOME "${EXTERNAL_HOME}/src/zlib")

set(ZLIB_LIBRARY_NAMES ZLIB)
get_debug_names(ZLIB_LIBRARY_NAMES)

if (ZLIB_HOME)
  set(ZLIB_INC_SEARCH_PATH ${ZLIB_INC_SEARCH_PATH} ${ZLIB_HOME})
  set(ZLIB_LIB_SEARCH_PATH ${ZLIB_LIB_SEARCH_PATH} ${EXTERNAL_LIB})
endif()

find_path(ZLIB_INCLUDE_DIR NAMES zlib.h HINTS ${ZLIB_INC_SEARCH_PATH} ${ZLIB_PKGC_INCLUDE_DIRS} PATH_SUFFIXES ZLIB)
find_library(ZLIB_LIBRARY_REL NAMES ${ZLIB_LIBRARY_NAMES} HINTS ${ZLIB_LIB_SEARCH_PATH} ${ZLIB_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(ZLIB_LIBRARY_DBG NAMES ${ZLIB_LIBRARY_NAMES_DBG} HINTS ${ZLIB_LIB_SEARCH_PATH} ${ZLIB_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" debug)

make_library_set(ZLIB_LIBRARY)

findpkg_finish(ZLIB)

#-------------------------------------------------------------------
# Find zziplib
# - Try to find zziplib
# Once done, this will define
#
#  ZZIPLIB_FOUND - system has ZZIPLIB
#  ZZIPLIB_INCLUDE_DIRS - the ZZIPLIB include directories 
#  ZZIPLIB_LIBRARIES - link these to use ZZIPLIB

findpkg_begin(ZZIPLIB)
set(ZZIPLIB_HOME "${EXTERNAL_HOME}/src/zziplib/zzip")

set(ZZIPLIB_LIBRARY_NAMES ZZIPLIB)
get_debug_names(ZZIPLIB_LIBRARY_NAMES)

if (ZZIPLIB_HOME)
  set(ZZIPLIB_INC_SEARCH_PATH ${ZZIPLIB_INC_SEARCH_PATH} ${ZZIPLIB_HOME})
  set(ZZIPLIB_LIB_SEARCH_PATH ${ZZIPLIB_LIB_SEARCH_PATH} ${EXTERNAL_LIB})
endif()

find_path(ZZIPLIB_INCLUDE_DIR NAMES zzip.h HINTS ${ZZIPLIB_INC_SEARCH_PATH} ${ZZIPLIB_PKGC_INCLUDE_DIRS} PATH_SUFFIXES ZZIPLIB)
find_library(ZZIPLIB_LIBRARY_REL NAMES ${ZZIPLIB_LIBRARY_NAMES} HINTS ${ZZIPLIB_LIB_SEARCH_PATH} ${ZZIPLIB_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(ZZIPLIB_LIBRARY_DBG NAMES ${ZZIPLIB_LIBRARY_NAMES_DBG} HINTS ${ZZIPLIB_LIB_SEARCH_PATH} ${ZZIPLIB_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" debug)

make_library_set(ZZIPLIB_LIBRARY)

findpkg_finish(ZZIPLIB)

#-------------------------------------------------------------------
# Find Ogre
# - Try to find Ogre
# Once done, this will define
#
#  OGRE_FOUND - system has Ogre
#  OGRE_INCLUDE_DIRS - the Ogre include directories 
#  OGRE_LIBRARIES - link these to use Ogre
#  OGRE_BINARY_REL / OGRE_BINARY_DBG - DLL names (windows only)

if (WIN32)
  findpkg_begin(OGRE)
  set(OGRE_INCLUDE_DIRS "${EXTERNAL_HOME}/include/ogremain")
  
  set(OGRE_LIBRARY_NAMES OgreMain)
  get_debug_names(OGRE_LIBRARY_NAMES)
  
  if (OGRE_INCLUDE_DIRS)
    set(OGRE_INC_SEARCH_PATH ${ORRE_INC_SEARCH_PATH} ${OGRE_INCLUDE_DIRS})
    set(OGRE_LIB_SEARCH_PATH ${OGRE_LIB_SEARCH_PATH} ${EXTERNAL_LIB})
  endif()
  
  find_path(OGRE_INCLUDE_DIR NAMES Ogre.h HINTS ${OGRE_INC_SEARCH_PATH} ${OGRE_PKGC_INCLUDE_DIRS} PATH_SUFFIXES OGRE)
  find_library(OGRE_LIBRARY_REL NAMES ${OGRE_LIBRARY_NAMES} HINTS ${OGRE_LIB_SEARCH_PATH} ${OGRE_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
  find_library(OGRE_LIBRARY_DBG NAMES ${OGRE_LIBRARY_NAMES_DBG} HINTS ${OGRE_LIB_SEARCH_PATH} ${OGRE_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" debug)
  
  make_library_set(OGRE_LIBRARY)
  
  if (WIN32)
  	set(OGRE_BIN_SEARCH_PATH ${EXTERNAL_HOME}/bin)
  	find_file(OGRE_BINARY_REL NAMES "OgreMain.dll" HINTS ${OGRE_BIN_SEARCH_PATH}
  	  PATH_SUFFIXES "" release relwithdebinfo minsizerel)
  	find_file(OGRE_BINARY_DBG NAMES "OgreMain_d.dll" HINTS ${OGRE_BIN_SEARCH_PATH}
  	  PATH_SUFFIXES "" debug )
  endif()
  mark_as_advanced(OGRE_BINARY_REL OGRE_BINARY_DBG)
  
  findpkg_finish(OGRE)
endif()

if (APPLE)
  find_package(iOSSDK)
  macro_log_feature(iOSSDK_FOUND "iOS SDK" "iOS SDK" "http://developer.apple.com/ios" FALSE "" "")
  
  if (NOT DEMI_BUILD_PLATFORM_APPLE_IOS)
    find_package(Carbon)
    macro_log_feature(Carbon_FOUND "Carbon" "Carbon" "http://developer.apple.com/mac" TRUE "" "")

    find_package(Cocoa)
    macro_log_feature(Cocoa_FOUND "Cocoa" "Cocoa" "http://developer.apple.com/mac" TRUE "" "")

    find_package(IOKit)
    macro_log_feature(IOKit_FOUND "IOKit" "IOKit HID framework needed by the samples" "http://developer.apple.com/mac" FALSE "" "")

    find_package(CoreVideo)
    macro_log_feature(CoreVideo_FOUND "CoreVideo" "CoreVideo" "http://developer.apple.com/mac" TRUE "" "")
  endif (NOT DEMI_BUILD_PLATFORM_APPLE_IOS)
endif(APPLE)