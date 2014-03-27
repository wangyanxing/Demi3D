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

#ifndef DiK2GameEntity_h__
#define DiK2GameEntity_h__

#include "K2Prerequisites.h"
#include "K2Entity.h"
#include "K2GameDefines.h"

namespace Demi
{
    class DEMI_K2_API DiK2GameEntity : public DiK2Entity
    {
    public:

        DiK2GameEntity();

        virtual         ~DiK2GameEntity();

    public:

        void            SetModel(const DiString& model);

        void            Init(K2ObjTypes type);

        K2ObjTypes      GetType() const { return mType; }

        virtual void    InitComponents(){}

    private:

        DiK2RenderObject*   mRenderObj;

        K2ObjTypes          mType;

        DiString            mModel;
    };
}

#endif
