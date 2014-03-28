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
#include "K2AnimatedObj.h"
#include "K2StaticObj.h"
#include "K2GameCamera.h"

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
    }

    DiK2RenderObject* DiK2World::AddRenderObj(const DiString& mdf, K2ObjSubTypes::Type subtype, const Trans& trans, int id)
    {
        auto objtype = K2ObjSubTypes::GetObjType(subtype);

        DiK2RenderObject* renderObj = CreateRenderObject(objtype);
        renderObj->LoadModel(mdf);

        renderObj->SetWorldPosition(trans.pos);
        renderObj->SetRotation(trans.rot);
        renderObj->SetScale(trans.scale);

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
        DiCullNode* node = renderObj->GetNode();

        DiVec3 pos = node->GetPosition();
        DiQuat rot = node->GetOrientation();

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

        uint32 submodels = model->GetNumSubModels();
        for (uint32 i = 0; i < submodels; ++i)
        {
            DiSubModel* sm = model->GetSubModel(i);
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
            ret = DI_NEW DiK2AnimatedObj();
            break;
        case GAME_STATIC:
            ret = DI_NEW DiK2StaticObj();
            break;
        default:
            break;
        }

        if (!ret)
            mRenderObjs.insert(ret);

        return ret;
    }

    bool DiK2World::RemoveRenderObj(DiK2RenderObject* obj)
    {
        auto i = mRenderObjs.find(obj);
        if (i == mRenderObjs.end())
            return false;
        
        DI_DELETE obj;

        mRenderObjs.erase(i);
        return true;
    }

    void DiK2World::RemoveAllRenderObjs()
    {
        for (auto i = mRenderObjs.begin(); i != mRenderObjs.end(); ++i)
        {
            DI_DELETE *i;
        }
        mRenderObjs.clear();
    }

    void DiK2World::Update(float dt)
    {
    }

    void DiK2World::OnKeyInput(const K2KeyEvent& event)
    {
    }

    void DiK2World::OnMouseInput(const K2MouseEvent& event)
    {
    }

    void DiK2World::ProcessWorldEntity(const DiString& mdf, const DiString& type, 
        const Trans& trans, int id, int team)
    {
        auto subtype = K2ObjSubTypes::FromString(type);

        if (subtype == K2ObjSubTypes::SUB_STATIC_CLIFF || 
            subtype == K2ObjSubTypes::SUB_STATIC_TREE  ||
            subtype == K2ObjSubTypes::SUB_STATIC_MODEL ||
            subtype == K2ObjSubTypes::SUB_STATIC_SCENERY)
        {
            // directly add to the scene
            AddRenderObj(mdf, subtype, trans, id);
        }
        else if (subtype == K2ObjSubTypes::SUB_LOGIC_TRIGGER_SPAWN_POINT)
        {
            mSpawnPoint[team] = trans.pos;
        }
    }

    Demi::DiVec3 DiK2World::GetSpawnPoint(int id)
    {
        auto i = mSpawnPoint.find(id);
        if (i != mSpawnPoint.end())
            return i->second;
        return DiVec3::ZERO;
    }

}