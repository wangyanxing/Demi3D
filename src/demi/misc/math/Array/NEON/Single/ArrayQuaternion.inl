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

namespace Demi
{
    // Arithmetic operations
#define DEFINE_OPERATION( leftClass, rightClass, op, op_func )\
    inline ArrayQuaternion operator op ( const leftClass &lhs, const rightClass &rhs )\
    {\
        const ArrayFloat * RESTRICT_ALIAS lhsChunkBase = lhs.mChunkBase;\
        const ArrayFloat * RESTRICT_ALIAS rhsChunkBase = rhs.mChunkBase;\
        return ArrayQuaternion(\
                op_func( lhsChunkBase[0], rhsChunkBase[0] ),\
                op_func( lhsChunkBase[1], rhsChunkBase[1] ),\
                op_func( lhsChunkBase[2], rhsChunkBase[2] ),\
                op_func( lhsChunkBase[3], rhsChunkBase[3] ) );\
    }
#define DEFINE_L_OPERATION( leftType, rightClass, op, op_func )\
    inline ArrayQuaternion operator op ( const leftType lhs, const rightClass &rhs )\
    {\
        return ArrayQuaternion(\
                op_func( lhs, rhs.mChunkBase[0] ),\
                op_func( lhs, rhs.mChunkBase[1] ),\
                op_func( lhs, rhs.mChunkBase[2] ),\
                op_func( lhs, rhs.mChunkBase[3] ) );\
    }
#define DEFINE_R_OPERATION( leftClass, rightType, op, op_func )\
    inline ArrayQuaternion operator op ( const leftClass &lhs, const rightType rhs )\
    {\
        return ArrayQuaternion(\
                op_func( lhs.mChunkBase[0], rhs ),\
                op_func( lhs.mChunkBase[1], rhs ),\
                op_func( lhs.mChunkBase[2], rhs ),\
                op_func( lhs.mChunkBase[3], rhs ) );\
    }

    // Update operations
#define DEFINE_UPDATE_OPERATION( leftClass, op, op_func )\
    inline void ArrayQuaternion::operator op ( const leftClass &a )\
    {\
        ArrayFloat * RESTRICT_ALIAS chunkBase = mChunkBase;\
        const ArrayFloat * RESTRICT_ALIAS aChunkBase = a.mChunkBase;\
        chunkBase[0] = op_func( chunkBase[0], aChunkBase[0] );\
        chunkBase[1] = op_func( chunkBase[1], aChunkBase[1] );\
        chunkBase[2] = op_func( chunkBase[2], aChunkBase[2] );\
        chunkBase[3] = op_func( chunkBase[3], aChunkBase[3] );\
    }
#define DEFINE_UPDATE_R_OPERATION( rightType, op, op_func )\
    inline void ArrayQuaternion::operator op ( const rightType a )\
    {\
        mChunkBase[0] = op_func( mChunkBase[0], a );\
        mChunkBase[1] = op_func( mChunkBase[1], a );\
        mChunkBase[2] = op_func( mChunkBase[2], a );\
        mChunkBase[3] = op_func( mChunkBase[3], a );\
    }

    // + Addition
    DEFINE_OPERATION( ArrayQuaternion, ArrayQuaternion, +, vaddq_f32 );

    // - Subtraction
    DEFINE_OPERATION( ArrayQuaternion, ArrayQuaternion, -, vsubq_f32 );

    // * Multiplication (scalar only)
    DEFINE_L_OPERATION( ArrayFloat, ArrayQuaternion, *, vmulq_f32 );
    DEFINE_R_OPERATION( ArrayQuaternion, ArrayFloat, *, vmulq_f32 );

    // Update operations
    // +=
    DEFINE_UPDATE_OPERATION(            ArrayQuaternion,        +=, vaddq_f32 );

    // -=
    DEFINE_UPDATE_OPERATION(            ArrayQuaternion,        -=, vsubq_f32 );

    // *=
    DEFINE_UPDATE_R_OPERATION(          ArrayFloat,          *=, vmulq_f32 );

