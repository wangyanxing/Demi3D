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
#include "SceneManager.h"
#include "CullNode.h"
#include "Model.h"
#include "SubModel.h"
#include "ShaderParam.h"

#include "K2TerrainMap.h"
#include "K2World.h"
#include "K2WorldSerial.h"
#include "K2Model.h"
#include "K2Configs.h"

#include "DebugHelper.h"
#include "ShaderManager.h"

namespace Demi
{
    DiK2World::DiK2World()
        : mRootNode(nullptr)
    {
        mRootNode = Driver->GetSceneManager()->GetRootNode()->CreateChild();

#ifdef _DEBUG_CLIFF_POS
        mDebugger = make_shared<DiDebugHelper>();
        DiMaterialPtr helpermat = DiMaterial::QuickCreate(
            "basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
        helpermat->SetDepthCheck(false);
        mDebugger->SetMaterial(helpermat);
        mRootNode->AttachObject(mDebugger);
#endif
    }

    DiK2World::~DiK2World()
    {
        Unload();
    }

    void DiK2World::Unload()
    {
        for (auto i = mModels.begin(); i != mModels.end(); ++i)
            DI_DELETE (*i);
        mModels.clear();
        mTerrain->Unload();
    }

    void DiK2World::Load(const DiString& path)
    {
        DiK2WorldSerial serial;
        serial.Load(path,this);
        mRootNode->AttachObject(mTerrain);
    }

    DiK2Model* DiK2World::AddModel(const DiString& mdf, const DiString& type, const Trans& trans)
    {
        DiK2Model* md = DI_NEW DiK2Model(mdf);
        DiCullNode* node = md->CreateNode(mRootNode);
        mModels.push_back(md);

        node->SetPosition(trans.pos);
        node->SetOrientation(trans.rot);
        node->SetScale(trans.scale);

        if (type == "Prop_Tree")
            ProcessTrees(md);
        else if (DiString::StartsWith(type, "Prop_Cliff"))
            ProcessCliff(md);

        return md;
    }

    void DiK2World::ProcessTrees(DiK2Model* model)
    {
    }

    void DiK2World::ProcessCliff(DiK2Model* model)
    {
        DiVec3 pos = model->GetNode()->GetPosition();
        DiQuat rot = model->GetNode()->GetOrientation();

        DiVec2 worldSize = mTerrain->GetWorldSize();

        DiTerrainDescPtr terDesc = mTerrain->GetDesc();

        float cliffsize = terDesc->mCliffSize * terDesc->mGridSize / 2.0f;
        DiVec3 transl(-cliffsize, 0, cliffsize);
        pos += rot * transl;
        
#ifdef _DEBUG_CLIFF_POS
        mDebugger->AddBoundingBox(pos, 1.5f, DiColor::Red);
#endif

        DiTerrainDescPtr desc = mTerrain->GetDesc();

        int gridx = pos.x / CHUNK_GRID_SIZE;
        int gridy = pos.z / CHUNK_GRID_SIZE;
        int gridSizeX = (CHUNK_GRID_SIZE * desc->mSizeX);
        int gridSizeY = (CHUNK_GRID_SIZE * desc->mSizeY);

        int realGrid = gridx + gridy * gridSizeX;
        realGrid = DiMath::Clamp(realGrid, 0, gridSizeX*gridSizeY - 1);
        
        uint16 diftexid = desc->mTextureIDMap->GetBuffer(0)[realGrid].diffuseID;
        uint16 normtexid = desc->mTextureIDMap->GetBuffer(0)[realGrid].normalID;

        DiString diffuse = desc->mTextureTable[diftexid];
        DiString normal = desc->mTextureTable[normtexid];

        DiTexturePtr textureDif = DiK2Configs::GetTexture(diffuse);
        DiTexturePtr textureNor = DiK2Configs::GetTexture(normal + "_rxgb");
        DiTexturePtr textureSpe = DiK2Configs::GetTexture(normal + "_s");

        DiModelPtr md = model->GetMesh();
        uint32 submodels = md->GetNumSubModels();
        for (uint32 i = 0; i < submodels; ++i)
        {
            DiSubModel* sm = md->GetSubModel(i);
            auto material = sm->GetMaterial();

            auto newMat = material->Clone();
            sm->SetMaterial(newMat);

            //newMat->SetWireframe(true);

            DiShaderParameter* shaderparam = newMat->GetShaderParameter();
            shaderparam->WriteTexture2D("terrainMap", textureDif);
            shaderparam->WriteTexture2D("terrainNormalMap", textureNor);
            shaderparam->WriteTexture2D("terrainSpecularMap", textureSpe);
            shaderparam->WriteFloat("cliffUVScale", 1.0f / (terDesc->mTextureScale * terDesc->mGridSize));
        }
    }
}