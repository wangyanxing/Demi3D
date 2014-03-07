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
#include "Q3BspNode.h"
#include "DataStream.h"

namespace Demi
{
    DiBspNode::DiBspNode(DiBspScene* scene, bool isLeaf)
        :mParent(scene)
        ,mIsLeaf(isLeaf)
    {
    }
    
    DiBspNode::DiBspNode()
    {
    }
    
    DiBspNode::~DiBspNode()
    {
    }
    
    DiBspNode* DiBspNode::GetFront(void) const
    {
        return mIsLeaf ? nullptr : mFront;
    }
    
    DiBspNode* DiBspNode::GetBack(void) const
    {
        return mIsLeaf ? nullptr : mBack;
    }
    
    DiPlane::Side DiBspNode::GetSide (const DiVec3& point) const
    {
        DI_ASSERT(!mIsLeaf);
        return mSplitPlane.getSide(point);
    }

    const DiPlane& DiBspNode::GetSplitPlane(void) const
    {
        DI_ASSERT(!mIsLeaf);
        return mSplitPlane;
    }
    
    DiBspNode* DiBspNode::GetNextNode(const DiVec3& point) const
    {
        DI_ASSERT(!mIsLeaf);
        
        DiPlane::Side sd = GetSide(point);
        return sd == DiPlane::NEGATIVE_SIDE ? GetBack() : GetFront();
    }

    void DiBspNode::AddTransUnit(const DiTransUnitPtr tu)
    {
        mMovables.insert(tu);
    }
    
    void DiBspNode::RemoveTransUnit(const DiTransUnitPtr tu)
    {
        mMovables.insert(tu);
    }
    
    float DiBspNode::GetDistance(const DiVec3& pos) const
    {
        DI_ASSERT(!mIsLeaf);
        return mSplitPlane.getDistance(pos);
    }
    
    const DiBspNode::NodeBrushList& DiBspNode::GetSolidBrushes(void) const
    {
        return mSolidBrushes;
    }
}