    // Notes: This operator doesn't get inlined. The generated instruction count is actually high so
    // the compiler seems to be clever in not inlining. There is no gain in doing a "mul()" equivalent
    // like we did with mul( const ArrayQuaternion&, ArrayVector3& ) because we would still need
    // a temporary variable to hold all the operations (we can't overwrite to any heap value
    // since all values are used until the last operation)
    inline ArrayQuaternion operator * ( const ArrayQuaternion &lhs, const ArrayQuaternion &rhs )
    {
        return ArrayQuaternion(
            /* w = (w * rkQ.w - x * rkQ.x) - (y * rkQ.y + z * rkQ.z) */
            vsubq_f32( vsubq_f32(
                    vmulq_f32( lhs.mChunkBase[0], rhs.mChunkBase[0] ),
                    vmulq_f32( lhs.mChunkBase[1], rhs.mChunkBase[1] ) ),
            vaddq_f32(
                    vmulq_f32( lhs.mChunkBase[2], rhs.mChunkBase[2] ),
                    vmulq_f32( lhs.mChunkBase[3], rhs.mChunkBase[3] ) ) ),
            /* x = (w * rkQ.x + x * rkQ.w) + (y * rkQ.z - z * rkQ.y) */
            vaddq_f32( vaddq_f32(
                    vmulq_f32( lhs.mChunkBase[0], rhs.mChunkBase[1] ),
                    vmulq_f32( lhs.mChunkBase[1], rhs.mChunkBase[0] ) ),
            vsubq_f32(
                    vmulq_f32( lhs.mChunkBase[2], rhs.mChunkBase[3] ),
                    vmulq_f32( lhs.mChunkBase[3], rhs.mChunkBase[2] ) ) ),
            /* y = (w * rkQ.y + y * rkQ.w) + (z * rkQ.x - x * rkQ.z) */
            vaddq_f32( vaddq_f32(
                    vmulq_f32( lhs.mChunkBase[0], rhs.mChunkBase[2] ),
                    vmulq_f32( lhs.mChunkBase[2], rhs.mChunkBase[0] ) ),
            vsubq_f32(
                    vmulq_f32( lhs.mChunkBase[3], rhs.mChunkBase[1] ),
                    vmulq_f32( lhs.mChunkBase[1], rhs.mChunkBase[3] ) ) ),
            /* z = (w * rkQ.z + z * rkQ.w) + (x * rkQ.y - y * rkQ.x) */
            vaddq_f32( vaddq_f32(
                    vmulq_f32( lhs.mChunkBase[0], rhs.mChunkBase[3] ),
                    vmulq_f32( lhs.mChunkBase[3], rhs.mChunkBase[0] ) ),
            vsubq_f32(
                    vmulq_f32( lhs.mChunkBase[1], rhs.mChunkBase[2] ),
                    vmulq_f32( lhs.mChunkBase[2], rhs.mChunkBase[1] ) ) ) );
    }
    
