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
#include "ArenaGUIProperty.h"
#include "ArenaStateProperty.h"
#include "K2Terrain.h"
#include "K2World.h"
#include "K2Model.h"
#include "ArenaGameApp.h"
#include "ArenaGameEntity.h"
#include "ArenaNPCEntity.h"
#include "K2RenderObjects.h"

#define HP_BAR_WIDTH 80
#define HP_BAR_HEIGHT 9

namespace Demi
{
    ArGUIProperty::ArGUIProperty()
    {
        InitHPBar();
    }

    ArGUIProperty::~ArGUIProperty()
    {
        if (mHPBar)
        {
            MyGUI::Gui::getInstance().destroyWidget(mHPBar);
            mHPBar = nullptr;
        }
    }
    
    void ArGUIProperty::InitHPBar()
    {
        mHPBar = createIcon("hpslotframe.png", DiVec2(0, 0), DiVec2(HP_BAR_WIDTH, HP_BAR_HEIGHT));
        auto insidehp = createIcon("hpslot.png", DiVec2(1, 1), DiVec2(HP_BAR_WIDTH-2, HP_BAR_HEIGHT-2), mHPBar);
        insidehp->setCoord(1, 1, 80, 7);
    }

    void ArGUIProperty::Update(float dt)
    {
        ArProperty::Update(dt);
        
        auto renderObj = GetEntity<ArNPCEntity>()->GetRenderObj();
        auto model = renderObj->GetModel();
        if (!model || !model->GetVisible())
        {
            mHPBar->setVisible(false);
            return;
        }
        else
        {
            mHPBar->setVisible(true);
        }
        
        auto pos = renderObj->GetWorldPosition();
        pos.y += model->GetWorldBoundingBox().GetSize().y * 0.9;
        
        DiCamera* cam = DiBase::Driver->GetSceneManager()->GetCamera();
        DiMat4 p = cam->GetProjectionMatrix();
        DiMat4 v = cam->GetViewMatrix();
        DiMat4 vp = p * v;
        pos = vp * pos;
        pos.x = (pos.x + 1.0f) * 0.5f;
        pos.y = (pos.y + 1.0f) * 0.5f;
        auto width = DiBase::Driver->GetMainRenderWindow()->GetWidth();
        auto height = DiBase::Driver->GetMainRenderWindow()->GetHeight();
        mHPBar->setPosition(pos.x * width - HP_BAR_WIDTH/2,(1.0f-pos.y)*height);
    }
    
    MyGUI::ImageBox* ArGUIProperty::createIcon(const DiString& texture, DiVec2 pos, DiVec2 size, MyGUI::Widget* parent)
    {
        if (parent)
        {
            auto image = parent->createWidget<MyGUI::ImageBox>("ImageBox",
                                                               MyGUI::IntCoord(pos.x, pos.y, size.x, size.y),
                                                               MyGUI::Align::Stretch, "Back");
            image->setImageTexture(texture.c_str());
            return image;
        }
        else
        {
            auto image = MyGUI::Gui::getInstance().createWidget<MyGUI::ImageBox>("ImageBox",
                                                                    MyGUI::IntCoord(pos.x, pos.y, size.x, size.y),
                                                                    MyGUI::Align::Stretch, "Back");
            image->setImageTexture(texture.c_str());
            return image;
        }
    }
}