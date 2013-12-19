
/********************************************************************
    File:       KeyFrame.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    class DI_GFX_API DiKeyFrame : public DiBase
    {
    public:

        DiKeyFrame(const DiAnimationClip* parent, float time);

        virtual                  ~DiKeyFrame() {}

        float                    GetTime(void) const { return mTime; }

        virtual DiKeyFrame*      Clone(DiAnimationClip* newParent) const;


    protected:

        float                    mTime;

        const DiAnimationClip*   mClip;
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_GFX_API DiTransformKeyFrame : public DiKeyFrame
    {
    public:
        DiTransformKeyFrame(const DiAnimationClip* parent, float time);

        virtual             ~DiTransformKeyFrame() {}

        void                SetTranslate(const DiVec3& trans);

        const DiVec3&       GetTranslate(void) const;

        void                SetScale(const DiVec3& scale);

        const DiVec3&       GetScale(void) const;

        void                SetRotation(const DiQuat& rot);

        const DiQuat&       GetRotation(void) const;

        DiKeyFrame*         Clone(DiAnimationClip* newParent) const;

    protected:
        
        DiVec3              mTranslate;

        DiVec3              mScale;

        DiQuat              mRotate;
    };
}