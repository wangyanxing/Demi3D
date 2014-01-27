
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

if (DEMI_BUILD_PLATFORM_APPLE_IOS)
  set(DEMI_SET_BUILD_PLATFORM_APPLE_IOS 1)
  set(DEMI_STATIC TRUE)
  set(DEMI_STATIC_LIB TRUE)
  set(DEMI_CONFIG_ENABLE_PVRTC TRUE)
endif()

if (DEMI_STATIC)
  set(DEMI_LIB_TYPE STATIC)
else ()
  set(DEMI_LIB_TYPE SHARED)
endif ()

if(DEMI_CONFIG_STATIC_LINK_CRT)
#We statically link to reduce dependancies
foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
    if(${flag_var} MATCHES "/MDd")
        string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MDd")
endforeach(flag_var)
endif(DEMI_CONFIG_STATIC_LINK_CRT)