
#include "GfxPch.h"
#include "KeyFrame.h"
#include "AnimationClip.h"

namespace Demi
{
    DiKeyFrame::DiKeyFrame( const DiAnimationClip* parent, float time )
        :mTime(time),mClip(parent)
    {

    }

    DiKeyFrame* DiKeyFrame::Clone( DiAnimationClip* newParent ) const
    {
        return DI_NEW DiKeyFrame(newParent, mTime);
    }

    DiTransformKeyFrame::DiTransformKeyFrame( const DiAnimationClip* parent, float time )
        :DiKeyFrame(parent,time),
        mTranslate(DiVec3::ZERO),
        mScale(DiVec3::UNIT_SCALE),
        mRotate(DiQuat::IDENTITY)
    {

    }

    void DiTransformKeyFrame::SetTranslate( const DiVec3& trans )
    {
        mTranslate = trans;
        if (mClip)
        {
            mClip->KeyFrameDataChanged();
        }
    }

    const DiVec3& DiTransformKeyFrame::GetTranslate( void ) const
    {
        return mTranslate;
    }

    void DiTransformKeyFrame::SetScale( const DiVec3& scale )
    {
        mScale = scale;
        if (mClip)
        {
            mClip->KeyFrameDataChanged();
        }
    }

    const DiVec3& DiTransformKeyFrame::GetScale( void ) const
    {
        return mScale;
    }

    void DiTransformKeyFrame::SetRotation( const DiQuat& rot )
    {
        mRotate = rot;
        if (mClip)
        {
            mClip->KeyFrameDataChanged();
        }
    }

    const DiQuat& DiTransformKeyFrame::GetRotation( void ) const
    {
        return mRotate;
    }

    DiKeyFrame* DiTransformKeyFrame::Clone( DiAnimationClip* newParent ) const
    {
        DiTransformKeyFrame* newKf = DI_NEW DiTransformKeyFrame(newParent, mTime);
        newKf->mTranslate    = mTranslate;
        newKf->mScale        = mScale;
        newKf->mRotate        = mRotate;
        return newKf;
    }
}