
#include "GfxPch.h"
#include "TransPlane.h"
#include "Node.h"

namespace Demi
{
    DiTransPlane::DiTransPlane( const DiString& name )
        :DiPlane(),DiTransformUnit(name)
        , mLastTranslate(DiVec3::ZERO)
        , mLastRotate(DiQuat::IDENTITY)
        , mDirty(true)
    {

    }

    DiTransPlane::DiTransPlane( const DiPlane& rhs )
        :DiPlane(rhs)
        , mLastTranslate(DiVec3::ZERO)
        , mLastRotate(DiQuat::IDENTITY)
        , mDirty(true)
    {

    }

    DiTransPlane::DiTransPlane( const DiVec3& rkNormal, float fConstant )
        : DiPlane (rkNormal, fConstant)
        , mLastTranslate(DiVec3::ZERO)
        , mLastRotate(DiQuat::IDENTITY)
        , mDirty(true)
    {

    }

    DiTransPlane::DiTransPlane( const DiVec3& rkNormal, const DiVec3& rkPoint )
        : DiPlane(rkNormal, rkPoint)
        , mLastTranslate(DiVec3::ZERO)
        , mLastRotate(DiQuat::IDENTITY)
        , mDirty(true)
    {

    }

    DiTransPlane::DiTransPlane( const DiVec3& rkPoint0, const DiVec3& rkPoint1,const DiVec3& rkPoint2 )
        : DiPlane(rkPoint0, rkPoint1, rkPoint2)
        , mLastTranslate(DiVec3::ZERO)
        , mLastRotate(DiQuat::IDENTITY)
        , mDirty(true)
    {

    }

    DiString& DiTransPlane::GetType()
    {
        static DiString type = "TransPlane";
        return type;
    }

    const DiPlane& DiTransPlane::GetDerivedPlane( void ) const
    {
        if (mParentNode)
        {
            if (mDirty ||
                !(mParentNode->GetDerivedOrientation() == mLastRotate &&
                mParentNode->GetDerivedPosition() == mLastTranslate))
            {
                mLastRotate = mParentNode->GetDerivedOrientation();
                mLastTranslate = mParentNode->GetDerivedPosition();
                mDerivedPlane.normal = mLastRotate * normal;
                mDerivedPlane.d = d;
                mDerivedPlane.d -= mDerivedPlane.normal.dotProduct(mLastTranslate);

                mDirty = false;

            }
        }
        else
        {
            return *this;
        }

        return mDerivedPlane;
    }
}