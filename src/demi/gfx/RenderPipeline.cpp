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
#include "GfxPch.h"
#include "RenderPipeline.h"
#include "GpuProgram.h"
#include "PostEffectManager.h"
#include "math/MathMisc.h"
#include "RenderUnit.h"
#include "Material.h"
#include "ShaderParam.h"
#include "SceneManager.h"
#include "Camera.h"
#include "RenderWindow.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SkyLight.h"
#include "GBuffer.h"
#include "GfxDriver.h"
#include "RenderTarget.h"
#include "Node.h"
#include "ShaderManager.h"

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   pragma warning(disable:4701)
#endif

namespace Demi 
{
    DiRenderPipeline::DiRenderPipeline()
        :
        mActiveVp(NULL),
        mEnableInterface(true),
        mPostEnable(true),
        mCurrentPass(P_LIGHTING_PASS),
        mCurrentSceneMgr(NULL),
        mEnableGbuffer(false)
    {
        Init();
    }

    DiRenderPipeline::~DiRenderPipeline()
    {
        Release();
    }

    void DiRenderPipeline::Init()
    {
        mShaderEnv = DI_NEW DiShaderEnvironment();

        DI_ASSERT(mBatchGroups.empty());

        for (uint32 i=0; i<NUM_BATCH_TYPES; i++)
            mBatchGroups.push_back(DI_NEW DiRenderBatchGroup((DiBatchGroupType)i,this));

        mBatchGroups[BATCH_POST_FILTER]->SetUseIdentityProj(true);
        mBatchGroups[BATCH_2D]->SetUseIdentityProj(true);
        mBatchGroups[BATCH_GUI]->SetUseIdentityProj(true);

        mBatchGroups[BATCH_TERRAIN]->SetShadowEnable(true);
        mBatchGroups[BATCH_MODEL]->SetShadowEnable(true);
    }

    void DiRenderPipeline::Release()
    {
        for (uint32 i = 0; i < mBatchGroups.size(); i++)
            DI_DELETE mBatchGroups[i];
        
        mBatchGroups.clear();

        SAFE_DELETE(mShaderEnv);
    }

    void DiRenderPipeline::SetCameraParams(DiSceneManager*sm, DiCamera* cam)
    {
        mShaderEnv->viewMatrix = cam->GetViewMatrix();
        cam->GetProjectionMatrixRs(mShaderEnv->projMatrix);

        if (Driver->NeedTextureFlipping())
        {
            mShaderEnv->projMatrix[1][0] = -mShaderEnv->projMatrix[1][0];
            mShaderEnv->projMatrix[1][1] = -mShaderEnv->projMatrix[1][1];
            mShaderEnv->projMatrix[1][2] = -mShaderEnv->projMatrix[1][2];
            mShaderEnv->projMatrix[1][3] = -mShaderEnv->projMatrix[1][3];
        }

        mShaderEnv->eyePosition = cam->GetDerivedPosition();
        mShaderEnv->eyeDirection = cam->GetDirection();
        mShaderEnv->farnearPlane = DiVec4(cam->GetFarClipDistance(),cam->GetNearClipDistance(),0,0);
        mShaderEnv->viewProjMatrix = mShaderEnv->projMatrix * mShaderEnv->viewMatrix;
        
        auto& visbounds = cam->GetVisBoundsInfo();
        float depthRange = visbounds.maxDistanceInFrustum - visbounds.minDistanceInFrustum;
        mShaderEnv->depthRange = DiVec4(visbounds.minDistanceInFrustum,
                                        visbounds.maxDistanceInFrustum,
                                        depthRange,
                                        1.0f / depthRange);
    }

    void DiRenderPipeline::SetGlobalParams(DiSceneManager*sm, DiCamera* cam)
    {
        SetCameraParams(sm, cam);
        
        Driver->SetFillMode(cam->GetFillMode());

        mShaderEnv->time = Driver->GetElapsedSecond();
        mShaderEnv->viewportSize.x = (float)DiRenderWindow::ActiveWindow->GetWidth();
        mShaderEnv->viewportSize.y = (float)DiRenderWindow::ActiveWindow->GetHeight();
        mShaderEnv->viewportSize.z = 1.0f / mShaderEnv->viewportSize.x;
        mShaderEnv->viewportSize.w = 1.0f / mShaderEnv->viewportSize.y;

        mShaderEnv->texelOffsets.x = Driver->GetTexelOffset().x;
        mShaderEnv->texelOffsets.y = Driver->GetTexelOffset().y;
        mShaderEnv->texelOffsets.z = Driver->GetTexelOffset().x / mShaderEnv->viewportSize.x;
        mShaderEnv->texelOffsets.w = Driver->GetTexelOffset().y / mShaderEnv->viewportSize.y;
    }

