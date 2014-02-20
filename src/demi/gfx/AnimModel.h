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

#ifndef DiAnimModel_h__
#define DiAnimModel_h__


#include "Model.h"
#include "Motion.h"

namespace Demi
{
    typedef shared_ptr<DiAnimModel> DiAnimModelPtr;

    class DI_GFX_API DiAnimModel : public DiModel
    {
    public:

        DiAnimModel(const DiString& name,
            const DiString& modelName,const DiString& motionName);

        DiAnimModel(const DiString& name,DiMeshPtr model,DiMotionPtr motion);

        DiAnimModel(const DiString& name,const DiString& modelName,
            const DiString& motionName,DiSkeletonInstance * pkSkeleton,
            DiAttachSetInstance * pkAttachSet,DiMat4 * pkBoneMatrics,
            DiClipControllerSet* pkClipSet);

        DiAnimModel(const DiString& name,DiMeshPtr model,DiMotionPtr motion,
            DiSkeletonInstance * pkSkeleton,DiAttachSetInstance * pkAttachSet,
            DiMat4 * pkBoneMatrics,DiClipControllerSet* pkClipSet);

        virtual                 ~DiAnimModel();

    public:

        const DiMat4*           GetBoneMatrices(void) const { return mBoneMatrices;}

        unsigned int            GetNumBoneMatrices(void) const { return mNumBoneMatrices; }

        bool                    HasSkeleton(void) const { return mSkeleton != 0; }

        DiSkeletonInstance*     GetSkeleton(void) const { return mSkeleton; }

        DiClipControllerSet*    GetClipSet()const{return mClipSet;}

        bool                    CacheBoneMatrices();

        void                    Update(DiCamera* camera);

        void                    UpdateAnimation();

        void                    UpdateAnimation(float fTimePos);

        bool                    IsAutoUpdateAnims() const { return mAutoUpdateAnims; }

        void                    SetAutoUpdateAnims(bool val) { mAutoUpdateAnims = val; }

        inline void             SetAniPlaySpeed(float fSpeed) {mSpeed = fSpeed; ++ mLastUpdateAnimFrame;}
    
        inline float            GetAniPlaySpeed() {return mSpeed;}

        virtual DiString&       GetType();

        void                    AddPostController(DiPostController* postctr);

    protected:

        void                    Init();

        void                    InitClipSet();

        void                    UpdatePostControllers();

        void                    ConcatenateAffineMatrices(
                                    const DiMat4& baseMatrix,
                                    const DiMat4* srcMatrices,
                                    DiMat4* dstMatrices,
                                    size_t numMatrices);

    protected:

        DiMotionPtr             mMotion;

        DiSkeletonInstance*     mSkeleton;

        bool                    mRefSkeleton;
        
        DiAttachSetInstance*    mAttachSet;
        
        bool                    mRefAttachSet;

        DiClipControllerSet*    mClipSet;
        
        bool                    mRefClipSet;

        DiMat4*                 mBoneMatrices;
        
        bool                    mRefBoneMatrics;

        DiMat4                  mLastWorldTrans;

        uint32                  mNumBoneMatrices;

        uint64                  mLastUpdateBonesFrame;

        uint64                  mLastUpdateAnimFrame;
        
        bool                    mAutoUpdateAnims;
        
        bool                    mHardwareSkining;

        float                   mSpeed;

        typedef DiVector<DiPostController*> PostControllers;

        PostControllers         mPostControllers;
    };
}


#endif
