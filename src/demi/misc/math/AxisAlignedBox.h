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

#ifndef DiAxisAlignedBox_h__
#define DiAxisAlignedBox_h__


#include "MiscPrerequisites.h"

#include "math/mathMisc.h"
#include "math/vector3.h"
#include "math/matrix4.h"

namespace Demi 
{
    class DI_MISC_API DiAABB
    {
    public:

        enum Extent
        {
            EXTENT_NULL,
            EXTENT_FINITE,
            EXTENT_INFINITE
        };

        DiVec3            mMinimum;
        DiVec3            mMaximum;

    protected:

        Extent            mExtent;
        mutable DiVec3* mCorners;

    public:
        /*
           1-----2
          /|    /|
         / |   / |
        5-----4  |
        |  0--|--3
        | /   | /
        |/    |/
        6-----7
        */
        typedef enum 
        {
            FAR_LEFT_BOTTOM        = 0,
            FAR_LEFT_TOP        = 1,
            FAR_RIGHT_TOP        = 2,
            FAR_RIGHT_BOTTOM    = 3,
            NEAR_RIGHT_BOTTOM    = 7,
            NEAR_LEFT_BOTTOM    = 6,
            NEAR_LEFT_TOP        = 5,
            NEAR_RIGHT_TOP        = 4
        }CornerEnum;

        inline DiAABB() :
        mMinimum(DiVec3::ZERO), mMaximum(DiVec3::UNIT_SCALE), mCorners(NULL)
        {
            SetMinimum( -0.5, -0.5, -0.5 );
            SetMaximum( 0.5, 0.5, 0.5 );
            mExtent = EXTENT_NULL;
        }
        inline DiAABB(Extent e) : 
        mMinimum(DiVec3::ZERO), mMaximum(DiVec3::UNIT_SCALE), mCorners(NULL)
        {
            SetMinimum( -0.5, -0.5, -0.5 );
            SetMaximum( 0.5, 0.5, 0.5 );
            mExtent = e;
        }

        inline DiAABB(const DiAABB & rkBox) : 
        mMinimum(DiVec3::ZERO), mMaximum(DiVec3::UNIT_SCALE), mCorners(NULL)
        {
            if (rkBox.IsNull())
            {
                SetNull();
            }
            else if (rkBox.IsInfinite())
            {
                SetInfinite();
            }
            else
            {
                SetExtents( rkBox.mMinimum, rkBox.mMaximum );
            }
        }

        inline DiAABB( const DiVec3& min, const DiVec3& max ) : 
        mMinimum(DiVec3::ZERO), mMaximum(DiVec3::UNIT_SCALE), mCorners(NULL)
        {
            SetExtents( min, max );
        }

        inline DiAABB(
            float mx, float my, float mz,
            float Mx, float My, float Mz ) : 
        mMinimum(DiVec3::ZERO),mMaximum(DiVec3::UNIT_SCALE), mCorners(NULL)
        {
            SetExtents( mx, my, mz, Mx, My, Mz );
        }

        inline DiAABB(float min, float max) :
            mMinimum(DiVec3::ZERO), mMaximum(DiVec3::UNIT_SCALE), mCorners(NULL)
        {
            SetExtents(min, min, min, max, max, max);
        }

        DiAABB& operator=(const DiAABB& rhs)
        {
            if (rhs.IsNull())
            {
                SetNull();
            }
            else if (rhs.IsInfinite())
            {
                SetInfinite();
            }
            else
            {
                SetExtents(rhs.mMinimum, rhs.mMaximum);
            }

            return *this;
        }

        ~DiAABB()
        {
            SAFE_ARRAY_DELETE(mCorners);
        }


        inline const DiVec3& GetMinimum(void) const
        { 
            return mMinimum; 
        }

        inline DiVec3& GetMinimum(void)
        { 
            return mMinimum; 
        }

        inline const DiVec3& GetMaximum(void) const
        { 
            return mMaximum;
        }

        inline DiVec3& GetMaximum(void)
        { 
            return mMaximum;
        }

        inline void SetVector(const DiVec3& pos)
        {
            if ( pos.x < mMinimum.x ) mMinimum.x = pos.x;
            if ( pos.y < mMinimum.y ) mMinimum.y = pos.y;
            if ( pos.z < mMinimum.z ) mMinimum.z = pos.z;

            if ( pos.x > mMaximum.x ) mMaximum.x = pos.x;
            if ( pos.y > mMaximum.y ) mMaximum.y = pos.y;
            if ( pos.z > mMaximum.z ) mMaximum.z = pos.z;
        }

