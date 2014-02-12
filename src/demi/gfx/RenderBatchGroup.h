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

#ifndef RenderBatchGroup_h__
#define RenderBatchGroup_h__

#include "GfxPrerequisites.h"
#include "Callback.h"

namespace Demi 
{
    enum DiBatchGroupType
    {
        BATCH_PRE_TERRAIN = 0,
        BATCH_TERRAIN,
        BATCH_POST_TERRAIN,
        BATCH_MODEL,
        BATCH_CHARACTER,
        BATCH_EFFECT,
        BATCH_HELPER,
        BATCH_POST_FILTER,
        BATCH_2D,
        BATCH_GUI,

        NUM_BATCH_TYPES
    };

    struct DI_GFX_API DiRenderUnitList
    {
        typedef DiVector<DiPair<DiRenderUnit*,DiMat4*>> List;

        List ruList;

        void clear(DiRenderPipeline* pipeline);

        void process(DiRenderBatchGroup* group);
    };

    class DI_GFX_API DiRenderBatchGroup : public DiBase
    {
    public:

        DiRenderBatchGroup(DiBatchGroupType type, DiRenderPipeline* pipeline);

        virtual                     ~DiRenderBatchGroup();

        friend struct DiRenderUnitList; 

    public:

        typedef DiVector<DiPair<DiRenderUnit*,DiMat4*>> RenderUnitList;

        typedef Functor0            ExtraProcess;

        virtual DiBatchGroupType    GetType() const    {return mType;}

        void                        Clear();

        virtual void                Process();

        virtual void                AddRenderUnit(DiRenderUnit* ru);

        virtual void                AddRenderUnit(DiRenderUnit* ru,DiMat4* matrix);

        virtual void                BindIdentityViewProj();

        bool                        GetUseIdentityProj() const { return mUseIdentityProj; }

        void                        SetUseIdentityProj(bool val) { mUseIdentityProj = val; }

        // return if this group is for UI usage
        bool                        IsInterfaceGroup();

        void                        EnableStatis(bool val) { mEnableStatis = val; }

        void                        ClearStatis();

        uint32                      GetBatchesNum() const { return mBatchesNum; }

        uint32                      GetPrimitivesNum() const { return mPrimitivesNum; }

        DiRenderPipeline*           GetPipeline() const { return mPipeline; }

        bool                        GetShadowEnable() const {return mEnableShadow;}

        void                        SetShadowEnable(bool enableShadow) { mEnableShadow = enableShadow; }

        void                        SetExtraProcess(ExtraProcess val) { mExtraProcess = val; }

    protected:

        // sort by the material or camera distance
        virtual void                Sort();

    protected:

        DiBatchGroupType            mType;

        DiRenderPipeline*           mPipeline;
        
        DiRenderUnitList            mRenders;

        // transparent objects
        // it should be sortted before by distance adding to the pipeline
        DiRenderUnitList            mTransRenders;

        // for UI
        bool                        mUseIdentityProj;

        uint32                      mBatchesNum;
        
        uint32                      mPrimitivesNum;
        
        bool                        mEnableStatis;

        bool                        mEnableShadow;

        ExtraProcess                mExtraProcess;
    };

}
#endif // RenderBatchGroup_h__