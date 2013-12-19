
/********************************************************************
    File:       Bone.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "Node.h"

namespace Demi
{
    class DI_GFX_API DiBone : public DiNode
    {
    public:

        DiBone(DiSkeleton* creator, uint32 handle);

        DiBone(DiSkeleton* creator, uint32 handle, const DiString& name);

        ~DiBone();

    public:

        DiBone*             CreateChild(unsigned short handle);

        DiNode*             CreateChild(
                             const DiVec3& translate = DiVec3::ZERO, 
                             const DiQuat& rotate = DiQuat::IDENTITY );

        DiNode*             CreateChild(const DiString& name, const DiVec3& translate = DiVec3::ZERO,
                             const DiQuat& rotate = DiQuat::IDENTITY);

        uint32              GetHandle() const {return mHandle;}

        void                SetBindingPose();

        void                Reset();

        void                GetOffsetTransform(DiMat4& m) const;

        void                NeedUpdate(bool forceParentUpdate = false);

        const DiVec3&       GetBindingPoseInverseScale(void) const { return mBindDerivedInverseScale; }
        
        const DiVec3&       GetBindingPoseInversePosition(void) const { return mBindDerivedInversePosition; }

        const DiQuat&       GetBindingPoseInverseOrientation(void) const { return mBindDerivedInverseOrientation; }

    protected:

        uint32              mHandle;

        DiSkeleton*         mCreator;

        DiVec3              mBindDerivedInverseScale;

        DiQuat              mBindDerivedInverseOrientation;

        DiVec3              mBindDerivedInversePosition;
    };
}