        inline void SetMinimum( const DiVec3& vec )
        {
            mExtent = EXTENT_FINITE;
            mMinimum = vec;
        }

        inline void SetMinimum( float x, float y, float z )
        {
            mExtent = EXTENT_FINITE;
            mMinimum.x = x;
            mMinimum.y = y;
            mMinimum.z = z;
        }

        inline void SetMinimumX(float x)
        {
            mMinimum.x = x;
        }

        inline void SetMinimumY(float y)
        {
            mMinimum.y = y;
        }

        inline void SetMinimumZ(float z)
        {
            mMinimum.z = z;
        }

        inline void SetMaximum( const DiVec3& vec )
        {
            mExtent = EXTENT_FINITE;
            mMaximum = vec;
        }

        inline void SetMaximum( float x, float y, float z )
        {
            mExtent = EXTENT_FINITE;
            mMaximum.x = x;
            mMaximum.y = y;
            mMaximum.z = z;
        }

        inline void SetMaximumX( float x )
        {
            mMaximum.x = x;
        }

        inline void SetMaximumY( float y )
        {
            mMaximum.y = y;
        }

        inline void SetMaximumZ( float z )
        {
            mMaximum.z = z;
        }

        inline void SetExtents( const DiVec3& min, const DiVec3& max )
        {
            mExtent = EXTENT_FINITE;
            mMinimum = min;
            mMaximum = max;
        }

        inline void SetExtents(
            float mx, float my, float mz,
            float Mx, float My, float Mz )
        {
            mExtent = EXTENT_FINITE;

            mMinimum.x = mx;
            mMinimum.y = my;
            mMinimum.z = mz;

            mMaximum.x = Mx;
            mMaximum.y = My;
            mMaximum.z = Mz;

        }

        inline const DiVec3* GetAllCorners(void) const
        {
            DI_ASSERT( (mExtent == EXTENT_FINITE) );

            if (!mCorners)
            {
                //m_pCorners = DI_NEW DiVec3[8];
                mCorners = new DiVec3[8];
            }

            mCorners[0] = mMinimum;
            mCorners[1].x = mMinimum.x; mCorners[1].y = mMaximum.y; mCorners[1].z = mMinimum.z;
            mCorners[2].x = mMaximum.x; mCorners[2].y = mMaximum.y; mCorners[2].z = mMinimum.z;
            mCorners[3].x = mMaximum.x; mCorners[3].y = mMinimum.y; mCorners[3].z = mMinimum.z;            

            mCorners[4] = mMaximum;
            mCorners[5].x = mMinimum.x; mCorners[5].y = mMaximum.y; mCorners[5].z = mMaximum.z;
            mCorners[6].x = mMinimum.x; mCorners[6].y = mMinimum.y; mCorners[6].z = mMaximum.z;
            mCorners[7].x = mMaximum.x; mCorners[7].y = mMinimum.y; mCorners[7].z = mMaximum.z;

            return mCorners;
        }

        DiVec3 GetCorner(CornerEnum cornerToGet) const
        {
            switch(cornerToGet)
            {
            case FAR_LEFT_BOTTOM:
                return mMinimum;
            case FAR_LEFT_TOP:
                return DiVec3(mMinimum.x, mMaximum.y, mMinimum.z);
            case FAR_RIGHT_TOP:
                return DiVec3(mMaximum.x, mMaximum.y, mMinimum.z);
            case FAR_RIGHT_BOTTOM:
                return DiVec3(mMaximum.x, mMinimum.y, mMinimum.z);
            case NEAR_RIGHT_BOTTOM:
                return DiVec3(mMaximum.x, mMinimum.y, mMaximum.z);
            case NEAR_LEFT_BOTTOM:
                return DiVec3(mMinimum.x, mMinimum.y, mMaximum.z);
            case NEAR_LEFT_TOP:
                return DiVec3(mMinimum.x, mMaximum.y, mMaximum.z);
            case NEAR_RIGHT_TOP:
                return mMaximum;
            default:
                return DiVec3();
            }
        }

        void Merge( const DiAABB& rhs )
        {
            if ((rhs.mExtent == EXTENT_NULL) || (mExtent == EXTENT_INFINITE))
            {
                return;
            }
            else if (rhs.mExtent == EXTENT_INFINITE)
            {
                mExtent = EXTENT_INFINITE;
            }
            else if (mExtent == EXTENT_NULL)
            {
                SetExtents(rhs.mMinimum, rhs.mMaximum);
            }
            else
            {
                DiVec3 min = mMinimum;
                DiVec3 max = mMaximum;

                max.makeCeil(rhs.mMaximum);
                min.makeFloor(rhs.mMinimum);

                SetExtents(min, max);
            }

        }

