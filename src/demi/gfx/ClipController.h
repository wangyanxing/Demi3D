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

#ifndef ClipController_h__
#define ClipController_h__

#include "Callback.h"

namespace Demi
{
    typedef DiMap<DiString, DiClipController*>      ClipControllerMap;
    typedef DiMapIterator<ClipControllerMap>        ClipControllerIt;
    typedef DiConstMapIterator<ClipControllerMap>   ConstClipControllerIt;
    typedef DiList<DiClipController*>               EnabledClipsList;
    typedef DiConstVectorIterator<EnabledClipsList> ConstEnabledClipsIt;

    struct DiAniEvent
    {
        typedef Functor1<DiClipController*>    AniCallback;

        AniCallback                 callBackFunc;
        float                       triggerTime;        
        bool                        isTriggerInLoop;    
    };

    typedef DiHashMap<void*, DiAniEvent >           PlayEndCallbackMap;

    class DI_GFX_API DiClipController
    {
    public:
        friend class DiClipControllerSet;

        typedef DiVector<float>                     BoneBlendMask;
        typedef DiStrHash<float>                    AttachBlendMask;

    protected:
        
        DiClipController(DiClipControllerSet* parent, const DiString& name,
            float timePos, float length, float weight = 1.0, bool enabled = false);

        DiClipController(DiClipControllerSet* parent,
            const DiClipController &rhs);

        virtual                 ~DiClipController();

    public:

        inline    void          SetAnimationName(const DiString& strName) { mAnimationName = strName; }

        const DiString&         GetAnimationName() const { return mAnimationName; }

        void                    SetEnabled(bool enabled);

        bool                    GetEnabled()const {return mEnabled;}

        inline    bool          IsEnd() {return (mTimePos == mLength && !mLoop);}

        float                   GetTimePosition(void) const{return mTimePos;}

        void                    SetTimePosition(float timePos);

        inline    void          SetTimeRatio(float fTimeRatio)    {mTimeRatio = fTimeRatio;}

        inline    float         GetTimeRatio()                    {return mTimeRatio;}

        void                    AddTime(float delta);

        float                   GetLength() const{return mLength;}

        void                    SetLength(float len);

        float                   GetWeight(void) const{return mWeight;}

        void                    SetWeight(float weight);

        void                    SetLoop(bool loop) { mLoop = loop; }

        bool                    GetLoop(void) const { return mLoop; }

        DiClipControllerSet*    GetParent(void) const { return mParent; }

        void                    RegisterAniEventCallback(void * pkCallee , DiAniEvent rAniEvent);

        void                    DegisterAniEventCallback(void * pkCallee);

        /// Bone Blend Mask 
        void                    CreateBoneBlendMask(size_t blendMaskSizeHint, float initialWeight = 1.0f);

        void                    DestroyBoneBlendMask();

        void                    SetBoneBlendMaskData(const float* blendMaskData);

        void                    SetBoneBlendMask(const BoneBlendMask* blendMask);

        const BoneBlendMask*    GetBoneBlendMask() const {return mBoneBlendMask;}

        bool                    HasBoneBlendMask() const {return mBoneBlendMask != NULL;}

        void                    SetBoneBlendMaskEntry(size_t boneHandle, float weight);

        inline float            GetBoneBlendMaskEntry(size_t boneHandle) const
        {
            DI_ASSERT(mBoneBlendMask && mBoneBlendMask->size() > boneHandle);
            return (*mBoneBlendMask)[boneHandle];
        }

        /// Attach Node Blend Mask
        void                    ClearAttachNodeBlendMask();

        void                    SetAttachNodeBlendMaskData( const AttachBlendMask& mapBlendMask );

        const AttachBlendMask&  GetAttachNodeBlendMask() const {return mAttachNodeBlendMask;}

        bool                    HasAttachNodeBlendMask() const {return !mAttachNodeBlendMask.empty();}

        void                    SetAttachNodeBlendMaskEntry(DiString strAttachName, float weight);

        float                   GetAttachNodeBlendMaskEntry(DiString strAttachName) const;

    protected:

        BoneBlendMask*          mBoneBlendMask;

        AttachBlendMask         mAttachNodeBlendMask;

        DiString                mAnimationName;

        DiClipControllerSet*    mParent;

        float                   mTimePos;

        float                   mTimeRatio;

        float                   mLength;

        float                   mWeight;

        bool                    mEnabled;

        bool                    mLoop;

        PlayEndCallbackMap      mMapEndCallbacks;
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_GFX_API DiClipControllerSet
    {
    public:

        DiClipControllerSet();

        DiClipControllerSet(const DiClipControllerSet& rhs);

        virtual              ~DiClipControllerSet();

    public:

        DiClipController*     CreateClipController(const DiString& animName,  
                                    float timePos, float length, float weight = 1.0, bool enabled = false);
        
        DiClipController*     GetClipController(const DiString& name) const;
                              
        uint32                  GetClipNums() const {return mClipControllers.size();}
                              
        bool                  HasClipController(const DiString& name) const;
                              
        bool                  RenameClipController(const DiString& name,const DiString& strNewName);
                              
        void                  RemoveClipController(const DiString& name);
                              
        void                  RemoveClipControllers(void);
                              
        void                  DisableAllClips();
                              
        void                  NotifyDirty(void);
                              
        void                  NotifyClipControllerEnabled(DiClipController* clip,bool enabled);
                              
        bool                  HasEnabledClipController(void) const { return !mEnabledClips.empty(); }
                              
        uint64                GetDirtyFrameNumber(void) const { return mDirtyFrameNumber; }

        ConstClipControllerIt GetClipControllerIterator(void) const;

        ClipControllerIt      GetClipControllerIterator(void);

        ConstEnabledClipsIt   GetEnabledClipsIterator(void) const;

    protected:

        uint64                mDirtyFrameNumber;

        ClipControllerMap     mClipControllers;

        EnabledClipsList      mEnabledClips;
    };
}

#endif // ClipController_h__