    inline ArrayQuaternion ArrayQuaternion::Slerp( ArrayFloat fT, const ArrayQuaternion &rkP,
                                                        const ArrayQuaternion &rkQ /*, bool shortestPath*/ )
    {
        ArrayFloat fCos = rkP.Dot( rkQ );
        /* Clamp fCos to [-1; 1] range */
        fCos = vminq_f32( MathlibNEON::ONE, vmaxq_f32( MathlibNEON::NEG_ONE, fCos ) );
        
        /* Invert the rotation for shortest path? */
        /* m = fCos < 0.0f ? -1.0f : 1.0f; */
        ArrayFloat m = MathlibNEON::Cmov4( MathlibNEON::NEG_ONE, MathlibNEON::ONE,
                                            vcltq_f32( fCos, vdupq_n_f32(0.0f) ) /*&& shortestPath*/ );
        ArrayQuaternion rkT(
                        vmulq_f32( rkQ.mChunkBase[0], m ),
                        vmulq_f32( rkQ.mChunkBase[1], m ),
                        vmulq_f32( rkQ.mChunkBase[2], m ),
                        vmulq_f32( rkQ.mChunkBase[3], m ) );
        
        ArrayFloat fSin = vrsqrteq_f32( vsubq_f32( MathlibNEON::ONE, vmulq_f32( fCos, fCos ) ) );
        
        /* ATan2 in original Quaternion is slightly absurd, because fSin was derived from
           fCos (hence never negative) which makes ACos a better replacement. ACos is much
           faster than ATan2, as long as the input is whithin range [-1; 1], otherwise the generated
           NaNs make it slower (whether clamping the input outweights the benefit is
           arguable). We use ACos4 to avoid implementing ATan2_4.
        */
        ArrayFloat fAngle = MathlibNEON::ACos4( fCos );

        // mask = Abs( fCos ) < 1-epsilon
        ArrayFloat mask    = vcltq_f32( MathlibNEON::Abs4( fCos ), MathlibNEON::OneMinusEpsilon );
        ArrayFloat fInvSin = MathlibNEON::InvNonZero4( fSin );
        ArrayFloat oneSubT = vsubq_f32( MathlibNEON::ONE, fT );
        // fCoeff1 = Sin( fT * fAngle ) * fInvSin
        ArrayFloat fCoeff0 = vmulq_f32( MathlibNEON::Sin4( vmulq_f32( oneSubT, fAngle ) ), fInvSin );
        ArrayFloat fCoeff1 = vmulq_f32( MathlibNEON::Sin4( vmulq_f32( fT, fAngle ) ), fInvSin );
        // fCoeff1 = mask ? fCoeff1 : fT; (switch to lerp when rkP & rkQ are too close->fSin=0, or 180°)
        fCoeff0 = MathlibNEON::CmovRobust( fCoeff0, oneSubT, mask );
        fCoeff1 = MathlibNEON::CmovRobust( fCoeff1, fT, mask );

        // retVal = fCoeff0 * rkP + fCoeff1 * rkT;
        rkT.mChunkBase[0] = vaddq_f32( vmulq_f32( rkP.mChunkBase[0], fCoeff0 ),
                                         vmulq_f32( rkT.mChunkBase[0], fCoeff1 ) ),
        rkT.mChunkBase[1] = vaddq_f32( vmulq_f32( rkP.mChunkBase[1], fCoeff0 ),
                                         vmulq_f32( rkT.mChunkBase[1], fCoeff1 ) ),
        rkT.mChunkBase[2] = vaddq_f32( vmulq_f32( rkP.mChunkBase[2], fCoeff0 ),
                                         vmulq_f32( rkT.mChunkBase[2], fCoeff1 ) ),
        rkT.mChunkBase[3] = vaddq_f32( vmulq_f32( rkP.mChunkBase[3], fCoeff0 ),
                                         vmulq_f32( rkT.mChunkBase[3], fCoeff1 ) );

        rkT.normalise();

        return rkT;
    }
    
    inline ArrayQuaternion ArrayQuaternion::nlerpShortest( ArrayFloat fT, const ArrayQuaternion &rkP,
                                                            const ArrayQuaternion &rkQ )
    {
        //Flip the sign of rkQ when p.dot( q ) < 0 to get the shortest path
        ArrayFloat signMask = vdupq_n_f32( -0.0f );
        ArrayFloat sign = vandq_s32( signMask, rkP.Dot( rkQ ) );
        ArrayQuaternion tmpQ = ArrayQuaternion( veorq_s32( rkQ.mChunkBase[0], sign ),
                                                veorq_s32( rkQ.mChunkBase[1], sign ),
                                                veorq_s32( rkQ.mChunkBase[2], sign ),
                                                veorq_s32( rkQ.mChunkBase[3], sign ) );

        ArrayQuaternion retVal(
                _mm_madd_ps( fT, vsubq_f32( tmpQ.mChunkBase[0], rkP.mChunkBase[0] ), rkP.mChunkBase[0] ),
                _mm_madd_ps( fT, vsubq_f32( tmpQ.mChunkBase[1], rkP.mChunkBase[1] ), rkP.mChunkBase[1] ),
                _mm_madd_ps( fT, vsubq_f32( tmpQ.mChunkBase[2], rkP.mChunkBase[2] ), rkP.mChunkBase[2] ),
                _mm_madd_ps( fT, vsubq_f32( tmpQ.mChunkBase[3], rkP.mChunkBase[3] ), rkP.mChunkBase[3] ) );
        retVal.normalise();

        return retVal;
    }
    