        inline void Merge( const DiVec3& point )
        {
            switch (mExtent)
            {
            case EXTENT_NULL: 
                SetExtents(point, point);
                return;

            case EXTENT_FINITE:
                mMaximum.makeCeil(point);
                mMinimum.makeFloor(point);
                return;

            case EXTENT_INFINITE:
                return;
            }
        }

        inline float IsResetSel(float ifReset, float ifNotReset) const
        {
            return mMaximum.x - mMinimum.x < 0 ? ifNotReset : ifReset;
        }

        inline float GetRadius() const
        {
            return mMaximum.x > mMinimum.x ? (mMaximum - mMinimum).length() * 0.5f : 0.0f;
        }

        inline float GetRadiusSqr() const
        {
            return mMaximum.x > mMinimum.x ? ((mMaximum - mMinimum)*0.5f).squaredLength() : 0.0f;
        }

        inline void SetNull()
        {
            mExtent = EXTENT_NULL;
        }

        inline bool IsNull(void) const
        {
            return (mExtent == EXTENT_NULL);
        }

        bool IsFinite(void) const
        {
            return (mExtent == EXTENT_FINITE);
        }

        inline void SetInfinite()
        {
            mExtent = EXTENT_INFINITE;
        }

        bool IsInfinite(void) const
        {
            return (mExtent == EXTENT_INFINITE);
        }

        inline bool Intersects(const DiAABB& b2) const
        {
            if (this->IsNull() || b2.IsNull())
                return false;

            if (this->IsInfinite() || b2.IsInfinite())
                return true;

            if (mMaximum.x < b2.mMinimum.x)
                return false;
            if (mMaximum.y < b2.mMinimum.y)
                return false;
            if (mMaximum.z < b2.mMinimum.z)
                return false;
            if (mMinimum.x > b2.mMaximum.x)
                return false;
            if (mMinimum.y > b2.mMaximum.y)
                return false;
            if (mMinimum.z > b2.mMaximum.z)
                return false;

            return true;

        }

        inline DiAABB Intersection(const DiAABB& b2) const
        {
            if (this->IsNull() || b2.IsNull())
            {
                return DiAABB();
            }
            else if (this->IsInfinite())
            {
                return b2;
            }
            else if (b2.IsInfinite())
            {
                return *this;
            }

            DiVec3 intMin = mMinimum;
            DiVec3 intMax = mMaximum;

            intMin.makeCeil(b2.GetMinimum());
            intMax.makeFloor(b2.GetMaximum());

            if (intMin.x < intMax.x &&
                intMin.y < intMax.y &&
                intMin.z < intMax.z)
            {
                return DiAABB(intMin, intMax);
            }

            return DiAABB();
        }

        float Volume(void) const
        {
            switch (mExtent)
            {
            case EXTENT_NULL:
                return 0.0f;

            case EXTENT_FINITE:
                {
                    DiVec3 diff = mMaximum - mMinimum;
                    return diff.x * diff.y * diff.z;
                }

            case EXTENT_INFINITE:
                return DiMath::POS_INFINITY;

            default:
                DI_ASSERT(false);
                return 0.0f;
            }
        }

        bool Intersects(const DiVec3& v) const
        {
            switch (mExtent)
            {
            case EXTENT_NULL:
                return false;

            case EXTENT_FINITE:
                return(v.x >= mMinimum.x  &&  v.x <= mMaximum.x  && 
                    v.y >= mMinimum.y  &&  v.y <= mMaximum.y  && 
                    v.z >= mMinimum.z  &&  v.z <= mMaximum.z);

            case EXTENT_INFINITE:
                return true;

            default:
                DI_ASSERT(false);
                return false;
            }
        }

        bool Intersects(const DiPlane& p) const
        {
            return DiMath::intersects(p, *this);
        }

        bool Intersects(const DiSphere& s) const
        {
            return DiMath::intersects(s, *this); 
        }

        DiVec3 GetCenter(void) const
        {
            DI_ASSERT( (mExtent == EXTENT_FINITE) );

            return DiVec3(
                (mMaximum.x + mMinimum.x) * 0.5f,
                (mMaximum.y + mMinimum.y) * 0.5f,
                (mMaximum.z + mMinimum.z) * 0.5f);
        }

