#
# Copyright (c) 2012-2013 the Demi3D project.
#

if (CMAKE_GENERATOR STREQUAL Xcode)
    set (XCODE TRUE)
endif ()

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release)
endif ()

if (CMAKE_HOST_WIN32)
    execute_process (COMMAND uname -o COMMAND tr -d '\n' RESULT_VARIABLE UNAME_EXIT_CODE OUTPUT_VARIABLE UNAME_OPERATING_SYSTEM ERROR_QUIET)
    if (UNAME_EXIT_CODE EQUAL 0 AND UNAME_OPERATING_SYSTEM STREQUAL Msys)
        set (MSYS 1)
    endif ()
endif ()

# Mini dumps are only for visual c++ projects
if (MSVC)
    if (NOT DEFINED ENABLE_MINIDUMPS)
        set (ENABLE_MINIDUMPS 1)
    endif ()
    if (ENABLE_MINIDUMPS)
        add_definitions (-DENABLE_MINIDUMPS)
    endif ()
endif ()

if (MSVC)
    if (USE_STATIC_RUNTIME)
        set (RELEASE_RUNTIME /MT)
        set (DEBUG_RUNTIME /MTd)
    else ()
        set (RELEASE_RUNTIME "")
        set (DEBUG_RUNTIME "")
    endif ()
endif ()

# Unix mode
if (NOT WIN32)
    add_definitions (-DUNIX)
endif ()

# Dynamic libs for default 
if (DEMI3D_STATIC_API)
    add_definitions (-DDEMI_STATIC_API)
endif ()

# Find D3D SDK
if (WIN32)
    find_package (Direct3D)
    if (DIRECT3D_FOUND)
        include_directories (${DIRECT3D_INCLUDE_DIR})
    endif ()
endif ()

# Platform and compiler specific options
if (MSVC)
    # Visual Studio-specific setup
    add_definitions (-D_CRT_SECURE_NO_WARNINGS)
    # Note: All CMAKE_xxx_FLAGS variables are not in list context (although they should be)
    set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${DEBUG_RUNTIME}")
    set (CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELEASE} ${RELEASE_RUNTIME} /fp:fast /Zi /GS-")
    set (CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELWITHDEBINFO})
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${DEBUG_RUNTIME}")
    set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} ${RELEASE_RUNTIME} /fp:fast /Zi /GS- /D _SECURE_SCL=0")
    set (CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
    set (CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF /DEBUG")
    set (CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF")
endif ()

include (GenerateExportHeader)

get_filename_component (PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR} PATH)

IF(WIN32)
    IF(MSVC)
        SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Horde3D/Binaries
            CACHE STRING "Where binaries and .dll files go" FORCE)  
    ELSE(MSVC)
        SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Horde3D/Binaries/Win32
        CACHE STRING "Where binaries and .dll files go" FORCE)  
    ENDIF(MSVC)
ELSE(WIN32)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Horde3D/Binaries/${CMAKE_SYSTEM_NAME}
    CACHE STRING "Where binaries and .dll files go" FORCE)
ENDIF(WIN32)

