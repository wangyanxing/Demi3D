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

#ifndef __MathlibSSE2_H__
#define __MathlibSSE2_H__

#if __DEMI_HAVE_SSE

#ifndef __Mathlib_H__
    #error "Don't include this file directly. include Math/Array/Mathlib.h"
#endif

#include "MiscPrerequisites.h"

namespace Demi
{
    class ArrayRadian
    {
        Arrayfloat mRad;

    public:
        explicit ArrayRadian ( Arrayfloat r ) : mRad( r ) {}
        //ArrayDiRadian ( const ArrayDegree& d );
        ArrayRadian& operator = ( const Arrayfloat &f )      { mRad = f; return *this; }
        ArrayRadian& operator = ( const ArrayRadian &r )    { mRad = r.mRad; return *this; }
        //ArrayDiRadian& operator = ( const ArrayDegree& d );

        //Arrayfloat valueDegrees() const; // see bottom of this file
        Arrayfloat valueRadians() const                      { return mRad; }

        inline const ArrayRadian& operator + () const;
        inline ArrayRadian operator + ( const ArrayRadian& r ) const;
        //inline ArrayDiRadian operator + ( const ArrayDegree& d ) const;
        inline ArrayRadian& operator += ( const ArrayRadian& r );
        //inline ArrayDiRadian& operator += ( const ArrayDegree& d );
        inline ArrayRadian operator - () const;
        inline ArrayRadian operator - ( const ArrayRadian& r ) const;
        //inline ArrayDiRadian operator - ( const ArrayDegree& d ) const;
        inline ArrayRadian& operator -= ( const ArrayRadian& r );
        //inline ArrayDiRadian& operator -= ( const ArrayDegree& d );
        inline ArrayRadian operator * ( Arrayfloat f ) const;
        inline ArrayRadian operator * ( const ArrayRadian& f ) const;
        inline ArrayRadian& operator *= ( Arrayfloat f );
        inline ArrayRadian operator / ( Arrayfloat f ) const;
        inline ArrayRadian& operator /= ( Arrayfloat f );

        inline Arrayfloat operator <  ( const ArrayRadian& r ) const;
        inline Arrayfloat operator <= ( const ArrayRadian& r ) const;
        inline Arrayfloat operator == ( const ArrayRadian& r ) const;
        inline Arrayfloat operator != ( const ArrayRadian& r ) const;
        inline Arrayfloat operator >= ( const ArrayRadian& r ) const;
        inline Arrayfloat operator >  ( const ArrayRadian& r ) const;
    };

    class DI_MISC_API MathlibSSE2
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
        //INFINITE is taken in Windows, INFINITY by C99 (bloody macros). A joke on Infinite Tea
        static const Arrayfloat INFINITEA;   //Inf, Inf, Inf, Inf
        static const Arrayfloat MAX_NEG;     //Max negative number (x4)
        static const Arrayfloat MAX_POS;     //Max negative number (x4)
        static const Arrayfloat LAST_AFFINE_COLUMN;//0, 0, 0, 1

