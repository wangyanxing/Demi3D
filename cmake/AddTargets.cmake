
# add a new library target
# usage: demi_add_library(TARGETNAME LIBTYPE SOURCE_FILES [SEPARATE SOURCE_FILES])
MACRO(DI_ADD_LIBRARY TARGETNAME)
  if(DEMI_STATIC)
    add_library(${TARGETNAME} STATIC ${ARGN})
  else(DEMI_STATIC)
    add_library(${TARGETNAME} SHARED ${ARGN})
  endif(DEMI_STATIC)

  if (DEMI_STATIC)
    # add static prefix, if compiling static version
    set_target_properties(${TARGETNAME} PROPERTIES OUTPUT_NAME ${PLUGINNAME}Static)

    if(DEMI_BUILD_PLATFORM_APPLE_IOS)
      set_target_properties(${TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_UNROLL_LOOPS "YES")
      set_target_properties(${TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")
    endif(DEMI_BUILD_PLATFORM_APPLE_IOS)
  else (DEMI_STATIC)
    if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
      # add GCC visibility flags to shared library build
      set_target_properties(${TARGETNAME} PROPERTIES COMPILE_FLAGS "${DEMI_GCC_VISIBILITY_FLAGS}")
      set_target_properties(${TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_SYMBOLS_PRIVATE_EXTERN "${XCODE_ATTRIBUTE_GCC_SYMBOLS_PRIVATE_EXTERN}")
      set_target_properties(${TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "NO")
      set_target_properties(${TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_INLINES_ARE_PRIVATE_EXTERN[arch=x86_64] "YES")
      # disable "lib" prefix on Unix
      set_target_properties(${TARGETNAME} PROPERTIES PREFIX "")
    endif (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
  endif (DEMI_STATIC)

ENDMACRO(DI_ADD_LIBRARY)


# add a new executable target
# usage: demi_add_executable(TARGETNAME [WIN32] [MACOSX_BUNDLE] SOURCE_FILES [SEPARATE SOURCE_FILES])
MACRO(DI_ADD_EXECUTABLE TARGETNAME)
  # test if WIN32 or MACOSX_BUNDLE options were provided
  set(_WIN32 "")
  set(_OSX "")
  list(FIND ARGN "WIN32" _W32_IDX)
  if (_W32_IDX GREATER "-1")
    set(_WIN32 "WIN32")
    list(REMOVE_AT ARGN ${_W32_IDX})
  endif ()
  list(FIND ARGN "MACOSX_BUNDLE" _OSX_IDX)
  if (_OSX_IDX GREATER "-1")
    set(_OSX "MACOSX_BUNDLE")
    list(REMOVE_AT ARGN ${_OSX_IDX})
  endif ()
  add_executable(${TARGETNAME} ${_WIN32} ${_OSX} ${ARGN})

  if (APPLE)
    # On OS X, create .app bundle
    set_property(TARGET ${TARGETNAME} PROPERTY MACOSX_BUNDLE TRUE)
    if (NOT DEMI_BUILD_PLATFORM_APPLE_IOS)
      # Add the path where the Demi3D framework was found
      if(${DEMI_FRAMEWORK_PATH})
        set_target_properties(${TARGETNAME} PROPERTIES
          COMPILE_FLAGS "-F${DEMI_FRAMEWORK_PATH}"
          LINK_FLAGS "-F${DEMI_FRAMEWORK_PATH}"
        )
      endif()
    endif(NOT DEMI_BUILD_PLATFORM_APPLE_IOS)
  endif (APPLE)
  if (NOT DEMI_STATIC)
    if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
      # add GCC visibility flags to shared library build
      set_target_properties(${TARGETNAME} PROPERTIES COMPILE_FLAGS "${DEMI_GCC_VISIBILITY_FLAGS}")
      set_target_properties(${TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_SYMBOLS_PRIVATE_EXTERN "${XCODE_ATTRIBUTE_GCC_SYMBOLS_PRIVATE_EXTERN}")
      set_target_properties(${TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "NO")
      set_target_properties(${TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_INLINES_ARE_PRIVATE_EXTERN[arch=x86_64] "YES")
      # disable "lib" prefix on Unix
      set_target_properties(${TARGETNAME} PROPERTIES PREFIX "")
    endif (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
  endif()
ENDMACRO(DI_ADD_EXECUTABLE)
