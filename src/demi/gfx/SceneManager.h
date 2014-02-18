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
#include "TerrainMap.h"
#include "Octree.h"

namespace Demi 
{
    struct DI_GFX_API DiVisibleObjectsBoundsInfo
    {
        DiAABB aabb;
        
        DiAABB receiverAabb;
        
        float minDistance;
        
        float maxDistance;
        
        float minDistanceInFrustum;
        
        float maxDistanceInFrustum;

        DiVisibleObjectsBoundsInfo();

        void Reset();

        void Merge(const DiAABB& boxBounds, const DiSphere& sphereBounds, 
            const DiCamera* cam, bool receiver=true);
        
        void MergeNonRenderedButInFrustum(const DiAABB& boxBounds, 
            const DiSphere& sphereBounds, const DiCamera* cam);
    };

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
        void    Clear()
        {
            dirLights.clear();
            pointLights.clear();
            skyLight = nullptr;
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

        typedef DiMap<DiString, DiCullNode*> NodeList;

        friend class DiRenderPipeline;
        friend class DiOctree;

        typedef DiVector<DiDirLightPtr>    DirLightList;
        typedef DiVector<DiPointLightPtr>  PointLightList;
        typedef DiVector<DiPair<DiRenderTarget*, DiCamera*> > RenderTargets;

        static uint32     ENTITY_TYPE_MASK;
        static uint32     FX_TYPE_MASK;
        static uint32     LIGHT_TYPE_MASK;
        static uint32     FRUSTUM_TYPE_MASK;
        static uint32     USER_TYPE_MASK_LIMIT;

    public:

        DiCullNode*             CreateNode(void);

        DiCullNode*             CreateNode(const DiString& name);

        void                    DestroyNode(DiCullNode* node);

        void                    DestroyNode(const DiString& name);

        bool                    HasSceneNode(const DiString& name);

        DiCullNode*             GetSceneNode(const DiString& name);

        uint32                  GetSceneNodeNums(void) const { return mSceneNodes.size(); }

        bool                    RenameSceneNode(const DiString& oldName, const DiString& newName);

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

        void                    Cull(DiCamera* camera, DiVisibleObjectsBoundsInfo* visibleBounds);

        void                    ClearNodes(void);

        DiCamera*               GetCamera(void) const { return mCamera; }

        DiTerrainPtr            GetTerrain(void) const { return mTerrain; }

        const DiColor&          GetAmbientColor(void) const { return mAmbientColor; }

        /** Set the global ambient color
         */
        void                    SetAmbientColor(DiColor val) { mAmbientColor = val; }

        bool                    LoadScene(const DiString& sceneName);

        bool                    LoadTerrain(void);

        DiString                GetCurrentSceneName(void) const { return mCurrentScene; }

        DiScenePtr              GetCurrentScene(void);

        bool                    HasScene(void) const;

        void                    UnloadScene(void);

        void                    DestroyTerrain();

        DiRenderWindow*         GetRenderWindow(){return mParentWindow;}

        void                    PreUpdate();

        void                    Cull(DiCamera* cam);

        void                    DirtyInstanceManager( DiInstanceManager *dirtyManager );

        void                    UpdateOctreeNode( DiCullNode* );

        void                    RemoveOctreeNode( DiCullNode* );

        void                    AddOctreeNode(DiCullNode *, DiOctreePtr octree, int depth = 0);

        void                    FindNodesIn( const DiAABB &box, DiList<DiCullNode*>& list, DiCullNode *exclude = 0 );

        void                    FindNodesIn( const DiSphere &sphere, DiList<DiCullNode*>& list, DiCullNode *exclude = 0 );

        void                    FindNodesIn( const DiPlaneBoundedVol &volume, DiList<DiCullNode*>& list, DiCullNode *exclude = 0 );

        void                    FindNodesIn( const DiRay &ray, DiList<DiCullNode*>& list, DiCullNode *exclude = 0 );

        DiAABBQuery*            CreateAABBQuery(const DiAABB& box, unsigned long mask);
        
        DiSphereSceneQuery*     CreateSphereQuery(const DiSphere& sphere, unsigned long mask);
        
        DiPBVListSceneQuery*    CreatePlaneBoundedVolumeQuery(const PlaneBoundedVolumeList& volumes, unsigned long mask);

        DiRaySceneQuery*        CreateRayQuery(const DiRay& ray, unsigned long mask);

        /** Get current render pass
         */
        RenderPass              GetCurrentPass(void) const { return mCurrentRenderPass; }

        void                    SetCurrentPass(RenderPass pass) { mCurrentRenderPass = pass; }

        const DiVisibleObjectsBoundsInfo& GetVisibleObjectsBoundsInfo(const DiCamera* cam) const;

        DiSkybox*               GetSkybox(){return mSkybox;}

        /** Get visible objects including lights
        */
        DiVisibleObjs&          GetVisibleObjects() { return mVisibleObjects; }

        /** Get visible lights
         */
        DiVisibleLights&        GetVisibleLights() { return mVisibleLights; }
        
        /** Add extra render target
            will be rendered per frame
         */
        void                    AddExtraRenderTarget(DiRenderTarget* rt, DiCamera* camera)
        {
            mExtraRTs.push_back(DiPair<DiRenderTarget*, DiCamera*>(rt, camera));
        }
        
        RenderTargets&          GetExtraRenderTargets() {return mExtraRTs;}

        CameraPool&             GetCameraPool() { return mCameraPool; }

    protected:

        void                    UpdateDirtyInstanceManagers();

        void                    WalkTree(DiCamera* camera, DiVisibleObjectsBoundsInfo* visibleBounds, DiOctreePtr octree, bool foundvisible);

    protected:

        DiCullNode*             mRootNode;

        NodeList                mSceneNodes;

        /// Main camera
        DiCamera*               mCamera;

        /// Visible lights, updated after the culling
        DiVisibleLights         mVisibleLights;
        
        RenderTargets           mExtraRTs;
        
        DiTerrainPtr            mTerrain;

        DiRenderPipeline*       mPipeline;

        DiColor                 mAmbientColor;

        DiString                mCurrentScene;

        DiRenderWindow*         mParentWindow;

        typedef DiVector<DiInstanceManager*> InstanceManagerVec;
        InstanceManagerVec      mDirtyInstanceManagers;

        InstanceManagerVec      mDirtyInstanceMgrsTmp;

        typedef DiVector<DiCullNode*> ReentryNodes;

        DiOctreePtr             mOctree;

        int                     mMaxDepth;

        DiAABB                  mBox;

        typedef DiMap<DiString,DiCamera*> CameraList;
        CameraList              mCameras;

        RenderPass              mCurrentRenderPass;
        
        unsigned long           mLastFrameNumber;

        typedef DiMap< const DiCamera*, DiVisibleObjectsBoundsInfo> CamVisibleObjectsMap;
        CamVisibleObjectsMap    mCamVisibleObjectsMap; 

        DiSkybox*               mSkybox;

        /// visible objects, will be updated every frame
        DiVisibleObjs           mVisibleObjects;

        CameraPool              mCameraPool;

        /// Z range of the scene
        /// Should be updated per frame
        DiVec2                  mZRange;

    public:

        static int              intersect_call;
    };
}

#endif
