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

#include "K2Pch.h"
#include "K2Hero.h"
#include "K2Game.h"
#include "K2Clip.h"
#include "K2TerrainMap.h"
#include "K2GameApp.h"
#include "K2World.h"
#include "K2MoveProperty.h"
#include "K2RenderObjects.h"
#include "K2HeroAttribute.h"

#include "CullNode.h"
#include "GfxDriver.h"
#include "SceneManager.h"

namespace Demi
{
    DiK2HeroEntity::DiK2HeroEntity()
    {
    }

    DiK2HeroEntity::~DiK2HeroEntity()
    {
    }

    void DiK2HeroEntity::OnKeyInput(const K2KeyEvent& event)
    {
    }

    void DiK2HeroEntity::OnMouseInput(const K2MouseEvent& event)
    {
        if (event.button == OIS::MB_Left && event.type == K2MouseEvent::MOUSE_PRESS)
        {
            // click on map
            auto terrain = DiK2GameApp::Get()->GetWorld()->GetTerrain();

            float screenPosX = float(event.state.X.abs) / float(event.state.width);
            float screenPosY = float(event.state.Y.abs) / float(event.state.height);

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

    void DiK2HeroEntity::InitAttribute()
    {
        SetAttribute<DiK2HeroAttr>();
    }

}