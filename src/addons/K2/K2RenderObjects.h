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

#ifndef DiK2RenderObject_h__
#define DiK2RenderObject_h__

#include "K2Prerequisites.h"

namespace Demi
{
    /** base class for renderable object
     */
    class DEMI_K2_API DiK2RenderObject : public DiBase
    {
    public:

        DiK2RenderObject(DiK2World* world);

        virtual ~DiK2RenderObject();

    public:

        virtual void            Release();

        DiCullNode*             GetNode() { return mNode; }

        virtual DiK2ModelPtr    LoadModel(const DiString& mdf) = 0;

        virtual void	        SetVisible(bool vis);

        bool                    GetVisible() { return mVisible; }

        void                    SetPosition(const DiK2Pos& pos, bool autoHeight = true);
        
        void                    SetPosition(const DiK2Pos& pos, float height);

        DiK2Pos                 GetPosition() { return mPosition; }

        void                    SetWorldPosition(const DiVec3& pos);

        DiVec3                  GetWorldPosition();

        void                    Update(float dt);

        void                    SetRotation(const float rotRad);

        void                    SetRotation(const DiQuat& quat);

        void                    SetScale(const DiVec3& scale);

        DiVec3                  GetScale() const { return mScale; }

        float                   GetRotation() { return mRotRadian; }

        DiQuat                  GetRotQuat() { return mRotation; }

        DiK2ModelPtr            GetModel() { return mModel; }

        void                    PlayClip(K2PrefabClip::Clips clip);

        void                    PlayClip(const DiString& clip);

        void                    _CreateDebugger();

        /** Radius on X-Z plane
         */
        float                   GetRadius();

    protected:

        DiK2ModelPtr            mModel;

        DiCullNode*             mNode{ nullptr };

        bool                    mVisible{ true };

        DiQuat                  mRotation;

        float                   mRotRadian{ 0 };

        DiVec3                  mScale{ DiVec3::UNIT_SCALE };

        DiK2Pos                 mPosition;
        
        DiK2World*              mWorld;

        // for debug
        DiDebugHelperPtr        mDebugger;
    };
}

#endif
