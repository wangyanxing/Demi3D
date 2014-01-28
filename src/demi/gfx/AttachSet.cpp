
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
        if(strAttach.empty())
        {
            DI_WARNING("绑点名不能为空！");
            return NULL;
        }

        if(HasAttachNode(strAttach))
        {
            DI_WARNING("已存在名为\"%s\"的绑点，添加失败！");
            return NULL;
        }

        DiAttachNode * pkNewNode = new DiAttachNode(strAttach);

        if(!pkNewNode)
        {
            DI_WARNING("绑点添加失败！");
            return NULL;
        }

        mMapAttachNodes[strAttach] = pkNewNode;

        return pkNewNode;
    }

    DiAttachNode* DiAttachSet::GetAttachNode(const DiString& strAttach)
    {
        if(strAttach.empty())
        {
            DI_WARNING("挂点名不能为空！");
            return NULL;
        }

        AttachNodeMap::iterator itAttach = mMapAttachNodes.find(strAttach);

        if(itAttach == mMapAttachNodes.end())
        {
            DI_WARNING("名为\"%s\"的挂点不存在！",strAttach.c_str());
            return NULL;
        }

        return itAttach->second;
    }

    const DiAttachNode* DiAttachSet::GetAttachNode(const DiString& strAttach) const
    {
        if(strAttach.empty())
        {
            DI_WARNING("挂点名不能为空！");
            return NULL;
        }

        AttachNodeMap::const_iterator itAttach = mMapAttachNodes.find(strAttach);

        if(itAttach == mMapAttachNodes.end())
        {
            DI_WARNING("名为\"%s\"的挂点不存在！",strAttach.c_str());
            return NULL;
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

        AttachNodeMap::const_iterator itAttach, itEnd;
        itEnd = mMapAttachNodes.end();
        size_t unCount = 0;
        for (itAttach = mMapAttachNodes.begin(); itAttach != itEnd && unCount < unArrayLen; ++ itAttach)
        {
            DiAttachNode * pkAttach = itAttach->second;
            pkAttach->GetOffsetTransform(* pMatrices);
            ++ pMatrices;
            ++ unCount;
        }
    }

    bool DiAttachSet::HasAttachNode(const DiString& strAttach)
    {
        return mMapAttachNodes.find(strAttach) != mMapAttachNodes.end();
    }

    void DiAttachSet::DeriveRootAttaches() const
    {
        if (mMapAttachNodes.empty())
        {
            DI_ERROR("绑点集为空！");
            return;
        }

        mVecRootAttaches.clear();

        AttachNodeMap::const_iterator itAttach;
        AttachNodeMap::const_iterator itEnd = mMapAttachNodes.end();
        for (itAttach = mMapAttachNodes.begin(); itAttach != itEnd; ++ itAttach)
        {
            DiAttachNode * pkCurNode = itAttach->second;
            if (pkCurNode && (!pkCurNode->GetParent() || pkCurNode->GetParent() && DiNode::NT_ATTACHNODE != pkCurNode->GetParent()->GetNodeType()))
            {
                mVecRootAttaches.push_back(pkCurNode);
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
            DiAttachNode * pkNode = itRootAttach.GetNext();
            if(mSkeleton)
            {
                DiString strParent;
                DiNode * pkParent = NULL;
                if(pkNode->GetParent())
                {
                    strParent = pkNode->GetParent()->GetName();
                    pkParent = mSkeleton->GetBone(strParent);
                }
                CloneAttachNodes( pkNode , pkParent);
            }
            else
            {
                CloneAttachNodes( pkNode , NULL);
            }
            pkNode->_Update(true,false);
        }

        SetBindingPose();
    }

    void DiAttachSetInstance::CloneAttachNodes(DiAttachNode * pkSource, DiNode * pkParentNode)
    {
        DiAttachNode* pkNewAttach = NULL;

        if (pkSource->GetName().empty())
        {
            DI_WARNING("不合法的绑点，不支持无名绑点");
            return;
        }

        pkNewAttach = CreateAttachNode(pkSource->GetName());

        if(!pkParentNode)
        {
            mVecRootAttaches.push_back(pkNewAttach);
        }
        else if(DiNode::NT_ATTACHNODE != pkParentNode->GetNodeType())
        {
            pkParentNode->AddChild(pkNewAttach);
            mVecRootAttaches.push_back(pkNewAttach);
        }
        else
        {
            pkParentNode->AddChild(pkNewAttach);
        }

        pkNewAttach->SetOrientation(pkSource->GetOrientation());
        pkNewAttach->SetPosition(pkSource->GetPosition());
        pkNewAttach->SetScale(pkSource->GetScale());

        size_t childrenSize = pkSource->GetChildrenNum();
        for (size_t i = 0; i < childrenSize; ++i)
        {
            CloneAttachNodes(static_cast<DiAttachNode*>(pkSource->GetChild(i)), pkNewAttach);
        }
    }
}