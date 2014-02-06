
#include "GfxPch.h"
#include "RenderPipeline.h"
#include "GpuProgram.h"
#include "PostEffectManager.h"
#include "MathMisc.h"
#include "RenderUnit.h"
#include "Material.h"
#include "ShaderParam.h"
#include "SceneManager.h"
#include "Camera.h"
#include "RenderWindow.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "GBuffer.h"
#include "GfxDriver.h"
#include "RenderTarget.h"

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   pragma warning(disable:4701)
#endif

namespace Demi 
{
    const DiMat4 PROJECTIONCLIPSPACE2DTOIMAGESPACE_PERSPECTIVE(
        0.5f,   0,     0,  0.5f, 
        0,   -0.5f,    0,  0.5f, 
        0,      0,     1,    0,
        0,      0,     0,    1);

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

    void DiRenderPipeline::BindShadowCamera( DiCamera* cam )
    {
        DiMat4 proj;
        cam->GetProjectionMatrixRs(proj);

        mShaderEnv->texViewProjMatrix = 
            PROJECTIONCLIPSPACE2DTOIMAGESPACE_PERSPECTIVE * 
            proj * cam->GetViewMatrix();
    }

    void DiRenderPipeline::SetGlobalParams(DiSceneManager*sm, DiCamera* cam)
    {
        const DiCamera* realCam = cam->GetLodCamera();
        mShaderEnv->viewMatrix = realCam->GetViewMatrix();
        realCam->GetProjectionMatrixRs(mShaderEnv->projMatrix);
        mShaderEnv->eyePosition = realCam->GetDerivedPosition();
        mShaderEnv->eyeDirection = realCam->GetDirection();
        mShaderEnv->farnearPlane = DiVec4(cam->GetFarClipDistance(),cam->GetNearClipDistance(),0,0);

        mShaderEnv->viewProjMatrix = mShaderEnv->projMatrix * mShaderEnv->viewMatrix;

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

        // bind lights
        // global parameters
        mShaderEnv->globalAmbient = sm->GetAmbientColor();

        mShaderEnv->numDirLights = (int)sm->GetDirLightNum();
        for(int i=0; i < mShaderEnv->numDirLights; i++)
        {
            mShaderEnv->dirLightsColor[i] = sm->GetDirLight(i)->GetColor();
            DiVec3 dir = sm->GetDirLight(i)->GetDirection().normalisedCopy();
            mShaderEnv->dirLightsDir[i] = dir;
        }

        mShaderEnv->numPointLights = (int)sm->GetPointLightNum();
        for(int i=0; i < mShaderEnv->numPointLights; i++)
        {
            mShaderEnv->pointLightsColor[i] = sm->GetPointLight(i)->GetColor();
            DiVec3 pos = sm->GetPointLight(i)->GetPosition();
            mShaderEnv->pointLightsPosition[i] = pos;
            mShaderEnv->pointLightsAttenuation[i].x = sm->GetPointLight(i)->GetAttenuationBegin();
            mShaderEnv->pointLightsAttenuation[i].y = sm->GetPointLight(i)->GetAttenuationEnd();
        }
    }

    void DiRenderPipeline::Render(DiSceneManager*sm, DiCamera* cam, DiRenderTarget* rt)
    {
        DI_PROFILE(RenderPipeline_Render);
        if (!rt)
            return;

        SetGlobalParams(sm, cam);

        ProcessGBuffers();

        rt->Bind();

        uint32 dps = 0;
        uint32 faces = 0;

        bool needStatics = DiRenderWindow::ActiveWindow->GetRenderBuffer() == rt;
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

        for (uint32 i = 0; i < BATCH_POST_FILTER; i++)
        {
            mBatchGroups[i]->ClearStatis();
            mBatchGroups[i]->EnableStatis(needStatics);
        }

        if (mPostEnable)
            DiRenderWindow::ActiveWindow->GetPostEffectManager()->Process(this);

        for (uint32 i = BATCH_POST_FILTER; i<mBatchGroups.size(); i++)
        {
            mBatchGroups[i]->ClearStatis();
            mBatchGroups[i]->EnableStatis(needStatics);
        }

        for (uint32 i = BATCH_POST_FILTER; i < mBatchGroups.size(); i++)
        {
            if (!mBatchGroups[i]->IsInterfaceGroup() ||
                (mBatchGroups[i]->IsInterfaceGroup() && mEnableInterface))
            {
                mBatchGroups[i]->Process();

                if (needStatics)
                {
                    dps += mBatchGroups[i]->GetBatchesNum();
                    faces += mBatchGroups[i]->GetPrimitivesNum();
                }
            }
        }

        if (needStatics)
        {
            DiRenderWindow::ActiveWindow->SetCurrentBatchesNum(dps);
            DiRenderWindow::ActiveWindow->SetCurrentPrimitivesNum(faces);
        }
    }

    bool DiRenderPipeline::RenderSingleBatch( DiRenderUnit* batch ,DiMat4* mat )
    {
        DI_ASSERT(batch);

        batch->PreRender();

        BindMeshContext(batch,mat);

        DiMaterial* material = NULL;

        if (mCurrentPass == P_GBUFFER_PASS)
            material = DiRenderWindow::ActiveWindow->GetGBuffer()->GetMaterial().get();
        else
            material = batch->mMaterial.get();

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
            mShaderEnv->modelViewMatrix = mShaderEnv->viewMatrix *
                mShaderEnv->modelMatrix;
            mShaderEnv->modelViewProjMatrix = mShaderEnv->projMatrix * 
                mShaderEnv->modelViewMatrix;    

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

        DiMat4 worldInv = worldmat->inverseAffine();
        DiVec4 ep;
        ep.x = mShaderEnv->eyePosition[0];
        ep.y = mShaderEnv->eyePosition[1];
        ep.z = mShaderEnv->eyePosition[2];
        ep.w = 1;
        DiVec4 epos = worldInv.transformAffine(ep);
        mShaderEnv->eyePositionObjSpace = epos;

        if (batch->mBoneMatrices && batch->mBoneNum > 0 
            && batch->mBoneNum < MAX_BONE_NUM) 
        {
            for (uint32 i=0; i<batch->mBoneNum; i++)
                memcpy(&mShaderEnv->boneMatrices[i*3],&batch->mBoneMatrices[i],sizeof(float)*12);
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
}