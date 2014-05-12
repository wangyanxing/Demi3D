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
#include "ArenaNPCAttribute.h"
#include "ArenaConfigsLoader.h"

namespace Demi
{
    ArNPCAttr::~ArNPCAttr()
    {
        SAFE_DELETE(mNpcEntityConfig);
    }

    void ArNPCAttr::LoadAttribute(const DiXMLElement& node)
    {
        mNpcEntityConfig = DI_NEW (mNpcEntityConfig) ArConfigEntity();

        ArConfigEntityLoader loader(mNpcEntityConfig);
        loader.Load(node);
        
        mEntityConfig.Load(mNpcEntityConfig->entity);
    }
}