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

#ifndef DiTransPlane_h__
#define DiTransPlane_h__


#include "math/plane.h"
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

#endif
