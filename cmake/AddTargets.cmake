
# add a new library target
# usage: demi_add_library(TARGETNAME LIBTYPE SOURCE_FILES [SEPARATE SOURCE_FILES])
MACRO(DI_ADD_LIBRARY TARGETNAME)
  if(DEMI_STATIC)
    add_library(${TARGETNAME} STATIC ${ARGN})
  else(DEMI_STATIC)
    add_library(${TARGETNAME} SHARED ${ARGN})
  endif(DEMI_STATIC)
ENDMACRO(DI_ADD_LIBRARY)


# add a new executable target
# usage: demi_add_executable(TARGETNAME [WIN32] [MACOSX_BUNDLE] SOURCE_FILES [SEPARATE SOURCE_FILES])
function(demi_add_executable TARGETNAME)
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
  add_executable(${TARGETNAME} ${_WIN32} ${_OSX} ${_SOURCES})
endfunction()
