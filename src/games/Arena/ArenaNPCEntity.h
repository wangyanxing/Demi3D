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

#ifndef ArenaNPCEntity_h__
#define ArenaNPCEntity_h__

#include "ArenaPrerequisites.h"
#include "ArenaDynEntity.h"
#include "ArenaAIProperty.h"
#include "ArenaConfigs.h"

namespace Demi
{
    /** NPC entity
     */
    class ArNPCEntity : public ArDynEntity
    {
        DEF_DECLARE_PRIORITY(AIProperty)

    public:

        ArNPCEntity();

        virtual         ~ArNPCEntity();

    public:

        virtual void    InitComponents() override;

        virtual void    InitAttribute(const DiXMLElement& node);

        virtual void    InitAttribute(const DiString& config) override;

        virtual void    SetupAttribute();

        virtual void    InitAttribute() override;

    protected:

        void            SetupEntityConfig(const ArEntityConfigs* ett);

    private:
    };
}

#endif