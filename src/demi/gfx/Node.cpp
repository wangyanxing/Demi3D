
#include "Node.h"
#include <sstream>

namespace Demi 
{
    DiNode::QueuedUpdates    DiNode::sQueuedUpdates;

    DiNode::DiNode()
        :mParent(0),
        mNodeType(NT_NODEBASE),
        mNeedParentUpdate(false),
        mNeedChildUpdate(false),
        mParentNotified(false),
        mQueuedForUpdate(false),
        mOrientation(DiQuat::IDENTITY),
        mPosition(DiVec3::ZERO),
        mScale(DiVec3::UNIT_SCALE),
        mInheritOrientation(true),
        mInheritScale(true),
        mDerivedOrientation(DiQuat::IDENTITY),
        mDerivedPosition(DiVec3::ZERO),
        mDerivedScale(DiVec3::UNIT_SCALE),
        mInitialPosition(DiVec3::ZERO),
        mInitialOrientation(DiQuat::IDENTITY),
        mInitialScale(DiVec3::UNIT_SCALE),
        mCachedTransformOutOfDate(true)
    {
        static int nodeid = 0;
        std::ostringstream s;
        s << "nd_" << nodeid++;
        mName = s.str();

        NeedUpdate();

    }

    DiNode::DiNode(const DiString& name)
        :
    mParent(0),
        mNodeType(NT_NODEBASE),
        mNeedParentUpdate(false),
        mNeedChildUpdate(false),
        mParentNotified(false),
        mQueuedForUpdate(false),
        mName(name),
        mOrientation(DiQuat::IDENTITY),
        mPosition(DiVec3::ZERO),
        mScale(DiVec3::UNIT_SCALE),
        mInheritOrientation(true),
        mInheritScale(true),
        mDerivedOrientation(DiQuat::IDENTITY),
        mDerivedPosition(DiVec3::ZERO),
        mDerivedScale(DiVec3::UNIT_SCALE),
        mInitialPosition(DiVec3::ZERO),
        mInitialOrientation(DiQuat::IDENTITY),
        mInitialScale(DiVec3::UNIT_SCALE),
        mCachedTransformOutOfDate(true)
    {
        NeedUpdate();
    }


    DiNode::~DiNode()
    {
        RemoveAllChildren();

        if(mParent)
        {
            mParent->RemoveChild(this);
        }

        if (mQueuedForUpdate)
        {
            QueuedUpdates::iterator it = sQueuedUpdates.find(this);
            DI_ASSERT(it != sQueuedUpdates.end());

            if (it != sQueuedUpdates.end())
            {
                *it = sQueuedUpdates.back();
                sQueuedUpdates.pop_back();
            }
        }
    }

    DiNode* DiNode::GetParent(void) const
    {
        return mParent;
    }


    void DiNode::SetParent(DiNode* parent)
    {
        mParent = parent;
        mParentNotified = false ;
        NeedUpdate();
    }


    const DiMat4& DiNode::GetFullTransform(void) const
    {
        if (mCachedTransformOutOfDate)
        {
            mCachedTransform.makeTransform(
                GetDerivedPosition(),
                GetDerivedScale(),
                GetDerivedOrientation());
            mCachedTransformOutOfDate = false;
        }
        return mCachedTransform;
    }

    void DiNode::_Update(bool updateChildren, bool parentHasChanged)
    {
        mParentNotified = false ;

        if (!updateChildren && !mNeedParentUpdate && !mNeedChildUpdate && !parentHasChanged )
            return;

        if (mNeedParentUpdate || parentHasChanged)
            _UpdateFromParent();

        if (mNeedChildUpdate || parentHasChanged)
        {
            for (size_t i = 0; i < mChildren.size(); ++i)
                mChildren[i]->_Update(true, true);
            mChildrenToUpdate.clear();
        }
        else
        {
            // Just update selected children
            ChildUpdateSet::iterator it, itend;
            itend = mChildrenToUpdate.end();
            for(it = mChildrenToUpdate.begin(); it != itend; ++it)
            {
                DiNode* child = *it;
                child->_Update(true, false);
            }

            mChildrenToUpdate.clear();
        }

        mNeedChildUpdate = false;
    }

