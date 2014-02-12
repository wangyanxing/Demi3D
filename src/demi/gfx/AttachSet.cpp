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

#include "AttachSet.h"
#include "Skeleton.h"

namespace Demi
{
    DiAttachSet::DiAttachSet()
    {
    }

    DiAttachSet::~DiAttachSet()
    {
        AttachNodeMap::iterator itAttach;
        DiAttachNode * pkAttachNode = NULL;
        for (itAttach = mMapAttachNodes.begin(); itAttach != mMapAttachNodes.end(); ++ itAttach)
        {
            pkAttachNode = itAttach->second;

            if(pkAttachNode)
            {
                DI_DELETE pkAttachNode;
            }
        }
        mMapAttachNodes.clear();
        mVecRootAttaches.clear();
    }

    void DiAttachSet::Reset()
    {
        AttachNodeMap::iterator itAttach;
        for (itAttach = mMapAttachNodes.begin(); itAttach != mMapAttachNodes.end(); ++ itAttach)
        {
            (*itAttach).second->Reset();
        }
    }

    DiAttachNode* DiAttachSet::CreateAttachNode(const DiString& strAttach)
    {
        if(HasAttachNode(strAttach))
        {
            DI_WARNING("Cannot locate the attach node: %s", strAttach.c_str());
            return nullptr;
        }

        DiAttachNode * newnode = new DiAttachNode(strAttach);
        DI_ASSERT(newnode);

        mMapAttachNodes[strAttach] = newnode;

        return newnode;
    }

    DiAttachNode* DiAttachSet::GetAttachNode(const DiString& strAttach)
    {
        auto itAttach = mMapAttachNodes.find(strAttach);
        if(itAttach == mMapAttachNodes.end())
        {
            DI_WARNING("Cannot locate the attach node: %s",strAttach.c_str());
            return nullptr;
        }

        return itAttach->second;
    }

    const DiAttachNode* DiAttachSet::GetAttachNode(const DiString& strAttach) const
    {
        auto itAttach = mMapAttachNodes.find(strAttach);
        if(itAttach == mMapAttachNodes.end())
        {
            DI_WARNING("Cannot locate the attach node: %s",strAttach.c_str());
            return nullptr;
        }

        return itAttach->second;
    }

    DiAttachSet::RootAttachListIterator DiAttachSet::GetRootAttechesIterator(void)
    {
        if (mVecRootAttaches.empty())
        {
            DeriveRootAttaches();
        }

        return RootAttachListIterator(mVecRootAttaches.begin(),mVecRootAttaches.end());
    }

    void DiAttachSet::UpdateTransforms(void)
    {
        RootAttachList::iterator itAttach, itEnd;
        itEnd = mVecRootAttaches.end();

        for (itAttach = mVecRootAttaches.begin(); itAttach != itEnd; ++ itAttach)
        {
            (*itAttach)->_Update(true, false);
        }
    }

    void DiAttachSet::SetBindingPose(void)
    {
        UpdateTransforms();

        AttachNodeMap::iterator itAttach;
        for (itAttach = mMapAttachNodes.begin(); itAttach != mMapAttachNodes.end(); ++ itAttach )
        {            
            itAttach->second->SetBindingPose();
        }
    }

    void DiAttachSet::GetAttachMatrices(Demi::DiMat4 * pMatrices,size_t unArrayLen)
    {
        UpdateTransforms();
        
        size_t unCount = 0;
        for (auto itAttach = mMapAttachNodes.begin(); itAttach != mMapAttachNodes.end()
             && unCount < unArrayLen; ++ itAttach)
        {
            DiAttachNode * pkAttach = itAttach->second;
            pkAttach->GetOffsetTransform(* pMatrices);
            ++pMatrices;
            ++unCount;
        }
    }

    bool DiAttachSet::HasAttachNode(const DiString& strAttach)
    {
        return mMapAttachNodes.find(strAttach) != mMapAttachNodes.end();
    }

    void DiAttachSet::DeriveRootAttaches() const
    {
        mVecRootAttaches.clear();
        for (auto itAttach = mMapAttachNodes.begin(); itAttach != mMapAttachNodes.end(); ++ itAttach)
        {
            DiAttachNode * nd = itAttach->second;
            if (nd && (!nd->GetParent() ||
                       (nd->GetParent() && DiNode::NT_ATTACHNODE != nd->GetParent()->GetNodeType())))
            {
                mVecRootAttaches.push_back(nd);
            }
        }
    }

    DiAttachSetInstance::DiAttachSetInstance(DiAttachSet* pkTemplet,DiSkeletonInstance * pSkeleton)
        : mTemplet(pkTemplet),
        mSkeleton(pSkeleton)
    {
        Init();
    }

    DiAttachSetInstance::~DiAttachSetInstance()
    {

    }

    void DiAttachSetInstance::Init()
    {
        if(!mTemplet)
        {
            return;
        }

        RootAttachListIterator itRootAttach = mTemplet->GetRootAttechesIterator();
        while (itRootAttach.HasMoreElements())
        {
            DiAttachNode * nd = itRootAttach.GetNext();
            if(mSkeleton)
            {
                DiString strParent;
                DiNode * parent = nullptr;
                if(nd->GetParent())
                {
                    strParent = nd->GetParent()->GetName();
                    parent = mSkeleton->GetBone(strParent);
                }
                CloneAttachNodes( nd , parent);
            }
            else
            {
                CloneAttachNodes(nd , nullptr);
            }
            nd->_Update(true,false);
        }

        SetBindingPose();
    }

    void DiAttachSetInstance::CloneAttachNodes(DiAttachNode * pkSource, DiNode * pkParentNode)
    {
        DiAttachNode* newAttach = nullptr;

        newAttach = CreateAttachNode(pkSource->GetName());

        if(!pkParentNode)
        {
            mVecRootAttaches.push_back(newAttach);
        }
        else if(DiNode::NT_ATTACHNODE != pkParentNode->GetNodeType())
        {
            pkParentNode->AddChild(newAttach);
            mVecRootAttaches.push_back(newAttach);
        }
        else
        {
            pkParentNode->AddChild(newAttach);
        }

        newAttach->SetOrientation(pkSource->GetOrientation());
        newAttach->SetPosition(pkSource->GetPosition());
        newAttach->SetScale(pkSource->GetScale());

        size_t childrenSize = pkSource->GetChildrenNum();
        for (size_t i = 0; i < childrenSize; ++i)
        {
            CloneAttachNodes(static_cast<DiAttachNode*>(pkSource->GetChild(i)), newAttach);
        }
    }
}