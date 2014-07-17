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
#include "SceneManager.h"
#include "GfxDriver.h"
#include "RenderPipeline.h"
#include "Camera.h"
#include "CullNode.h"
#include "AssetManager.h"
#include "CullNode.h"
#include "InstanceManager.h"
#include "OctreeQuery.h"
#include "RenderWindow.h"
#include "RenderTarget.h"
#include "Skybox.h"
#include "Command.h"
#include "ConsoleVariable.h"
#include "GfxDriver.h"
#include "ShadowPolicy.h"

namespace Demi 
{
    void DiVisibleObjs::UpdateAll(DiCamera* camera)
    {
        std::for_each(objs.begin(), objs.end(),
            [camera](DiTransUnitPtr obj) {
            obj->Update(camera); 
        });
    }

    void DiVisibleObjs::AddToBatch(DiRenderPipeline* pipeline)
    {
        std::for_each(objs.begin(), objs.end(),
        [pipeline](DiTransUnitPtr obj) 
        { 
            obj->AddToBatchGroup(pipeline->GetBatchGroup(obj->GetBatchGroup()));
        });
    }

    DiSceneManager::DiSceneManager(DiRenderWindow* parentWnd)
        :
        mParentWindow(parentWnd),
        mCurrentRenderPass(GEOMETRY_PASS),
        mSkybox(nullptr),
        mAmbientColor(0.3f,0.3f,0.3f),
        mCuller(nullptr),
        mCullerFactory(nullptr),
        mDefaultShadowFarDist(0),
        mDefaultShadowFarDistSquared(0),
        mShadowTextureOffset(0.6f)
    {
        mCullerFactory = DI_NEW DiSceneCullerFactory(this);
        mPipeline = Driver->GetPipeline();
        mCamera = CreateCamera("_sm_camera");

        mBox = DiAABB( -10000, -10000, -10000, 10000, 10000, 10000 );
        
        DiString sceneType = CommandMgr->GetConsoleVar("scene_type")->GetString();

        mCuller = mCullerFactory->CreateSceneCuller(sceneType);
        mRootNode = DI_NEW DiCullNode(this,"_root");
        
        mSkybox = DI_NEW DiSkybox(this);
        
        mShadowPolicy = DI_NEW DiNormalShadowPolicy();
    }

    DiSceneManager::~DiSceneManager(void)
    {
        //DestroyTerrain();
        
        mVisibleObjects.objs.clear();

        ClearNodes();
        
        DestroyAllCameras();
        
        DI_DELETE mShadowPolicy;
        mShadowPolicy = nullptr;
        
        DI_DELETE mCuller;
        mCuller = nullptr;
        
        DI_DELETE mSkybox;
        mSkybox = nullptr;
        
        DI_DELETE mCullerFactory;
        mCullerFactory = nullptr;
    }

    DiCullNode* DiSceneManager::CreateNode()
    {
        DiCullNode* node = DI_NEW DiCullNode(this);
        mSceneNodes.insert(node);
        return node;
    }

    DiCullNode* DiSceneManager::CreateNode( const DiString& name )
    {
        DiCullNode* node = DI_NEW DiCullNode(this,name);
        mSceneNodes.insert(node);
        return node;
    }

    void DiSceneManager::DestroyNode(DiCullNode* node)
    {
        auto i = mSceneNodes.find(node);

        if (i == mSceneNodes.end())
        {
            DI_WARNING("Cannot find the node, failed to destory scene node");
        }

        if (*i)
            mCuller->RemoveUnit((*i)->GetCullUnit());

        DiNode* parentNode = (*i)->GetParent();
        if (parentNode)
        {
            parentNode->RemoveChild(*i);
        }
        DI_DELETE (*i);
        mSceneNodes.erase(i);
    }

    void DiSceneManager::ClearNodes()
    {
        DI_LOG("Cleanning scene nodes [%d]..", mSceneNodes.size());

        mRootNode->RemoveAllChildren();
        mRootNode->DetachAllObjects();

        for (auto it = mSceneNodes.begin(); it != mSceneNodes.end(); ++it)
        {
            DI_DELETE (*it);
        }
        mSceneNodes.clear();
    }

    void DiSceneManager::PreUpdate()
    {
        UpdateDirtyInstanceManagers();
        mVisibleLights.Clear();
        mSkybox->Update();
        mRootNode->_Update(true, false);
        for (auto i = mSceneNodes.begin(); i != mSceneNodes.end(); ++i)
            (*i)->SetCulled(true);
    }

