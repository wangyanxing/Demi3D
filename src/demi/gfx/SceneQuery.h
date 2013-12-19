
/********************************************************************
    File:       SceneQuery.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "SceneManager.h"
#include "TransformUnit.h"

namespace Demi
{
    class SceneQueryListener;
    
    class DI_GFX_API DiSceneQuery
    {
    public:
 
        enum WorldFragmentType 
        {
            WFT_NONE,
            WFT_PLANE_BOUNDED_REGION,
            WFT_SINGLE_INTERSECTION,
            WFT_CUSTOM_GEOMETRY,
            WFT_RENDER_OPERATION
        };

        struct WorldFragment 
        {
            WorldFragmentType   fragmentType;
            DiVec3              singleIntersection;
            DiList<DiPlane>*    planes;
            void*               geometry;
        };
    protected:

        DiSceneManager*           mParentSceneMgr;

        uint32                    mQueryMask;

        uint32                    mQueryTypeMask;

        DiSet<WorldFragmentType>  mSupportedWorldFragments;
        
        WorldFragmentType         mWorldFragmentType;
    
    public:
        DiSceneQuery(DiSceneManager* mgr);

        virtual                         ~DiSceneQuery();

        virtual void                    SetQueryMask(uint32 mask);

        virtual uint32                  GetQueryMask(void) const;

        virtual void                    SetQueryTypeMask(uint32 mask);

        virtual uint32                  GetQueryTypeMask(void) const;

        virtual void                    SetWorldFragmentType(enum WorldFragmentType wft);

        virtual WorldFragmentType       GetWorldFragmentType(void) const;

        virtual const DiSet<WorldFragmentType>* GetSupportedWorldFragmentTypes(void) const
            {return &mSupportedWorldFragments;}
    };

    class DI_GFX_API SceneQueryListener
    {
    public:
        virtual                          ~SceneQueryListener() { }
        
        virtual bool                     QueryResult(DiTransUnitPtr object) = 0;
      
        virtual bool                     QueryResult(DiSceneQuery::WorldFragment* fragment) = 0;

    };

    typedef DiList<DiTransUnitPtr> SceneQueryResultMovableList;
    typedef DiList<DiSceneQuery::WorldFragment*> SceneQueryResultWorldFragmentList;
    struct DI_GFX_API SceneQueryResult 
    {
        SceneQueryResultMovableList       movables;
        SceneQueryResultWorldFragmentList worldFragments;
    };

    class DI_GFX_API DiRegionSceneQuery
        : public DiSceneQuery, public SceneQueryListener
    {
    protected:
        SceneQueryResult*                mLastResult;
    public:
        
        DiRegionSceneQuery(DiSceneManager* mgr);

        virtual                         ~DiRegionSceneQuery();

        virtual SceneQueryResult&       Execute(void);

        virtual void                    Execute(SceneQueryListener* listener) = 0;

        virtual SceneQueryResult&       GetLastResults(void) const;

        virtual void                    ClearResults(void);

        bool                            QueryResult(DiTransUnitPtr first);

        bool                            QueryResult(DiSceneQuery::WorldFragment* fragment);
    };

    class DI_GFX_API DiAABBQuery : public DiRegionSceneQuery
    {
    protected:
        DiAABB                          mAABB;
    public:
        DiAABBQuery(DiSceneManager* mgr);

        virtual                         ~DiAABBQuery();

        void                            SetBox(const DiAABB& box);

        const DiAABB&                   GetBox(void) const;

    };

    class DI_GFX_API DiSphereSceneQuery : public DiRegionSceneQuery
    {
    protected:
        DiSphere                        mSphere;

    public:
        DiSphereSceneQuery(DiSceneManager* mgr);
        
        virtual                         ~DiSphereSceneQuery();

        void                            SetSphere(const DiSphere& sphere);

        const DiSphere&                 GetSphere() const;

    };

    class DI_GFX_API DiPBVListSceneQuery : public DiRegionSceneQuery
    {
    protected:
        PlaneBoundedVolumeList            mVolumes;
    public:
        DiPBVListSceneQuery(DiSceneManager* mgr);
        
        virtual                         ~DiPBVListSceneQuery();

        void                            SetVolumes(const PlaneBoundedVolumeList& volumes);

        const PlaneBoundedVolumeList&   GetVolumes() const;

    };

    class DI_GFX_API RaySceneQueryListener 
    {
    public:
        virtual                          ~RaySceneQueryListener() { }

        virtual bool                     QueryResult(DiTransUnitPtr obj, float distance) = 0;

        virtual bool                     QueryResult(DiSceneQuery::WorldFragment* fragment, float distance) = 0;

    };
      
    struct DI_GFX_API RaySceneQueryResultEntry
    {
        float                           distance;

        DiTransUnitPtr                  movable;

        DiSceneQuery::WorldFragment*    worldFragment;

        bool operator < (const RaySceneQueryResultEntry& rhs) const
        {
            return this->distance < rhs.distance;
        }

    };
    typedef DiVector<RaySceneQueryResultEntry> RaySceneQueryResult;

    class DI_GFX_API DiRaySceneQuery : public DiSceneQuery, public RaySceneQueryListener
    {
    protected:
        DiRay                           mRay;

        bool                            mSortByDistance;
        
        uint16                          mMaxResults;
        
        RaySceneQueryResult             mResult;

    public:
        DiRaySceneQuery(DiSceneManager* mgr);
        
        virtual                         ~DiRaySceneQuery();
        
        virtual void                    SetRay(const DiRay& ray);

        virtual const DiRay&            GetRay(void) const;
      
        virtual void                    SetSortByDistance(bool sort, uint16 maxresults = 0);
        
        virtual bool                    GetSortByDistance(void) const;
        
        virtual uint16                  GetMaxResults(void) const;
      
        virtual RaySceneQueryResult&    Execute(void);

        virtual void                    Execute(RaySceneQueryListener* listener) = 0;

        virtual RaySceneQueryResult&    GetLastResults(void);
       
        virtual void                    ClearResults(void);

        bool                            QueryResult(DiTransUnitPtr obj, float distance);

        bool                            QueryResult(DiSceneQuery::WorldFragment* fragment, float distance);
    };

    class DI_GFX_API IntersectionSceneQueryListener 
    {
    public:
        virtual                         ~IntersectionSceneQueryListener() { }
      
        virtual bool                    QueryResult(DiTransUnitPtr first, DiTransUnitPtr second) = 0;

        virtual bool                    QueryResult(DiTransUnitPtr movable, DiSceneQuery::WorldFragment* fragment) = 0;
    };
        
    typedef DiPair<DiTransUnitPtr, DiTransUnitPtr> SceneQueryMovableObjectPair;
    typedef DiPair<DiTransUnitPtr, DiSceneQuery::WorldFragment*> SceneQueryMovableObjectWorldFragmentPair;
    typedef DiList<SceneQueryMovableObjectPair> SceneQueryMovableIntersectionList;
    typedef DiList<SceneQueryMovableObjectWorldFragmentPair> SceneQueryMovableWorldFragmentIntersectionList;
    
    struct DI_GFX_API DiIntersectionSceneQueryResult 
    {
        SceneQueryMovableIntersectionList                movables2movables;

        SceneQueryMovableWorldFragmentIntersectionList   movables2world;
    };

    class DI_GFX_API DiIntersectionSceneQuery
        : public DiSceneQuery, public IntersectionSceneQueryListener 
    {
    protected:
        DiIntersectionSceneQueryResult*          mLastResult;

    public:
        DiIntersectionSceneQuery(DiSceneManager* mgr);

        virtual                                  ~DiIntersectionSceneQuery();

        virtual DiIntersectionSceneQueryResult&  Execute(void);

        virtual void                             Execute(IntersectionSceneQueryListener* listener) = 0;

        virtual DiIntersectionSceneQueryResult&  GetLastResults(void) const;
       
        virtual void                             ClearResults(void);

        bool                                     QueryResult(DiTransUnitPtr first, DiTransUnitPtr second);
        
        bool                                     QueryResult(DiTransUnitPtr movable, DiSceneQuery::WorldFragment* fragment);
    };
}