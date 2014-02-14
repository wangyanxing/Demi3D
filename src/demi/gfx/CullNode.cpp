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
#include "CullNode.h"
#include "TransformUnit.h"
#include "SceneManager.h"
#include "RenderPipeline.h"

namespace Demi
{
    DiCullNode::DiCullNode(DiSceneManager* sm):
        DiNode(),
        mCreator(sm),
        mOctant(NULL)
    {
    }

    DiCullNode::DiCullNode( DiSceneManager* sm,const DiString& name ):
        DiNode(name),
        mCreator(sm),
        mOctant(NULL)
    {
    }

    DiCullNode::~DiCullNode(void)
    {
        for ( auto itr = mObjectsByName.begin(); itr != mObjectsByName.end(); ++itr )
        {
            (*itr)->NotifyAttached((DiCullNode*)NULL);
        }
        mObjectsByName.clear();
    }

    void DiCullNode::UpdateBounds()
    {
        mWorldAABB.SetNull();
        mLocalAABB.SetNull();

        for ( auto itr = mObjectsByName.begin(); itr != mObjectsByName.end(); ++itr )
        {
            mWorldAABB.Merge((*itr)->GetWorldBoundingBox(true));
            mLocalAABB.Merge((*itr)->GetBoundingBox());
        }

        if (!mWorldAABB.IsNull())
            mCreator->UpdateOctreeNode( this );
    }

    void DiCullNode::AttachObject(DiTransUnitPtr obj)
    {
        if (obj->IsAttached())
        {
            DI_WARNING("The object has been attached");
            return;
        }

        obj->NotifyAttached(this);

        mObjectsByName.push_back(obj);

        NeedUpdate();
    }

    void DiCullNode::AttachSilently(DiTransUnitPtr obj)
    {
        mObjectsByName.push_back(obj);
        
        NeedUpdate();
    }

    uint32 DiCullNode::NumAttachedObjects( void ) const
    {
        return mObjectsByName.size();
    }

    DiTransUnitPtr DiCullNode::GetAttachedObject( uint32 index )
    {
        if (index < mObjectsByName.size())
        {
            return mObjectsByName[index];
        }
        else
        {
            DI_WARNING("Cannot find the attachment object by id : %d",index);
            return nullptr;
        }
    }

    DiTransUnitPtr DiCullNode::DetachObject( uint32 index )
    {
        if (index < mObjectsByName.size())
        {
            DiTransUnitPtr ret = mObjectsByName[index];
            if (ret->GetParentNode() == this)
            {
                ret->NotifyAttached((DiCullNode*)0);
            }
            
            ObjectMap::iterator i = mObjectsByName.begin() + index;
            mObjectsByName.erase(i);

            NeedUpdate();

            return ret;

        }
        else
        {
            DI_WARNING("Cannot detach object, invalid index");
            return nullptr;
        }
    }

    void DiCullNode::DetachObject(DiTransUnitPtr obj)
    {
        for (auto i = mObjectsByName.begin(); i != mObjectsByName.end(); ++i)
        {
            if (*i == obj)
            {
                mObjectsByName.erase(i);
                break;
            }
        }

        if (obj->GetParentNode() == this)
        {
            obj->NotifyAttached(nullptr);
        }

        NeedUpdate();
    }
    
    void DiCullNode::DetachAllObjects( void )
    {
        for ( auto itr = mObjectsByName.begin();
            itr != mObjectsByName.end(); ++itr )
        {
            if ((*itr)->GetParentNode() == this)
            {
                (*itr)->NotifyAttached(nullptr);
            }
        }
        mObjectsByName.clear();

        NeedUpdate();
    }

    void DiCullNode::ProcessVisibleObjects(std::function<void(DiTransUnitPtr)> func)
    {
        auto mit = mObjectsByName.begin();
        while (mit != mObjectsByName.end())
        {
            if ((*mit)->GetVisible())
                func((*mit));
            ++mit;
        }
    }

    void DiCullNode::ProcessBatchGroup( DiCamera* camera,DiRenderPipeline* pipeline, 
        bool onlyShadowCaster, DiVisibleObjectsBoundsInfo* visbleBounds)
    {
        auto mit = mObjectsByName.begin();
        while ( mit != mObjectsByName.end() )
        {
            DiTransUnitPtr mo = (*mit);

            if (mo->GetVisible())
            {
                DiRenderBatchGroup* gp = pipeline->GetBatchGroup(mo->GetBatchGroup());
                bool receiveShadows = gp->GetShadowEnable();

                if (!onlyShadowCaster || mo->GetShadowCastEnable())
                {
                    mo->NotifyCurrentCamera(camera);
                    mo->CullingUpdate(gp,camera);
                    if (visbleBounds)
                    {
                        visbleBounds->Merge(mo->GetWorldBoundingBox(true), 
                            mo->GetWorldBoundingSphere(this, true), camera, 
                            receiveShadows);
                    }
                }
                else if (onlyShadowCaster && !mo->GetShadowCastEnable() && 
                    receiveShadows)
                {
                    visbleBounds->MergeNonRenderedButInFrustum(mo->GetWorldBoundingBox(true), 
                        mo->GetWorldBoundingSphere(true), camera);
                }
            }
            ++mit;
        }
    }