    inline ArrayQuaternion ArrayQuaternion::nlerp( ArrayFloat fT, const ArrayQuaternion &rkP,
                                                        const ArrayQuaternion &rkQ )
    {
        ArrayQuaternion retVal(
                _mm_madd_ps( fT, vsubq_f32( rkQ.mChunkBase[0], rkP.mChunkBase[0] ), rkP.mChunkBase[0] ),
                _mm_madd_ps( fT, vsubq_f32( rkQ.mChunkBase[1], rkP.mChunkBase[1] ), rkP.mChunkBase[1] ),
                _mm_madd_ps( fT, vsubq_f32( rkQ.mChunkBase[2], rkP.mChunkBase[2] ), rkP.mChunkBase[2] ),
                _mm_madd_ps( fT, vsubq_f32( rkQ.mChunkBase[3], rkP.mChunkBase[3] ), rkP.mChunkBase[3] ) );
        retVal.normalise();

        return retVal;
    }
    
    inline ArrayQuaternion ArrayQuaternion::Cmov4( const ArrayQuaternion &arg1,
                                                    const ArrayQuaternion &arg2, ArrayMaskR mask )
    {
        return ArrayQuaternion(
                MathlibNEON::Cmov4( arg1.mChunkBase[0], arg2.mChunkBase[0], mask ),
                MathlibNEON::Cmov4( arg1.mChunkBase[1], arg2.mChunkBase[1], mask ),
                MathlibNEON::Cmov4( arg1.mChunkBase[2], arg2.mChunkBase[2], mask ),
                MathlibNEON::Cmov4( arg1.mChunkBase[3], arg2.mChunkBase[3], mask ) );
    }
    
    inline void ArrayQuaternion::mul( const ArrayQuaternion &inQ, ArrayVector3 &inOutVec )
    {
        // nVidia SDK implementation
        ArrayVector3 qVec( inQ.mChunkBase[1], inQ.mChunkBase[2], inQ.mChunkBase[3] );

        ArrayVector3 uv = qVec.crossProduct( inOutVec );
        ArrayVector3 uuv    = qVec.crossProduct( uv );

        // uv = uv * (2.0f * w)
        ArrayFloat w2 = vaddq_f32( inQ.mChunkBase[0], inQ.mChunkBase[0] );
        uv.mChunkBase[0] = vmulq_f32( uv.mChunkBase[0], w2 );
        uv.mChunkBase[1] = vmulq_f32( uv.mChunkBase[1], w2 );
        uv.mChunkBase[2] = vmulq_f32( uv.mChunkBase[2], w2 );

        // uuv = uuv * 2.0f
        uuv.mChunkBase[0] = vaddq_f32( uuv.mChunkBase[0], uuv.mChunkBase[0] );
        uuv.mChunkBase[1] = vaddq_f32( uuv.mChunkBase[1], uuv.mChunkBase[1] );
        uuv.mChunkBase[2] = vaddq_f32( uuv.mChunkBase[2], uuv.mChunkBase[2] );

        //inOutVec = v + uv + uuv
        inOutVec.mChunkBase[0] = vaddq_f32( inOutVec.mChunkBase[0],
                                    vaddq_f32( uv.mChunkBase[0], uuv.mChunkBase[0] ) );
        inOutVec.mChunkBase[1] = vaddq_f32( inOutVec.mChunkBase[1],
                                    vaddq_f32( uv.mChunkBase[1], uuv.mChunkBase[1] ) );
        inOutVec.mChunkBase[2] = vaddq_f32( inOutVec.mChunkBase[2],
                                    vaddq_f32( uv.mChunkBase[2], uuv.mChunkBase[2] ) );
    }
    
    inline void ArrayQuaternion::FromAngleAxis( const ArrayRadian& rfAngle, const ArrayVector3& rkAxis )
    {
        // assert:  axis[] is unit length
        //
        // The quaternion representing the rotation is
        //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

        ArrayFloat fHalfAngle( vmulq_f32( rfAngle.valueRadians(), MathlibNEON::HALF ) );

        ArrayFloat fSin;
        MathlibNEON::SinCos4( fHalfAngle, fSin, mChunkBase[0] );

        ArrayFloat * RESTRICT_ALIAS chunkBase = mChunkBase;
        const ArrayFloat * RESTRICT_ALIAS rkAxisChunkBase = rkAxis.mChunkBase;

        chunkBase[1] = vmulq_f32( fSin, rkAxisChunkBase[0] ); //x = fSin*rkAxis.x;
        chunkBase[2] = vmulq_f32( fSin, rkAxisChunkBase[1] ); //y = fSin*rkAxis.y;
        chunkBase[3] = vmulq_f32( fSin, rkAxisChunkBase[2] ); //z = fSin*rkAxis.z;
    }
    