    void DiNode::_UpdateFromParent(void) const
    {
        if (mParent)
        {
            const DiQuat& parentOrientation = mParent->GetDerivedOrientation();
            if (mInheritOrientation)
            {
                mDerivedOrientation = parentOrientation * mOrientation;
            }
            else
            {
                mDerivedOrientation = mOrientation;
            }

            const DiVec3& parentScale = mParent->GetDerivedScale();
            if (mInheritScale)
            {
                mDerivedScale = parentScale * mScale;
            }
            else
            {
                mDerivedScale = mScale;
            }

            mDerivedPosition = parentOrientation * (parentScale * mPosition);
            mDerivedPosition += mParent->GetDerivedPosition();
        }
        else
        {
            mDerivedOrientation = mOrientation;
            mDerivedPosition = mPosition;
            mDerivedScale = mScale;
        }

        mCachedTransformOutOfDate = true;
        mNeedParentUpdate = false;
    }

    void DiNode::AddChild(DiNode* child)
    {
        if (child->mParent)
        {
            DI_WARNING("The node already has a parent.");
            return;
        }

        mChildren.push_back(child);
        child->SetParent(this);
    }

    DiNode* DiNode::RemoveChild(unsigned short index)
    {
        Children::iterator i = mChildren.begin() + index;
        DiNode* ret = *i;
        CancelUpdate(ret);
        mChildren.erase(i);
        ret->SetParent(NULL);
        return ret;
    }

    DiNode* DiNode::RemoveChild(DiNode* child)
    {
        auto i = mChildren.find(child);
        if (i != mChildren.end())
        {
            CancelUpdate(child);
            mChildren.erase(i);
            child->SetParent(NULL);
        }
        return child;
    }

    const DiQuat& DiNode::GetOrientation() const
    {
        return mOrientation;
    }

    void DiNode::SetOrientation( const DiQuat & q )
    {
        DI_ASSERT(!q.isNaN());
        mOrientation = q;
        mOrientation.normalise();
        NeedUpdate();
    }

    void DiNode::SetOrientation( float w, float x, float y, float z)
    {
        SetOrientation(DiQuat(w, x, y, z));
    }

    void DiNode::ResetOrientation(void)
    {
        mOrientation = DiQuat::IDENTITY;
        NeedUpdate();
    }


    void DiNode::SetPosition(const DiVec3& pos)
    {
        DI_ASSERT(!pos.isNaN());
        mPosition = pos;
        NeedUpdate();
    }



    void DiNode::SetPosition(float x, float y, float z)
    {
        DiVec3 v(x,y,z);
        SetPosition(v);
    }


    const DiVec3 & DiNode::GetPosition(void) const
    {
        return mPosition;
    }

    DiMat3 DiNode::GetLocalAxes(void) const
    {
        DiVec3 axisX = DiVec3::UNIT_X;
        DiVec3 axisY = DiVec3::UNIT_Y;
        DiVec3 axisZ = DiVec3::UNIT_Z;

        axisX = mOrientation * axisX;
        axisY = mOrientation * axisY;
        axisZ = mOrientation * axisZ;

        return DiMat3(axisX.x, axisY.x, axisZ.x,
            axisX.y, axisY.y, axisZ.y,
            axisX.z, axisY.z, axisZ.z);
    }


    void DiNode::Translate(const DiVec3& d, TransformSpace relativeTo)
    {
        switch(relativeTo)
        {
        case TS_LOCAL:
            // position is relative to parent so transform downwards
            mPosition += mOrientation * d;
            break;
        case TS_WORLD:
            // position is relative to parent so transform upwards
            if (mParent)
            {
                mPosition += (mParent->GetDerivedOrientation().Inverse() * d)
                    / mParent->GetDerivedScale();
            }
            else
            {
                mPosition += d;
            }
            break;
        case TS_PARENT:
            mPosition += d;
            break;
        }
        NeedUpdate();

    }

    void DiNode::Translate(float x, float y, float z, TransformSpace relativeTo)
    {
        DiVec3 v(x,y,z);
        Translate(v, relativeTo);
    }

