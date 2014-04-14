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

/// This file is adapted from Ogre 2.0 (working version)

#ifndef __MathlibC_H__
#define __MathlibC_H__

#ifndef __Mathlib_H__
    #error "Don't include this file directly. include Math/Array/Mathlib.h"
#endif

#include "MiscPrerequisites.h"
#include "Math.h"

namespace Demi
{
    class DI_MISC_API MathlibC
    {
    public:
        static const Arrayfloat HALF;        //0.5f, 0.5f, 0.5f, 0.5f
        static const Arrayfloat ONE;         //1.0f, 1.0f, 1.0f, 1.0f
        static const Arrayfloat THREE;       //3.0f, 3.0f, 3.0f, 3.0f
        static const Arrayfloat NEG_ONE;     //-1.0f, -1.0f, -1.0f, -1.0f
        static const Arrayfloat PI;          //PI, PI, PI, PI
        static const Arrayfloat TWO_PI;      //2*PI, 2*PI, 2*PI, 2*PI
        static const Arrayfloat ONE_DIV_2PI; //1 / 2PI, 1 / 2PI, 1 / 2PI, 1 / 2PI
        static const Arrayfloat fEpsilon;    //1e-6f, 1e-6f, 1e-6f, 1e-6f
        static const Arrayfloat fSqEpsilon;  //1e-12f, 1e-12f, 1e-12f, 1e-12f
        static const Arrayfloat OneMinusEpsilon;//1 - 1e-6f, 1 - 1e-6f, 1 - 1e-6f, 1 - 1e-6f
        static const Arrayfloat fDeg2Rad;    //Math::fDeg2Rad, Math::fDeg2Rad, Math::fDeg2Rad, Math::fDeg2Rad
        static const Arrayfloat fRad2Deg;    //Math::fRad2Deg, Math::fRad2Deg, Math::fRad2Deg, Math::fRad2Deg
        static const Arrayfloat FLOAT_MIN;   //FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN
        static const Arrayfloat SIGN_MASK;   //0x80000000, 0x80000000, 0x80000000, 0x80000000
        static const Arrayfloat INFINITEA;   //Inf, Inf, Inf, Inf
        static const Arrayfloat MAX_NEG;     //Max negative number (x4)
        static const Arrayfloat MAX_POS;     //Max negative number (x4)

        /** Returns the absolute values of each 4 floats
            @param
                4 floating point values
            @return
                abs( a )
        */
        static inline Arrayfloat Abs4( Arrayfloat a )
        {
            return Math::Abs( a );
        }

        /** Branchless conditional move for 4 floating point values
            @remarks
                Will NOT work if any of the arguments contains Infinite
                or NaNs or non-floating point values. If an exact binary
                copy is needed, @see CmovRobust
            @param
                4 floating point values. Can't be NaN or Inf
            @param
                4 floating point values. Can't be NaN or Inf
            @param
                4 values containing either 0 or 0xffffffff
                Any other value, the result is undefined
            @return
                r[i] = mask[i] != 0 ? arg1[i] : arg2[i]
                Another way to say it:
                    if( maskCondition[i] == true )
                        r[i] = arg1[i];
                    else
                        arg2[i];
        */
        static inline Arrayfloat Cmov4( Arrayfloat arg1, Arrayfloat arg2, ArrayMaskR mask )
        {
            assert( !Math::isNaN( arg1 ) && !Math::isNaN( arg2 ) &&
                    "Passing NaN values to CMov4" );
#ifndef  NDEBUG
            Arrayfloat newNan1 = arg1 * 0; //+-Inf * 0 = nan
            Arrayfloat newNan2 = arg2 * 0; //+-Inf * 0 = nan
            assert( !Math::isNaN( newNan1 ) && !Math::isNaN( newNan2 ) &&
                    "Passing +/- Infinity values to CMov4" );
#endif

            return mask ? arg1 : arg2;
        }

