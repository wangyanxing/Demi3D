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

#ifndef DiNode_h__
#define DiNode_h__


namespace Demi
{
    class DI_GFX_API DiNode : public DiBase
    {
    public:

        enum TransformSpace
        {
            TS_LOCAL,
            TS_PARENT,
            TS_WORLD
        };

        enum NodeType
        {
            NT_NODEBASE,
            NT_BONENODE,
            NT_ATTACHNODE,
        };

        typedef DiVector<DiNode*> Children;
        friend class DiSceneManager;

    protected:

        DiNode();

        DiNode(const DiString& name);

        virtual                  ~DiNode();  

    public:

        const DiString&          GetName(void) const;

        DiNode*                  GetParent(void) const;

        inline    NodeType       GetNodeType(void) const {return mNodeType;}

        const DiQuat&            GetOrientation() const;

        void                     SetOrientation( const DiQuat& q );

        void                     SetOrientation( float w, float x, float y, float z);

        void                     ResetOrientation(void);

        void                     SetPosition(const DiVec3& pos);

        void                     SetPosition(float x, float y, float z);

        const DiVec3&            GetPosition(void) const;

        void                     SetScale(const DiVec3& scale);

        void                     SetScale(float x, float y, float z);

        void                     SetScale(float unit)
        {
            SetScale(unit,unit,unit);
        }

        const DiVec3&            GetScale(void) const;

        void                     SetInheritOrientation(bool inherit);

        bool                     GetInheritOrientation(void) const;

        void                     SetInheritScale(bool inherit);

        bool                     GetInheritScale(void) const;

        void                     Scale(const DiVec3& scale);

        void                     Scale(float x, float y, float z);

        void                     Translate(const DiVec3& d, TransformSpace relativeTo = TS_LOCAL);

        void                     Translate(float x, float y, float z, TransformSpace relativeTo = TS_LOCAL);

        void                     Translate(const DiMat3& axes, const DiVec3& move, TransformSpace relativeTo = TS_LOCAL);

        void                     Translate(const DiMat3& axes, float x, float y, float z, TransformSpace relativeTo = TS_LOCAL);

        void                     Roll(const DiRadian& angle, TransformSpace relativeTo = TS_LOCAL);

        void                     Pitch(const DiRadian& angle, TransformSpace relativeTo = TS_LOCAL);

        void                     Yaw(const DiRadian& angle, TransformSpace relativeTo = TS_LOCAL);

        void                     Rotate(const DiVec3& axis, const DiRadian& angle, TransformSpace relativeTo = TS_LOCAL);

        void                     Rotate(const DiQuat& q, TransformSpace relativeTo = TS_LOCAL);

        DiMat3                   GetLocalAxes(void) const;

        virtual DiNode*          CreateChild(
            const DiVec3& translate = DiVec3::ZERO, 
            const DiQuat& rotate = DiQuat::IDENTITY ) = 0;

        virtual DiNode*          CreateChild(const DiString& name, const DiVec3& translate = DiVec3::ZERO,
            const DiQuat& rotate = DiQuat::IDENTITY) = 0;

        size_t                   GetChildrenNum() const
        {
            return mChildren.size();
        }

        void                     AddChild(DiNode* child);

        inline DiNode*           GetChild(unsigned short index)
        {
            return mChildren[index];
        }

        virtual DiNode*          RemoveChild(unsigned short index);
                                 
        virtual DiNode*          RemoveChild(DiNode* child);
                                 
        virtual void             RemoveAllChildren(void);
                                 
        void                     ChangeParent(DiNode* newparent);

        void                     SetDerivedPosition(const DiVec3& pos);

        void                     SetDerivedOrientation(const DiQuat& q);

        const DiQuat&            GetDerivedOrientation(void) const;

        const DiVec3&            GetDerivedPosition(void) const;

        const DiVec3&            GetDerivedScale(void) const;

        const DiMat4&            GetFullTransform(void) const;

        void                     SetName(const DiString& newName);

        virtual void             _Update(bool updateChildren, bool parentHasChanged);

        void                     SetInitialState(void);

        void                     ResetToInitialState(void);

        const DiVec3&            GetInitialPosition(void) const;

        DiVec3                   ConvertWorldToLocalPosition( const DiVec3 &worldPos );

        DiVec3                   ConvertLocalToWorldPosition( const DiVec3 &localPos );

        DiQuat                   ConvertWorldToLocalOrientation( const DiQuat &worldOrientation );

        DiQuat                   ConvertLocalToWorldOrientation( const DiQuat &localOrientation );

        const DiQuat&            GetInitialOrientation(void) const;

        const DiVec3&            GetInitialScale(void) const;

        virtual void             NeedUpdate(bool forceParentUpdate = false);

        void                     RequestUpdate(DiNode* child, bool forceParentUpdate = false);

        void                     CancelUpdate(DiNode* child);
        
        uint64                   GetLastUpdateFrameNumber() const {return mLastUpdateFrame;}

        static void              QueueNeedUpdate(DiNode* n);

        static void              ProcessQueuedUpdates(void);

    protected:

        void                    SetParent(DiNode* parent);

        void                    _UpdateFromParent(void) const;

    protected:

        DiNode*                 mParent;

        NodeType                mNodeType;

        Children                mChildren;

        typedef DiSet<DiNode*>  ChildUpdateSet;

        mutable ChildUpdateSet  mChildrenToUpdate;

        mutable bool            mNeedParentUpdate;

        mutable bool            mNeedChildUpdate;

        mutable bool            mParentNotified ;

        mutable bool            mQueuedForUpdate;

        DiString                mName;

        DiQuat                  mOrientation;

        DiVec3                  mPosition;

        DiVec3                  mScale;

        bool                    mInheritOrientation;

        bool                    mInheritScale;

        mutable DiQuat          mDerivedOrientation;
                                
        mutable DiVec3          mDerivedPosition;
                                
        mutable DiVec3          mDerivedScale;
                                
        DiVec3                  mInitialPosition;
                                
        DiQuat                  mInitialOrientation;
                                
        DiVec3                  mInitialScale;
                                
        mutable DiMat4          mCachedTransform;

        mutable bool            mCachedTransformOutOfDate;

        typedef DiVector<DiNode*> QueuedUpdates;

        static QueuedUpdates    sQueuedUpdates;
        
        /// record the frame# of the last update
        uint64                  mLastUpdateFrame;
    };
}

#endif
