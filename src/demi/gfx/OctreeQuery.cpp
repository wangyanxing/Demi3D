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
#include "OctreeQuery.h"
#include "CullNode.h"
#include "TransformUnit.h"

namespace Demi
{
#if 0
    DiOcAABBQuery::DiOcAABBQuery(DiSceneManager* creator)
        : DiAABBQuery(creator)
    {
    }
    
    DiOcAABBQuery::~DiOcAABBQuery()
    {}

    void DiOcAABBQuery::Execute(SceneQueryListener* listener)
    {
        DiList<DiCullNode*> _list;
        
        mParentSceneMgr->FindNodesIn( mAABB, _list, 0 );

        DiList<DiCullNode*>::iterator it = _list.begin();
        while( it != _list.end() )
        {
            DiCullNode::ObjectIterator oit = (*it)->GetAttachedObjectIterator();
            while( oit.HasMoreElements() )
            {
                DiTransUnitPtr m = oit.GetNext();
                if( (m->GetQueryFlags() & mQueryMask) && 
                    //(m->GetTypeFlags() & m_uiQueryTypeMask) && 
                    mAABB.Intersects( m->GetWorldBoundingBox() ) )
                {
                    listener->QueryResult( m );
                }
            }
            ++it;
        }
    }
    
    DiOcRayQuery::
        DiOcRayQuery(DiSceneManager* creator) : DiRaySceneQuery(creator)
    {
    }
    
    DiOcRayQuery::~DiOcRayQuery()
    {}
    
    void DiOcRayQuery::Execute(RaySceneQueryListener* listener)
    {
        DiList< DiCullNode * > _list;
        mParentSceneMgr->FindNodesIn( mRay, _list, 0 );

        DiList< DiCullNode * >::iterator it = _list.begin();
        while( it != _list.end() )
        {
            DiCullNode::ObjectIterator oit = (*it)->GetAttachedObjectIterator();
            while( oit.HasMoreElements() )
            {
                DiTransUnitPtr m = oit.GetNext();
                if( (m->GetQueryFlags() & mQueryMask) //&& 
                    /*(m->GetTypeFlags() & m_uiQueryTypeMask) */)
                {
                    std::pair<bool, float> result = mRay.intersects(m->GetWorldBoundingBox());

                    if( result.first )
                    {
                        listener->QueryResult( m, result.second );
                    }
                }
            }

            ++it;
        }

    }

    DiOcSphereQuery::DiOcSphereQuery(DiSceneManager* creator) : DiSphereSceneQuery(creator)
    {
    }
    
    DiOcSphereQuery::~DiOcSphereQuery()
    {}
    
    void DiOcSphereQuery::Execute(SceneQueryListener* listener)
    {
        DiList< DiCullNode * > _list;
        mParentSceneMgr->FindNodesIn( mSphere, _list, 0 );

        DiList< DiCullNode * >::iterator it = _list.begin();
        while( it != _list.end() )
        {
            DiCullNode::ObjectIterator oit = (*it) -> GetAttachedObjectIterator();
            while( oit.HasMoreElements() )
            {
                DiTransUnitPtr m = oit.GetNext();
                if( (m->GetQueryFlags() & mQueryMask) && 
                    //(m->GetTypeFlags() & m_uiQueryTypeMask) && 
                    mSphere.intersects( m->GetWorldBoundingBox() ) )
                {
                    listener->QueryResult( m );
                }
            }

            ++it;
        }
    }
    
    DiOcPBVQuery::
        DiOcPBVQuery(DiSceneManager* creator)
        : DiPBVListSceneQuery(creator)
    {

    }
    
    DiOcPBVQuery::~DiOcPBVQuery()
    {}
    
    void DiOcPBVQuery::Execute(SceneQueryListener* listener)
    {
        DiSet<DiCullNode*> checkedSceneNodes;

        PlaneBoundedVolumeList::iterator pi, piend;
        piend = mVolumes.end();
        for (pi = mVolumes.begin(); pi != piend; ++pi)
        {
            DiList<DiCullNode*> _list;
            mParentSceneMgr->FindNodesIn( *pi, _list, 0 );

            DiList<DiCullNode*>::iterator it, itend;
            itend = _list.end();
            for (it = _list.begin(); it != itend; ++it)
            {
                if (!checkedSceneNodes.insert(*it).second)
                {
                    continue;
                }
                DiCullNode::ObjectIterator oit = (*it)->GetAttachedObjectIterator();
                while( oit.HasMoreElements() )
                {
                    DiTransUnitPtr m = oit.GetNext();
                    if( (m->GetQueryFlags() & mQueryMask) && 
                        //(m->GetTypeFlags() & m_uiQueryTypeMask) && 
                        (*pi).intersects( m->GetWorldBoundingBox() ) )
                    {
                        listener->QueryResult( m );
                    }
                }
            }
        }
    }
#endif
}