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

#ifndef ArenaNPCAttribute_h__
#define ArenaNPCAttribute_h__

#include "ArenaPrerequisites.h"
#include "ArenaAttribute.h"
#include "ArenaConfigs.h"

namespace Demi
{
    /** Attribute interface for entities
     */
    class ArNPCAttr final: public ArAttribute
    {
    public:

        ArNPCAttr(ArObjID id) : ArAttribute(id) {}

        virtual ~ArNPCAttr();

    public:

        virtual void      Update(float dt) override {}

        virtual void      LoadAttribute(const DiXMLElement& node) override;

        ArConfigEntity*   mNpcEntityConfig { DI_NEW ArConfigEntity() };
    };
}

#endif