    void DiNode::Translate(const DiMat3& axes, const DiVec3& move, TransformSpace relativeTo)
    {
        DiVec3 derived = axes * move;
        Translate(derived, relativeTo);
    }

    void DiNode::Translate(const DiMat3& axes, float x, float y, float z, TransformSpace relativeTo)
    {
        DiVec3 d(x,y,z);
        Translate(axes,d,relativeTo);
    }

    void DiNode::Roll(const DiRadian& angle, TransformSpace relativeTo)
    {
        Rotate(DiVec3::UNIT_Z, angle, relativeTo);
    }

    void DiNode::Pitch(const DiRadian& angle, TransformSpace relativeTo)
    {
        Rotate(DiVec3::UNIT_X, angle, relativeTo);
    }

    void DiNode::Yaw(const DiRadian& angle, TransformSpace relativeTo)
    {
        Rotate(DiVec3::UNIT_Y, angle, relativeTo);

    }

    void DiNode::Rotate(const DiVec3& axis, const DiRadian& angle, TransformSpace relativeTo)
    {
        DiQuat q;
        q.FromAngleAxis(angle,axis);
        Rotate(q, relativeTo);
    }


    void DiNode::Rotate(const DiQuat& q, TransformSpace relativeTo)
    {
        // Normalise quaternion to avoid drift
        DiQuat qnorm = q;
        qnorm.normalise();

        switch(relativeTo)
        {
        case TS_PARENT:
            // Rotations are normally relative to local axes, transform up
            mOrientation = qnorm * mOrientation;
            break;
        case TS_WORLD:
            // Rotations are normally relative to local axes, transform up
            mOrientation = mOrientation * GetDerivedOrientation().Inverse()
                * qnorm * GetDerivedOrientation();
            break;
        case TS_LOCAL:
            // Note the order of the mult, i.e. q comes after
            mOrientation = mOrientation * qnorm;
            break;
        }
        NeedUpdate();
    }



    void DiNode::SetDerivedPosition( const DiVec3& pos )
    {
        //find where the node would end up in parent's local space
        SetPosition( mParent->ConvertWorldToLocalPosition( pos ) );
    }

    void DiNode::SetDerivedOrientation( const DiQuat& q )
    {
        //find where the node would end up in parent's local space
        SetOrientation( mParent->ConvertWorldToLocalOrientation( q ) );
    }


    const DiQuat & DiNode::GetDerivedOrientation(void) const
    {
        if (mNeedParentUpdate)
        {
            _UpdateFromParent();
        }
        return mDerivedOrientation;
    }

    const DiVec3 & DiNode::GetDerivedPosition(void) const
    {
        if (mNeedParentUpdate)
        {
            _UpdateFromParent();
        }
        return mDerivedPosition;
    }

    const DiVec3 & DiNode::GetDerivedScale(void) const
    {
        if (mNeedParentUpdate)
        {
            _UpdateFromParent();
        }
        return mDerivedScale;
    }

    DiVec3 DiNode::ConvertWorldToLocalPosition( const DiVec3 &worldPos )
    {
        if (mNeedParentUpdate)
        {
            _UpdateFromParent();
        }
        return mDerivedOrientation.Inverse() * (worldPos - mDerivedPosition) / mDerivedScale;
    }

    DiVec3 DiNode::ConvertLocalToWorldPosition( const DiVec3 &localPos )
    {
        if (mNeedParentUpdate)
        {
            _UpdateFromParent();
        }
        return (mDerivedOrientation * localPos * mDerivedScale) + mDerivedPosition;
    }

    DiQuat DiNode::ConvertWorldToLocalOrientation( const DiQuat &worldOrientation )
    {
        if (mNeedParentUpdate)
        {
            _UpdateFromParent();
        }
        return mDerivedOrientation.Inverse() * worldOrientation;
    }

    DiQuat DiNode::ConvertLocalToWorldOrientation( const DiQuat &localOrientation )
    {
        if (mNeedParentUpdate)
        {
            _UpdateFromParent();
        }
        return mDerivedOrientation * localOrientation;

    }

