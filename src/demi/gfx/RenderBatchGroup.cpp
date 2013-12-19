
#include "RenderBatchGroup.h"
#include "RenderPipeline.h"
#include "RenderUnit.h"

namespace Demi
{
    void DiRenderUnitList::clear(DiRenderPipeline* pipeline)
    {
        for (size_t i=0; i<ruList.size(); i++)
        {
            if (ruList[i].first->GetWorldTransformNums() == 1)
            {
                pipeline->GetMatrixPool().ReleaseObject(ruList[i].second);
            }
            else
            {
                DiRenderPipeline::DiMat4Array* arr = (DiRenderPipeline::DiMat4Array*)(ruList[i].second);
                pipeline->GetMatricsArrayPool().ReleaseObject(arr);
            }
        }
        ruList.clear();
    }

    void DiRenderUnitList::process(DiRenderBatchGroup* group)
    {
        for (auto it = ruList.begin(); it != ruList.end(); ++it)
        {
            if(group->mPipeline->RenderSingleBatch(it->first,it->second) && group->mEnableStatis)
            {
                if (!((*it).first->GetFlag() & DiRenderUnit::RU_FLAG_NO_BACHES_STATIS))
                    group->mBatchesNum++;
                if (!((*it).first->GetFlag() & DiRenderUnit::RU_FLAG_NO_PRIMITIVE_STATIS))
                    group->mPrimitivesNum += (*it).first->mPrimitiveCount;
            }
        }
    }

    DiRenderBatchGroup::DiRenderBatchGroup( DiBatchGroupType type,DiRenderPipeline* pipeline )
        :mType(type),mPipeline(pipeline),mUseIdentityProj(false),
        mBatchesNum(0),mPrimitivesNum(0),mEnableStatis(false),mEnableShadow(false)
    {

    }

    DiRenderBatchGroup::~DiRenderBatchGroup()
    {

    }

    void DiRenderBatchGroup::Process()
    {
        if (mRenders.ruList.empty() && mRenders.ruList.empty() 
            && !mExtraProcess)
            return;

        if (mUseIdentityProj)
            BindIdentityViewProj();

        mRenders.process(this);
        mTransRenders.process(this);
        
        if (mExtraProcess)
            mExtraProcess();
    }

    void DiRenderBatchGroup::Sort()
    {
    }

    void DiRenderBatchGroup::AddRenderUnit( DiRenderUnit* ru )
    {
        DI_ASSERT(ru->GetWorldTransformNums() < MAX_WORLD_MATS);

        DiMat4* mat = NULL;
        if (ru->GetWorldTransformNums() == 1)
        {
            mat = mPipeline->GetMatrixPool().GetFreeObject();
            ru->GetWorldTransform(mat);
        }
        else
        {
            mat = mPipeline->GetMatricsArrayPool().GetFreeObject()->matrixArray;
            ru->GetWorldTransform(mat);
        }

        if (ru->GetMaterial())
        {
            if(ru->GetMaterial()->IsTransparent())
                mTransRenders.ruList.push_back(DiPair<DiRenderUnit*,DiMat4*>(ru,mat));
            else
                mRenders.ruList.push_back(DiPair<DiRenderUnit*,DiMat4*>(ru,mat));
        }
    }

    void DiRenderBatchGroup::AddRenderUnit( DiRenderUnit* ru,DiMat4* matrix )
    {
        if (ru->GetMaterial())
        {
            if(ru->GetMaterial()->IsTransparent())
                mTransRenders.ruList.push_back(DiPair<DiRenderUnit*,DiMat4*>(ru,matrix));
            else
                mRenders.ruList.push_back(DiPair<DiRenderUnit*,DiMat4*>(ru,matrix));
        }
    }

    void DiRenderBatchGroup::Clear()
    {
        mTransRenders.clear(mPipeline);
        mRenders.clear(mPipeline);
    }

    void DiRenderBatchGroup::BindIdentityViewProj()
    {
        DiShaderEnvironment* se = mPipeline->GetShaderEnvironment();
        se->SetIdentity();
    }

    bool DiRenderBatchGroup::IsInterfaceGroup()
    {
        return mType >= BATCH_2D;
    }

    void DiRenderBatchGroup::ClearStatis()
    {
        mPrimitivesNum = 0;
        mBatchesNum = 0;
    }
}