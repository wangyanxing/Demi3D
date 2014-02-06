
#include "GfxPch.h"
#include "SceneQuery.h"

namespace Demi
{
     DiSceneQuery::DiSceneQuery(DiSceneManager* mgr)
        : mParentSceneMgr(mgr), 
        mQueryMask(0xFFFFFFFF), 
        mWorldFragmentType(DiSceneQuery::WFT_NONE)
    {
        mQueryTypeMask = (0xFFFFFFFF & ~DiSceneManager::FX_TYPE_MASK) 
            & ~DiSceneManager::LIGHT_TYPE_MASK;
    }

    DiSceneQuery::~DiSceneQuery()
    {
    }

    void DiSceneQuery::SetQueryMask(uint32 mask)
    {
        mQueryMask = mask;
    }

    uint32 DiSceneQuery::GetQueryMask(void) const
    {
        return mQueryMask;
    }

    void DiSceneQuery::SetQueryTypeMask(uint32 mask)
    {
        mQueryTypeMask = mask;
    }

    uint32 DiSceneQuery::GetQueryTypeMask(void) const
    {
        return mQueryTypeMask;
    }

    void DiSceneQuery::SetWorldFragmentType(enum DiSceneQuery::WorldFragmentType wft)
    {
        if (mSupportedWorldFragments.find(wft) == mSupportedWorldFragments.end())
        {
            DI_WARNING("The world fragment type is not supported: %d", wft);
        }
        mWorldFragmentType = wft;
    }

    DiSceneQuery::WorldFragmentType DiSceneQuery::GetWorldFragmentType(void) const
    {
        return mWorldFragmentType;
    }

    DiRegionSceneQuery::DiRegionSceneQuery(DiSceneManager* mgr)
        :DiSceneQuery(mgr), mLastResult(NULL)
    {
    }

    DiRegionSceneQuery::~DiRegionSceneQuery()
    {
        ClearResults();
    }

    SceneQueryResult& DiRegionSceneQuery::GetLastResults(void) const
    {
        DI_ASSERT(mLastResult);
        return *mLastResult;
    }

    void DiRegionSceneQuery::ClearResults(void)
    {
        if (mLastResult)
        {
            DI_DELETE mLastResult;
        }
        mLastResult = NULL;
    }

    SceneQueryResult& DiRegionSceneQuery::Execute(void)
    {
        ClearResults();
        mLastResult = DI_NEW SceneQueryResult();
        Execute(this);
        return *mLastResult;
    }

    bool DiRegionSceneQuery::QueryResult(DiTransUnitPtr obj)
    {
        mLastResult->movables.push_back(obj);
        return true;
    }

    bool DiRegionSceneQuery::QueryResult(DiSceneQuery::WorldFragment* fragment)
    {
        mLastResult->worldFragments.push_back(fragment);
        return true;
    }

    DiAABBQuery::DiAABBQuery(DiSceneManager* mgr)
        : DiRegionSceneQuery(mgr)
    {
    }

    DiAABBQuery::~DiAABBQuery()
    {
    }

    void DiAABBQuery::SetBox(const DiAABB& box)
    {
        mAABB = box;
    }

    const DiAABB& DiAABBQuery::GetBox(void) const
    {
        return mAABB;
    }

    DiSphereSceneQuery::DiSphereSceneQuery(DiSceneManager* mgr)
        : DiRegionSceneQuery(mgr)
    {
    }

    DiSphereSceneQuery::~DiSphereSceneQuery()
    {
    }

    void DiSphereSceneQuery::SetSphere(const DiSphere& sphere)
    {
        mSphere = sphere;
    }

    const DiSphere& DiSphereSceneQuery::GetSphere() const
    {
        return mSphere;
    }

    DiPBVListSceneQuery::DiPBVListSceneQuery(DiSceneManager* mgr)
        : DiRegionSceneQuery(mgr)
    {
    }

    DiPBVListSceneQuery::~DiPBVListSceneQuery()
    {
    }

