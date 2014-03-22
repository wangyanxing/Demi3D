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
#include "K2Pch.h"
#include "K2FoliageMap.h"
#include "K2FoliageLayer.h"
#include "K2FoliageChunk.h"
#include "K2TerrainMap.h"
#include "RandomTable.h"
#include "ShaderParam.h"

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   pragma warning(disable : 4244)
#endif

namespace Demi
{
    DiFoliageMap::DiFoliageMap( DiTerrainMap* terrainmap )
        :mTable(NULL),
        mTerrainMap(terrainmap)
    {
        mWindTimer.Start();
        mLastTime = 0;
        mWindDir = DiVec3::UNIT_X;
    }

    DiFoliageMap::~DiFoliageMap()
    {
        Unload();
    }

    void DiFoliageMap::Load()
    {
        mTable = DI_NEW DiRandomTable();
    }

    void DiFoliageMap::Unload()
    {
        {
            DiVector<DiFoliageChunk*>::iterator it;
            DiVector<DiFoliageChunk*>::iterator itEnd = mChunks.end();
            for (it = mChunks.begin(); it != itEnd; ++it)
            {
                SAFE_DELETE(*it);
            }
            mChunks.clear();
        }
        
        {
            LayerList::iterator it;
            LayerList::iterator itEnd = mLayers.end();
            for (it = mLayers.begin(); it != itEnd; ++it)
            {
                SAFE_DELETE(*it);
            }
            mLayers.clear();
        }
        

        SAFE_DELETE(mTable);
    }

    void DiFoliageMap::AddChunk( uint16 x, uint16 y )
    {
        DiFoliageChunk* wc = DI_NEW DiFoliageChunk(x,y,this);
        mChunks.push_back(wc);
    }

    DiFoliageChunk* DiFoliageMap::GetChunk( int id )
    {
        return mChunks[id];
    }

    DiFoliageLayer* DiFoliageMap::GetLayer( size_t id )
    {
        return mLayers[id];
    }

    void DiFoliageMap::CreateLayerFromDesc( )
    {
        DiTerrainDescPtr desc = mTerrainMap->GetDesc();

        for (auto it = desc->mFoliageLayers.begin(); it != desc->mFoliageLayers.end(); ++it)
        {
            DiFoliageLayer* l = nullptr;
            l = DI_NEW DiFoliageLayer(this,(*it));
            mLayers.push_back(l);
        }
    }

    void DiFoliageMap::Update()
    {
        unsigned long currentTime = mWindTimer.GetElapse() * 1000;
        unsigned long ellapsedTime = currentTime - mLastTime;
        mLastTime = currentTime;

        float ellapsed = ellapsedTime / 1000.0f;

        LayerList::iterator it;
        for (it = mLayers.begin(); it != mLayers.end(); ++it)
        {
            DiFoliageLayer* layer = *it;

            layer->UpdateMaterial();

            DiShaderParameter* sp = layer->mMaterial->GetShaderParameter();
            if (layer->mDesc->mAnimate)
            {
                layer->mWaveCount += ellapsed * (layer->mDesc->mAnimSpeed * DiMath::PI);
                if (layer->mWaveCount > DiMath::PI*2)
                {
                    layer->mWaveCount -= DiMath::PI*2;
                }

                sp->WriteFloat("time",layer->mWaveCount);
                sp->WriteFloat("frequency",layer->mDesc->mAnimFreq);

                DiVec3 direction = mWindDir * layer->mDesc->mAnimMag;
                sp->WriteFloat4("direction",DiVec4(direction.x, direction.y, direction.z, 0));
            }
            // ����
            DiColor c;
            c.SetAsARGB(layer->mDesc->mColor);
            layer->mMaterial->SetDiffuse(c);
        }
    }

    DiFoliageLayer* DiFoliageMap::AddNewLayer( DiFoliageLayerDesc* desc )
    {
        DiFoliageLayer* l = NULL;
        l = DI_NEW DiFoliageLayer(this,desc);
        mLayers.push_back(l);
        return l;
    }

    void DiFoliageMap::DeleteLayer( int layerID )
    {
        DI_ASSERT(layerID >= 0 && layerID < (int)mLayers.size());

        DiFoliageLayer* l = mLayers[layerID];

        DeleteChunkLayer(l);

        SAFE_DELETE(l);

        LayerList::iterator it = mLayers.begin() + layerID;
        mLayers.erase(it);
    }

    void DiFoliageMap::DeleteChunkLayer( DiFoliageLayer* layer )
    {
        DiVector<DiFoliageChunk*>::iterator it;
        DiVector<DiFoliageChunk*>::iterator itEnd = mChunks.end();
        for (it = mChunks.begin(); it != itEnd; ++it)
        {
            (*it)->DeleteBatch(layer);
        }
    }
}