/**************************************************************************
    File:        CommonPCH.h
    Creator:    demiwangya
***************************************************************************/

#pragma once

//#ifndef WINVER            
//#define WINVER 0x0501    
//#endif
//
//#ifndef _WIN32_WINNT        
//#define _WIN32_WINNT 0x0501    
//#endif                        
//
//#ifndef _WIN32_WINDOWS            
//#define _WIN32_WINDOWS 0x0410   
//#endif
//
//#ifndef _WIN32_IE   
//#define _WIN32_IE 0x0600    
//#endif
//
//#define WIN32_LEAN_AND_MEAN 

//#include <windows.h>

//#define _CRTDBG_MAP_ALLOC
//#define _CRTDBG_MAP_ALLOC_NEW
//#include <crtdbg.h>

#include <tchar.h>
#include <stdio.h>

#include <utility>
#include <assert.h>
#include <limits>
#include <math.h>

#include "MiscDefines.h"
#include "SmartPtr.h"
#include "FileStream.h"
#include "MiscPrerequisites.h"
#include "MemoryConfig.h"
#include "Str.h"
#include "LogManager.h"

#include "Thread.h"

#include "Vec.h"
#include "List.h"
#include "Map.h"
#include "Set.h"
#include "HashMap.h"
#include "Pair.h"
#include "Sort.h"

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "quaternion.h"
#include "color.h"
#include "ray.h"
#include "plane.h"
#include "matrix3.h"
#include "matrix4.h"
#include "axisAlignedBox.h"
#include "mathMisc.h"
#include "polygen.h"

#include "DynamicAttribute.h"
#include "DynamicAttributeFactory.h"
#include "StlUtil.h"

//#define ENABLE_PROFILE

#include "Profiler.h"


