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

#ifndef DiSceneManager_h__
#define DiSceneManager_h__

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SkyLight.h"
#include "Scene.h"
#include "Octree.h"

namespace Demi 
{
    /** Visibility culling results
     */
    struct DiVisibleObjs
    {
        typedef DiVector<DiTransUnitPtr> VisibleObjs;
        VisibleObjs  objs;

        /** Update all transform units
         */
        void    UpdateAll(DiCamera* camera);

        /** Add to the final pipeline
         */
        void    AddToBatch(DiRenderPipeline* bg);
    };

    /** Visible lights
     */
    struct DiVisibleLights
    {
        DiVector<DiDirLight*>   dirLights;
        DiVector<DiPointLight*> pointLights;
        DiSkyLight*             skyLight;

        /** Clear all visible lights
            Called before the culling per frame
         */
        void Clear()
        {
            dirLights.clear();
            pointLights.clear();
            skyLight = nullptr;
        }
        
        /** Setup shadow cameras
         */
        void SetupShaodwCamera(DiSceneManager *sceneManager)
        {
            for(auto i = dirLights.begin(); i != dirLights.end(); ++i)
                (*i)->SetupShadowCamera(sceneManager);
            
            for(auto i = pointLights.begin(); i != pointLights.end(); ++i)
                (*i)->SetupShadowCamera(sceneManager);
            
            if(skyLight)
                skyLight->SetupShadowCamera(sceneManager);
        }
    };

    /** Render pass
     */
    enum RenderPass
    {
        /// Normal geometry pass
        GEOMETRY_PASS,
        
        /// Shadow mapping pass
        SHADOW_PASS,

        /// Other render-to-texture pass
        RTT_PASS,
    };

    /** Render target parameter struct
     */
    struct RenderTargetParam
    {
        typedef std::function<void(DiRenderTarget*)> RTListener;

        DiRenderTarget* rt;
        DiCamera* camera;

        RTListener preUpdateCallback;
        RTListener postUpdateCallback;
    };

    //////////////////////////////////////////////////////////////////////////

    /** Scene manager class
        Every render window holds a scene manager which is responsible for
        creating and destroying scene nodes, managing lights and skyboxs, etc.
     */
    class DI_GFX_API DiSceneManager : public DiBase
    {
    public:
        DiSceneManager(DiRenderWindow* parentWnd);
        
        virtual ~DiSceneManager(void);

        typedef DiSet<DiCullNode*> NodeList;

        friend class DiRenderPipeline;
        friend class DiOctree;

        typedef DiVector<DiDirLightPtr>     DirLightList;
        typedef DiVector<DiPointLightPtr>   PointLightList;
        typedef DiVector<RenderTargetParam> RenderTargets;

        static uint32     ENTITY_TYPE_MASK;
        static uint32     FX_TYPE_MASK;
        static uint32     LIGHT_TYPE_MASK;
        static uint32     FRUSTUM_TYPE_MASK;
        static uint32     USER_TYPE_MASK_LIMIT;

    public:

        DiCullNode*             CreateNode(void);

        DiCullNode*             CreateNode(const DiString& name);

        void                    DestroyNode(DiCullNode* node);

        uint32                  GetSceneNodeNums(void) const { return mSceneNodes.size(); }

        /** Attach an object to the root node
         */
        void                    AttachObject(DiTransUnitPtr obj);

        /** Detach an object to the root node
        */
        void                    DetachObject(DiTransUnitPtr obj);

        DiCullNode*             GetRootNode(void){ return mRootNode; }

        DiCamera*               CreateCamera(const DiString& name);

        DiCamera*               GetCamera(const DiString& name) const;

        bool                    HasCamera(const DiString& name) const;

        void                    DestroyCamera(DiCamera *cam);

        void                    DestroyCamera(const DiString& name);

        void                    DestroyAllCameras(void);

        void                    ClearNodes(void);

        DiCamera*               GetCamera(void) const { return mCamera; }

        const DiColor&          GetAmbientColor(void) const { return mAmbientColor; }

