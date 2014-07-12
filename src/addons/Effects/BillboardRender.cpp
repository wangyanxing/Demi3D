
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

/****************************************************
  This module was originally from Particle Universe
  Repo: https://github.com/scrawl/particleuniverse
  License: MIT 
****************************************************/

#include "FxPch.h"
#include "BillboardRender.h"
#include "Billboard.h"
#include "VisualParticle.h"
#include "ParticlePool.h"
#include "ParticleElement.h"
#include "ParticleSystem.h"
#include "RenderBatchGroup.h"
#include "CullNode.h"

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   pragma warning(disable:4706)
#endif

namespace Demi
{
    const BillboardType          DiBillboardRenderer::DEFAULT_BILLBOARD_TYPE = BBT_POINT;
    const bool                   DiBillboardRenderer::DEFAULT_ACCURATE_FACING = false;
    const BillboardOrigin        DiBillboardRenderer::DEFAULT_ORIGIN = BBO_CENTER;
    const BillboardRotationType  DiBillboardRenderer::DEFAULT_ROTATION_TYPE = BBR_TEXCOORD;
    const DiVec3                 DiBillboardRenderer::DEFAULT_COMMON_DIRECTION = DiVec3::UNIT_Z;
    const DiVec3                 DiBillboardRenderer::DEFAULT_COMMON_UP_VECTOR = DiVec3::UNIT_Y;
    const bool                   DiBillboardRenderer::DEFAULT_POINT_RENDERING = false;
    
    DiBillboardRenderer::DiBillboardRenderer(void) : 
        DiParticleRenderer(),
        mBillboardType(DEFAULT_BILLBOARD_TYPE)
    {
        mBillboardSet = make_shared<DiBillboardSet>("", 0, true);
        mBillboardSet->SetBillboardsInWorldSpace(true);
        
        autoRotate = false;
    }
    
    DiBillboardRenderer::~DiBillboardRenderer(void)
    {
        DI_DEBUG("billboard renderer releasing");
    }
    
    void DiBillboardRenderer::Prepare(DiParticleElement* element)
    {
        if (!element || mRendererInitialised)
            return;

        NotifyParticleQuota(element->GetVisualParticleQuota());

        if (element->GetParentSystem()->GetParentNode())
            NotifyAttached(element->GetParentSystem()->GetParentNode());
        
        NotifyDefaultDimensions(mRendererScale.x * element->GetDefaultWidth(),
            mRendererScale.y * element->GetDefaultHeight(),
            mRendererScale.z * element->GetDefaultDepth());
        SetMaterialName(element->GetMaterialName());
        mBillboardSet->SetBatchGroup((DiBatchGroupType)mQueueId);

        if (mTextureCoordsSet)
            mBillboardSet->SetTextureCoords(*mUVList.begin(), static_cast<uint16>(mUVList.size()));
        else if (mTextureCoordsRowsAndColumnsSet)
            mBillboardSet->SetTextureStacksAndSlices(mTextureCoordsRows, mTextureCoordsColumns);

        mRendererInitialised = true;
    }
    
    void DiBillboardRenderer::Unprepare(DiParticleElement*)
    {
    }
    
    void DiBillboardRenderer::SetBillboardType(BillboardType bbt)
    {
        mBillboardType = bbt;
        mBillboardSet->SetBillboardType(bbt);
    }
    
    BillboardType DiBillboardRenderer::GetBillboardType(void) const
    {
        return mBillboardType;
    }
    
    void DiBillboardRenderer::SetBillboardRotationType(BillboardRotationType rotationType)
    {
        mBillboardSet->SetBillboardRotationType(rotationType);
    }
    
    BillboardRotationType DiBillboardRenderer::GetBillboardRotationType(void) const
    {
        return mBillboardSet->GetBillboardRotationType();
    }
    
    void DiBillboardRenderer::SetCommonDirection(const DiVec3& vec)
    {
        mBillboardSet->SetCommonDirection(vec);
    }
    
    const DiVec3& DiBillboardRenderer::GetCommonDirection(void) const
    {
        return mBillboardSet->GetCommonDirection();
    }
    
    void DiBillboardRenderer::SetCommonUpVector(const DiVec3& vec)
    {
        mBillboardSet->SetCommonUpVector(vec);
    }
    
    const DiVec3& DiBillboardRenderer::GetCommonUpVector(void) const
    {
        return mBillboardSet->GetCommonUpVector();
    }
    
