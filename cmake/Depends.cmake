
include(FindPkgMacros)

# Find OpenGL
find_package(OpenGL)
macro_log_feature(OPENGL_FOUND "OpenGL" "Support for the OpenGL render system" "http://www.opengl.org/" FALSE "" "")

# Find DirectX
if(WIN32)
	find_package(DirectX)
	macro_log_feature(DirectX_FOUND "DirectX" "Support for the DirectX render system" "http://msdn.microsoft.com/en-us/directx/" FALSE "" "")
endif()

set(EXTERNAL_HOME "${DEMI_SOURCE_DIR}/external")

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
  set(OIS_LIB_SEARCH_PATH ${OIS_LIB_SEARCH_PATH} ${EXTERNAL_HOME}/lib)
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
# Find Ogre
# - Try to find Ogre
# Once done, this will define
#
#  OGRE_FOUND - system has Ogre
#  OGRE_INCLUDE_DIRS - the Ogre include directories 
#  OGRE_LIBRARIES - link these to use Ogre
#  OGRE_BINARY_REL / OGRE_BINARY_DBG - DLL names (windows only)

findpkg_begin(OGRE)
set(OGRE_INCLUDE_DIRS "${EXTERNAL_HOME}/include/ogremain")

set(OGRE_LIBRARY_NAMES OgreMain)
get_debug_names(OGRE_LIBRARY_NAMES)

if (OGRE_INCLUDE_DIRS)
  set(OGRE_INC_SEARCH_PATH ${ORRE_INC_SEARCH_PATH} ${OGRE_INCLUDE_DIRS})
  set(OGRE_LIB_SEARCH_PATH ${OGRE_LIB_SEARCH_PATH} ${EXTERNAL_HOME}/lib)
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