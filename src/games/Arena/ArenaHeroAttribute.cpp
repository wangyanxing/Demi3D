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

#include "ArenaPch.h"
#include "ArenaHeroAttribute.h"
#include "ArenaConfigsLoader.h"

namespace Demi
{
    ArHeroAttr::~ArHeroAttr()
    {
        SAFE_DELETE(mHeroConfig);
    }

    void ArHeroAttr::LoadAttribute(const DiXMLElement& node)
    {
        SAFE_DELETE(mHeroConfig);
        mHeroConfig = DI_NEW ArConfigHero();

        ArConfigHeroLoader loader(mHeroConfig);
        loader.Load(node);
    }
}