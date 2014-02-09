
/********************************************************************
    File:       SceneManager.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "Light.h"
#include "Scene.h"
#include "TerrainMap.h"
#include "Octree.h"
#include "SceneNode.h"

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

    struct DiVisibleObjs
    {
        typedef DiVector<DiTransUnitPtr> VisibleObjs;
        VisibleObjs  objs;

        void    UpdateAll(DiCamera* camera);

        void    AddToBatch(DiRenderPipeline* bg);
    };

    //////////////////////////////////////////////////////////////////////////

    class DiOctreeNode;

    class DI_GFX_API DiSceneManager : public DiBase
    {
    public:
        DiSceneManager(DiRenderWindow* parentWnd);
        
        virtual ~DiSceneManager(void);

        typedef DiMap<DiString, DiCullNode*> NodeList;

        friend class DiRenderPipeline;
        friend class DiOctree;

        typedef DiVector<DiDirLight*>    DirLightList;
        typedef DiVector<DiPointLight*>    PointLightList;

        static uint32     ENTITY_TYPE_MASK;
        static uint32     FX_TYPE_MASK;
        static uint32     LIGHT_TYPE_MASK;
        static uint32     FRUSTUM_TYPE_MASK;
        static uint32     USER_TYPE_MASK_LIMIT;

        enum IllumState
        {
            IRS_NONE,                
            IRS_RENDER_SHADOW_PASS,    
            IRS_RENDER_RTT_PASS,    
        };

    public:

        DiSceneNodePtr          GetRootSceneNode() { return mRootSceneNode; }

        // create a child node of root node
        DiCullNode*             CreateNode();

        DiCullNode*             CreateNode(const DiString& name);

        void                    DestroyNode(DiCullNode* node);

        void                    DestroyNode(const DiString& name);

        bool                    HasSceneNode(const DiString& name);

        DiCullNode*             GetSceneNode(const DiString& name);

        size_t                  GetSceneNodeNums()const {return mSceneNodes.size();}

        bool                    RenameSceneNode(const DiString& oldName, const DiString& newName);

        DiCullNode*             GetRootNode(){return mRootNode;}

        DiCamera*               CreateCamera(const DiString& name);

        DiCamera*               GetCamera(const DiString& name) const;

        bool                    HasCamera(const DiString& name) const;

        void                    DestroyCamera(DiCamera *cam);

        void                    DestroyCamera(const DiString& name);

        void                    DestroyAllCameras(void);

        DiDirLight*             CreateDirLight();

        size_t                  GetDirLightNum()const {return mDirLights.size();}
        
        DiDirLight*             GetDirLight(size_t id){return mDirLights[id];}

        void                    DestroyDirLight(DiDirLight* light);

        DiPointLight*           CreatePointLight();

        size_t                  GetPointLightNum()const {return mPointLights.size();}
        
        DiPointLight*           GetPointLight(size_t id){return mPointLights[id];}

        void                    DestroyPointLight(DiPointLight* light);

        DiSkyLight*             GetSkyLight() { return mSkyLight; }

        void                    DestroySkyLight();

        DiSkyLight*             CreateSkyLight();

        void                    Cull(DiCamera* camera,
                                    DiVisibleObjectsBoundsInfo* visibleBounds);

        void                    Cull(DiCamera* camera, DiRenderPipeline* pipeline);

        void                    ClearNodes();

        void                    ClearLights();

        DiCamera*               GetCamera() const { return mCamera; }

        DiTerrainPtr            GetTerrain() const { return mTerrain; }

        const DiColor&          GetAmbientColor() const { return mAmbientColor; }

        void                    SetAmbientColor(DiColor val) { mAmbientColor = val; }

        bool                    LoadScene(const DiString& sceneName);

        bool                    LoadTerrain();

        DiString                GetCurrentSceneName() const { return mCurrentScene; }

        DiScenePtr              GetCurrentScene();

        bool                    HasScene() const;

        void                    UnloadScene();

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

        IllumState              GetIllumState() const { return mIllumState; }

        const DiVisibleObjectsBoundsInfo& GetVisibleObjectsBoundsInfo(const DiCamera* cam) const;

        DiSkybox*               GetSkybox(){return mSkybox;}

        DiOctreeNode*           GetOctreeNode() { return mOctreeNode; }

        DiVisibleObjs&          GetVisibleObjects() { return mVisibleObjects; }

    protected:

        void                    UpdateDirtyInstanceManagers();

        void                    WalkTree(DiCamera* camera, DiVisibleObjectsBoundsInfo* visibleBounds, DiOctreePtr octree, bool foundvisible);

        void                    WalkTree(DiCamera* camera, DiRenderPipeline* pipeline, DiOctreeNode* octree, bool foundvisible);

    protected:

        DiSceneNodePtr          mRootSceneNode;

        DiCullNode*             mRootNode;

        NodeList                mSceneNodes;

        DiCamera*               mCamera;

        DirLightList            mDirLights;

        PointLightList          mPointLights;

        DiSkyLight*             mSkyLight;

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

        DiOctreeNode*           mOctreeNode;

        ReentryNodes            mReentryNodes;

        int                     mMaxDepth;

        DiAABB                  mBox;

        typedef DiMap<DiString,DiCamera*> CameraList;
        CameraList              mCameras;

        IllumState              mIllumState;
        
        unsigned long           mLastFrameNumber;

        typedef DiMap< const DiCamera*, DiVisibleObjectsBoundsInfo> CamVisibleObjectsMap;
        CamVisibleObjectsMap    mCamVisibleObjectsMap; 

        DiSkybox*               mSkybox;

        DiVisibleObjs           mVisibleObjects;

    public:

        static int              intersect_call;
    };

}