        /** Robust, branchless conditional move for a 128-bit value.
            @remarks
                If you're looking to copy 4 floating point values that do
                not contain Inf or Nans, @see Cmov4 which is faster.
                This is because switching between registers flagged as
                floating point to integer and back has a latency delay

                For more information refer to Chapter 3.5.2.3
                Bypass between Execution Domains, Intel(R) 64 and IA-32
                Architectures Optimization Reference Manual Order
                Number: 248966-026 April (and also Table 2-12)
            @param
                A value containing 128 bits
            @param
                A value containing 128 bits
            @param
                Mask, each bit is evaluated
            @return
                For each bit:
                r[i] = mask[i] != 0 ? arg1[i] : arg2[i]
                Another way to say it:
                    if( maskCondition[i] == true )
                        r[i] = arg1[i];
                    else
                        arg2[i];
        */
        static inline Arrayfloat CmovRobust( Arrayfloat arg1, Arrayfloat arg2, ArrayMaskR mask )
        {
            return mask ? arg1 : arg2;
        }
        static inline ArrayInt CmovRobust( ArrayInt arg1, ArrayInt arg2, ArrayMaskI mask )
        {
            return mask ? arg1 : arg2;
        }

        /** Returns the result of "a & b"
        @return
            r[i] = a[i] & b[i];
        */
        static inline ArrayInt And( ArrayInt a, ArrayInt b )
        {
            return a & b;
        }

        static inline ArrayMaskI And( ArrayMaskI a, ArrayInt b )
        {
            return ((a ? 0xffffffff : 0) & b) != 0;
        }

        static inline ArrayMaskI And( ArrayInt a, ArrayMaskI b )
        {
            return (a & (b ? 0xffffffff : 0)) != 0;
        }

        static inline ArrayMaskI And( ArrayMaskI a, ArrayMaskI b )
        {
            return a & b;
        }

        /** Test if "a AND b" will result in non-zero, returning 0xffffffff on those cases
        @return
            r[i] = (a[i] & b[i]) ? 0xffffffff : 0;
        */
        static inline ArrayMaskI TestFlags4( ArrayInt a, ArrayInt b )
        {
            return (a & b) != 0;
        }

        static inline ArrayMaskI TestFlags4( ArrayMaskI a, ArrayInt b )
        {
            return ( (a ? 0xffffffff : 0) & b ) != 0;
        }

        static inline ArrayMaskI TestFlags4( ArrayInt a,  ArrayMaskI b )
        {
            return ( a & (b ? 0xffffffff : 0) ) != 0;
        }

        /** Returns the result of "a & ~b"
        @return
            r[i] = a[i] & ~b[i];
        */
        static inline ArrayInt AndNot( ArrayInt a, ArrayInt b )
        {
            return a & ~b;
        }

        static inline ArrayMaskI AndNot( ArrayMaskI a, ArrayInt b )
        {
            return ((a ? 0xffffffff : 0) & ~b) != 0;
        }

        static inline ArrayMaskI AndNot( ArrayInt a, ArrayMaskI b )
        {
            return (a & (b ? 0 : 0xffffffff)) != 0;
        }

        static inline ArrayMaskI AndNot( ArrayMaskI a, ArrayMaskI b )
        {
            return a & (!b);
        }

        /** Returns the result of "a | b"
        @return
            r[i] = a[i] | b[i];
        */
        static inline ArrayInt Or( ArrayInt a, ArrayInt b )
        {
            return a | b;
        }
        static inline ArrayMaskI Or( ArrayMaskI a, ArrayMaskI b )
        {
            return a | b;
        }

        static inline ArrayMaskI Or( ArrayMaskI a, ArrayInt b )
        {
            return ( (a ? 0xffffffff : 0) | b ) != 0;
        }
        static inline ArrayMaskI Or( ArrayInt a,  ArrayMaskI b )
        {
            return ( a | (b ? 0xffffffff : 0) ) != 0;
        }

        /** Returns the result of "a < b"
        @return
            r[i] = a[i] < b[i] ? 0xffffffff : 0;
        */
        static inline ArrayMaskR CompareLess( Arrayfloat a, Arrayfloat b )
        {
            return a < b;
        }

        /** Returns the result of "a <= b"
        @return
            r[i] = a[i] <= b[i] ? 0xffffffff : 0;
        */
        static inline ArrayMaskR CompareLessEqual( Arrayfloat a, Arrayfloat b )
        {
            return a <= b;
        }

