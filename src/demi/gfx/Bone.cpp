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
#include "GfxPch.h"
#include "Bone.h"
#include "Skeleton.h"

namespace Demi
{
    DiBone::DiBone( DiSkeleton* creator, uint32 handle )
        :DiNode(),
        mCreator(creator),
        mHandle(handle)
    {
        mNodeType = NT_BONENODE;
    }

    DiBone::DiBone( DiSkeleton* creator, uint32 handle, const DiString& name )
        :DiNode(name),
        mCreator(creator),
        mHandle(handle)
    {
        mNodeType = NT_BONENODE;
    }

    DiBone::~DiBone()
    {

    }

    DiBone* DiBone::CreateChild( unsigned short handle )
    {
        DiBone* retBone = mCreator->CreateBone(handle);
        this->AddChild(retBone);
        return retBone;
    }

    DiNode* DiBone::CreateChild( const DiVec3& translate /*= DiVec3::ZERO*/, 
        const DiQuat& rotate /*= DiQuat::IDENTITY */ )
    {
        DiBone* bone = mCreator->CreateBone();
        bone->Translate(translate);
        bone->Rotate(rotate);
        return bone;
    }

    DiNode* DiBone::CreateChild( const DiString& name, 
        const DiVec3& translate /*= DiVec3::ZERO*/,
        const DiQuat& rotate /*= DiQuat::IDENTITY*/ )
    {
        DiBone* bone = mCreator->CreateBone(name);
        bone->Translate(translate);
        bone->Rotate(rotate);
        return bone;
    }

    void DiBone::SetBindingPose()
    {
        SetInitialState();

        mBindDerivedInversePosition    = -GetDerivedPosition();
        mBindDerivedInverseScale       = DiVec3::UNIT_SCALE / GetDerivedScale();
        mBindDerivedInverseOrientation = GetDerivedOrientation().Inverse();
    }

    void DiBone::Reset()
    {
        ResetToInitialState();
    }

    void DiBone::GetOffsetTransform( DiMat4& m ) const
    {
        DiVec3 locScale = GetDerivedScale() * mBindDerivedInverseScale;
        DiQuat locRotate = GetDerivedOrientation() * mBindDerivedInverseOrientation;

        DiVec3 locTranslate = GetDerivedPosition() +
            locRotate * (locScale * mBindDerivedInversePosition);
        m.makeTransform(locTranslate, locScale, locRotate);
    }

    void DiBone::NeedUpdate( bool forceParentUpdate /*= false*/ )
    {
        DiNode::NeedUpdate(forceParentUpdate);
    }
}