    void DiCullNode::SetVisible( bool vis, bool cascade /*= true*/ )
    {
        for (auto oi = mObjectsByName.begin(); oi != mObjectsByName.end(); ++oi)
        {
            (*oi)->SetVisible(vis);
        }

        if (cascade)
        {
            for (size_t i = 0; i < mChildren.size(); ++i)
            {
                DiCullNode* sceneChild = static_cast<DiCullNode*>(mChildren[i]);
                sceneChild->SetVisible(vis, cascade);
            }
        }
    }

    void DiCullNode::FlipVisibility( bool cascade /*= true*/ )
    {
        for (auto oi = mObjectsByName.begin(); oi != mObjectsByName.end(); ++oi)
        {
            (*oi)->SetVisible(!(*oi)->GetVisible());
        }

        if (cascade)
        {
            for (size_t i = 0; i < mChildren.size(); ++i)
            {
                DiCullNode* sceneChild = static_cast<DiCullNode*>(mChildren[i]);
                sceneChild->FlipVisibility(cascade);
            }
        }
    }

    DiNode* DiCullNode::CreateChild(const DiVec3& inTranslate, const DiQuat& inRotate)
    {
        DiCullNode* newNode = mCreator->CreateNode();
        newNode->Translate(inTranslate);
        newNode->Rotate(inRotate);
        this->AddChild(newNode);

        return newNode;
    }

    DiNode* DiCullNode::CreateChild(const DiString& name,
        const DiVec3& inTranslate, const DiQuat& inRotate)
    {
        DiCullNode* newNode = mCreator->CreateNode(name);
        newNode->Translate(inTranslate);
        newNode->Rotate(inRotate);
        this->AddChild(newNode);

        return newNode;
    }

    DiCullNode* DiCullNode::CreateChild()
    {
        DiCullNode* newNode = mCreator->CreateNode();
        this->AddChild(newNode);
        return newNode;
    }

    DiCullNode* DiCullNode::CreateChild( const DiString& name )
    {
        DiCullNode* newNode = mCreator->CreateNode(name);
        this->AddChild(newNode);
        return newNode;
    }

    bool DiCullNode::IsIn( DiAABB &box )
    {
        if (box.IsNull()) 
        {
            return false;
        }

        if (box.IsInfinite())
        {
            return true;
        }

        DiVec3 center = mWorldAABB.GetMaximum().midPoint( mWorldAABB.GetMinimum() );

        DiVec3 bmin = box.GetMinimum();
        DiVec3 bmax = box.GetMaximum();

        bool centre = ( bmax > center && bmin < center );
        if (!centre)
        {
            return false;
        }

        DiVec3 octreeSize = bmax - bmin;
        DiVec3 nodeSize = mWorldAABB.GetMaximum() - mWorldAABB.GetMinimum();
        return nodeSize < octreeSize;
    }

    const DiAABB& DiCullNode::GetWorldAABB( void ) const
    {
        return mWorldAABB;
    }

    void DiCullNode::RemoveNodeAndChildren()
    {
        mCreator->RemoveOctreeNode( this ); 

        for (size_t i = 0; i < mChildren.size(); ++i)
        {
            static_cast<DiCullNode*>(mChildren[i])->RemoveNodeAndChildren();
        }
    }

    DiNode* DiCullNode::RemoveChild( uint32 index )
    {
        DiCullNode *on = static_cast<DiCullNode* >( DiNode::RemoveChild( index ) );
        on->RemoveNodeAndChildren(); 
        return on; 
    }

    DiNode* DiCullNode::RemoveChild( const DiString & name )
    {
        DiCullNode *on = static_cast<DiCullNode*>( DiNode::RemoveChild(  name ) );
        on -> RemoveNodeAndChildren( ); 
        return on; 
    }

    DiNode* DiCullNode::RemoveChild( DiNode* child )
    {
        DiCullNode *on = static_cast<DiCullNode*>( DiNode::RemoveChild( child ) );
        on -> RemoveNodeAndChildren(); 
        return on; 
    }

    void DiCullNode::RemoveAllChildren( void )
    {
        for (size_t i = 0; i < mChildren.size(); ++i)
        {
            DiCullNode* on = static_cast<DiCullNode*>(mChildren[i]);
            on->SetParent(nullptr);
            on->RemoveNodeAndChildren();
        }
        mChildren.clear();
        mChildrenToUpdate.clear();
    }

    void DiCullNode::_Update( bool updateChildren, bool parentHasChanged )
    {
        DiNode::_Update(updateChildren, parentHasChanged);
        UpdateBounds();
    }

    DiCullNode::ObjectIterator DiCullNode::GetAttachedObjectIterator()
    {
        return ObjectIterator(mObjectsByName.begin(), mObjectsByName.end());
    }

    DiCullNode::ConstObjectIterator DiCullNode::GetAttachedObjectIterator() const
    {
        return ConstObjectIterator(mObjectsByName.begin(), mObjectsByName.end());
    }
}