    void DiSceneManager::Cull(DiCamera* cam)
    {
        // clear all culled objects and lights
        if (mCurrentRenderPass == GEOMETRY_PASS)
            mMainVisibleObjects.objs.clear();
        mVisibleObjects.objs.clear();

        mBox.SetNull();

        mCuller->Cull(cam);
    }

    void DiSceneManager::DirtyInstanceManager( DiInstanceManager *dirtyManager )
    {
        mDirtyInstanceManagers.push_back( dirtyManager );
    }

    void DiSceneManager::UpdateDirtyInstanceManagers()
    {
        if (mDirtyInstanceManagers.empty())
            return;

        mDirtyInstanceMgrsTmp.insert( mDirtyInstanceMgrsTmp.end(),
                                      mDirtyInstanceManagers.begin(),
                                      mDirtyInstanceManagers.end() );
        mDirtyInstanceManagers.clear();

        while( !mDirtyInstanceMgrsTmp.empty() )
        {
            auto itor = mDirtyInstanceMgrsTmp.begin();
            auto end  = mDirtyInstanceMgrsTmp.end();

            while( itor != end )
            {
                (*itor)->UpdateDirtyBatches();
                ++itor;
            }

            mDirtyInstanceMgrsTmp.clear();

            mDirtyInstanceMgrsTmp.insert( mDirtyInstanceMgrsTmp.end(), mDirtyInstanceManagers.begin(),
                mDirtyInstanceManagers.end() );
            mDirtyInstanceManagers.clear();
        }
    }

    DiCamera* DiSceneManager::CreateCamera( const DiString& name )
    {
        if (mCameras.find(name) != mCameras.end())
        {
            DI_WARNING("The camera %s has existed",name.c_str());
            return mCameras[name];
        }

        DiCamera *c = DI_NEW DiCamera(name, this);
        mCameras.insert(CameraList::value_type(name, c));

        return c;
    }

    DiCamera* DiSceneManager::GetCamera( const DiString& name ) const
    {
        auto i = mCameras.find(name);
        if (i == mCameras.end())
        {
            DI_WARNING("Cannot find the camera:%s", name.c_str());
            return nullptr;
        }
        else
        {
            return i->second;
        }
    }

    bool DiSceneManager::HasCamera( const DiString& name ) const
    {
        return (mCameras.find(name) != mCameras.end());
    }

    void DiSceneManager::DestroyCamera( DiCamera *cam )
    {
        DestroyCamera(cam->GetName());
    }

    void DiSceneManager::DestroyCamera( const DiString& name )
    {
        DI_INFO("Destroying camera : %s", name.c_str());

        auto i = mCameras.find(name);
        if (i != mCameras.end())
        {
            DI_DELETE i->second;
            mCameras.erase(i);
        }
    }

    void DiSceneManager::DestroyAllCameras( void )
    {
        auto camIt = mCameras.begin();
        while( camIt != mCameras.end() )
        {
            DestroyCamera(camIt->second);
            camIt = mCameras.begin();
        }
    }

    void DiSceneManager::AttachObject(DiTransUnitPtr obj)
    {
        mRootNode->AttachObject(obj);
    }

    void DiSceneManager::DetachObject(DiTransUnitPtr obj)
    {
        mRootNode->DetachObject(obj);
    }

    void DiSceneManager::AddExtraRenderTarget(DiRenderTarget* rt, DiCamera* camera,
        const RenderTargetParam::RTListener& preListener,
        const RenderTargetParam::RTListener& postListener)
    {
        RenderTargetParam p;
        p.rt = rt;
        p.camera = camera;
        p.preUpdateCallback = preListener;
        p.postUpdateCallback = postListener;
        mExtraRTs.push_back(p);
    }

    void DiSceneManager::PushVisibleObject(DiTransUnitPtr unit)
    {
        if(mCurrentRenderPass == GEOMETRY_PASS )
            mMainVisibleObjects.objs.push_back(unit);
        else
            mVisibleObjects.objs.push_back(unit);
    }

    int     DiSceneManager::intersect_call         = 0;
    uint32  DiSceneManager::ENTITY_TYPE_MASK       = 0x80000000;
    uint32  DiSceneManager::FX_TYPE_MASK           = 0x40000000;
    uint32  DiSceneManager::LIGHT_TYPE_MASK        = 0x20000000;
    uint32  DiSceneManager::FRUSTUM_TYPE_MASK      = 0x10000000;
    uint32  DiSceneManager::USER_TYPE_MASK_LIMIT   = 0x10000000;
}