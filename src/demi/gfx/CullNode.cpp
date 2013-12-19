
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
        ObjectMap::iterator itr;
        DiTransUnitPtr ret;
        for ( itr = mObjectsByName.begin(); itr != mObjectsByName.end(); ++itr )
        {
            ret = itr->second;
            ret->NotifyAttached((DiCullNode*)NULL);
        }
        mObjectsByName.clear();
    }

    void DiCullNode::UpdateBounds()
    {
        mWorldAABB.SetNull();
        mLocalAABB.SetNull();

        ObjectMap::iterator itr;
        DiTransUnitPtr ret;
        for ( itr = mObjectsByName.begin(); itr != mObjectsByName.end(); ++itr )
        {
            ret = itr->second;
            mWorldAABB.Merge(ret->GetWorldBoundingBox(true));
            mLocalAABB.Merge( ret->GetBoundingBox() );
        }

        if (!mWorldAABB.IsNull())
            mCreator->UpdateOctreeNode( this );
    }

    void DiCullNode::AttachObject(DiTransUnitPtr obj)
    {
        if (obj->IsAttached())
        {
            DI_ERROR("The object has been attached");
        }

        obj->NotifyAttached(this);

        DI_ASSERT(!mObjectsByName.contains(obj->GetName()));
        mObjectsByName.insert(ObjectMap::value_type(obj->GetName(), obj));

        NeedUpdate();
    }

    void DiCullNode::AttachSilently(DiTransUnitPtr obj)
    {
        DI_ASSERT(!mObjectsByName.contains(obj->GetName()));
        mObjectsByName.insert(ObjectMap::value_type(obj->GetName(), obj));

        NeedUpdate();
    }

    unsigned short DiCullNode::NumAttachedObjects( void ) const
    {
        return static_cast< unsigned short >( mObjectsByName.size() );
    }

    DiTransUnitPtr DiCullNode::GetAttachedObject( unsigned short index )
    {
        if (index < mObjectsByName.size())
        {
            ObjectMap::iterator i = mObjectsByName.begin();
            while (index--)++i;
            return i->second;
        }
        else
        {
            DI_ERROR("Cannot find the attachment object by id : %d",index);
            return NULL;
        }
    }

    DiTransUnitPtr DiCullNode::GetAttachedObject( const DiString& name )
    {
        ObjectMap::iterator i = mObjectsByName.find(name);

        if (i == mObjectsByName.end())
        {
            DI_ERROR("Cannot find the attachment object by name : %s",name.c_str());
        }

        return i->second;
    }

    DiTransUnitPtr DiCullNode::DetachObject( unsigned short index )
    {
        DiTransUnitPtr ret;
        if (index < mObjectsByName.size())
        {

            ObjectMap::iterator i = mObjectsByName.begin();
            while (index--)++i;

            ret = i->second;
            mObjectsByName.erase(i);

            if (ret->GetParentNode() == this)
            {
                ret->NotifyAttached((DiCullNode*)0);
            }

            NeedUpdate();

            return ret;

        }
        else
        {
            DI_ERROR("Cannot detach object, invalid index");
            return NULL;
        }
    }

    void DiCullNode::DetachObject(DiTransUnitPtr obj)
    {
        ObjectMap::iterator i, iend;
        iend = mObjectsByName.end();
        for (i = mObjectsByName.begin(); i != iend; ++i)
        {
            if (i->second == obj)
            {
                mObjectsByName.erase(i);
                break;
            }
        }

        if (obj->GetParentNode() == this)
        {
            obj->NotifyAttached((DiCullNode*)0);
        }

        NeedUpdate();
    }

    DiTransUnitPtr DiCullNode::DetachObject( const DiString& name )
    {
        ObjectMap::iterator it = mObjectsByName.find(name);
        if (it == mObjectsByName.end())
        {
            DI_ERROR("Cannot find the object : %s", name.c_str());
        }
        DiTransUnitPtr ret = it->second;
        mObjectsByName.erase(it);
        if (ret->GetParentNode() == this)
        {
            ret->NotifyAttached((DiCullNode*)0);
        }
        NeedUpdate();

        return ret;
    }

    void DiCullNode::DetachAllObjects( void )
    {
        ObjectMap::iterator itr;
        DiTransUnitPtr ret;
        for ( itr = mObjectsByName.begin(); 
            itr != mObjectsByName.end(); ++itr )
        {
            ret = itr->second;
            if (ret->GetParentNode() == this)
            {
                ret->NotifyAttached((DiCullNode*)0);
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
            DiTransUnitPtr mo = mit->second;
            if (mo->GetVisible())
                func(mo);
            ++mit;
        }
    }

    void DiCullNode::ProcessBatchGroup( DiCamera* camera,DiRenderPipeline* pipeline, 
        bool onlyShadowCaster, DiVisibleObjectsBoundsInfo* visbleBounds)
    {
        auto mit = mObjectsByName.begin();
        while ( mit != mObjectsByName.end() )
        {
            DiTransUnitPtr mo = mit->second;

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
        ObjectMap::iterator oi, oiend;
        oiend = mObjectsByName.end();
        for (oi = mObjectsByName.begin(); oi != oiend; ++oi)
        {
            oi->second->SetVisible(vis);
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
        ObjectMap::iterator oi, oiend;
        oiend = mObjectsByName.end();
        for (oi = mObjectsByName.begin(); oi != oiend; ++oi)
        {
            oi->second->SetVisible(!oi->second->GetVisible());
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

    DiNode* DiCullNode::RemoveChild( unsigned short index )
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
