
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

namespace Demi
{
    /** HOW THIS WORKS:

        Instead of writing like 12 times the same code, we use macros:
        DEFINE_OPERATION( ArrayVector3, ArrayVector3, +, _mm_add_ps );
        Means: "define '+' operator that takes both arguments as ArrayVector3 and use
        the _mm_add_ps intrinsic to do the job"

        Note that for scalars (i.e. floats) we use DEFINE_L_SCALAR_OPERATION/DEFINE_R_SCALAR_OPERATION
        depending on whether the scalar is on the left or right side of the operation
        (i.e. 2 * a vs a * 2)
        And for ArrayFloat scalars we use DEFINE_L_OPERATION/DEFINE_R_OPERATION

        As for division, we use specific scalar versions to increase performance (calculate
        the inverse of the scalar once, then multiply) as well as placing asserts in
        case of trying to divide by zero.

        I've considered using templates, but decided against it because wrong operator usage
        would raise cryptic compile error messages, and templates inability to limit which
        types are being used, leave the slight possibility of mixing completely unrelated
        types (i.e. ArrayQuaternion + ArrayVector4) and quietly compiling wrong code.
        Furthermore some platforms may not have decent compilers handling templates for
        major operator usage.

        Advantages:
            * Increased readability
            * Ease of reading & understanding
        Disadvantages:
            * Debugger can't go inside the operator. See workaround.

        As a workaround to the disadvantage, you can compile this code using cl.exe /EP /P /C to
        generate this file with macros replaced as actual code (very handy!)
    */
    // Arithmetic operations
#define DEFINE_OPERATION( leftClass, rightClass, op, op_func )\
    inline ArrayVector3 operator op ( const leftClass &lhs, const rightClass &rhs )\
    {\
        const ArrayFloat * RESTRICT_ALIAS lhsChunkBase = lhs.mChunkBase;\
        const ArrayFloat * RESTRICT_ALIAS rhsChunkBase = rhs.mChunkBase;\
        return ArrayVector3(\
                op_func( lhsChunkBase[0], rhsChunkBase[0] ),\
                op_func( lhsChunkBase[1], rhsChunkBase[1] ),\
                op_func( lhsChunkBase[2], rhsChunkBase[2] ) );\
       }
#define DEFINE_L_SCALAR_OPERATION( leftType, rightClass, op, op_func )\
    inline ArrayVector3 operator op ( const leftType fScalar, const rightClass &rhs )\
    {\
        ArrayFloat lhs = _mm_set1_ps( fScalar );\
        return ArrayVector3(\
                op_func( lhs, rhs.mChunkBase[0] ),\
                op_func( lhs, rhs.mChunkBase[1] ),\
                op_func( lhs, rhs.mChunkBase[2] ) );\
    }
#define DEFINE_R_SCALAR_OPERATION( leftClass, rightType, op, op_func )\
    inline ArrayVector3 operator op ( const leftClass &lhs, const rightType fScalar )\
    {\
        ArrayFloat rhs = _mm_set1_ps( fScalar );\
        return ArrayVector3(\
                op_func( lhs.mChunkBase[0], rhs ),\
                op_func( lhs.mChunkBase[1], rhs ),\
                op_func( lhs.mChunkBase[2], rhs ) );\
    }
#define DEFINE_L_OPERATION( leftType, rightClass, op, op_func )\
    inline ArrayVector3 operator op ( const leftType lhs, const rightClass &rhs )\
    {\
        return ArrayVector3(\
                op_func( lhs, rhs.mChunkBase[0] ),\
                op_func( lhs, rhs.mChunkBase[1] ),\
                op_func( lhs, rhs.mChunkBase[2] ) );\
    }
#define DEFINE_R_OPERATION( leftClass, rightType, op, op_func )\
    inline ArrayVector3 operator op ( const leftClass &lhs, const rightType rhs )\
    {\
        return ArrayVector3(\
                op_func( lhs.mChunkBase[0], rhs ),\
                op_func( lhs.mChunkBase[1], rhs ),\
                op_func( lhs.mChunkBase[2], rhs ) );\
    }

#define DEFINE_L_SCALAR_DIVISION( leftType, rightClass, op, op_func )\
    inline ArrayVector3 operator op ( const leftType fScalar, const rightClass &rhs )\
    {\
        ArrayFloat lhs = _mm_set1_ps( fScalar );\
        return ArrayVector3(\
                op_func( lhs, rhs.mChunkBase[0] ),\
                op_func( lhs, rhs.mChunkBase[1] ),\
                op_func( lhs, rhs.mChunkBase[2] ) );\
    }
#define DEFINE_R_SCALAR_DIVISION( leftClass, rightType, op, op_func )\
    inline ArrayVector3 operator op ( const leftClass &lhs, const rightType fScalar )\
    {\
        assert( fScalar != 0.0 );\
        float fInv = 1.0f / fScalar;\
        ArrayFloat rhs = _mm_set1_ps( fInv );\
        return ArrayVector3(\
                op_func( lhs.mChunkBase[0], rhs ),\
                op_func( lhs.mChunkBase[1], rhs ),\
                op_func( lhs.mChunkBase[2], rhs ) );\
    }

#ifdef NDEBUG
    #define ASSERT_DIV_BY_ZERO( values ) ((void)0)
#else
    #define ASSERT_DIV_BY_ZERO( values ) {\
                assert( _mm_movemask_ps( _mm_cmpeq_ps( values, _mm_setzero_ps() ) ) == 0 &&\
                "One of the 4 floats is a zero. Can't divide by zero" ); }