    void DiRenderPipeline::SetLightingParams(DiSceneManager*sm)
    {
        // bind lights
        // global parameters
        mShaderEnv->globalAmbient = sm->GetAmbientColor();

        DiVisibleLights& visibleLights = sm->GetVisibleLights();
        mShaderEnv->numDirLights = (int)visibleLights.dirLights.size();
        for (int i = 0; i < mShaderEnv->numDirLights; i++)
        {
            mShaderEnv->dirLightsColor[i] = visibleLights.dirLights[i]->GetColor();
            DiVec3 dir = visibleLights.dirLights[i]->GetDirection().normalisedCopy();
            mShaderEnv->dirLightsDir[i] = dir;
        }

        mShaderEnv->numPointLights = (int)visibleLights.pointLights.size();
        for (int i = 0; i < mShaderEnv->numPointLights; i++)
        {
            mShaderEnv->pointLightsColor[i] = visibleLights.pointLights[i]->GetColor();
            //DiVec3 pos = sm->GetPointLight(i)->GetPosition();
            DiNode* lightNode = visibleLights.pointLights[i]->GetParentNode();
            if (!lightNode)
                DI_WARNING("The point light should be attached to a specific node");
            else
                mShaderEnv->pointLightsPosition[i] = lightNode->GetDerivedPosition();

            mShaderEnv->pointLightsAttenuation[i].x = visibleLights.pointLights[i]->GetAttenuationBegin();
            mShaderEnv->pointLightsAttenuation[i].y = visibleLights.pointLights[i]->GetAttenuationEnd();
        }

        mShaderEnv->hasSkyLight = visibleLights.skyLight != nullptr;
        if (mShaderEnv->hasSkyLight)
        {
            mShaderEnv->skyLightColor = visibleLights.skyLight->GetColor();
            mShaderEnv->groundColor = visibleLights.skyLight->GetGroundColor();
            mShaderEnv->skyLightDir = visibleLights.skyLight->GetDirection();
        }
    }
    
    void DiRenderPipeline::RenderPost(DiSceneManager*sm, DiCamera* cam)
    {
        // we don't care about the bone matrices at this moment
        SetCameraParams(sm, cam);
        
        if (mPostEnable)
            DiRenderWindow::ActiveWindow->GetPostEffectManager()->Process(this);

        if (DiRenderWindow::ActiveWindow->IsForceRenderToCanvas())
            DiRenderWindow::ActiveWindow->GetRenderBuffer()->Bind();

        for (uint32 i = BATCH_POST_FILTER; i < mBatchGroups.size(); i++)
        {
            if (!mBatchGroups[i]->IsInterfaceGroup() ||
                (mBatchGroups[i]->IsInterfaceGroup() && mEnableInterface))
            {
                mBatchGroups[i]->Process();
            }
        }
    }

    void DiRenderPipeline::Render(DiSceneManager*sm, DiCamera* cam, DiRenderTarget* rt)
    {
        DI_PROFILE(RenderPipeline_Render);
        if (!rt)
            return;
        
        rt->Bind();

        Driver->SetInvertVertexWinding(cam->IsReflected());

        SetGlobalParams(sm, cam);
        SetLightingParams(sm);

        // todo: move it to the outside
        ProcessGBuffers();

        uint32 dps = 0;
        uint32 faces = 0;

        bool needStatics = DiRenderWindow::ActiveWindow->GetSceneCanvas() == rt;
        mEnableInterface = true; //TODO

        for (uint32 i = 0; i<mBatchGroups.size(); i++)
        {
            mBatchGroups[i]->ClearStatis();
            mBatchGroups[i]->EnableStatis(needStatics);
        }

        // bind post batch groups
        for (uint32 i = 0; i < BATCH_POST_FILTER; i++)
        {
            mBatchGroups[i]->Process();

            if (needStatics)
            {
                dps += mBatchGroups[i]->GetBatchesNum();
                faces += mBatchGroups[i]->GetPrimitivesNum();
            }

            mBatchGroups[i]->EnableStatis(false);
        }
    }

