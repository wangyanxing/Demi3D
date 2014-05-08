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
#include "ArenaNPCEntity.h"
#include "ArenaNPCAttribute.h"
#include "ArenaConfigsLoader.h"
#include "ArenaGameApp.h"
#include "ArenaLevel.h"
#include "ArenaAttribute.h"

#include "K2RenderObjects.h"

namespace Demi
{
    ArNPCEntity::ArNPCEntity()
        :mAIProperty(nullptr)
    {
    }

    ArNPCEntity::~ArNPCEntity()
    {
    }

    void ArNPCEntity::InitComponents()
    {
        ArDynEntity::InitComponents();
        mAIProperty = CreateProperty<ArAIProperty>();
    }

    void ArNPCEntity::InitAttribute(const DiXMLElement& node)
    {
        InitAttribute();
        GetAttribute<ArNPCAttr>()->LoadAttribute(node);
        SetupAttribute();
    }

    void ArNPCEntity::InitAttribute(const DiString& config)
    {
        InitAttribute();
        GetAttribute<ArNPCAttr>()->LoadAttributeConfig(config);
        SetupAttribute();
    }

    void ArNPCEntity::InitAttribute()
    {
        SetAttribute<ArNPCAttr>();
    }

    void ArNPCEntity::SetupDynModelConfig(const ArConfigDynamicModel* dynModel)
    {
        SetModel(dynModel->model.path);

        auto level = ArGameApp::Get()->GetLevel();
        DiVec3 position = level->GetSpwanPosition();
        GetRenderObj()->SetWorldPosition(position);

        GetRenderObj()->SetScale(DiVec3(dynModel->model.scale));
        GetMoveProperty()->SetSpeed(dynModel->motion.runspeed);
        GetMoveProperty()->SetTurnSpeed(dynModel->motion.turnspeed);
    }

    void ArNPCEntity::SetupAttribute()
    {
        SetupDynModelConfig(GetAttribute<ArNPCAttr>()->mDynModelConfig);
    }
}