#endif

#define DEFINE_L_DIVISION( leftType, rightClass, op, op_func )\
    inline ArrayVector3 operator op ( const leftType lhs, const rightClass &rhs )\
    {\
        return ArrayVector3(\
                op_func( lhs, rhs.mChunkBase[0] ),\
                op_func( lhs, rhs.mChunkBase[1] ),\
                op_func( lhs, rhs.mChunkBase[2] ) );\
    }
#define DEFINE_R_DIVISION( leftClass, rightType, op, op_func )\
    inline ArrayVector3 operator op ( const leftClass &lhs, const rightType r )\
    {\
        ASSERT_DIV_BY_ZERO( r );\
        ArrayFloat rhs = MathlibSSE2::Inv4( r );\
        return ArrayVector3(\
                op_func( lhs.mChunkBase[0], rhs ),\
                op_func( lhs.mChunkBase[1], rhs ),\
                op_func( lhs.mChunkBase[2], rhs ) );\
    }

    // Update operations
#define DEFINE_UPDATE_OPERATION( leftClass, op, op_func )\
    inline void ArrayVector3::operator op ( const leftClass &a )\
    {\
        ArrayFloat * RESTRICT_ALIAS chunkBase = mChunkBase;\
        const ArrayFloat * RESTRICT_ALIAS aChunkBase = a.mChunkBase;\
        chunkBase[0] = op_func( chunkBase[0], aChunkBase[0] );\
        chunkBase[1] = op_func( chunkBase[1], aChunkBase[1] );\
        chunkBase[2] = op_func( chunkBase[2], aChunkBase[2] );\
    }
#define DEFINE_UPDATE_R_SCALAR_OPERATION( rightType, op, op_func )\
    inline void ArrayVector3::operator op ( const rightType fScalar )\
    {\
        ArrayFloat a = _mm_set1_ps( fScalar );\
        mChunkBase[0] = op_func( mChunkBase[0], a );\
        mChunkBase[1] = op_func( mChunkBase[1], a );\
        mChunkBase[2] = op_func( mChunkBase[2], a );\
    }
#define DEFINE_UPDATE_R_OPERATION( rightType, op, op_func )\
    inline void ArrayVector3::operator op ( const rightType a )\
    {\
        mChunkBase[0] = op_func( mChunkBase[0], a );\
        mChunkBase[1] = op_func( mChunkBase[1], a );\
        mChunkBase[2] = op_func( mChunkBase[2], a );\
    }
#define DEFINE_UPDATE_DIVISION( leftClass, op, op_func )\
    inline void ArrayVector3::operator op ( const leftClass &a )\
    {\
        ArrayFloat * RESTRICT_ALIAS chunkBase = mChunkBase;\
        const ArrayFloat * RESTRICT_ALIAS aChunkBase = a.mChunkBase;\
        chunkBase[0] = op_func( chunkBase[0], aChunkBase[0] );\
        chunkBase[1] = op_func( chunkBase[1], aChunkBase[1] );\
        chunkBase[2] = op_func( chunkBase[2], aChunkBase[2] );\
    }
#define DEFINE_UPDATE_R_SCALAR_DIVISION( rightType, op, op_func )\
    inline void ArrayVector3::operator op ( const rightType fScalar )\
    {\
        assert( fScalar != 0.0 );\
        float fInv = 1.0f / fScalar;\
        ArrayFloat a = _mm_set1_ps( fInv );\
        mChunkBase[0] = op_func( mChunkBase[0], a );\
        mChunkBase[1] = op_func( mChunkBase[1], a );\
        mChunkBase[2] = op_func( mChunkBase[2], a );\
    }
