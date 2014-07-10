project(ZipLib)

file(GLOB ZIPLIB_SRCS "*.cpp" "detail/*.cpp")

if (APPLE)
	file(GLOB ZIPLIB_C_SRCS "extlibs/bzip2/*.c" "extlibs/lzma/unix/*.c" "extlibs/zlib/*.c")
else()
	file(GLOB ZIPLIB_C_SRCS "extlibs/bzip2/*.c" "extlibs/lzma/*.c" "extlibs/zlib/*.c")
endif()

# need zlib includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
  add_definitions(-D_MSC_VER)
endif ()

add_library(ZipLib STATIC ${ZIPLIB_SRCS} ${ZIPLIB_C_SRCS})

SET_SOURCE_FILES_PROPERTIES( ${ZIPLIB_SRCS} PROPERTIES LANGUAGE CXX )
SET_SOURCE_FILES_PROPERTIES( ${ZIPLIB_C_SRCS} PROPERTIES LANGUAGE C )

if (APPLE)
 set_target_properties(ZipLib PROPERTIES XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "NO")

 if(DEMI_BUILD_PLATFORM_APPLE_IOS)
  set_target_properties(ZipLib PROPERTIES IPHONEOS_DEPLOYMENT_TARGET MIN_IOS_VERSION )
  set_target_properties(ZipLib PROPERTIES XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET[arch=arm64] "7.0" )
  set_target_properties(ZipLib PROPERTIES XCODE_ATTRIBUTE_GCC_THUMB_SUPPORT "NO")
 endif()
endif()
set(ZZip_INCLUDE_DIR "${ZipLib_SOURCE_DIR}" CACHE PATH "" FORCE)
set(ZZip_LIBRARY_DBG "ZipLib" CACHE STRING "" FORCE)
set(ZZip_LIBRARY_REL "ZipLib" CACHE STRING "" FORCE)
mark_as_advanced(ZZip_INCLUDE_DIR ZZip_LIBRARY_DBG ZZip_LIBRARY_REL)
