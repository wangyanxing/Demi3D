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
#include "ArenaMoveProperty.h"
#include "ArenaGameApp.h"
#include "ArenaHero.h"
#include "ArenaGame.h"
#include "ArenaConfigsLoader.h"
#include "ArenaLevel.h"

#include "K2Clip.h"
#include "K2Terrain.h"
#include "K2World.h"
#include "K2RenderObjects.h"

#include "CullNode.h"
#include "GfxDriver.h"
#include "SceneManager.h"
#include "RenderWindow.h"
#include "XMLFile.h"
#include "AssetManager.h"

namespace Demi
{
    ArHeroEntity::ArHeroEntity()
    {
    }

    ArHeroEntity::~ArHeroEntity()
    {
    }

    void ArHeroEntity::OnKeyInput(const K2KeyEvent& event)
    {
    }

    void ArHeroEntity::OnMouseInput(const K2MouseEvent& event)
    {
#ifdef DEMI_KEYMOUSE
        if (event.button == OIS::MB_Left && event.type == K2MouseEvent::MOUSE_PRESS)
#elif defined(DEMI_TOUCH)
        if (event.type == K2MouseEvent::MOUSE_PRESS)
#endif
        {
            // click on map
            auto terrain = ArGameApp::Get()->GetWorld()->GetTerrain();
            
            DiRay ray = ArInput::GetPickupRay(event);

            DiVec3 clickout;
            if (terrain->RayIntersects(ray, clickout))
            {
                DiK2Pos k2pos;
                k2pos.FromWorldPos(clickout);
                DiK2Pos source = mRenderObj->GetPosition();
                GetMoveProperty()->MoveTo(source, k2pos);
            }

            // test pickup
            DiTransUnitPtr out;
            if (Driver->GetSceneManager()->GetSceneCuller()->RayQuery(ray, out, QUERY_CHARACTER))
            {
                DI_DEBUG("Pickuped something");
            }
        }
    }

    void ArHeroEntity::InitAttribute()
    {
        SetAttribute<ArHeroAttr>();
    }

    void ArHeroEntity::SetupAttribute()
    {
        auto attr = GetAttribute<ArHeroAttr>();
        SetModel(attr->mHeroConfig->model);
        SetupEntityConfig(attr->GetEntityConfig());
    }
}