    inline void ArrayQuaternion::ToAngleAxis( ArrayRadian &rfAngle, ArrayVector3 &rkAxis ) const
    {
        // The quaternion representing the rotation is
        //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)
        ArrayFloat sqLength = vaddq_f32( vaddq_f32(
                                vmulq_f32( mChunkBase[1], mChunkBase[1] ),  //(x * x +
                                vmulq_f32( mChunkBase[2], mChunkBase[2] ) ),    //y * y) +
                                vmulq_f32( mChunkBase[3], mChunkBase[3] ) );    //z * z )

        ArrayFloat mask      = vcgtq_f32( sqLength, vdupq_n_f32(0.0f) ); //mask = sqLength > 0

        //sqLength = sqLength > 0 ? sqLength : 1; so that invSqrt doesn't give NaNs or Infs
        //when 0 (to avoid using CmovRobust just to select the non-nan results)
        sqLength = MathlibNEON::Cmov4( sqLength, MathlibNEON::ONE,
                                        vcgtq_f32( sqLength, MathlibNEON::FLOAT_MIN ) );
        ArrayFloat fInvLength = MathlibNEON::InvSqrtNonZero4( sqLength );

        const ArrayFloat acosW = MathlibNEON::ACos4( mChunkBase[0] );
        rfAngle = MathlibNEON::Cmov4( //sqLength > 0 ? (2 * ACos(w)) : 0
                    vaddq_f32( acosW, acosW ),
                    vdupq_n_f32(0.0f), mask );

        rkAxis.mChunkBase[0] = MathlibNEON::Cmov4(  //sqLength > 0 ? (x * fInvLength) : 1
                                    vmulq_f32( mChunkBase[1], fInvLength ), MathlibNEON::ONE, mask );
        rkAxis.mChunkBase[1] = MathlibNEON::Cmov4(  //sqLength > 0 ? (y * fInvLength) : 0
                                    vmulq_f32( mChunkBase[2], fInvLength ), vdupq_n_f32(0.0f), mask );
        rkAxis.mChunkBase[2] = MathlibNEON::Cmov4(  //sqLength > 0 ? (y * fInvLength) : 0
                                    vmulq_f32( mChunkBase[3], fInvLength ), vdupq_n_f32(0.0f), mask );
    }
    
    inline ArrayVector3 ArrayQuaternion::xAxis( void ) const
    {
        ArrayFloat fTy  = vaddq_f32( mChunkBase[2], mChunkBase[2] );     // 2 * y
        ArrayFloat fTz  = vaddq_f32( mChunkBase[3], mChunkBase[3] );     // 2 * z
        ArrayFloat fTwy = vmulq_f32( fTy, mChunkBase[0] );                   // fTy*w;
        ArrayFloat fTwz = vmulq_f32( fTz, mChunkBase[0] );                   // fTz*w;
        ArrayFloat fTxy = vmulq_f32( fTy, mChunkBase[1] );                   // fTy*x;
        ArrayFloat fTxz = vmulq_f32( fTz, mChunkBase[1] );                   // fTz*x;
        ArrayFloat fTyy = vmulq_f32( fTy, mChunkBase[2] );                   // fTy*y;
        ArrayFloat fTzz = vmulq_f32( fTz, mChunkBase[3] );                   // fTz*z;

        return ArrayVector3(
                vsubq_f32( MathlibNEON::ONE, vaddq_f32( fTyy, fTzz ) ),
                vaddq_f32( fTxy, fTwz ),
                vsubq_f32( fTxz, fTwy ) );
    }
    