        /** Returns the absolute values of each 4 floats
            @param
                4 floating point values
            @return
                abs( a )
        */
        static inline Arrayfloat Abs4( Arrayfloat a )
        {
            return _mm_andnot_ps( _mm_set1_ps( -0.0f ), a );
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
            assert( _mm_movemask_ps( _mm_cmpeq_ps( arg1, arg1 ) ) == 0x0f &&
                    _mm_movemask_ps( _mm_cmpeq_ps( arg2, arg2 ) ) == 0x0f &&
                    "Passing NaN values to CMov4" );
#ifndef  NDEBUG
            Arrayfloat newNan1 = _mm_mul_ps( arg1, _mm_setzero_ps() ); //+-Inf * 0 = nan
            Arrayfloat newNan2 = _mm_mul_ps( arg2, _mm_setzero_ps() ); //+-Inf * 0 = nan
            assert( _mm_movemask_ps( _mm_cmpeq_ps( newNan1, newNan1 ) ) == 0x0f &&
                    _mm_movemask_ps( _mm_cmpeq_ps( newNan2, newNan2 ) ) == 0x0f &&
                    "Passing +/- Infinity values to CMov4" );
#endif

            Arrayfloat t = _mm_sub_ps( arg1, arg2 );             // t = arg1 - arg2
            return _mm_add_ps( arg2, _mm_and_ps( t, mask ) );   // r = arg2 + (t & mask)
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
        #
        static inline __m128 CmovRobust( __m128 arg1, __m128 arg2, __m128 mask )
        {
            return _mm_or_ps( _mm_and_ps( arg1, mask ), _mm_andnot_ps( mask, arg2 ) );
        }
        static inline __m128d CmovRobust( __m128d arg1, __m128d arg2, __m128d mask )
        {
            return _mm_or_pd( _mm_and_pd( arg1, mask ), _mm_andnot_pd( mask, arg2 ) );
        }
        static inline ArrayInt CmovRobust( ArrayInt arg1, ArrayInt arg2, ArrayMaskI mask )
        {
            return _mm_or_si128( _mm_and_si128( arg1, mask ), _mm_andnot_si128( mask, arg2 ) );
        }

        /** Returns the result of "a & b"
        @return
            r[i] = a[i] & b[i];
        */
        static inline Arrayfloat And( Arrayfloat a, Arrayfloat b )
        {
            return _mm_and_ps( a, b );
        }
        static inline ArrayInt And( ArrayInt a, ArrayInt b )
        {
            return _mm_and_si128( a, b );
        }

        /** Returns the result of "a & b"
        @return
            r[i] = a[i] & b;
        */
        static inline __m128i And( __m128i a, uint32 b )
        {
            return _mm_and_si128( a, _mm_set1_epi32( b ) );
        }

        /** Test if "a AND b" will result in non-zero, returning 0xffffffff on those cases
        @remarks
            Because there is no "not-equal" instruction in integer SSE2, be need to do some
            bit flipping.
        @par
            Do not try to do a floating-point variation of this one. I already tried, but hit
            a wall: 0x80000000 & 0x80000000 => returns 0, because in floating point, -0 == 0
        @return
            r[i] = (a[i] & b[i]) ? 0xffffffff : 0;
        */
        static inline __m128i TestFlags4( __m128i a, __m128i b )
        {
            // !( (a & b) == 0 ) --> ( (a & b) == 0 ) ^ -1
            return _mm_xor_si128( _mm_cmpeq_epi32( _mm_and_si128( a, b ), _mm_setzero_si128() ),
                                    _mm_set1_epi32( -1 ) );
        }

        /** Returns the result of "a & ~b"
        @return
            r[i] = a[i] & ~b[i];
        */
        static inline __m128i AndNot( __m128i a, __m128i b )
        {
            return _mm_andnot_si128( b, a );
        }

        /** Returns the result of "a | b"
        @return
            r[i] = a[i] | b[i];
        */
        static inline __m128 Or( __m128 a, __m128 b )
        {
            return _mm_or_ps( a, b );
        }
        static inline __m128i Or( __m128i a, __m128i b )
        {
            return _mm_or_si128( a, b );
        }

        /** Returns the result of "a < b"
        @return
            r[i] = a[i] < b[i] ? 0xffffffff : 0;
        */
        static inline __m128 CompareLess( __m128 a, __m128 b )
        {
            return _mm_cmplt_ps( a, b );
        }

        /** Returns the result of "a <= b"
        @return
            r[i] = a[i] <= b[i] ? 0xffffffff : 0;
        */
        static inline __m128 CompareLessEqual( __m128 a, __m128 b )
        {
            return _mm_cmple_ps( a, b );
        }

        /** Returns the result of "a > b"
        @return
            r[i] = a[i] > b[i] ? 0xffffffff : 0;
        */
        static inline __m128 CompareGreater( __m128 a, __m128 b )
        {
            return _mm_cmpgt_ps( a, b );
        }

        /** Returns the result of "a >= b"
        @return
            r[i] = a[i] >= b[i] ? 0xffffffff : 0;
        */
        static inline __m128 CompareGreaterEqual( __m128 a, __m128 b )
        {
            return _mm_cmpge_ps( a, b );
        }

        static inline Arrayfloat SetAll( float val )
        {
            return _mm_set_ps1( val );
        }

        static inline ArrayInt SetAll( uint32 val )
        {
            return _mm_set1_epi32( val );
        }

        /** Returns the result of "a == std::numeric_limits<float>::infinity()"
        @return
            r[i] = a[i] == Inf ? 0xffffffff : 0;
        */
        static inline Arrayfloat isInfinity( Arrayfloat a )
        {
            return _mm_cmpeq_ps( a, MathlibSSE2::INFINITEA );
        }

        /// Returns the maximum value between a and b
        static inline Arrayfloat Max( Arrayfloat a, Arrayfloat b )
        {
            return _mm_max_ps( a, b );
        }

        /// Returns the minimum value between a and b
        static inline Arrayfloat Min( Arrayfloat a, Arrayfloat b )
        {
            return _mm_min_ps( a, b );
        }

        /** Returns the minimum value of all elements in a
        @return
            r[0] = min( a[0], a[1], a[2], a[3] )
        */
        static inline float CollapseMin( Arrayfloat a )
        {
            float r;
            Arrayfloat t0 = _mm_shuffle_ps( a, a, _MM_SHUFFLE( 2, 3, 2, 3 ) );
            t0 = _mm_min_ps( a, t0 );
            a = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 1, 1, 0, 0 ) );
            t0 = _mm_min_ps( a, t0 );
            _mm_store_ss( &r, t0 );
            return r;
        }

        /** Returns the maximum value of all elements in a
        @return
            r[0] = max( a[0], a[1], a[2], a[3] )
        */
        static inline float CollapseMax( Arrayfloat a )
        {
            float r;
            Arrayfloat t0 = _mm_shuffle_ps( a, a, _MM_SHUFFLE( 2, 3, 2, 3 ) );
            t0 = _mm_max_ps( a, t0 );
            a = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 1, 1, 0, 0 ) );
            t0 = _mm_max_ps( a, t0 );
            _mm_store_ss( &r, t0 );
            return r;
        }

        /** Returns the reciprocal of x
            @remarks
                If you have a very rough guarantees that you won't be feeding a zero,
                consider using @see InvNonZero4 because it's faster.

                Use SSE Newton-Raphson reciprocal estimate, accurate to 23 significant
                bits of the mantissa after an extra iteration, instead of the little
                12 bits of accuracy that _mm_rcp_ps gives us 
                In short, one Newton-Raphson Iteration:
                 f( i+1 ) = 2 * rcp( f ) - f * rcp( f ) * rcp( f )
                See Intel AP-803 (Application note), Order N° 243637-002 (if you can get it!)
                 "x0 = RCPSS(d)
                x1 = x0 * (2 - d * x0) = 2 * x0 - d * x0 * x0
                where x0 is the first approximation to the reciprocal of the divisor d, and x1 is a
                better approximation. You must use this formula before multiplying with the dividend."
            @param val
                4 floating point values. If it's zero, the returned value will be infinite,
                which is the correct result, but it's slower than InvNonZero4
            @return
                1 / x (packed as 4 floats)
        */
        static inline Arrayfloat Inv4( Arrayfloat val )
        {
            Arrayfloat inv = _mm_rcp_ps( val );
            Arrayfloat twoRcp    = _mm_add_ps( inv, inv );                   //2 * rcp( f )
            Arrayfloat rightSide= _mm_mul_ps( val, _mm_mul_ps( inv, inv ) ); //f * rcp( f ) * rcp( f )
            rightSide = _mm_and_ps( rightSide, _mm_cmpneq_ps( val, _mm_setzero_ps() ) ); //Nuke this NaN
            return _mm_sub_ps( twoRcp, rightSide );
        }

        /** Returns the reciprocal of x
            @remarks
                If the input is zero, it will produce a NaN!!! (but it's faster)
                Note: Some architectures may slowdown when a NaN is produced, making this
                function slower than Inv4 for those cases
                @see Inv4

                Use SSE Newton-Raphson reciprocal estimate, accurate to 23 significant
                bits of the mantissa after an extra iteration, instead of the little
                12 bits of accuracy that _mm_rcp_ps gives us 
                In short, one Newton-Raphson Iteration:
                 f( i+1 ) = 2 * rcp( f ) - f * rcp( f ) * rcp( f )
                See Intel AP-803 (Application note), Order N° 243637-002 (if you can get it!)
                 "x0 = RCPSS(d)
                x1 = x0 * (2 - d * x0) = 2 * x0 - d * x0 * x0
                where x0 is the first approximation to the reciprocal of the divisor d, and x1 is a
                better approximation. You must use this formula before multiplying with the dividend."
            @param val
                4 floating point values. If it's zero, the returned value will be NaN
            @return
                1 / x (packed as 4 floats)
        */
        static inline Arrayfloat InvNonZero4( Arrayfloat val )
        {
            Arrayfloat inv = _mm_rcp_ps( val );
            Arrayfloat twoRcp    = _mm_add_ps( inv, inv );                   //2 * rcp( f )
            Arrayfloat rightSide= _mm_mul_ps( val, _mm_mul_ps( inv, inv ) ); //f * rcp( f ) * rcp( f )
            return _mm_sub_ps( twoRcp, rightSide );
        }

        /** Returns the squared root of the reciprocal of x
            @remarks
                Use SSE Newton-Raphson reciprocal estimate, accurate to 23 significant
                bits of the mantissa after an extra iteration, instead of the little
                12 bits of accuracy that _mm_rcp_ps gives us 
                In short, one Newton-Raphson Iteration:
                     f( i+1 ) = 0.5 * rsqrt( f ) * ( 3 - f * rsqrt( f ) * rsqrt( f ) )
                See Intel AP-803 (Application note), Order N° 243637-002 (if you can get it!)
                "x0 = RSQRTSS(a)
                x1 = 0.5 * x0 * ( 3 - ( a * x0 ) * x0 )
                where x0 is the first approximation to the reciprocal square root of a, and x1 is a
                better approximation. The order of evaluation is important. You must use this formula
                before multiplying with a to get the square root."
            @param f
                4 floating point values
            @return
                1 / sqrt( x ) (packed as 4 floats)
        */
        static inline Arrayfloat InvSqrt4( Arrayfloat f )
        {
            Arrayfloat invSqrt   = _mm_rsqrt_ps( f );

            Arrayfloat halfInvSqrt= _mm_mul_ps( HALF, invSqrt );                     //0.5 * rsqrt( f )
            Arrayfloat rightSide  = _mm_mul_ps( invSqrt, _mm_mul_ps( f, invSqrt ) ); //f * rsqrt( f ) * rsqrt( f )
            rightSide = _mm_and_ps( rightSide, _mm_cmpneq_ps( f, _mm_setzero_ps() ) );//Nuke this NaN
            return _mm_mul_ps( halfInvSqrt, _mm_sub_ps( THREE, rightSide ) );       //halfInvSqrt*(3 - rightSide)
        }

        /** Returns the squared root of the reciprocal of x
            @remarks
                Use SSE Newton-Raphson reciprocal estimate, accurate to 23 significant
                bits of the mantissa after an extra iteration, instead of the little
                12 bits of accuracy that _mm_rcp_ps gives us 
                In short, one Newton-Raphson Iteration:
                     f( i+1 ) = 0.5 * rsqrt( f ) * ( 3 - f * rsqrt( f ) * rsqrt( f ) )
                See Intel AP-803 (Application note), Order N° 243637-002 (if you can get it!)
                "x0 = RSQRTSS(a)
                x1 = 0.5 * x0 * ( 3 - ( a * x0 ) * x0 )
                where x0 is the first approximation to the reciprocal square root of a, and x1 is a
                better approximation. The order of evaluation is important. You must use this formula
                before multiplying with a to get the square root."

                Warning: Passing a zero will return a NaN instead of infinity
            @param f
                4 floating point values
            @return
                1 / sqrt( x ) (packed as 4 floats)
        */
        static inline Arrayfloat InvSqrtNonZero4( Arrayfloat f )
        {
            Arrayfloat invSqrt = _mm_rsqrt_ps( f );

            Arrayfloat halfInvSqrt= _mm_mul_ps( HALF, invSqrt );                     //0.5 * rsqrt( f )
            Arrayfloat rightSide  = _mm_mul_ps( invSqrt, _mm_mul_ps( f, invSqrt ) ); //f * rsqrt( f ) * rsqrt( f )
            return _mm_mul_ps( halfInvSqrt, _mm_sub_ps( THREE, rightSide ) );       //halfInvSqrt*(3 - rightSide)
        }

        /** Break x into fractional and integral parts
            @param x
                4 floating point values. i.e. "2.57" (x4)
            @param outIntegral
                The integral part of x. i.e. 2
            @return
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
        static Arrayfloat Sin4( Arrayfloat x );

        /** Returns the cosine of x
            @param x
                4 floating point values
            @return
                cos( x ) (packed as 4 floats)
        */
        static Arrayfloat Cos4( Arrayfloat x );

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