    bool DiRenderPipeline::RenderSingleBatch( DiRenderUnit* batch ,DiMat4* mat )
    {
        DI_ASSERT(batch);

        batch->PreRender();

        BindMeshContext(batch,mat);

        DiMaterial* material = nullptr;

        if (mCurrentPass == P_GBUFFER_PASS)
            material = DiRenderWindow::ActiveWindow->GetGBuffer()->GetMaterial().get();
        else if (mCurrentPass == P_SHADOW_PASS && batch->mMaterial)
        {
            material = DiMaterial::GetShadowCasterMaterial(batch->mMaterial->GetShaderFlags()).get();
        }
        else
        {
            material = batch->mMaterial.get();
        }

        if (!material)
            return false;

        material->Bind();

        Driver->RenderGeometry(batch);
    
        return true;
    }

    void DiRenderPipeline::Render2DBatch( DiRenderUnit* batch, DiMaterial* material )
    {
        if (material)
        {
            mShaderEnv->modelMatrix.setIdentity();
            mShaderEnv->modelViewMatrix = mShaderEnv->viewMatrix * mShaderEnv->modelMatrix;
            mShaderEnv->modelViewProjMatrix = mShaderEnv->projMatrix * mShaderEnv->modelViewMatrix;

            material->Bind();
            Driver->RenderGeometry(batch);
        }
    }

    void DiRenderPipeline::ProcessGBuffers()
    {
        if (!mEnableGbuffer)
            return;

        mCurrentPass = P_GBUFFER_PASS;

        DiRenderWindow::ActiveWindow->GetGBuffer()->Bind();

        for (uint32 i = 0; i <= BATCH_CHARACTER; i++)
            mBatchGroups[i]->Process();

        // TODO
        mCurrentPass = P_LIGHTING_PASS;
    }

    void DiRenderPipeline::BindMeshContext( DiRenderUnit* batch ,DiMat4* worldmat)
    {
        uint32 modelTransNums = batch->GetWorldTransformNums();
        
        DI_ASSERT(modelTransNums > 0 && modelTransNums < MAX_WORLD_MATS);

        if (modelTransNums > 1) 
        {
            for (uint32 i=0; i<modelTransNums; i++)
                memcpy(&mShaderEnv->modelMatrices[i*3],&worldmat[i],sizeof(float)*12);
            mShaderEnv->numModelMatrices = modelTransNums;
        } 
        else 
        {
            mShaderEnv->modelMatrix = *worldmat;

            DiMat4 view = mShaderEnv->viewMatrix;
            mShaderEnv->modelViewMatrix = view * mShaderEnv->modelMatrix;

            DiMat4 modelview = mShaderEnv->modelViewMatrix;
            DiMat4 proj = mShaderEnv->projMatrix;
            mShaderEnv->modelViewProjMatrix = proj * modelview;            
        }

        if (batch->mBoneMatrices && batch->mBoneNum > 0 
            && batch->mBoneNum <= MAX_BONE_NUM) 
        {
            for (uint32 i=0; i<batch->mBoneNum; i++)
                memcpy(&mShaderEnv->boneMatrices[i*3], &batch->mBoneMatrices[i],sizeof(float)*12);
            mShaderEnv->numBones = batch->mBoneNum;
        }
    }

    DiRenderBatchGroup* DiRenderPipeline::GetBatchGroup(uint32 typeID)
    {
        DI_ASSERT(typeID < mBatchGroups.size());
        return mBatchGroups[typeID];
    }

    void DiRenderPipeline::ClearGroup()
    {
        for (uint32 i = 0; i < mBatchGroups.size(); i++)
            mBatchGroups[i]->Clear();
    }

    int DiRenderPipeline::GetBatchNums()
    {
        int ret = 0;
        for (uint32 i = 0; i < mBatchGroups.size(); i++)
        {
            ret += mBatchGroups[i]->GetBatchesNum();
        }
        return ret;
    }
}