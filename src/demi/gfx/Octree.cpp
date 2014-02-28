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
#include "Octree.h"
#include "CullNode.h"
#include "SceneManager.h"
#include "OctreeCuller.h"

namespace Demi
{
    bool DiOctree::IsTwiceSize( const DiAABB &box ) const
    {
        if (box.IsInfinite())
            return false;

        DiVec3 halfMBoxSize = mBox.GetHalfSize();
        DiVec3 boxSize = box.GetSize();
        return ((boxSize.x <= halfMBoxSize.x) && 
            (boxSize.y <= halfMBoxSize.y) &&
            (boxSize.z <= halfMBoxSize.z));
    }

    uint16 DiOctree::GetChildIndexes( const DiAABB &box ) const
    {
        DiVec3 center = mBox.GetMaximum().midPoint( mBox.GetMinimum() );
        DiVec3 ncenter = box.GetMaximum().midPoint( box.GetMinimum() );

        int x = ncenter.x > center.x ? 1 : 0;
        int y = ncenter.y > center.y ? 2 : 0;
        int z = ncenter.z > center.z ? 4 : 0;
        return x + y + z;
    }

    DiOctree::DiOctree(DiOctreePtr parent, DiOctreePtr root, uint16 index)
        : mHalfSize( 0, 0, 0 ),
        mRoot(root),
        mIndex(0)
    {
        mParent = parent;
        mNumNodes = 0;
    }

    DiOctree::~DiOctree()
    {
        for (uint16 i = 0; i < 8; i++)
            mChildren[i].reset();

        mNumNodes = 0;
        mParent = nullptr;
    }

    void DiOctree::AddNode( DiOctreeCullUnitPtr n )
    {
        mNodes.push_back(n);
        n->SetOctant( shared_from_this() );

        IncreaseNodeCount();
    }

    void DiOctree::RemoveNode( DiOctreeCullUnitPtr n )
    {
        mNodes.erase(n);
        n->SetOctant( nullptr );

        DecreaseNodeCount();
    }

    void DiOctree::GetCullBounds( DiAABB* b ) const
    {
        b->SetExtents( mBox.GetMinimum() - mHalfSize,
            mBox.GetMaximum() + mHalfSize );
    }
}