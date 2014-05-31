
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
#include "ParticleRenderer.h"
#include "RenderBatchGroup.h"

namespace Demi
{
    const uint8 DiParticleRenderer::DEFAULT_RENDER_QUEUE_GROUP = BATCH_EFFECT;
    const bool DiParticleRenderer::DEFAULT_SORTED = false;
    const uint8 DiParticleRenderer::DEFAULT_TEXTURECOORDS_ROWS = 1;
    const uint8 DiParticleRenderer::DEFAULT_TEXTURECOORDS_COLUMNS = 1;
    const bool DiParticleRenderer::DEFAULT_USE_SOFT_PARTICLES = false;
    const float DiParticleRenderer::DEFAULT_SOFT_PARTICLES_CONTRAST_POWER = 0.8f;
    const float DiParticleRenderer::DEFAULT_SOFT_PARTICLES_SCALE = 1.0f;
    const float DiParticleRenderer::DEFAULT_SOFT_PARTICLES_DELTA = -1.0f;
    const DiString SOFT_PREFIX = "NewSoft_";

    
    DiParticleRenderer::DiParticleRenderer(void) : 
        mParentTechnique(0),
        mCullIndividual(false),
        mHeight(100),
        mWidth(100),
        mDepth(100),
        mSorted(DEFAULT_SORTED),
        mQueueId(DEFAULT_RENDER_QUEUE_GROUP),
        mRendererInitialised(false),
        mRendererScale(DiVec3::UNIT_SCALE),
        mTextureCoordsRows(DEFAULT_TEXTURECOORDS_ROWS),
        mTextureCoordsColumns(DEFAULT_TEXTURECOORDS_COLUMNS),
        mTextureCoordsSet(false),
        mUseSoftParticles(DEFAULT_USE_SOFT_PARTICLES),
        mSoftParticlesContrastPower(DEFAULT_SOFT_PARTICLES_CONTRAST_POWER),
        mSoftParticlesScale(DEFAULT_SOFT_PARTICLES_SCALE),
        mSoftParticlesDelta(DEFAULT_SOFT_PARTICLES_DELTA),
        mNotifiedDepthMap(false),
        mTextureCoordsRowsAndColumnsSet(false),
        mVisible(true)
    {
    }
    
    DiParticleRenderer::~DiParticleRenderer(void)
    {
        for (auto it = mUVList.begin(); it != mUVList.end(); ++it)
        {
            DI_DELETE(*it);
        }

        if (mUseSoftParticles)
        {
            //ParticleSystemManager::getSingleton().unregisterSoftParticlesRenderer(this);
        }
    }
    
    void DiParticleRenderer::NotifyStart(void)
    {
        SetVisible(true);
    }
    
    void DiParticleRenderer::NotifyStop(void)
    {
        SetVisible(false);
    }
    
    const DiString& DiParticleRenderer::GetRendererType(void) const
    {
        return mRendererType;
    }
    
    void DiParticleRenderer::SetRendererType(DiString rendererType)
    {
        mRendererType = rendererType;
    }
    
    DiParticleElement* DiParticleRenderer::GetParentElement(void) const
    {
        return mParentTechnique;
    }
    
    void DiParticleRenderer::SetParentElement(DiParticleElement* parentTechnique)
    {
        mParentTechnique = parentTechnique;
    }
    
    bool DiParticleRenderer::IsRendererInitialised(void) const
    {
        return mRendererInitialised;
    }
    
    void DiParticleRenderer::SetRendererInitialised(bool rendererInitialised)
    {
        mRendererInitialised = rendererInitialised;
    }
    
    uint8 DiParticleRenderer::GetBatchGroupID(void) const
    {
        return mQueueId;
    }
    
    void DiParticleRenderer::SetBatchGroupID(uint8 queueId)
    {
        mQueueId = queueId;
    }
    
    const bool DiParticleRenderer::IsSorted(void) const
    {
        return mSorted;
    }
    
    void DiParticleRenderer::SetSorted(bool sorted)
    {
        mSorted = sorted;
    }
    
    const uint8 DiParticleRenderer::GetTextureCoordsRows(void) const
    {
        return mTextureCoordsRows;
    }
    
