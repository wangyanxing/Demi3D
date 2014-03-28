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
#include "OctreeCuller.h"
#include "CullNode.h"
#include "SceneManager.h"
#include "TransformUnit.h"

namespace Demi
{
    DiOctreeCuller::DiOctreeCuller(DiSceneManager* sm)
        :DiSceneCuller(sm)
    {
        mOctree = make_shared<DiOctree>( nullptr, nullptr, 0xffff );
        mMaxDepth = 8;
        
        mOctree->mBox = sm->GetBoundingBox();
        DiVec3 min = mOctree->mBox.GetMinimum();
        DiVec3 max = mOctree->mBox.GetMaximum();
        mOctree->mHalfSize = ( max - min ) / 2;
    }
    
    DiOctreeCuller::~DiOctreeCuller()
    {
    }
    
    void DiOctreeCuller::UpdateUnit(DiCullUnitPtr unit)
    {
        DiOctreeCullUnitPtr ocCullUnit = std::static_pointer_cast<DiOctreeCullUnit>(unit);
        DiCullNode* onode = unit->mParent;
        const DiAABB& box = onode->GetWorldAABB();
        
        if (box.IsNull())
            return;
        
        if (!mOctree)
            return;
        
        if (!ocCullUnit->GetOctant())
        {
            if (!onode->IsIn(mOctree->mBox ))
                mOctree->AddNode( ocCullUnit );
            else
                AddUnit(unit, mOctree);
            return;
        }
        
        if (!onode->IsIn(ocCullUnit->GetOctant()->mBox))
        {
            RemoveUnit(unit);
            
            if (!onode->IsIn(mOctree->mBox))
                mOctree->AddNode( ocCullUnit );
            else
                AddUnit(unit, mOctree);
        }
    }
    
    void DiOctreeCuller::RemoveUnit(DiCullUnitPtr unit)
    {
        if (!mOctree)
            return;
        
        auto ocCullUnit = std::static_pointer_cast<DiOctreeCullUnit>(unit);
        
        DiOctreePtr oct = ocCullUnit->GetOctant();
        
        if (oct)
            oct->RemoveNode(ocCullUnit);
        
        ocCullUnit->SetOctant(nullptr);
    }
    
    void DiOctreeCuller::AddUnit(DiCullUnitPtr n, DiOctreePtr octant, int depth)
    {
        if (!mOctree)
            return;
        
        const DiAABB& bx = n->mParent->GetWorldAABB();
        
        if ( ( depth < mMaxDepth ) && octant->IsTwiceSize( bx ) )
        {
            int id = octant->GetChildIndexes( bx );
            
            if (!octant->mChildren[id])
            {
                octant->mChildren[id] = make_shared<DiOctree>(octant, mOctree, id);
                const DiVec3& octantMin = octant->mBox.GetMinimum();
                const DiVec3& octantMax = octant->mBox.GetMaximum();
                
                DiVec3 min = octantMin;
                DiVec3 max = octantMax;
                if (id & 1)
                    min.x = (octantMin.x + octantMax.x) / 2;
                else
                    max.x = (octantMin.x + octantMax.x) / 2;
                
                if (id & 2)
                    min.y = (octantMin.y + octantMax.y) / 2;
                else
                    max.y = (octantMin.y + octantMax.y) / 2;
                
                if (id & 4)
                    min.z = (octantMin.z + octantMax.z) / 2;
                else
                    max.z = (octantMin.z + octantMax.z) / 2;
                
                octant->mChildren[id]->mBox.SetExtents( min, max );
                octant->mChildren[id]->mHalfSize = ( max - min ) / 2;
            }
            AddUnit( n, octant->mChildren[id], ++depth );
        }
        else
        {
            octant->AddNode( std::static_pointer_cast<DiOctreeCullUnit>(n) );
        }
    }
    
    void DiOctreeCuller::Cull(DiCamera* cam)
    {
        WalkTree(cam, mOctree, false);
    }
    
    void DiOctreeCuller::WalkTree(DiCamera* camera, DiOctreePtr octant, bool foundvisible)
    {
        if (octant->NumNodes() == 0)
            return;
        
        DiCamera::Visibility v = DiCamera::NONE;
        
        if ( foundvisible )
            v = DiCamera::FULL;
        else if ( octant == mOctree )
            v = DiCamera::PARTIAL;
        else
        {
            DiAABB box;
            octant->GetCullBounds( &box );
            v = camera->GetVisibility( box );
        }
        
        bool onlyShadowCaster = mSceneManager->GetCurrentPass() == SHADOW_PASS;
        if ( v != DiCamera::NONE )
        {
            bool vis = true;
            DiOctreeCullUnitPtr nodeUnit = octant->mNodes.mHead;
            for ( ;nodeUnit; nodeUnit = nodeUnit->mNext)
            {
                const DiAABB& nodeBounds = nodeUnit->mParent->GetWorldAABB();
                
                if (v == DiCamera::PARTIAL)
                    vis = camera->IsVisible(nodeBounds);
                
                if (vis)
                {
                    nodeUnit->mParent->SetCulled(false);
                    nodeUnit->mParent->ProcessVisibleObjects([=](DiTransUnitPtr tu)
                    {
                        if (!onlyShadowCaster || tu->GetShadowCastEnable())
                        {
                            mSceneManager->PushVisibleObject(tu);
                            tu->NotifyCurrentCamera(camera);
                        }
                    });
                }
            }
        }
        
        DiOctreePtr child;
        bool childfoundvisible = (v == DiCamera::FULL);
        
        for (uint16 i = 0; i < 8; i++)
            if ((child = octant->mChildren[i]))
                WalkTree(camera, child, childfoundvisible);

    }
    
    DiCullUnitPtr DiOctreeCuller::CreateUnit(DiCullNode* node)
    {
        return DiCullUnitPtr(DI_NEW DiOctreeCullUnit(node, this));
    }

    void DiOctreeCuller::LoadScene(const DiString& scene)
    {
        //TODO
    }

    DiOctreeCullUnit::DiOctreeCullUnit(DiCullNode* parentNode, DiSceneCuller* culler)
        :DiCullUnit(parentNode, culler)
    {
    }
    
    DiOctreeCullUnit::~DiOctreeCullUnit()
    {
    }
    
    void DiOctreeCullUnit::Update()
    {
        mCuller->UpdateUnit(shared_from_this());
    }
    
    bool DiOctreeCullUnit::IsIn(const DiAABB& box)
    {
        if (box.IsNull())
            return false;
        
        if (box.IsInfinite())
            return true;
        
        const DiAABB& worldAABB = mParent->GetWorldAABB();
        DiVec3 center = worldAABB.GetMaximum().midPoint( worldAABB.GetMinimum() );
        
        DiVec3 bmin = box.GetMinimum();
        DiVec3 bmax = box.GetMaximum();
        
        bool centre = ( bmax > center && bmin < center );
        if (!centre)
            return false;
        
        DiVec3 octreeSize = bmax - bmin;
        DiVec3 nodeSize = worldAABB.GetMaximum() - worldAABB.GetMinimum();
        return nodeSize < octreeSize;
    }
}