        DiVec3 GetSize(void) const
        {
            switch (mExtent)
            {
            case EXTENT_NULL:
                return DiVec3::ZERO;

            case EXTENT_FINITE:
                return mMaximum - mMinimum;

            case EXTENT_INFINITE:
                return DiVec3(
                    DiMath::POS_INFINITY,
                    DiMath::POS_INFINITY,
                    DiMath::POS_INFINITY);

            default:
                DI_ASSERT(false);
                return DiVec3::ZERO;
            }
        }

        DiVec3 GetHalfSize(void) const
        {
            switch (mExtent)
            {
            case EXTENT_NULL:
                return DiVec3::ZERO;

            case EXTENT_FINITE:
                return (mMaximum - mMinimum) * 0.5;

            case EXTENT_INFINITE:
                return DiVec3(
                    DiMath::POS_INFINITY,
                    DiMath::POS_INFINITY,
                    DiMath::POS_INFINITY);

            default:
                DI_ASSERT(false);
                return DiVec3::ZERO;
            }
        }

        bool Contains(const DiVec3& v) const
        {
            if (IsNull())
                return false;
            if (IsInfinite())
                return true;

            return mMinimum.x <= v.x && v.x <= mMaximum.x &&
                mMinimum.y <= v.y && v.y <= mMaximum.y &&
                mMinimum.z <= v.z && v.z <= mMaximum.z;
        }

        bool Contains(const DiAABB& other) const
        {
            if (other.IsNull() || this->IsInfinite())
                return true;

            if (this->IsNull() || other.IsInfinite())
                return false;

            return this->mMinimum.x <= other.mMinimum.x &&
                this->mMinimum.y <= other.mMinimum.y &&
                this->mMinimum.z <= other.mMinimum.z &&
                other.mMaximum.x <= this->mMaximum.x &&
                other.mMaximum.y <= this->mMaximum.y &&
                other.mMaximum.z <= this->mMaximum.z;
        }

        inline void Transform( const DiMat4& matrix )
        {
            if( mExtent != EXTENT_FINITE )
                return;

            DiVec3 oldMin, oldMax, currentCorner;

            oldMin = mMinimum;
            oldMax = mMaximum;

            SetNull();

            currentCorner = oldMin;
            Merge( matrix * currentCorner );

            // min,min,max
            currentCorner.z = oldMax.z;
            Merge( matrix * currentCorner );

            // min max max
            currentCorner.y = oldMax.y;
            Merge( matrix * currentCorner );

            // min max min
            currentCorner.z = oldMin.z;
            Merge( matrix * currentCorner );

            // max max min
            currentCorner.x = oldMax.x;
            Merge( matrix * currentCorner );

            // max max max
            currentCorner.z = oldMax.z;
            Merge( matrix * currentCorner );

            // max min max
            currentCorner.y = oldMin.y;
            Merge( matrix * currentCorner );

            // max min min
            currentCorner.z = oldMin.z;
            Merge( matrix * currentCorner ); 
        }

        void TransformAffine(const DiMat4& m)
        {
            DI_ASSERT(m.isAffine());

            if ( mExtent != EXTENT_FINITE )
                return;

            DiVec3 centre = GetCenter();
            DiVec3 halfSize = GetHalfSize();

            DiVec3 newCentre = m.transformAffine(centre);
            DiVec3 newHalfSize(
                fabs(m[0][0]) * halfSize.x + fabs(m[0][1]) * halfSize.y + fabs(m[0][2]) * halfSize.z, 
                fabs(m[1][0]) * halfSize.x + fabs(m[1][1]) * halfSize.y + fabs(m[1][2]) * halfSize.z,
                fabs(m[2][0]) * halfSize.x + fabs(m[2][1]) * halfSize.y + fabs(m[2][2]) * halfSize.z);

            SetExtents(newCentre - newHalfSize, newCentre + newHalfSize);
        }

        bool operator== (const DiAABB& rhs) const
        {
            if (this->mExtent != rhs.mExtent)
                return false;

            if (!this->IsFinite())
                return true;

            return this->mMinimum == rhs.mMinimum &&
                this->mMaximum == rhs.mMaximum;
        }

        bool operator!= (const DiAABB& rhs) const
        {
            return !(*this == rhs);
        }

        static const DiAABB BOX_NULL;
        static const DiAABB BOX_INFINITE;
    };

}

#endif