    void DiBillboardRenderer::Update(DiCamera* cam, DiParticlePool* pool)
    {
        DiParticleRenderer::Update(cam, pool);
        
        if (!mVisible)
            return;
        
        if (pool->IsEmpty(DiParticle::PT_VISUAL))
        {
            mBillboardSet->MakeEmpty();
            return;
        }
        
        size_t sz = pool->GetSize(DiParticle::PT_VISUAL);
        
        mBillboardSet->BeginBillboards(sz);
        DiBillboard bb;
        
        DiVisualParticle* particle = static_cast<DiVisualParticle*>(pool->GetFirst(DiParticle::PT_VISUAL));
        while (!pool->End(DiParticle::PT_VISUAL))
        {
            if (particle)
            {
                bb.mPosition = particle->position;
                
                if (mBillboardType == BBT_ORIENTED_SELF || mBillboardType == BBT_PERPENDICULAR_SELF)
                {
                    bb.mDirection = particle->direction;
                    bb.mDirection.normalise();
                }
                else if (mBillboardType == BBT_ORIENTED_SHAPE)
                {
                    bb.mDirection = DiVec3(particle->orientation.x, particle->orientation.y, particle->orientation.z);
                    bb.mDirection.normalise();
                }
                
                bb.mColour   = particle->colour;
                bb.mRotation = particle->zRotation;
                
                // this 'assignment' operator usage is not a bug
                if ((bb.mOwnDimensions = particle->ownDimensions))
                {
                    bb.mOwnDimensions = true;
                    bb.mWidth = particle->width;
                    bb.mHeight = particle->height;
                }
                
                bb.SetTexcoordIndex(particle->textureCoordsCurrent);
                mBillboardSet->InjectBillboard(bb);
            }
            particle = static_cast<DiVisualParticle*>(pool->GetNext(DiParticle::PT_VISUAL));
        }
        
        mBillboardSet->EndBillboards();
        mBillboardSet->Update(cam);
    }
    
    void DiBillboardRenderer::AddToBatchGroup(DiRenderBatchGroup* group)
    {
        DiParticleRenderer::AddToBatchGroup(group);
     
        mBillboardSet->AddToBatchGroup(group);
    }
    
    void DiBillboardRenderer::NotifyAttached(DiNode* parent)
    {
        //mBillboardSet->NotifyAttached(parent);
    }
    
    void DiBillboardRenderer::SetMaterialName(const DiString& materialName)
    {
        mBillboardSet->SetMaterial(materialName);
    }
    
    void DiBillboardRenderer::NotifyCurrentCamera(DiCamera* cam)
    {
        mBillboardSet->NotifyCurrentCamera(cam);
    }
    
    void DiBillboardRenderer::NotifyParticleQuota(size_t quota)
    {
        mBillboardSet->SetPoolSize(static_cast<unsigned int>(quota));
    }
    
    void DiBillboardRenderer::NotifyDefaultDimensions(float width, float height, float depth)
    {
        mBillboardSet->SetDefaultDimensions(width, height);
    }
    
    void DiBillboardRenderer::NotifyParticleResized(void)
    {
        mBillboardSet->NotifyBillboardResized();
    }
    
    void DiBillboardRenderer::NotifyParticleZRotated(void)
    {
        mBillboardSet->NotifyBillboardRotated();
    }
    
    void DiBillboardRenderer::SetBatchGroupID(uint8 queueId)
    {
        mQueueId = queueId;
        mBillboardSet->SetBatchGroup(DiBatchGroupType(mQueueId));
    }

    DiSortMode DiBillboardRenderer::GetSortMode(void) const
    {
        return mBillboardSet->GetSortMode();
    }
    
    void DiBillboardRenderer::SetVisible(bool visible)
    {
        DiParticleRenderer::SetVisible(visible);
        mBillboardSet->SetVisible(visible);
    }
    
    void DiBillboardRenderer::CopyTo (DiParticleRenderer* renderer)
    {
        DiParticleRenderer::CopyTo(renderer);

        DiBillboardRenderer* billboardRenderer = static_cast<DiBillboardRenderer*>(renderer);

        if (!billboardRenderer->GetBillboardSet())
            return;

        billboardRenderer->SetBillboardType(GetBillboardType());
        billboardRenderer->SetBillboardOrigin(GetBillboardOrigin());
        billboardRenderer->SetBillboardRotationType(GetBillboardRotationType());
        billboardRenderer->SetCommonDirection(GetCommonDirection());
        billboardRenderer->SetCommonUpVector(GetCommonUpVector());
    }

    void DiBillboardRenderer::SetBillboardOrigin( BillboardOrigin origin )
    {
        mBillboardSet->SetBillboardOrigin(origin);
    }

    Demi::BillboardOrigin DiBillboardRenderer::GetBillboardOrigin( void ) const
    {
        return mBillboardSet->GetBillboardOrigin();
    }
}