        /** Set the global ambient color
         */
        void                    SetAmbientColor(DiColor val) { mAmbientColor = val; }

        DiRenderWindow*         GetRenderWindow(){return mParentWindow;}

        void                    PreUpdate();

        void                    Cull(DiCamera* cam);

        void                    DirtyInstanceManager( DiInstanceManager *dirtyManager );

#if 0

        void                    FindNodesIn( const DiAABB &box, DiList<DiCullNode*>& list, DiCullNode *exclude = 0 );

        void                    FindNodesIn( const DiSphere &sphere, DiList<DiCullNode*>& list, DiCullNode *exclude = 0 );

        void                    FindNodesIn( const DiPlaneBoundedVol &volume, DiList<DiCullNode*>& list, DiCullNode *exclude = 0 );

        void                    FindNodesIn( const DiRay &ray, DiList<DiCullNode*>& list, DiCullNode *exclude = 0 );

        DiAABBQuery*            CreateAABBQuery(const DiAABB& box, unsigned long mask);
        
        DiSphereSceneQuery*     CreateSphereQuery(const DiSphere& sphere, unsigned long mask);
        
        DiPBVListSceneQuery*    CreatePlaneBoundedVolumeQuery(const PlaneBoundedVolumeList& volumes, unsigned long mask);

        DiRaySceneQuery*        CreateRayQuery(const DiRay& ray, unsigned long mask);
#endif

        /** Get current render pass
         */
        RenderPass              GetCurrentPass(void) const { return mCurrentRenderPass; }

        void                    SetCurrentPass(RenderPass pass) { mCurrentRenderPass = pass; }

        DiSkybox*               GetSkybox(){return mSkybox;}

        /** Get visible objects including lights
        */
        DiVisibleObjs&          GetVisibleObjects() { return mVisibleObjects; }

        DiVisibleObjs&          GetMainVisibleObjects() { return mMainVisibleObjects; }

        /** Push visible objects
         */
        void                    PushVisibleObject(DiTransUnitPtr unit);

        /** Get visible lights
         */
        DiVisibleLights&        GetVisibleLights() { return mVisibleLights; }
        
        /** Add extra render target
            will be rendered per frame
         */
        void                    AddExtraRenderTarget(DiRenderTarget* rt, DiCamera* camera,
                                    const RenderTargetParam::RTListener& preListener = nullptr,
                                    const RenderTargetParam::RTListener& postListener = nullptr);
        
        RenderTargets&          GetExtraRenderTargets() {return mExtraRTs;}

        /** Get the bounding box of the whole scene
         */
        const DiAABB&           GetBoundingBox() const { return mBox; }
        
        DiSceneCuller*          GetSceneCuller() const { return mCuller; }

    protected:

        void                    UpdateDirtyInstanceManagers();

    protected:

        DiCullNode*             mRootNode;

        NodeList                mSceneNodes;

        /// Main camera
        DiCamera*               mCamera;

        /// Visible lights, updated after the culling
        DiVisibleLights         mVisibleLights;
        
        RenderTargets           mExtraRTs;
        
        DiRenderPipeline*       mPipeline;

        DiColor                 mAmbientColor;

        DiRenderWindow*         mParentWindow;

        typedef DiVector<DiInstanceManager*> InstanceManagerVec;
        InstanceManagerVec      mDirtyInstanceManagers;

        InstanceManagerVec      mDirtyInstanceMgrsTmp;

        DiAABB                  mBox;

        typedef DiMap<DiString,DiCamera*> CameraList;
        CameraList              mCameras;

        RenderPass              mCurrentRenderPass;

        DiSkybox*               mSkybox;

        DiVisibleObjs           mMainVisibleObjects;

        /// visible objects for other kinds of passes
        DiVisibleObjs           mVisibleObjects;

        DiSceneCuller*          mCuller;
        
        DiSceneCullerFactory*   mCullerFactory;

    public:

        static int              intersect_call;
    };
}

#endif
