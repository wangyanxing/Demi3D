project(LinearMath)

# source files
file (GLOB C_FILES *.cpp)
file (GLOB H_FILES *.h)
set (SOURCE_FILES ${C_FILES} ${H_FILES})

add_library(LinearMath STATIC ${SOURCE_FILES})

if (APPLE)
 set_target_properties(LinearMath PROPERTIES XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "NO")

 if(DEMI_BUILD_PLATFORM_APPLE_IOS)
  set_target_properties(LinearMath PROPERTIES IPHONEOS_DEPLOYMENT_TARGET MIN_IOS_VERSION )
  set_target_properties(LinearMath PROPERTIES XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET[arch=arm64] "7.0" )
  set_target_properties(LinearMath PROPERTIES XCODE_ATTRIBUTE_GCC_THUMB_SUPPORT "NO")
 endif()
endif()

set(LinearMath_INCLUDE_DIR "${LinearMath_SOURCE_DIR}" CACHE PATH "" FORCE)
set(LinearMath_LIBRARY_DBG "LinearMath" CACHE STRING "" FORCE)
set(LinearMath_LIBRARY_REL "LinearMath" CACHE STRING "" FORCE)
mark_as_advanced(LinearMath_INCLUDE_DIR LinearMath_LIBRARY_DBG LinearMath_LIBRARY_REL)
