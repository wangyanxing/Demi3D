/**********************************************************************
This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/
#ifndef DiMemoryConfig_h__
#define DiMemoryConfig_h__


//#define ENABLE_MEMORY_TRACE
#ifdef ENABLE_MEMORY_TRACE
#    define USE_MEMORY_TRACE 1
#    pragma message("_memory trace enabled")
#else
#    define USE_MEMORY_TRACE 0
#endif

#define MEMORY_ALLOCATOR_STANDARD 1

#define MEMORY_ALLLOCATOR_USING MEMORY_ALLOCATOR_STANDARD

#ifdef DI_NEW
#    undef DI_NEW
#endif

#ifdef DI_DELETE
#    undef DI_DELETE
#endif

#ifdef new
#    undef new
#endif

//#if USE_MEMORY_TRACE    
//#    define DI_NEW new(__FILE__, __LINE__)
//#    define DI_DELETE delete
//#else
//#    define DI_NEW new
//#    define DI_DELETE delete
//#endif

#define DI_NEW new
#define DI_DELETE delete



#endif
