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

#ifndef ArenaHeroAttribute_h__
#define ArenaHeroAttribute_h__

#include "ArenaPrerequisites.h"
#include "ArenaAttribute.h"
#include "ArenaConfigs.h"

namespace Demi
{
    class ArHeroAttr final : public ArAttribute
    {
    public:

        ArHeroAttr(ArObjID id) : ArAttribute(id) {}

        ~ArHeroAttr();

    public:

        virtual void      Update(float dt) override {}

        virtual void      LoadAttribute(const DiXMLElement& node) override;

        ArConfigEntity*   mHeroConfig{ DI_NEW ArConfigEntity() };
    };
}

#endif