        /** Returns the result of "a > b"
        @return
            r[i] = a[i] > b[i] ? 0xffffffff : 0;
        */
        static inline ArrayMaskR CompareGreater( Arrayfloat a, Arrayfloat b )
        {
            return a > b;
        }

        /** Returns the result of "a >= b"
        @return
            r[i] = a[i] >= b[i] ? 0xffffffff : 0;
        */
        static inline ArrayMaskR CompareGreaterEqual( Arrayfloat a, Arrayfloat b )
        {
            return a >= b;
        }

        static inline Arrayfloat SetAll( float val )
        {
            return val;
        }

        static inline ArrayInt SetAll( uint32 val )
        {
            return val;
        }

        /** Returns the result of "a == std::numeric_limits<float>::infinity()"
        @return
            r[i] = a[i] == Inf ? 0xffffffff : 0;
        */
        static inline ArrayMaskR isInfinity( Arrayfloat a )
        {
            return a == std::numeric_limits<float>::infinity();
        }

        /// Returns the maximum value between a and b
        static inline Arrayfloat Max( Arrayfloat a, Arrayfloat b )
        {
            return Ogre::max( a, b );
        }

        /** Returns the minimum value of all elements in a
        @return
            r[0] = min( a[0], a[1], a[2], a[3] )
        */
        static inline float ColapseMin( Arrayfloat a )
        {
            return a;
        }

        /** Returns the maximum value of all elements in a
        @return
            r[0] = max( a[0], a[1], a[2], a[3] )
        */
        static inline float ColapseMax( Arrayfloat a )
        {
            return a;
        }

        /** Returns the reciprocal of x
            @remarks
                If you have a very rough guarantees that you won't be feeding a zero,
                consider using @see InvNonZero4 because it's faster.
                See MathlibSSE2 implementation
            @return
                1 / x (packed as 4 floats)
        */
        static inline Arrayfloat Inv4( Arrayfloat val )
        {
            return 1.0f / val;
        }

        /** Returns the reciprocal of x
            @remarks
                If the input is zero, it will produce a NaN!!! (but it's faster)
                Note: Some architectures may slowdown when a NaN is produced, making this
                function slower than Inv4 for those cases
                @see Inv4
            @param val
                If it's zero, the returned value could be NaN depending on implementation
            @return
                1 / x
        */
        static inline Arrayfloat InvNonZero4( Arrayfloat val )
        {
            return 1.0f / val;
        }

        /** Returns the squared root of the reciprocal of x
            @return
                1 / sqrt( x )
        */
        static inline Arrayfloat InvSqrt4( Arrayfloat f )
        {
            return 1.0f / sqrt( f );
        }

        /** Returns the squared root of the reciprocal of x
            @return
                1 / sqrt( x ) (packed as 4 floats)
        */
        static inline Arrayfloat InvSqrtNonZero4( Arrayfloat f )
        {
            return 1.0f / sqrt( f );
        }

        /** Break x into fractional and integral parts
            @param
                4 floating point values. i.e. "2.57" (x4)
            @param x
                The integral part of x. i.e. 2
            @return outIntegral
                The fractional part of x. i.e. 0.57
        */
        static inline Arrayfloat Modf4( Arrayfloat x, Arrayfloat &outIntegral );

        /** Returns the arccos of x
            @param x
                4 floating point values
            @return
                arccos( x ) (packed as 4 floats)
        */
        static inline Arrayfloat ACos4( Arrayfloat x );

        /** Returns the sine of x
            @param x
                4 floating point values
            @return
                sin( x ) (packed as 4 floats)
        */
        static inline Arrayfloat Sin4( Arrayfloat x );

        /** Returns the cosine of x
            @param x
                4 floating point values
            @return
                cos( x ) (packed as 4 floats)
        */
        static inline Arrayfloat Cos4( Arrayfloat x );

        /** Calculates the cosine & sine of x. Use this function if you need to calculate
            both, as it is faster than calling Cos4 & Sin4 together.
            @param x
                4 floating point values
            @param outSin
                Output value, sin( x ) (packed as 4 floats)
            @param outCos
                Output value, cos( x ) (packed as 4 floats)
        */
        static void SinCos4( Arrayfloat x, Arrayfloat &outSin, Arrayfloat &outCos );
    };
}

#include "MathlibC.inl"

#endif