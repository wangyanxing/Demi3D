
/********************************************************************
    File:       TransPlane.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "plane.h"
#include "TransformUnit.h"

namespace Demi
{
    class DI_GFX_API DiTransPlane : public DiPlane, public DiTransformUnit
    {
    public:

        DiTransPlane(const DiString& name);

        DiTransPlane (const DiPlane& rhs);
        
        DiTransPlane (const DiVec3& rkNormal, float fConstant);
        
        DiTransPlane (const DiVec3& rkNormal, const DiVec3& rkPoint);
        
        DiTransPlane (const DiVec3& rkPoint0, const DiVec3& rkPoint1,const DiVec3& rkPoint2);

        virtual                 ~DiTransPlane() {}

        virtual DiString&       GetType();

        virtual const DiAABB&   GetBoundingBox(void) const{ return mNullBB; }
        
        const DiPlane&          GetDerivedPlane(void) const;

    protected:
        mutable DiPlane         mDerivedPlane;

        mutable DiVec3          mLastTranslate;

        mutable DiQuat          mLastRotate;

        DiAABB                  mNullBB;

        mutable bool            mDirty;
    };
}