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
#include "ArenaEntityConfig.h"

#include "K2RenderObjects.h"
#include "K2Terrain.h"
#include "K2World.h"
#include "K2Model.h"

namespace Demi
{
    ArNPCEntity::ArNPCEntity()
        :mAIProperty(nullptr)
    {
        for (int i = 0; i < 9; ++i)
        {
            mLastBlockValues[i] = 0;
            mLastBlocksVerts[i] = DiIntVec2(-1, -1);
        }
    }

    ArNPCEntity::~ArNPCEntity()
    {
    }

    void ArNPCEntity::InitComponents()
    {
        ArDynEntity::InitComponents();
        mAIProperty = CreateProperty<ArAIProperty>();
        mGUIProperty = CreateProperty<ArGUIProperty>();
    }

    void ArNPCEntity::InitAttribute(const DiString& path, const DiXMLElement& node)
    {
        InitAttribute();
        GetAttribute<ArNPCAttr>()->LoadAttribute(path, node);
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

    void ArNPCEntity::SetupEntityConfig(const ArEntityConfigs* ett)
    {
        auto level = ArGameApp::Get()->GetLevel();
        DiVec3 position = level->GetSpwanPosition();
        GetRenderObj()->SetWorldPosition(position);
        GetRenderObj()->GetModel()->SetQueryFlags(QUERY_NPC);
        GetRenderObj()->GetModel()->SetCustomID(GetID());

        float scale = ett->GetMaxPreGlobalScale();
        GetRenderObj()->SetScale(DiVec3(scale));

        GetMoveProperty()->SetSpeed(DiK2Pos::FromWorldScale(ett->movespeed));
        GetMoveProperty()->SetTurnSpeed(ett->turnrate);
        
        // picking bounds
        GetRenderObj()->SetSelectionBounds(ett->boundsradius, ett->boundsheight);
    }

    void ArNPCEntity::SetupAttribute()
    {
        auto attr = GetAttribute<ArNPCAttr>();
        
        ArEntityConfigs* entityConfig = attr->GetEntityConfig();
        if(entityConfig->model.empty())
        {
            DI_WARNING("Cannot locate the model name.");
        }
        else
        {
            SetModel(entityConfig->path + entityConfig->model[0]);
        }
        
        SetupEntityConfig(entityConfig);
    }

    void ArNPCEntity::Update(float dt)
    {
        ArGameEntity::Update(dt);
    }

    void ArNPCEntity::UpdateBlocks(const DiVec3& pos)
    {
        return;
        auto terrain = ArGameApp::Get()->GetWorld()->GetTerrain();
        auto& pathFinder = terrain->GetPathFinder();

        DiIntVec2 vertPos = terrain->GetVertexPos(pos);
        int x = vertPos.x;
        int y = vertPos.y;

        // reset the last value
        for (int i = 0; i < 9; ++i)
        {
            if (mLastBlocksVerts[i].x > 0 && mLastBlocksVerts[i].y > 0)
            {
                pathFinder.SetLevel(mLastBlocksVerts[i].x, mLastBlocksVerts[i].y, mLastBlockValues[i]);
            }
        }

        uint32 vx, vy;
        terrain->GetVerticesNum(vx, vy);
        int offset_x[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
        int offset_y[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
        for (int i = 0; i < 9; i++)
        {
            mLastBlocksVerts[i].x = DiMath::Clamp(x + offset_x[i], 0, (int)vx - 1);
            mLastBlocksVerts[i].y = DiMath::Clamp(y + offset_y[i], 0, (int)vy - 1);
        }

        for (int i = 0; i < 9; ++i)
        {
            mLastBlockValues[i] = pathFinder.GetLevel(mLastBlocksVerts[i].x, mLastBlocksVerts[i].y);
            pathFinder.SetLevel(mLastBlocksVerts[i].x, mLastBlocksVerts[i].y, mLastBlockValues[i] + 5);
        }
    }
}