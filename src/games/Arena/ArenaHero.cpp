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
            
            DiRenderWindow* rw = Driver->GetMainRenderWindow();

            float screenPosX = float(event.state.X.abs) / float(rw->GetWidth());
            float screenPosY = float(event.state.Y.abs) / float(rw->GetHeight());

            screenPosX = DiMath::Clamp<float>(screenPosX, 0, 1);
            screenPosY = DiMath::Clamp<float>(screenPosY, 0, 1);

            auto camera = Driver->GetSceneManager()->GetCamera();
            DiRay ray = camera->GetCameraToViewportRay(screenPosX, screenPosY);
            DiVec3 clickout;
            if (terrain->RayIntersects(ray, clickout))
            {
                DiK2Pos k2pos;
                k2pos.FromWorldPos(clickout);
                DiK2Pos source = mRenderObj->GetPosition();
                GetMoveProperty()->MoveTo(source, k2pos);
            }
        }
    }

    void ArHeroEntity::InitAttribute()
    {
        SetAttribute<DiK2HeroAttr>();
    }

    void ArHeroEntity::LoadHero(const DiString& configfile)
    {
        auto file = DiAssetManager::GetInstance().OpenArchive(configfile, true);
        if (!file)
            return;

        auto level = ArGameApp::Get()->GetLevel();

        shared_ptr<DiXMLFile> xmlfile(new DiXMLFile());
        xmlfile->Load(file->GetAsString());
        DiXMLElement root = xmlfile->GetRoot();

        ArConfigHeroLoader loader(&mHeroConfig);
        loader.Load(root);

        SetModel(mHeroConfig.model.path);

        DiVec3 position = level->GetSpwanPosition();
        GetRenderObj()->SetWorldPosition(position);

        GetRenderObj()->SetScale(DiVec3(mHeroConfig.model.scale, mHeroConfig.model.scale, mHeroConfig.model.scale));

        GetMoveProperty()->SetSpeed(mHeroConfig.motion.runspeed);
        GetMoveProperty()->SetTurnSpeed(mHeroConfig.motion.turnspeed);
    }
}