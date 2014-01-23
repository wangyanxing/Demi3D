#
# Copyright (c) 2012-2013 the Demi3D project.
#

# Find Microsoft DirectX installation
# The search paths are based on realXtend Tundra CMake build system (https://github.com/realXtend/naali)
#
#  DIRECT3D_FOUND
#  DIRECT3D_INCLUDE_DIR
#  DIRECT3D_LIBRARY
#  DIRECT3D_COMPILER_LIBRARY

if (NOT WIN32)
    return ()
endif ()
if (DIRECT3D_FOUND)
    return ()
endif ()

set (DIRECTX_INC_SEARCH_PATH
    "$ENV{DIRECTX_ROOT}/Include"
    "$ENV{DXSDK_DIR}/Include"
    "C:/Program Files (x86)/Microsoft DirectX SDK*/Include"
    "C:/Program Files/Microsoft DirectX SDK*/Include")
find_path (DIRECT3D_INCLUDE_DIR d3dx9.h ${DIRECTX_INC_SEARCH_PATH})

if (CMAKE_CL_64)
    set (DIRECTX_LIB_SEARCH_PATH
        "$ENV{DIRECTX_ROOT}/Lib/x64"
        "$ENV{DXSDK_DIR}/Lib/x64"
        "C:/Program Files (x86)/Microsoft DirectX SDK*/Lib/x64"
        "C:/Program Files/Microsoft DirectX SDK*/Lib/x64")
else ()
    set (DIRECTX_LIB_SEARCH_PATH
        "$ENV{DIRECTX_ROOT}/Lib"
        "$ENV{DIRECTX_ROOT}/Lib/x86"
        "$ENV{DXSDK_DIR}/Lib"
        "$ENV{DXSDK_DIR}/Lib/x86"
        "C:/Program Files (x86)/Microsoft DirectX SDK*/Lib"
        "C:/Program Files (x86)/Microsoft DirectX SDK*/Lib/x86"
        "C:/Program Files/Microsoft DirectX SDK*/Lib"
        "C:/Program Files/Microsoft DirectX SDK*/Lib/x86")
endif ()
find_library (DIRECT3D_LIBRARY d3d9 ${DIRECTX_LIB_SEARCH_PATH})
find_library (DIRECT3D_COMPILER_LIBRARY d3dcompiler ${DIRECTX_LIB_SEARCH_PATH})

if (DIRECT3D_INCLUDE_DIR AND DIRECT3D_LIBRARY AND DIRECT3D_COMPILER_LIBRARY)
    set (DIRECT3D_FOUND 1)
endif ()

if (DIRECT3D_FOUND)
    include (FindPackageMessage)
    FIND_PACKAGE_MESSAGE (Direct3D "Found DirectX SDK: ${DIRECT3D_LIBRARY} ${DIRECT3D_INCLUDE_DIR}" "[${DIRECT3D_LIBRARY}][${DIRECT3D_INCLUDE_DIR}]")
else ()
    message (STATUS "DirectX SDK not found. This is not fatal if a recent Windows SDK is installed")
    # Set non-absolute fallback library names and assume they are found in default library directories
    set (DIRECT3D_LIBRARY d3d9)
    set (DIRECT3D_COMPILER_LIBRARY d3dcompiler)
endif ()

mark_as_advanced (DIRECT3D_INCLUDE_DIR DIRECT3D_LIBRARY DIRECT3D_COMPILER_LIBRARY)
