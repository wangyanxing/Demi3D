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

#ifndef DiK2HeroEntity_h__
#define DiK2HeroEntity_h__

#include "K2Prerequisites.h"

namespace Demi
{
    /** base class for renderable object
     */
    class DEMI_K2_API DiK2RenderObject : public DiBase
    {
    public:

        DiK2RenderObject();

        virtual ~DiK2RenderObject();

    public:

        void	                SetParent(DiK2GameEntity* entity){ mEnity = entity; }
        
        DiK2GameEntity*         GetParent(){ return mEnity; }

        virtual void            Release();

        DiCullNode*             GetNode() { return mNode; }

        virtual DiK2ModelPtr    LoadModel(const DiString& mdf) = 0;

        virtual void	        SetVisible(bool vis);

        bool                    GetVisible() { return mVisible; }

        void                    SetPosition(const DiK2Pos& pos);

        DiK2Pos                 GetPosition();

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

    protected:

        DiK2ModelPtr            mModel;

        DiCullNode*             mNode;

        bool                    mVisible;

        DiK2GameEntity*         mEnity;

        DiQuat                  mRotation;

        float                   mRotRadian;

        DiVec3                  mScale;
    };
}

#endif