#if DEMI_COMPILER != DEMI_COMPILER_CLANG && DEMI_COMPILER != DEMI_COMPILER_GNUC
//  inline Arrayfloat operator - ( Arrayfloat l )                 { return _mm_xor_ps( l, MathlibSSE2::SIGN_MASK ); }
//  inline Arrayfloat operator + ( Arrayfloat l, float r )         { return _mm_add_ps( l, _mm_set1_ps( r ) ); }
//  inline Arrayfloat operator + ( float l, Arrayfloat r )         { return _mm_add_ps( _mm_set1_ps( l ), r ); }
    inline Arrayfloat operator + ( Arrayfloat l, Arrayfloat r )    { return _mm_add_ps( l, r ); }
//  inline Arrayfloat operator - ( Arrayfloat l, float r )         { return _mm_sub_ps( l, _mm_set1_ps( r ) ); }
//  inline Arrayfloat operator - ( float l, Arrayfloat r )         { return _mm_sub_ps( _mm_set1_ps( l ), r ); }
    inline Arrayfloat operator - ( Arrayfloat l, Arrayfloat r )    { return _mm_sub_ps( l, r ); }
//  inline Arrayfloat operator * ( Arrayfloat l, float r )         { return _mm_mul_ps( l, _mm_set1_ps( r ) ); }
//  inline Arrayfloat operator * ( float l, Arrayfloat r )         { return _mm_mul_ps( _mm_set1_ps( l ), r ); }
    inline Arrayfloat operator * ( Arrayfloat l, Arrayfloat r )    { return _mm_mul_ps( l, r ); }
//  inline Arrayfloat operator / ( Arrayfloat l, float r )         { return _mm_div_ps( l, _mm_set1_ps( r ) ); }
//  inline Arrayfloat operator / ( float l, Arrayfloat r )         { return _mm_div_ps( _mm_set1_ps( l ), r ); }
    inline Arrayfloat operator / ( Arrayfloat l, Arrayfloat r )    { return _mm_div_ps( l, r ); }
#endif
}

#include "MathlibSSE2.inl"

#endif
#endif