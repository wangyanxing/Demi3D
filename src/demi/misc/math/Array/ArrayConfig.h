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

/// This file is adapted from Ogre 2.0 (unstable version)
#ifndef __ArrayConfig_H__
#define __ArrayConfig_H__

#include "PlatformSpecific.h"

#if DEMI_USE_SIMD == 1
    #if DEMI_CPU == DEMI_CPU_X86
        //x86/x64 - SSE2
        #if DEMI_DOUBLE_PRECISION == 1
            #include <emmintrin.h>
            #define ARRAY_PACKED_REALS 2
            namespace Demi 
            {
                typedef __m128d ArrayFloat;
            }
        #else
            #include <xmmintrin.h>
            #include <emmintrin.h>  //SSE Math library still needs SSE2
            #define ARRAY_PACKED_REALS 4
            namespace Demi 
            {
                typedef __m128 ArrayFloat;
                typedef __m128 ArrayMaskR;

                #define ARRAY_REAL_ZERO _mm_setzero_ps()
                #define ARRAY_INT_ZERO _mm_setzero_si128()

                class ArrayRadian;
            }

            #define DEMI_PREFETCH_T0( x ) _mm_prefetch( x, _MM_HINT_T0 )
            #define DEMI_PREFETCH_T1( x ) _mm_prefetch( x, _MM_HINT_T1 )
            #define DEMI_PREFETCH_T2( x ) _mm_prefetch( x, _MM_HINT_T2 )
            #define DEMI_PREFETCH_NTA( x ) _mm_prefetch( x, _MM_HINT_NTA )

            //Distance (in ArrayMemoryManager's slots) used to keep fetching data. This also
            //means the memory manager needs to allocate extra memory for them.
            #define DEMI_PREFETCH_SLOT_DISTANCE     4*ARRAY_PACKED_REALS //Must be multiple of ARRAY_PACKED_REALS
        #endif

        namespace Demi 
        {
            typedef __m128i ArrayInt;
            typedef __m128i ArrayMaskI;
        }

        ///r = (a * b) + c
        #define _mm_madd_ps( a, b, c )      _mm_add_ps( c, _mm_mul_ps( a, b ) )
        ///r = -(a * b) + c
        #define _mm_nmsub_ps( a, b, c )     _mm_sub_ps( c, _mm_mul_ps( a, b ) )

        /// Does not convert, just cast ArrayFloat to ArrayInt
        #define CastfloatToInt( x )          _mm_castps_si128( x )
        #define CastIntTofloat( x )          _mm_castsi128_ps( x )
        /// Input must be 16-byte aligned
        #define CastArrayTofloat( outFloatPtr, arraySimd )       _mm_store_ps( outFloatPtr, arraySimd )

    #elif DEMI_CPU == DEMI_CPU_ARM
        // ARM - NEON
        #include <arm_neon.h>
        #if DEMI_DOUBLE_PRECISION == 1
            #error Double precision with SIMD on ARM is not supported
        #else
            #define ARRAY_PACKED_REALS 4
            namespace Demi 
            {
                typedef float32x4_t ArrayFloat;
                typedef float32x4_t ArrayMaskR;

                #define ARRAY_REAL_ZERO vdupq_n_f32( 0.0f )
                #define ARRAY_INT_ZERO vdupq_n_s32( 0.0f )

                class ArrayRadian;
            }

            // Make sure that we have the preload macro. Might not be available with some compilers.
            #ifndef __pld
            #define __pld(x) asm volatile ( "pld [%[addr]]\n" :: [addr] "r" (x) : "cc" );
            #endif

            #if defined(__arm64__)
                #define DEMI_PREFETCH_T0( x ) asm volatile ( "prfm pldl1keep, [%[addr]]\n" :: [addr] "r" (x) : "cc" );
                #define DEMI_PREFETCH_T1( x ) asm volatile ( "prfm pldl2keep, [%[addr]]\n" :: [addr] "r" (x) : "cc" );
                #define DEMI_PREFETCH_T2( x ) asm volatile ( "prfm pldl3keep, [%[addr]]\n" :: [addr] "r" (x) : "cc" );
                #define DEMI_PREFETCH_NTA( x ) asm volatile ( "prfm pldl1strm, [%[addr]]\n" :: [addr] "r" (x) : "cc" );
            #else
                #define DEMI_PREFETCH_T0( x ) __pld(x)
                #define DEMI_PREFETCH_T1( x ) __pld(x)
                #define DEMI_PREFETCH_T2( x ) __pld(x)
                #define DEMI_PREFETCH_NTA( x ) __pld(x)
            #endif

            //Distance (in ArrayMemoryManager's slots) used to keep fetching data. This also
            //means the memory manager needs to allocate extra memory for them.
            #define DEMI_PREFETCH_SLOT_DISTANCE     4*ARRAY_PACKED_REALS //Must be multiple of ARRAY_PACKED_REALS
        #endif

        namespace Demi 
        {
            typedef int32x4_t ArrayInt;
            typedef int32x4_t ArrayMaskI;
        }

        ///r = (a * b) + c
        #define _mm_madd_ps( a, b, c )      vmlaq_f32( c, a, b )
        ///r = -(a * b) + c
        #define _mm_nmsub_ps( a, b, c )     vmlsq_f32( c, a, b )

        /// Does not convert, just cast ArrayFloat to ArrayInt
        #define CastfloatToInt( x )          vreinterpretq_s32_f32( x )
        #define CastIntTofloat( x )          vreinterpretq_f32_s32( x )
        /// Input must be 16-byte aligned
        #define CastArrayTofloat( outFloatPtr, arraySimd )       vst1q_f32( outFloatPtr, arraySimd )

    #else
        //Unsupported architecture, tell user to reconfigure. We could silently fallback to C,
        //but this is very green code, and architecture may be x86 with a rare compiler.
        #error "Unknown platform or platform not supported for SIMD. Build Ogre without DEMI_USE_SIMD"
    #endif
#else
    //No SIMD, use C implementation
    #define ARRAY_PACKED_REALS 1
    namespace Demi 
    {
        typedef float ArrayFloat;
        typedef DiRadian ArrayRadian;
        typedef DiRadian ArrayRadian;
        typedef uint32 ArrayInt;
        typedef bool ArrayMaskR;
        typedef bool ArrayMaskI;

        //Do NOT I REPEAT DO NOT change these to static_cast<float>(x) and static_cast<int>(x)
        //These are not conversions. They're reinterpretations!
        #define CastIntTofloat( x ) (x)
        #define CastfloatToInt( x ) (x)

        #define ogre_madd( a, b, c )        ( (c) + ( (a) * (b) ) )

        #define DEMI_PREFETCH_T0( x ) ((void)0)
        #define DEMI_PREFETCH_T1( x ) ((void)0)
        #define DEMI_PREFETCH_T2( x ) ((void)0)
        #define DEMI_PREFETCH_NTA( x ) ((void)0)

        #define ARRAY_INT_ZERO 0

        /// Input must be 16-byte aligned
        #define CastArrayTofloat( outFloatPtr, arraySimd )       (*(outFloatPtr) = arraySimd)

        //Distance (in ArrayMemoryManager's slots) used to keep fetching data. This also
        //means the memory manager needs to allocate extra memory for them.
        #define DEMI_PREFETCH_SLOT_DISTANCE     0 //Must be multiple of ARRAY_PACKED_REALS
    }
#endif

#endif