    inline ArrayVector3 ArrayQuaternion::yAxis( void ) const
    {
        ArrayFloat fTx  = vaddq_f32( mChunkBase[1], mChunkBase[1] );     // 2 * x
        ArrayFloat fTy  = vaddq_f32( mChunkBase[2], mChunkBase[2] );     // 2 * y
        ArrayFloat fTz  = vaddq_f32( mChunkBase[3], mChunkBase[3] );     // 2 * z
        ArrayFloat fTwx = vmulq_f32( fTx, mChunkBase[0] );                   // fTx*w;
        ArrayFloat fTwz = vmulq_f32( fTz, mChunkBase[0] );                   // fTz*w;
        ArrayFloat fTxx = vmulq_f32( fTx, mChunkBase[1] );                   // fTx*x;
        ArrayFloat fTxy = vmulq_f32( fTy, mChunkBase[1] );                   // fTy*x;
        ArrayFloat fTyz = vmulq_f32( fTz, mChunkBase[2] );                   // fTz*y;
        ArrayFloat fTzz = vmulq_f32( fTz, mChunkBase[3] );                   // fTz*z;

        return ArrayVector3(
                vsubq_f32( fTxy, fTwz ),
                vsubq_f32( MathlibNEON::ONE, vaddq_f32( fTxx, fTzz ) ),
                vaddq_f32( fTyz, fTwx ) );
    }
    
    inline ArrayVector3 ArrayQuaternion::zAxis( void ) const
    {
        ArrayFloat fTx  = vaddq_f32( mChunkBase[1], mChunkBase[1] );     // 2 * x
        ArrayFloat fTy  = vaddq_f32( mChunkBase[2], mChunkBase[2] );     // 2 * y
        ArrayFloat fTz  = vaddq_f32( mChunkBase[3], mChunkBase[3] );     // 2 * z
        ArrayFloat fTwx = vmulq_f32( fTx, mChunkBase[0] );                   // fTx*w;
        ArrayFloat fTwy = vmulq_f32( fTy, mChunkBase[0] );                   // fTy*w;
        ArrayFloat fTxx = vmulq_f32( fTx, mChunkBase[1] );                   // fTx*x;
        ArrayFloat fTxz = vmulq_f32( fTz, mChunkBase[1] );                   // fTz*x;
        ArrayFloat fTyy = vmulq_f32( fTy, mChunkBase[2] );                   // fTy*y;
        ArrayFloat fTyz = vmulq_f32( fTz, mChunkBase[2] );                   // fTz*y;

        return ArrayVector3(
                vaddq_f32( fTxz, fTwy ),
                vsubq_f32( fTyz, fTwx ),
                vsubq_f32( MathlibNEON::ONE, vaddq_f32( fTxx, fTyy ) ) );
    }
    
    inline ArrayFloat ArrayQuaternion::Dot( const ArrayQuaternion& rkQ ) const
    {
        return
        vaddq_f32( vaddq_f32( vaddq_f32(
            vmulq_f32( mChunkBase[0], rkQ.mChunkBase[0] ) , //((w * vec.w   +
            vmulq_f32( mChunkBase[1], rkQ.mChunkBase[1] ) ),    //  x * vec.x ) +
            vmulq_f32( mChunkBase[2], rkQ.mChunkBase[2] ) ), //  y * vec.y ) +
            vmulq_f32( mChunkBase[3], rkQ.mChunkBase[3] ) );    //  z * vec.z
    }
    
    inline ArrayFloat ArrayQuaternion::Norm( void ) const
    {
        return
        vaddq_f32( vaddq_f32( vaddq_f32(
            vmulq_f32( mChunkBase[0], mChunkBase[0] ) , //((w * w   +
            vmulq_f32( mChunkBase[1], mChunkBase[1] ) ),    //  x * x ) +
            vmulq_f32( mChunkBase[2], mChunkBase[2] ) ), //  y * y ) +
            vmulq_f32( mChunkBase[3], mChunkBase[3] ) );    //  z * z
    }
    