    void DiParticleRenderer::SetTextureCoordsRows(uint8 const textureCoordsRows)
    {
        mTextureCoordsRows = textureCoordsRows;
        mTextureCoordsRowsAndColumnsSet = true;
    }
    
    const uint8 DiParticleRenderer::GetTextureCoordsColumns(void) const
    {
        return mTextureCoordsColumns;
    }
    
    void DiParticleRenderer::SetTextureCoordsColumns(uint8 const textureCoordsColumns)
    {
        mTextureCoordsColumns = textureCoordsColumns;
        mTextureCoordsRowsAndColumnsSet = true;
    }
    
    size_t DiParticleRenderer::GetNumTextureCoords(void)
    {
        if (mTextureCoordsRowsAndColumnsSet)
        {
            return mTextureCoordsRows * mTextureCoordsColumns;
        }
        else
        {
            return mUVList.size();
        }
    }
    
    void DiParticleRenderer::NotifyRescaled(const DiVec3& scale)
    {
        mRendererScale = scale;
    }
    
    void DiParticleRenderer::AddTextureCoords(const float u, 
        const float v, 
        const float width, 
        const float height)
    {
        mUVList.push_back(new DiFloatRect(u, v, u+width, v+height));
        mTextureCoordsSet = true;
    }
    
    const DiVector<DiFloatRect*>& DiParticleRenderer::GetTextureCoords(void) const
    {
        return mUVList;
    }
    
    bool DiParticleRenderer::GetUseSoftParticles(void) const
    {
        return mUseSoftParticles;
    }
    
    void DiParticleRenderer::SetUseSoftParticles(bool useSoftParticles)
    {
        mUseSoftParticles = useSoftParticles;
        if (!mUseSoftParticles)
        {
            
        }
    }
    
    void DiParticleRenderer::StripNameFromSoftPrefix(DiString& name)
    {
//         if (name.find(SOFT_PREFIX) != DiString::npos)
//         {
//             name.erase(0, SOFT_PREFIX.length());
//         }
    }
    
    float DiParticleRenderer::GetSoftParticlesContrastPower(void) const
    {
        return mSoftParticlesContrastPower;
    }
    
    float DiParticleRenderer::GetSoftParticlesScale(void) const
    {
        return mSoftParticlesScale;
    }
    
    float DiParticleRenderer::GetSoftParticlesDelta(void) const
    {
        return mSoftParticlesDelta;
    }
    
    void DiParticleRenderer::SetSoftParticlesContrastPower(float softParticlesContrastPower)
    {
        
    }
    
    void DiParticleRenderer::SetSoftParticlesScale(float softParticlesScale)
    {
        
    }
    
    void DiParticleRenderer::SetSoftParticlesDelta(float softParticlesDelta)
    {
        
    }
    
    void DiParticleRenderer::CreateSoftMaterial(void)
    {
        
    }
    
    void DiParticleRenderer::CopyTo (DiParticleRenderer* renderer)
    {
        CopyParentTo(renderer);
    }
    
    void DiParticleRenderer::CopyParentTo (DiParticleRenderer* renderer)
    {
        renderer->SetBatchGroupID(mQueueId);
        renderer->SetSorted(mSorted);
        renderer->mTextureCoordsRows = mTextureCoordsRows;
        renderer->mTextureCoordsColumns = mTextureCoordsColumns;
        renderer->mTextureCoordsRowsAndColumnsSet = mTextureCoordsRowsAndColumnsSet;
        renderer->mTextureCoordsSet = mTextureCoordsSet;
        renderer->mUseSoftParticles = mUseSoftParticles;
        renderer->mSoftParticlesContrastPower = mSoftParticlesContrastPower;
        renderer->mSoftParticlesScale = mSoftParticlesScale;
        renderer->mSoftParticlesDelta = mSoftParticlesDelta;
        renderer->mNotifiedDepthMap = mNotifiedDepthMap;
        renderer->mRendererScale = mRendererScale;

        if (mUVList.empty())
        {
            return;
        }
        DiVector<DiFloatRect*>::iterator it;
        DiVector<DiFloatRect*>::iterator itEnd = mUVList.end();
        for (it = mUVList.begin(); it != itEnd; ++it)
        {
            renderer->AddTextureCoords((*it)->left, (*it)->top, (*it)->Width(), (*it)->Height());
        }

    }
}

