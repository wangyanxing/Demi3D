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

namespace Demi
{
    /*inline ArrayFloat ArrayRadian::valueDegrees() const
    {
        return _mm_mul_ps( mRad, MathlibSSE2::fRad2Deg );
    }*/
    
    inline const ArrayRadian& ArrayRadian::operator + () const
    {
        return *this;
    }
    
    inline ArrayRadian ArrayRadian::operator + ( const ArrayRadian& r ) const
    {
        return ArrayRadian( _mm_add_ps( mRad, r.mRad ) );
    }
    
    /*inline ArrayRadian ArrayRadian::operator + ( const ArrayDegree& d ) const
    {
        return ArrayRadian( _mm_add_ps( mRad, r.valueRadians() ) );
    }*/
    
    inline ArrayRadian& ArrayRadian::operator += ( const ArrayRadian& r )
    {
        mRad = _mm_add_ps( mRad, r.mRad );
        return *this;
    }
    
    /*inline ArrayRadian& ArrayRadian::operator += ( const ArrayDegree& r )
    {
        mRad = _mm_add_ps( mRad, r.valueRadians() );
        return *this;
    }*/
    
    inline ArrayRadian ArrayRadian::operator - () const
    {
        return ArrayRadian( _mm_xor_ps( mRad, MathlibSSE2::SIGN_MASK ) );
    }
    
    inline ArrayRadian ArrayRadian::operator - ( const ArrayRadian& r ) const
    {
        return ArrayRadian( _mm_sub_ps( mRad, r.mRad ) );
    }
    
    /*inline ArrayRadian ArrayRadian::operator - ( const ArrayDegree& d ) const
    {
        return ArrayRadian( _mm_sub_ps( mRad, r.valueRadians() ) );
    }*/
    
    inline ArrayRadian& ArrayRadian::operator -= ( const ArrayRadian& r )
    {
        mRad = _mm_sub_ps( mRad, r.mRad );
        return *this;
    }
    
    /*inline ArrayRadian& ArrayRadian::operator -= ( const ArrayDegree& r )
    {
        mRad = _mm_sub_ps( mRad, r.valueRadians() );
        return *this;
    }*/
    
    inline ArrayRadian ArrayRadian::operator * ( const ArrayRadian& r ) const
    {
        return ArrayRadian( _mm_mul_ps( mRad, r.mRad ) );
    }
    
    inline ArrayRadian ArrayRadian::operator / ( ArrayFloat r ) const
    {
        return ArrayRadian( _mm_div_ps( mRad, r ) );
    }
    
    inline ArrayRadian& ArrayRadian::operator /= ( ArrayFloat r )
    {
        mRad = _mm_div_ps( mRad, r );
        return *this;
    }

    inline ArrayFloat ArrayRadian::operator <  ( const ArrayRadian& r ) const { return _mm_cmplt_ps( mRad, r.mRad ); }
    inline ArrayFloat ArrayRadian::operator <= ( const ArrayRadian& r ) const { return _mm_cmple_ps( mRad, r.mRad ); }
    inline ArrayFloat ArrayRadian::operator == ( const ArrayRadian& r ) const { return _mm_cmpeq_ps( mRad, r.mRad ); }
    inline ArrayFloat ArrayRadian::operator != ( const ArrayRadian& r ) const { return _mm_cmpneq_ps( mRad, r.mRad );}
    inline ArrayFloat ArrayRadian::operator >= ( const ArrayRadian& r ) const { return _mm_cmpge_ps( mRad, r.mRad ); }
    inline ArrayFloat ArrayRadian::operator >  ( const ArrayRadian& r ) const { return _mm_cmpgt_ps( mRad, r.mRad ); }

    
    inline ArrayFloat MathlibSSE2::Modf4( ArrayFloat x, ArrayFloat &outIntegral )
    {
        outIntegral = _mm_cvtepi32_ps( _mm_cvttps_epi32( x ) ); //outIntegral = floor( x )
        return _mm_sub_ps( x, outIntegral );
    }
    
    inline ArrayFloat MathlibSSE2::ACos4( ArrayFloat x)
    {
        // This function, ACos4, is under Copyright (C) 2006, 2007
        // Sony Computer Entertainment Inc. (BSD style license) See
        // header for details. Adapted/ported to Ogre intrinsics.
        __m128 xabs = Abs4( x );
        __m128 select = _mm_cmplt_ps( x, _mm_setzero_ps() );
        __m128 t1 = _mm_sqrt_ps( _mm_sub_ps( ONE, xabs ) );
        
        /* Instruction counts can be reduced if the polynomial was
         * computed entirely from nested (dependent) fma's. However, 
         * to reduce the number of pipeline stalls, the polygon is evaluated 
         * in two halves (hi amd lo). 
         */
        __m128 xabs2 = _mm_mul_ps( xabs,  xabs );
        __m128 xabs4 = _mm_mul_ps( xabs2, xabs2 );
        __m128 hi = _mm_madd_ps(_mm_madd_ps(_mm_madd_ps(_mm_set1_ps(-0.0012624911f),
            xabs, _mm_set1_ps(0.0066700901f)),
                xabs, _mm_set1_ps(-0.0170881256f)),
                    xabs, _mm_set1_ps( 0.0308918810f));
        __m128 lo = _mm_madd_ps(_mm_madd_ps(_mm_madd_ps(_mm_set1_ps(-0.0501743046f),
            xabs, _mm_set1_ps(0.0889789874f)),
                xabs, _mm_set1_ps(-0.2145988016f)),
                    xabs, _mm_set1_ps( 1.5707963050f));
        
        __m128 result = _mm_madd_ps( hi, xabs4, lo );
        
        // Adjust the result if x is negactive.
        return Cmov4(   _mm_nmsub_ps( t1, result, PI ), // Negative
                        _mm_mul_ps( t1, result ),       // Positive
                        select );
    }

}