#define DEFINE_UPDATE_R_DIVISION( rightType, op, op_func )\
    inline void ArrayVector3::operator op ( const rightType _a )\
    {\
        ASSERT_DIV_BY_ZERO( _a );\
        ArrayFloat a = MathlibSSE2::Inv4( _a );\
        mChunkBase[0] = op_func( mChunkBase[0], a );\
        mChunkBase[1] = op_func( mChunkBase[1], a );\
        mChunkBase[2] = op_func( mChunkBase[2], a );\
    }

    inline const ArrayVector3& ArrayVector3::operator + () const
    {
        return *this;
    };
    
    inline ArrayVector3 ArrayVector3::operator - () const
    {
        return ArrayVector3(
            _mm_xor_ps( mChunkBase[0], MathlibSSE2::SIGN_MASK ),    //-x
            _mm_xor_ps( mChunkBase[1], MathlibSSE2::SIGN_MASK ),    //-y
            _mm_xor_ps( mChunkBase[2], MathlibSSE2::SIGN_MASK ) );  //-z
    }
    

    // + Addition
    DEFINE_OPERATION( ArrayVector3, ArrayVector3, +, _mm_add_ps );
    DEFINE_L_SCALAR_OPERATION( float, ArrayVector3, +, _mm_add_ps );
    DEFINE_R_SCALAR_OPERATION( ArrayVector3, float, +, _mm_add_ps );

    DEFINE_L_OPERATION( ArrayFloat, ArrayVector3, +, _mm_add_ps );
    DEFINE_R_OPERATION( ArrayVector3, ArrayFloat, +, _mm_add_ps );

    // - Subtraction
    DEFINE_OPERATION( ArrayVector3, ArrayVector3, -, _mm_sub_ps );
    DEFINE_L_SCALAR_OPERATION( float, ArrayVector3, -, _mm_sub_ps );
    DEFINE_R_SCALAR_OPERATION( ArrayVector3, float, -, _mm_sub_ps );

    DEFINE_L_OPERATION( ArrayFloat, ArrayVector3, -, _mm_sub_ps );
    DEFINE_R_OPERATION( ArrayVector3, ArrayFloat, -, _mm_sub_ps );

    // * Multiplication
    DEFINE_OPERATION( ArrayVector3, ArrayVector3, *, _mm_mul_ps );
    DEFINE_L_SCALAR_OPERATION( float, ArrayVector3, *, _mm_mul_ps );
    DEFINE_R_SCALAR_OPERATION( ArrayVector3, float, *, _mm_mul_ps );

    DEFINE_L_OPERATION( ArrayFloat, ArrayVector3, *, _mm_mul_ps );
    DEFINE_R_OPERATION( ArrayVector3, ArrayFloat, *, _mm_mul_ps );

    // / Division (scalar versions use mul instead of div, because they mul against the reciprocal)
    DEFINE_OPERATION( ArrayVector3, ArrayVector3, /, _mm_div_ps );
    DEFINE_L_SCALAR_DIVISION( float, ArrayVector3, /, _mm_div_ps );
    DEFINE_R_SCALAR_DIVISION( ArrayVector3, float, /, _mm_mul_ps );

    DEFINE_L_DIVISION( ArrayFloat, ArrayVector3, /, _mm_div_ps );
    DEFINE_R_DIVISION( ArrayVector3, ArrayFloat, /, _mm_mul_ps );

    inline ArrayVector3 ArrayVector3::Cmov4( const ArrayVector3 &arg1, const ArrayVector3 &arg2, ArrayFloat mask )
    {
        return ArrayVector3(
                MathlibSSE2::Cmov4( arg1.mChunkBase[0], arg2.mChunkBase[0], mask ),
                MathlibSSE2::Cmov4( arg1.mChunkBase[1], arg2.mChunkBase[1], mask ),
                MathlibSSE2::Cmov4( arg1.mChunkBase[2], arg2.mChunkBase[2], mask ) );
    }

    // Update operations
    // +=
    DEFINE_UPDATE_OPERATION(            ArrayVector3,       +=, _mm_add_ps );
    DEFINE_UPDATE_R_SCALAR_OPERATION(   float,               +=, _mm_add_ps );
    DEFINE_UPDATE_R_OPERATION(          ArrayFloat,          +=, _mm_add_ps );

    // -=
    DEFINE_UPDATE_OPERATION(            ArrayVector3,       -=, _mm_sub_ps );
    DEFINE_UPDATE_R_SCALAR_OPERATION(   float,               -=, _mm_sub_ps );
    DEFINE_UPDATE_R_OPERATION(          ArrayFloat,          -=, _mm_sub_ps );

    // *=
    DEFINE_UPDATE_OPERATION(            ArrayVector3,       *=, _mm_mul_ps );
    DEFINE_UPDATE_R_SCALAR_OPERATION(   float,               *=, _mm_mul_ps );
    DEFINE_UPDATE_R_OPERATION(          ArrayFloat,          *=, _mm_mul_ps );

    // /=
    DEFINE_UPDATE_DIVISION(             ArrayVector3,       /=, _mm_div_ps );
    DEFINE_UPDATE_R_SCALAR_DIVISION(    float,               /=, _mm_mul_ps );
    DEFINE_UPDATE_R_DIVISION(           ArrayFloat,          /=, _mm_mul_ps );

    //Functions
    
    inline ArrayFloat ArrayVector3::length() const
    {
        return
        _mm_sqrt_ps( _mm_add_ps( _mm_add_ps(                    //sqrt(
                _mm_mul_ps( mChunkBase[0], mChunkBase[0] ), //(x * x +
                _mm_mul_ps( mChunkBase[1], mChunkBase[1] ) ),   //y * y) +
            _mm_mul_ps( mChunkBase[2], mChunkBase[2] ) ) ); //z * z )
    }
    
    inline ArrayFloat ArrayVector3::squaredLength() const
    {
        return
        _mm_add_ps( _mm_add_ps(
            _mm_mul_ps( mChunkBase[0], mChunkBase[0] ), //(x * x +
            _mm_mul_ps( mChunkBase[1], mChunkBase[1] ) ),   //y * y) +
        _mm_mul_ps( mChunkBase[2], mChunkBase[2] ) );       //z * z )
    }
    
    inline ArrayFloat ArrayVector3::distance( const ArrayVector3& rhs ) const
    {
        return (*this - rhs).length();
    }
    
    inline ArrayFloat ArrayVector3::squaredDistance( const ArrayVector3& rhs ) const
    {
        return (*this - rhs).squaredLength();
    }
    
    inline ArrayFloat ArrayVector3::dotProduct( const ArrayVector3& vec ) const
    {
        return
        _mm_add_ps( _mm_add_ps(
            _mm_mul_ps( mChunkBase[0], vec.mChunkBase[0] ) ,    //( x * vec.x   +
            _mm_mul_ps( mChunkBase[1], vec.mChunkBase[1] ) ),   //  y * vec.y ) +
            _mm_mul_ps( mChunkBase[2], vec.mChunkBase[2] ) );   //  z * vec.z
    }
    
    inline ArrayFloat ArrayVector3::absDotProduct( const ArrayVector3& vec ) const
    {
        return
        _mm_add_ps( _mm_add_ps(
            MathlibSSE2::Abs4( _mm_mul_ps( mChunkBase[0], vec.mChunkBase[0] ) ),    //( abs( x * vec.x )   +
            MathlibSSE2::Abs4( _mm_mul_ps( mChunkBase[1], vec.mChunkBase[1] ) ) ),//  abs( y * vec.y ) ) +
            MathlibSSE2::Abs4( _mm_mul_ps( mChunkBase[2], vec.mChunkBase[2] ) ) );//  abs( z * vec.z )
    }
    
    inline void ArrayVector3::normalise( void )
    {
        ArrayFloat sqLength = _mm_add_ps( _mm_add_ps(
            _mm_mul_ps( mChunkBase[0], mChunkBase[0] ), //(x * x +
            _mm_mul_ps( mChunkBase[1], mChunkBase[1] ) ),   //y * y) +
        _mm_mul_ps( mChunkBase[2], mChunkBase[2] ) );       //z * z )

        //Convert sqLength's 0s into 1, so that zero vectors remain as zero
        //Denormals are treated as 0 during the check.
        //Note: We could create a mask now and nuke nans after InvSqrt, however
        //generating the nans could impact performance in some architectures
        sqLength = MathlibSSE2::Cmov4( sqLength, MathlibSSE2::ONE,
                                        _mm_cmpgt_ps( sqLength, MathlibSSE2::FLOAT_MIN ) );
        ArrayFloat invLength = MathlibSSE2::InvSqrtNonZero4( sqLength );
        mChunkBase[0] = _mm_mul_ps( mChunkBase[0], invLength ); //x * invLength
        mChunkBase[1] = _mm_mul_ps( mChunkBase[1], invLength ); //y * invLength
        mChunkBase[2] = _mm_mul_ps( mChunkBase[2], invLength ); //z * invLength
    }
    
    inline ArrayVector3 ArrayVector3::crossProduct( const ArrayVector3& rkVec ) const
    {
        return ArrayVector3(
            _mm_sub_ps(
                _mm_mul_ps( mChunkBase[1], rkVec.mChunkBase[2] ),
                _mm_mul_ps( mChunkBase[2], rkVec.mChunkBase[1] ) ), //y * rkVec.z - z * rkVec.y
            _mm_sub_ps(
                _mm_mul_ps( mChunkBase[2], rkVec.mChunkBase[0] ),
                _mm_mul_ps( mChunkBase[0], rkVec.mChunkBase[2] ) ), //z * rkVec.x - x * rkVec.z
            _mm_sub_ps(
                _mm_mul_ps( mChunkBase[0], rkVec.mChunkBase[1] ),
                _mm_mul_ps( mChunkBase[1], rkVec.mChunkBase[0] ) ) );   //x * rkVec.y - y * rkVec.x
    }
    
    inline ArrayVector3 ArrayVector3::midPoint( const ArrayVector3& rkVec ) const
    {
        return ArrayVector3(
            _mm_mul_ps( _mm_add_ps( mChunkBase[0], rkVec.mChunkBase[0] ), MathlibSSE2::HALF ),
            _mm_mul_ps( _mm_add_ps( mChunkBase[1], rkVec.mChunkBase[1] ), MathlibSSE2::HALF ),
            _mm_mul_ps( _mm_add_ps( mChunkBase[2], rkVec.mChunkBase[2] ), MathlibSSE2::HALF ) );
    }
    
    inline void ArrayVector3::makeFloor( const ArrayVector3& cmp )
    {
        ArrayFloat * RESTRICT_ALIAS aChunkBase = mChunkBase;
        const ArrayFloat * RESTRICT_ALIAS bChunkBase = cmp.mChunkBase;
        aChunkBase[0] = _mm_min_ps( aChunkBase[0], bChunkBase[0] );
        aChunkBase[1] = _mm_min_ps( aChunkBase[1], bChunkBase[1] );
        aChunkBase[2] = _mm_min_ps( aChunkBase[2], bChunkBase[2] );
    }
    
    inline void ArrayVector3::makeCeil( const ArrayVector3& cmp )
    {
        ArrayFloat * RESTRICT_ALIAS aChunkBase = mChunkBase;
        const ArrayFloat * RESTRICT_ALIAS bChunkBase = cmp.mChunkBase;
        aChunkBase[0] = _mm_max_ps( aChunkBase[0], bChunkBase[0] );
        aChunkBase[1] = _mm_max_ps( aChunkBase[1], bChunkBase[1] );
        aChunkBase[2] = _mm_max_ps( aChunkBase[2], bChunkBase[2] );
    }
    
    inline ArrayFloat ArrayVector3::getMinComponent() const
    {
        return _mm_min_ps( mChunkBase[0], _mm_min_ps( mChunkBase[1], mChunkBase[2] ) );
    }
    
    inline ArrayFloat ArrayVector3::getMaxComponent() const
    {
        return _mm_max_ps( mChunkBase[0], _mm_max_ps( mChunkBase[1], mChunkBase[2] ) );
    }
    
    inline void ArrayVector3::setToSign()
    {
        // x = 1.0f | (x & 0x80000000)
        ArrayFloat signMask = _mm_set1_ps( -0.0f );
        mChunkBase[0] = _mm_or_ps( MathlibSSE2::ONE, _mm_and_ps( signMask, mChunkBase[0] ) );
        mChunkBase[1] = _mm_or_ps( MathlibSSE2::ONE, _mm_and_ps( signMask, mChunkBase[1] ) );
        mChunkBase[2] = _mm_or_ps( MathlibSSE2::ONE, _mm_and_ps( signMask, mChunkBase[2] ) );
    }
    
    inline ArrayVector3 ArrayVector3::perpendicular( void ) const
    {
        ArrayVector3 perp = this->crossProduct( ArrayVector3::UNIT_X );

        const ArrayFloat mask = _mm_cmple_ps( perp.squaredLength(), MathlibSSE2::fSqEpsilon );
        // Check length
        if( _mm_movemask_ps( mask ) )
        {
            /* One or more of these vectors are the X axis multiplied by a scalar,
               so we have to use another axis for those.
            */
            ArrayVector3 perp1 = this->crossProduct( ArrayVector3::UNIT_Y );
            perp.mChunkBase[0] = MathlibSSE2::Cmov4( perp1.mChunkBase[0], perp.mChunkBase[0], mask );
            perp.mChunkBase[1] = MathlibSSE2::Cmov4( perp1.mChunkBase[1], perp.mChunkBase[1], mask );
            perp.mChunkBase[2] = MathlibSSE2::Cmov4( perp1.mChunkBase[2], perp.mChunkBase[2], mask );
        }
        perp.normalise();

        return perp;
    }
    
    inline ArrayVector3 ArrayVector3::normalisedCopy( void ) const
    {
        ArrayFloat sqLength = _mm_add_ps( _mm_add_ps(
            _mm_mul_ps( mChunkBase[0], mChunkBase[0] ), //(x * x +
            _mm_mul_ps( mChunkBase[1], mChunkBase[1] ) ),   //y * y) +
        _mm_mul_ps( mChunkBase[2], mChunkBase[2] ) );       //z * z )

        //Convert sqLength's 0s into 1, so that zero vectors remain as zero
        //Denormals are treated as 0 during the check.
        //Note: We could create a mask now and nuke nans after InvSqrt, however
        //generating the nans could impact performance in some architectures
        sqLength = MathlibSSE2::Cmov4( sqLength, MathlibSSE2::ONE,
                                        _mm_cmpgt_ps( sqLength, MathlibSSE2::FLOAT_MIN ) );
        ArrayFloat invLength = MathlibSSE2::InvSqrtNonZero4( sqLength );

        return ArrayVector3(
            _mm_mul_ps( mChunkBase[0], invLength ), //x * invLength
            _mm_mul_ps( mChunkBase[1], invLength ), //y * invLength
            _mm_mul_ps( mChunkBase[2], invLength ) );   //z * invLength
    }
    
    inline ArrayVector3 ArrayVector3::reflect( const ArrayVector3& normal ) const
    {
        const ArrayFloat twoPointZero = _mm_set_ps1( 2.0f );
        return ( *this - ( _mm_mul_ps( twoPointZero, this->dotProduct( normal ) ) * normal ) );
    }
    
    inline void ArrayVector3::inverseLeaveZeroes( void )
    {
        //Use InvNonZero, we're gonna nuke the NaNs anyway.
        mChunkBase[0] = MathlibSSE2::CmovRobust( mChunkBase[0], MathlibSSE2::InvNonZero4(mChunkBase[0]),
                                                 _mm_cmpeq_ps( mChunkBase[0], _mm_setzero_ps() ) );
        mChunkBase[1] = MathlibSSE2::CmovRobust( mChunkBase[1], MathlibSSE2::InvNonZero4(mChunkBase[1]),
                                                 _mm_cmpeq_ps( mChunkBase[1], _mm_setzero_ps() ) );
        mChunkBase[2] = MathlibSSE2::CmovRobust( mChunkBase[2], MathlibSSE2::InvNonZero4(mChunkBase[2]),
                                                 _mm_cmpeq_ps( mChunkBase[2], _mm_setzero_ps() ) );
    }
    
    inline int ArrayVector3::isNaN( void ) const
    {
        ArrayFloat mask = _mm_and_ps( _mm_and_ps( 
            _mm_cmpeq_ps( mChunkBase[0], mChunkBase[0] ),
            _mm_cmpeq_ps( mChunkBase[1], mChunkBase[1] ) ),
            _mm_cmpeq_ps( mChunkBase[2], mChunkBase[2] ) );

        return _mm_movemask_ps( mask ) ^ 0x0000000f;
    }
    
    inline ArrayVector3 ArrayVector3::primaryAxis( void ) const
    {
        // We could've used some operators, i.e.
        // xVec = MathlibSSE2::Cmov( ArrayVector3::UNIT_X, ArrayVector3::NEGATIVE_UNIT_X )
        // and so forth, which would've increased readability considerably. However,
        // MSVC's compiler ability to do constant propagation & remove dead code sucks,
        // which means it would try to cmov the Y & Z component even though we already
        // know it's always zero for both +x & -x. Therefore, we do it the manual
        // way. Doing this the "human readable way" results in massive amounts of wasted
        // instructions and stack memory abuse.
        // See DiVec3::primaryAxis() to understand what's actually going on.
        ArrayFloat absx = MathlibSSE2::Abs4( mChunkBase[0] );
        ArrayFloat absy = MathlibSSE2::Abs4( mChunkBase[1] );
        ArrayFloat absz = MathlibSSE2::Abs4( mChunkBase[2] );

        //xVec = x > 0 ? DiVec3::UNIT_X : DiVec3::NEGATIVE_UNIT_X;
        ArrayFloat sign = MathlibSSE2::Cmov4( _mm_set1_ps( 1.0f ), _mm_set1_ps( -1.0f ),
                                            _mm_cmpgt_ps( mChunkBase[0], _mm_setzero_ps() ) );
        ArrayVector3 xVec( sign, _mm_setzero_ps(), _mm_setzero_ps() );

        //yVec = y > 0 ? DiVec3::UNIT_Y : DiVec3::NEGATIVE_UNIT_Y;
        sign = MathlibSSE2::Cmov4( _mm_set1_ps( 1.0f ), _mm_set1_ps( -1.0f ),
                                    _mm_cmpgt_ps( mChunkBase[1], _mm_setzero_ps() ) );
        ArrayVector3 yVec( _mm_setzero_ps(), sign, _mm_setzero_ps() );

        //zVec = z > 0 ? DiVec3::UNIT_Z : DiVec3::NEGATIVE_UNIT_Z;
        sign = MathlibSSE2::Cmov4( _mm_set1_ps( 1.0f ), _mm_set1_ps( -1.0f ),
                                    _mm_cmpgt_ps( mChunkBase[2], _mm_setzero_ps() ) );
        ArrayVector3 zVec( _mm_setzero_ps(), _mm_setzero_ps(), sign );

        //xVec = absx > absz ? xVec : zVec
        ArrayFloat mask = _mm_cmpgt_ps( absx, absz );
        xVec.mChunkBase[0] = MathlibSSE2::Cmov4( xVec.mChunkBase[0], zVec.mChunkBase[0], mask );
        xVec.mChunkBase[2] = MathlibSSE2::Cmov4( xVec.mChunkBase[2], zVec.mChunkBase[2], mask );

        //yVec = absy > absz ? yVec : zVec
        mask = _mm_cmpgt_ps( absy, absz );
        yVec.mChunkBase[1] = MathlibSSE2::Cmov4( yVec.mChunkBase[1], zVec.mChunkBase[1], mask );
        yVec.mChunkBase[2] = MathlibSSE2::Cmov4( yVec.mChunkBase[2], zVec.mChunkBase[2], mask );

        yVec.Cmov4( _mm_cmpgt_ps( absx, absy ), xVec );
        return yVec;
    }
    
    inline DiVec3 ArrayVector3::collapseMin( void ) const
    {
        DEMI_ALIGNED_DECL( float, vals[4], DEMI_SIMD_ALIGNMENT );
        ArrayFloat aosVec0, aosVec1, aosVec2, aosVec3;

        //Transpose XXXX YYYY ZZZZ to XYZZ XYZZ XYZZ XYZZ
        ArrayFloat tmp2, tmp0;
        tmp0   = _mm_shuffle_ps( mChunkBase[0], mChunkBase[1], 0x44 );
        tmp2   = _mm_shuffle_ps( mChunkBase[0], mChunkBase[1], 0xEE );

        aosVec0 = _mm_shuffle_ps( tmp0, mChunkBase[2], 0x08 );
        aosVec1 = _mm_shuffle_ps( tmp0, mChunkBase[2], 0x5D );
        aosVec2 = _mm_shuffle_ps( tmp2, mChunkBase[2], 0xA8 );
        aosVec3 = _mm_shuffle_ps( tmp2, mChunkBase[2], 0xFD );

        //Do the actual operation
        aosVec0 = _mm_min_ps( aosVec0, aosVec1 );
        aosVec2 = _mm_min_ps( aosVec2, aosVec3 );
        aosVec0 = _mm_min_ps( aosVec0, aosVec2 );

        _mm_store_ps( vals, aosVec0 );

        return DiVec3( vals[0], vals[1], vals[2] );
    }
    
    inline DiVec3 ArrayVector3::collapseMax( void ) const
    {
        DEMI_ALIGNED_DECL( float, vals[4], DEMI_SIMD_ALIGNMENT );
        ArrayFloat aosVec0, aosVec1, aosVec2, aosVec3;

        //Transpose XXXX YYYY ZZZZ to XYZZ XYZZ XYZZ XYZZ
        ArrayFloat tmp2, tmp0;
        tmp0   = _mm_shuffle_ps( mChunkBase[0], mChunkBase[1], 0x44 );
        tmp2   = _mm_shuffle_ps( mChunkBase[0], mChunkBase[1], 0xEE );

        aosVec0 = _mm_shuffle_ps( tmp0, mChunkBase[2], 0x08 );
        aosVec1 = _mm_shuffle_ps( tmp0, mChunkBase[2], 0x5D );
        aosVec2 = _mm_shuffle_ps( tmp2, mChunkBase[2], 0xA8 );
        aosVec3 = _mm_shuffle_ps( tmp2, mChunkBase[2], 0xFD );

        //Do the actual operation
        aosVec0 = _mm_max_ps( aosVec0, aosVec1 );
        aosVec2 = _mm_max_ps( aosVec2, aosVec3 );
        aosVec0 = _mm_max_ps( aosVec0, aosVec2 );

        _mm_store_ps( vals, aosVec0 );

        return DiVec3( vals[0], vals[1], vals[2] );
    }
    
    inline void ArrayVector3::Cmov4( ArrayFloat mask, const ArrayVector3 &replacement )
    {
        ArrayFloat * RESTRICT_ALIAS aChunkBase = mChunkBase;
        const ArrayFloat * RESTRICT_ALIAS bChunkBase = replacement.mChunkBase;
        aChunkBase[0] = MathlibSSE2::Cmov4( aChunkBase[0], bChunkBase[0], mask );
        aChunkBase[1] = MathlibSSE2::Cmov4( aChunkBase[1], bChunkBase[1], mask );
        aChunkBase[2] = MathlibSSE2::Cmov4( aChunkBase[2], bChunkBase[2], mask );
    }
    
    inline void ArrayVector3::CmovRobust( ArrayFloat mask, const ArrayVector3 &replacement )
    {
        ArrayFloat * RESTRICT_ALIAS aChunkBase = mChunkBase;
        const ArrayFloat * RESTRICT_ALIAS bChunkBase = replacement.mChunkBase;
        aChunkBase[0] = MathlibSSE2::CmovRobust( aChunkBase[0], bChunkBase[0], mask );
        aChunkBase[1] = MathlibSSE2::CmovRobust( aChunkBase[1], bChunkBase[1], mask );
        aChunkBase[2] = MathlibSSE2::CmovRobust( aChunkBase[2], bChunkBase[2], mask );
    }
    
    inline void ArrayVector3::loadFromAoS( const float * RESTRICT_ALIAS src )
    {
        //Do not use the unpack version, use the shuffle. Shuffle is faster in k10 processors
        //("The conceptual shuffle" http://developer.amd.com/community/blog/the-conceptual-shuffle/)
        //and the unpack version uses 64-bit moves, which can cause store forwarding issues when
        //then loading them with 128-bit movaps
#define _MM_TRANSPOSE3_SRC_DST_PS(row0, row1, row2, row3, dst0, dst1, dst2) { \
            __m128 tmp3, tmp2, tmp1, tmp0;                          \
                                                                    \
            tmp0   = _mm_shuffle_ps((row0), (row1), 0x44);          \
            tmp2   = _mm_shuffle_ps((row0), (row1), 0xEE);          \
            tmp1   = _mm_shuffle_ps((row2), (row3), 0x44);          \
            tmp3   = _mm_shuffle_ps((row2), (row3), 0xEE);          \
                                                                    \
            (dst0) = _mm_shuffle_ps(tmp0, tmp1, 0x88);              \
            (dst1) = _mm_shuffle_ps(tmp0, tmp1, 0xDD);              \
            (dst2) = _mm_shuffle_ps(tmp2, tmp3, 0x88);              \
        }

        _MM_TRANSPOSE3_SRC_DST_PS(
                            _mm_load_ps( &src[0] ), _mm_load_ps( &src[4] ),
                            _mm_load_ps( &src[8] ), _mm_load_ps( &src[12] ),
                            this->mChunkBase[0], this->mChunkBase[1],
                            this->mChunkBase[2] );
    }
    

#undef DEFINE_OPERATION
#undef DEFINE_L_SCALAR_OPERATION
#undef DEFINE_R_SCALAR_OPERATION
#undef DEFINE_L_OPERATION
#undef DEFINE_R_OPERATION
#undef DEFINE_DIVISION
#undef DEFINE_L_DIVISION
#undef DEFINE_R_SCALAR_DIVISION
#undef DEFINE_L_SCALAR_DIVISION
#undef DEFINE_R_DIVISION

#undef DEFINE_UPDATE_OPERATION
#undef DEFINE_UPDATE_R_SCALAR_OPERATION
#undef DEFINE_UPDATE_R_OPERATION
#undef DEFINE_UPDATE_DIVISION
#undef DEFINE_UPDATE_R_SCALAR_DIVISION
#undef DEFINE_UPDATE_R_DIVISION

#undef _MM_TRANSPOSE3_SRC_DST_PS
}
