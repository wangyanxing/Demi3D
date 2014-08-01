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
#include "DirectionalLight.h"
#include "ShaderParam.h"

#include "K2Terrain.h"
#include "K2TerrainChunk.h"
#include "K2World.h"
#include "K2WorldSerial.h"
#include "K2Model.h"
#include "K2Configs.h"
#include "K2AnimatedObj.h"
#include "K2StaticObj.h"

#include "DebugHelper.h"
#include "ShaderManager.h"
#include "AssetManager.h"
#include "RenderPipeline.h"

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
        DI_LOG("Unloading world");

        RemoveAllRenderObjs();

        mTerrain->Unload();
    }

    void DiK2World::Load(const DiString& path)
    {
        DI_LOG("Loading world: %s", path.c_str());

        DiK2WorldSerial serial;
        serial.Load(path,this);
        mRootNode->AttachObject(mTerrain);
        
        // sun light
        mSun = make_shared<DiDirLight>();
        DiSceneManager* sm = DiBase::Driver->GetSceneManager();
        DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
        dirNode->AttachObject(mSun);
        mSun->SetColor(DiColor());
        
        DiRadian altitude(DiDegree(mConfigs.mSunAltitude));
        DiVec3 dir(0,-DiMath::Sin(altitude),DiMath::Cos(altitude));
        
        DiRadian azimuth(DiDegree(mConfigs.mSunAzimuth));
        DiQuat rot(azimuth, DiVec3::UNIT_Y);
        dir = rot * dir;
        mSun->SetDirection(dir);
        
        sm->SetAmbientColor(DiColor());
    
        Demi::DiRenderBatchGroup* group = Driver->GetPipeline()->GetBatchGroup(Demi::BATCH_MODEL);
        group->SetPreProcess([this](){
            Driver->GetPipeline()->GetShaderEnvironment()->globalAmbient = mConfigs.mEntityAmbient;
            Driver->GetPipeline()->GetShaderEnvironment()->dirLightsColor[0] = mConfigs.mEntitySunColor;
        });
    }

    DiK2RenderObject* DiK2World::AddRenderObj(const DiString& mdf, 
        K2ObjSubTypes::Type subtype, const Trans& trans, int id, uint32 queryFlag)
    {
        auto objtype = K2ObjSubTypes::GetObjType(subtype);

        DiK2RenderObject* renderObj = CreateRenderObject(objtype);
        renderObj->LoadModel(mdf);

        renderObj->SetWorldPosition(trans.pos);
        renderObj->SetRotation(trans.rot);
        renderObj->SetScale(trans.scale);

        renderObj->GetModel()->AddQueryFlags(queryFlag);

        if (subtype == K2ObjSubTypes::SUB_STATIC_TREE)
            ProcessTrees(renderObj);
        else if (subtype == K2ObjSubTypes::SUB_STATIC_CLIFF)
            ProcessCliff(renderObj);

        return renderObj;
    }

    void DiK2World::ProcessTrees(DiK2RenderObject* renderObj)
    {
    }

    void DiK2World::ProcessCliff(DiK2RenderObject* renderObj)
    {
        DiK2ModelPtr model = renderObj->GetModel();
        model->SetBatchGroup(BATCH_TERRAIN);
        DiCullNode* node = renderObj->GetNode();

        DiVec3 pos = node->GetPosition();
        DiQuat rot = node->GetOrientation();

        DiTerrainDescPtr terDesc = mTerrain->GetDesc();

        float cliffsize = terDesc->mCliffSize * terDesc->mGridSize / 2.0f;
        DiVec3 transl(-cliffsize, 0, cliffsize);
        pos += rot * transl;
       
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

        DiTexturePtr textureDif = DiAssetManager::GetInstance().ManualLoadAsset<DiTexture>(diffuse);
        
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
        DiTexturePtr textureNor = DiAssetManager::GetInstance().ManualLoadAsset<DiTexture>(normal);
#else
        DiTexturePtr textureNor = DiAssetManager::GetInstance().ManualLoadAsset<DiTexture>(normal + "_rxgb");
        DiTexturePtr textureSpe = DiAssetManager::GetInstance().ManualLoadAsset<DiTexture>(normal + "_s");
#endif

        uint32 submodels = model->GetNumSubModels();
        for (uint32 i = 0; i < submodels; ++i)
        {
            DiSubModel* sm = model->GetSubModel(i);
            auto material = sm->GetMaterial();

            auto newMat = material->Clone();
            sm->SetMaterial(newMat);

            DiShaderParameter* shaderparam = newMat->GetShaderParameter();
            shaderparam->WriteTexture2D("terrainMap", textureDif);
            shaderparam->WriteTexture2D("terrainNormalMap", textureNor);
#if DEMI_PLATFORM != DEMI_PLATFORM_IOS
            shaderparam->WriteTexture2D("terrainSpecularMap", textureSpe);
#endif
            shaderparam->WriteFloat("cliffUVScale", 1.0f / (terDesc->mTextureScale * terDesc->mGridSize));
        }
    }

    DiK2RenderObject* DiK2World::CreateRenderObject(K2ObjTypes type)
    {
        DiK2RenderObject* ret = nullptr;
        switch (type)
        {
        case INVALID_TYPE:
            DI_WARNING("Invalid game object type");
            return ret;
            break;
        case GAME_HERO:
        case GAME_NPC:
        case GAME_SCENE_ITEM:
            ret = DI_NEW DiK2AnimatedObj(this);
            mAnimatedObjs.insert(ret);
            break;
        case GAME_STATIC:
            ret = DI_NEW DiK2StaticObj(this);
            mRenderObjs.insert(ret);
            break;
        default:
            break;
        }

        return ret;
    }

    void DiK2World::RemoveAllRenderObjs()
    {
        for (auto i = mRenderObjs.begin(); i != mRenderObjs.end(); ++i)
        {
            DI_DELETE *i;
        }
        mRenderObjs.clear();

        for (auto i = mAnimatedObjs.begin(); i != mAnimatedObjs.end(); ++i)
        {
            DI_DELETE *i;
        }
        mAnimatedObjs.clear();
    }

    void DiK2World::Update(float dt)
    {
#ifdef _DEBUG_CLIFF_POS
        mDebugger->Clear();
        DiTerrainDescPtr desc = mTerrain->GetDesc();
        //uint8* blockerBuffer = desc->mVertBlockerMap->GetBuffer();
        uint32 vertX = CHUNK_GRID_SIZE*desc->mSizeX + 1;
        uint32 vertY = CHUNK_GRID_SIZE*desc->mSizeY + 1;
        for (uint32 x = 0; x < vertX; ++x)
        {
            for (uint32 y = 0; y < vertY; ++y)
            {
                auto block = mTerrain->GetPathFinder().GetLevel(x, y);
                if (block > 0 && block <= 10)
                {
                    DiVec3 pos = mTerrain->GetPoint(x, y);
                    mDebugger->AddBoundingBox(pos, 1.5f, DiColor::Red);
                }
            }
        }
#endif
        
        for (auto i = mAnimatedObjs.begin(); i != mAnimatedObjs.end(); ++i)
        {
            (*i)->Update(dt);
        }
    }

    void DiK2World::ProcessWorldEntity(const DiString& mdf, const DiString& type,
        const Trans& trans, int id, int team)
    {
        auto subtype = K2ObjSubTypes::FromString(type);

        switch (subtype)
        {
        case Demi::K2ObjSubTypes::SUB_STATIC_TREE:
            AddRenderObj(mdf, subtype, trans, id, QUERY_TREE);
            break;
        case Demi::K2ObjSubTypes::SUB_STATIC_CLIFF:
            AddRenderObj(mdf, subtype, trans, id, QUERY_TERRAIN);
            break;
        case Demi::K2ObjSubTypes::SUB_STATIC_SCENERY:
        case Demi::K2ObjSubTypes::SUB_STATIC_BUILDING:
            AddRenderObj(mdf, subtype, trans, id, QUERY_STATIC_MODEL);
            break;
        //case Demi::K2ObjSubTypes::SUB_STATIC_MODEL:
        //case Demi::K2ObjSubTypes::SUB_STATIC_WATER:
        //case Demi::K2ObjSubTypes::SUB_SCENE_ITEM:
        //case Demi::K2ObjSubTypes::SUB_NPC:
            //break;
        
        case Demi::K2ObjSubTypes::SUB_LOGIC_TRIGGER_SPAWN_POINT:
        case Demi::K2ObjSubTypes::SUB_LOGIC_ENTITY:
        case Demi::K2ObjSubTypes::SUB_LOGIC_TELPORT:
            mTriggers[id] = trans.pos;
            break;
        default:
            break;
        }
    }

    bool DiK2World::GetTriggerPosition(int id, DiVec3& out)
    {
        auto i = mTriggers.find(id);
        if (i != mTriggers.end())
        {
            out = i->second;
            return true;
        }
        return false;
    }

}