    void DiPBVListSceneQuery::SetVolumes(const PlaneBoundedVolumeList& volumes)
    {
        mVolumes = volumes;
    }

    const PlaneBoundedVolumeList& DiPBVListSceneQuery::GetVolumes() const
    {
        return mVolumes;
    }

    DiRaySceneQuery::DiRaySceneQuery(DiSceneManager* mgr) : DiSceneQuery(mgr)
    {
        mSortByDistance = false;
        mMaxResults = 0;
    }

    DiRaySceneQuery::~DiRaySceneQuery()
    {
    }

    void DiRaySceneQuery::SetRay(const DiRay& ray)
    {
        mRay = ray;
    }

    const DiRay& DiRaySceneQuery::GetRay(void) const
    {
        return mRay;
    }

    void DiRaySceneQuery::SetSortByDistance(bool sort, uint16 maxresults)
    {
        mSortByDistance = sort;
        mMaxResults = maxresults;
    }

    bool DiRaySceneQuery::GetSortByDistance(void) const
    {
        return mSortByDistance;
    }

    uint16 DiRaySceneQuery::GetMaxResults(void) const
    {
        return mMaxResults;
    }

    RaySceneQueryResult& DiRaySceneQuery::Execute(void)
    {
        mResult.clear();
        
        this->Execute(this);

        if (mSortByDistance)
        {
            if (mMaxResults != 0 && mMaxResults < mResult.size())
            {
                std::partial_sort(mResult.begin(), mResult.begin()+mMaxResults, mResult.end());
                mResult.resize(mMaxResults);
            }
            else
            {
                std::sort(mResult.begin(), mResult.end());
            }
        }

        return mResult;
    }

    RaySceneQueryResult& DiRaySceneQuery::GetLastResults(void)
    {
        return mResult;
    }

    void DiRaySceneQuery::ClearResults(void)
    {
        RaySceneQueryResult().swap(mResult);
    }

    bool DiRaySceneQuery::QueryResult(DiTransUnitPtr obj, float distance)
    {
        RaySceneQueryResultEntry dets;
        dets.distance = distance;
        dets.movable = obj;
        dets.worldFragment = NULL;
        mResult.push_back(dets);
        return true;
    }

    bool DiRaySceneQuery::QueryResult(DiSceneQuery::WorldFragment* fragment, float distance)
    {
        RaySceneQueryResultEntry dets;
        dets.distance = distance;
        dets.movable = NULL;
        dets.worldFragment = fragment;
        mResult.push_back(dets);
        return true;
    }
 
    DiIntersectionSceneQuery::DiIntersectionSceneQuery(DiSceneManager* mgr)
    : DiSceneQuery(mgr), mLastResult(NULL)
    {
    }

    DiIntersectionSceneQuery::~DiIntersectionSceneQuery()
    {
        ClearResults();
    }

    DiIntersectionSceneQueryResult& DiIntersectionSceneQuery::GetLastResults(void) const
    {
        DI_ASSERT(mLastResult);
        return *mLastResult;
    }
    void DiIntersectionSceneQuery::ClearResults(void)
    {
        if (mLastResult)
        {
            DI_DELETE mLastResult;
        }
        mLastResult = NULL;
    }

    DiIntersectionSceneQueryResult& DiIntersectionSceneQuery::Execute(void)
    {
        ClearResults();
        mLastResult = DI_NEW DiIntersectionSceneQueryResult();
        Execute(this);
        return *mLastResult;
    }

    bool DiIntersectionSceneQuery::
        QueryResult(DiTransUnitPtr first, DiTransUnitPtr second)
    {
        mLastResult->movables2movables.push_back(
            SceneQueryMovableObjectPair(first, second)
            );
        return true;
    }

    bool DiIntersectionSceneQuery::
        QueryResult(DiTransUnitPtr movable, DiSceneQuery::WorldFragment* fragment)
    {
        mLastResult->movables2world.push_back(
            SceneQueryMovableObjectWorldFragmentPair(movable, fragment)
            );
        return true;
    }
}