    void DiNode::RemoveAllChildren(void)
    {
        for (size_t i = 0; i < mChildren.size(); ++i)
            mChildren[i]->SetParent(nullptr);

        mChildren.clear();
        mChildrenToUpdate.clear();
    }

    void DiNode::SetScale(const DiVec3& inScale)
    {
        DI_ASSERT(!inScale.isNaN());
        mScale = inScale;
        NeedUpdate();
    }

    void DiNode::SetScale(float x, float y, float z)
    {
        SetScale(DiVec3(x, y, z));
    }

    const DiVec3 & DiNode::GetScale(void) const
    {
        return mScale;
    }

    void DiNode::SetInheritOrientation(bool inherit)
    {
        mInheritOrientation = inherit;
        NeedUpdate();
    }

    bool DiNode::GetInheritOrientation(void) const
    {
        return mInheritOrientation;
    }

    void DiNode::SetInheritScale(bool inherit)
    {
        mInheritScale = inherit;
        NeedUpdate();
    }

    bool DiNode::GetInheritScale(void) const
    {
        return mInheritScale;
    }

    void DiNode::Scale(const DiVec3& inScale)
    {
        mScale = mScale * inScale;
        NeedUpdate();

    }

    void DiNode::Scale(float x, float y, float z)
    {
        mScale.x *= x;
        mScale.y *= y;
        mScale.z *= z;
        NeedUpdate();

    }

    const DiString& DiNode::GetName(void) const
    {
        return mName;
    }

    void DiNode::SetInitialState(void)
    {
        mInitialPosition = mPosition;
        mInitialOrientation = mOrientation;
        mInitialScale = mScale;
    }

    void DiNode::ResetToInitialState(void)
    {
        mPosition = mInitialPosition;
        mOrientation = mInitialOrientation;
        mScale = mInitialScale;

        NeedUpdate();
    }

    const DiVec3& DiNode::GetInitialPosition(void) const
    {
        return mInitialPosition;
    }

    const DiQuat& DiNode::GetInitialOrientation(void) const
    {
        return mInitialOrientation;

    }

    const DiVec3& DiNode::GetInitialScale(void) const
    {
        return mInitialScale;
    }

    void DiNode::NeedUpdate(bool forceParentUpdate)
    {

        mNeedParentUpdate = true;
        mNeedChildUpdate = true;
        mCachedTransformOutOfDate = true;

        if (mParent && (!mParentNotified || forceParentUpdate))
        {
            mParent->RequestUpdate(this, forceParentUpdate);
            mParentNotified = true ;
        }

        mChildrenToUpdate.clear();
    }

    void DiNode::RequestUpdate(DiNode* child, bool forceParentUpdate)
    {
        if (mNeedChildUpdate)
        {
            return;
        }

        mChildrenToUpdate.insert(child);
        if (mParent && (!mParentNotified || forceParentUpdate))
        {
            mParent->RequestUpdate(this, forceParentUpdate);
            mParentNotified = true ;
        }

    }

    void DiNode::CancelUpdate(DiNode* child)
    {
        mChildrenToUpdate.erase(child);

        if (mChildrenToUpdate.empty() && 
            mParent && !mNeedChildUpdate)
        {
            mParent->CancelUpdate(this);
            mParentNotified = false ;
        }
    }

    void DiNode::QueueNeedUpdate(DiNode* n)
    {
        if (!n->mQueuedForUpdate)
        {
            n->mQueuedForUpdate = true;
            sQueuedUpdates.push_back(n);
        }
    }

    void DiNode::ProcessQueuedUpdates(void)
    {
        for (auto i = sQueuedUpdates.begin();
            i != sQueuedUpdates.end(); ++i)
        {
            DiNode* n = *i;
            n->mQueuedForUpdate = false;
            n->NeedUpdate(true);
        }
        sQueuedUpdates.clear();
    }

    void DiNode::ChangeParent( DiNode* newparent )
    {
        if (mParent)
        {
            mParent->RemoveChild(this);
            mParent = nullptr;
        }
        if (newparent)
        {
            newparent->AddChild(this);
            newparent->NeedUpdate();
        }
        NeedUpdate();
    }

    void DiNode::SetName( const DiString& newName )
    {
        mName = newName;
    }
}