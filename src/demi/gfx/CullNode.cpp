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
#include "SceneCuller.h"

namespace Demi
{
    DiCullNode::DiCullNode(DiSceneManager* sm):
        DiNode(),
        mCreator(sm),
        mCullUnit(nullptr),
        mIsCulled(false)
    {
        if(sm)
            mCullUnit = sm->GetSceneCuller()->CreateUnit(this);
    }

    DiCullNode::DiCullNode( DiSceneManager* sm,const DiString& name ):
        DiNode(name),
        mCreator(sm),
        mCullUnit(nullptr)
    {
        if(sm)
            mCullUnit = sm->GetSceneCuller()->CreateUnit(this);
    }

    DiCullNode::~DiCullNode(void)
    {
        for ( auto itr = mObjects.begin(); itr != mObjects.end(); ++itr )
        {
            (*itr)->NotifyAttached(nullptr);
        }
        mObjects.clear();
        mCullUnit.reset();
    }

    void DiCullNode::UpdateBounds()
    {
        mWorldAABB.SetNull();
        mLocalAABB.SetNull();

        for ( auto itr = mObjects.begin(); itr != mObjects.end(); ++itr )
        {
            mWorldAABB.Merge((*itr)->GetWorldBoundingBox(true));
            mLocalAABB.Merge((*itr)->GetBoundingBox());
        }

        if (!mWorldAABB.IsNull() && mCullUnit)
            mCullUnit->Update();
    }

    void DiCullNode::AttachObject(DiTransUnitPtr obj)
    {
        if (obj->IsAttached())
        {
            DI_WARNING("The object has been attached");
            return;
        }

        obj->NotifyAttached(this);

        mObjects.push_back(obj);

        NeedUpdate();
    }

    void DiCullNode::AttachSilently(DiTransUnitPtr obj)
    {
        mObjects.push_back(obj);
        
        NeedUpdate();
    }

    uint32 DiCullNode::NumAttachedObjects( void ) const
    {
        return mObjects.size();
    }

    DiTransUnitPtr DiCullNode::GetAttachedObject( uint32 index )
    {
        if (index < mObjects.size())
        {
            return mObjects[index];
        }
        else
        {
            DI_WARNING("Cannot find the attachment object by id : %d",index);
            return nullptr;
        }
    }

    DiTransUnitPtr DiCullNode::DetachObject( uint32 index )
    {
        if (index < mObjects.size())
        {
            DiTransUnitPtr ret = mObjects[index];
            if (ret->GetParentNode() == this)
            {
                ret->NotifyAttached((DiCullNode*)0);
            }
            
            ObjectMap::iterator i = mObjects.begin() + index;
            mObjects.erase(i);

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
        for (auto i = mObjects.begin(); i != mObjects.end(); ++i)
        {
            if (*i == obj)
            {
                mObjects.erase(i);
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
        for ( auto itr = mObjects.begin();
            itr != mObjects.end(); ++itr )
        {
            if ((*itr)->GetParentNode() == this)
            {
                (*itr)->NotifyAttached(nullptr);
            }
        }
        mObjects.clear();

        NeedUpdate();
    }

    void DiCullNode::ProcessVisibleObjects(std::function<void(DiTransUnitPtr)> func)
    {
        auto mit = mObjects.begin();
        while (mit != mObjects.end())
        {
            if ((*mit)->GetVisible())
                func((*mit));
            ++mit;
        }
    }

    void DiCullNode::SetVisible( bool vis, bool cascade /*= true*/ )
    {
        for (auto oi = mObjects.begin(); oi != mObjects.end(); ++oi)
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
        for (auto oi = mObjects.begin(); oi != mObjects.end(); ++oi)
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
            return false;

        if (box.IsInfinite())
            return true;

        DiVec3 center = mWorldAABB.GetMaximum().midPoint( mWorldAABB.GetMinimum() );

        DiVec3 bmin = box.GetMinimum();
        DiVec3 bmax = box.GetMaximum();

        bool centre = ( bmax > center && bmin < center );
        if (!centre)
        {
            return false;
        }

        DiVec3 size = bmax - bmin;
        DiVec3 nodeSize = mWorldAABB.GetMaximum() - mWorldAABB.GetMinimum();
        return nodeSize < size;
    }

    const DiAABB& DiCullNode::GetWorldAABB( void ) const
    {
        return mWorldAABB;
    }

    void DiCullNode::RemoveNodeAndChildren()
    {
        if (mCullUnit)
            mCullUnit->mCuller->RemoveUnit(mCullUnit);

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
        return ObjectIterator(mObjects.begin(), mObjects.end());
    }

    DiCullNode::ConstObjectIterator DiCullNode::GetAttachedObjectIterator() const
    {
        return ConstObjectIterator(mObjects.begin(), mObjects.end());
    }
}
