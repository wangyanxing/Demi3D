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

#ifndef DiKeyFrame_h__
#define DiKeyFrame_h__


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

#endif