    inline void ArrayQuaternion::normalise( void )
    {
        ArrayFloat sqLength = vaddq_f32( vaddq_f32( vaddq_f32(
            vmulq_f32( mChunkBase[0], mChunkBase[0] ) , //((w * w   +
            vmulq_f32( mChunkBase[1], mChunkBase[1] ) ),    //  x * x ) +
            vmulq_f32( mChunkBase[2], mChunkBase[2] ) ), //  y * y ) +
            vmulq_f32( mChunkBase[3], mChunkBase[3] ) );    //  z * z

        //Convert sqLength's 0s into 1, so that zero vectors remain as zero
        //Denormals are treated as 0 during the check.
        //Note: We could create a mask now and nuke nans after InvSqrt, however
        //generating the nans could impact performance in some architectures
        sqLength = MathlibNEON::Cmov4( sqLength, MathlibNEON::ONE,
                                        vcgtq_f32( sqLength, MathlibNEON::FLOAT_MIN ) );
        ArrayFloat invLength = MathlibNEON::InvSqrtNonZero4( sqLength );
        mChunkBase[0] = vmulq_f32( mChunkBase[0], invLength ); //w * invLength
        mChunkBase[1] = vmulq_f32( mChunkBase[1], invLength ); //x * invLength
        mChunkBase[2] = vmulq_f32( mChunkBase[2], invLength ); //y * invLength
        mChunkBase[3] = vmulq_f32( mChunkBase[3], invLength ); //z * invLength
    }
    
    inline ArrayQuaternion ArrayQuaternion::Inverse( void ) const
    {
        ArrayFloat fNorm = vaddq_f32( vaddq_f32( vaddq_f32(
            vmulq_f32( mChunkBase[0], mChunkBase[0] ) , //((w * w   +
            vmulq_f32( mChunkBase[1], mChunkBase[1] ) ),    //  x * x ) +
            vmulq_f32( mChunkBase[2], mChunkBase[2] ) ), //  y * y ) +
            vmulq_f32( mChunkBase[3], mChunkBase[3] ) );    //  z * z;

        //Will return a zero Quaternion if original is zero length (Quaternion's behavior)
        fNorm = MathlibNEON::Cmov4( fNorm, MathlibNEON::ONE,
                                    vcgtq_f32( fNorm, MathlibNEON::fEpsilon ) );
        ArrayFloat invNorm    = MathlibNEON::Inv4( fNorm );
        ArrayFloat negInvNorm = vmulq_f32( invNorm, MathlibNEON::NEG_ONE );

        return ArrayQuaternion(
            vmulq_f32( mChunkBase[0], invNorm ),        //w * invNorm
            vmulq_f32( mChunkBase[1], negInvNorm ), //x * -invNorm
            vmulq_f32( mChunkBase[2], negInvNorm ), //y * -invNorm
            vmulq_f32( mChunkBase[3], negInvNorm ) );   //z * -invNorm
    }
    
    inline ArrayQuaternion ArrayQuaternion::UnitInverse( void ) const
    {
        return ArrayQuaternion(
            mChunkBase[0],                                          //w
            vmulq_f32( mChunkBase[1], MathlibNEON::NEG_ONE ),       //-x
            vmulq_f32( mChunkBase[2], MathlibNEON::NEG_ONE ),       //-y
            vmulq_f32( mChunkBase[3], MathlibNEON::NEG_ONE ) ); //-z
    }
    
    inline ArrayQuaternion ArrayQuaternion::Exp( void ) const
    {
        // If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
        // exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
        // use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

        ArrayFloat fAngle = vrsqrteq_f32( vaddq_f32( vaddq_f32(                      //sqrt(
                                vmulq_f32( mChunkBase[1], mChunkBase[1] ),      //(x * x +
                                vmulq_f32( mChunkBase[2], mChunkBase[2] ) ),        //y * y) +
                                vmulq_f32( mChunkBase[3], mChunkBase[3] ) ) );  //z * z )

        ArrayFloat w, fSin;
        MathlibNEON::SinCos4( fAngle, fSin, w );

        //coeff = Abs(fSin) >= msEpsilon ? (fSin / fAngle) : 1.0f;
        ArrayFloat coeff = MathlibNEON::CmovRobust( vdivq_f32( fSin, fAngle ), MathlibNEON::ONE,
                                vcgeq_f32( MathlibNEON::Abs4( fSin ), MathlibNEON::fEpsilon ) );
        return ArrayQuaternion(
            w,                                          //cos( fAngle )
            vmulq_f32( mChunkBase[1], coeff ),      //x * coeff
            vmulq_f32( mChunkBase[2], coeff ),      //y * coeff
            vmulq_f32( mChunkBase[3], coeff ) );        //z * coeff
    }
    
    inline ArrayQuaternion ArrayQuaternion::Log( void ) const
    {
        // If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
        // log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
        // sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

        ArrayFloat fAngle    = MathlibNEON::ACos4( mChunkBase[0] );
        ArrayFloat fSin      = MathlibNEON::Sin4( fAngle );

        //mask = Math::Abs(w) < 1.0 && Math::Abs(fSin) >= msEpsilon
        ArrayFloat mask = vandq_s32(
                            vcltq_f32( MathlibNEON::Abs4( mChunkBase[0] ), MathlibNEON::ONE ),
                            vcgeq_f32( MathlibNEON::Abs4( fSin ), MathlibNEON::fEpsilon ) );

        //coeff = mask ? (fAngle / fSin) : 1.0
        //Unlike Exp(), we can use InvNonZero4 (which is faster) instead of div because we know for
        //sure CMov will copy the 1 instead of the NaN when fSin is close to zero, guarantee we might
        //not have in Exp()
        ArrayFloat coeff = MathlibNEON::CmovRobust( vmulq_f32( fAngle, MathlibNEON::InvNonZero4( fSin ) ),
                                                    MathlibNEON::ONE, mask );

        return ArrayQuaternion(
            vdupq_n_f32(0.0f),                          //w = 0
            vmulq_f32( mChunkBase[1], coeff ),      //x * coeff
            vmulq_f32( mChunkBase[2], coeff ),      //y * coeff
            vmulq_f32( mChunkBase[3], coeff ) );        //z * coeff
    }
    
    inline ArrayVector3 ArrayQuaternion::operator * ( const ArrayVector3 &v ) const
    {
        // nVidia SDK implementation
        ArrayVector3 qVec( mChunkBase[1], mChunkBase[2], mChunkBase[3] );

        ArrayVector3 uv = qVec.crossProduct( v );
        ArrayVector3 uuv    = qVec.crossProduct( uv );

        // uv = uv * (2.0f * w)
        ArrayFloat w2 = vaddq_f32( mChunkBase[0], mChunkBase[0] );
        uv.mChunkBase[0] = vmulq_f32( uv.mChunkBase[0], w2 );
        uv.mChunkBase[1] = vmulq_f32( uv.mChunkBase[1], w2 );
        uv.mChunkBase[2] = vmulq_f32( uv.mChunkBase[2], w2 );

        // uuv = uuv * 2.0f
        uuv.mChunkBase[0] = vaddq_f32( uuv.mChunkBase[0], uuv.mChunkBase[0] );
        uuv.mChunkBase[1] = vaddq_f32( uuv.mChunkBase[1], uuv.mChunkBase[1] );
        uuv.mChunkBase[2] = vaddq_f32( uuv.mChunkBase[2], uuv.mChunkBase[2] );

        //uv = v + uv + uuv
        uv.mChunkBase[0] = vaddq_f32( v.mChunkBase[0],
                                vaddq_f32( uv.mChunkBase[0], uuv.mChunkBase[0] ) );
        uv.mChunkBase[1] = vaddq_f32( v.mChunkBase[1],
                                vaddq_f32( uv.mChunkBase[1], uuv.mChunkBase[1] ) );
        uv.mChunkBase[2] = vaddq_f32( v.mChunkBase[2],
                                vaddq_f32( uv.mChunkBase[2], uuv.mChunkBase[2] ) );

        return uv;
    }
    
    inline void ArrayQuaternion::Cmov4( ArrayMaskR mask, const ArrayQuaternion &replacement )
    {
        ArrayFloat * RESTRICT_ALIAS aChunkBase = mChunkBase;
        const ArrayFloat * RESTRICT_ALIAS bChunkBase = replacement.mChunkBase;
        aChunkBase[0] = MathlibNEON::Cmov4( aChunkBase[0], bChunkBase[0], mask );
        aChunkBase[1] = MathlibNEON::Cmov4( aChunkBase[1], bChunkBase[1], mask );
        aChunkBase[2] = MathlibNEON::Cmov4( aChunkBase[2], bChunkBase[2], mask );
        aChunkBase[3] = MathlibNEON::Cmov4( aChunkBase[3], bChunkBase[3], mask );
    }
}
