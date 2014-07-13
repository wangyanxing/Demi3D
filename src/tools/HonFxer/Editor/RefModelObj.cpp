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

#include "FxerPch.h"
#include "RefModelObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "K2Model.h"
#include "PropertyTypes.h"

namespace Demi
{
    DiRefModelObj::DiRefModelObj()
    {
    }

    DiRefModelObj::~DiRefModelObj()
    {
    }

    void DiRefModelObj::OnMenuPopup(MyGUI::PopupMenu* menu, bool multiSelection)
    {
        DiEditorManager::Get()->SetCurrentSelection(this);

        menu->removeAllItems();
        menu->addItem("Delete", MyGUI::MenuItemType::Normal, "removeObj");
    }

    void DiRefModelObj::OnCreate()
    {
        DiBaseEditorObj::OnCreate();
        
        mName = DiEditorManager::Get()->GenerateRefModelName();
        //mSceneNode->AttachObject(mModel);
    }
    
    void DiRefModelObj::OnCreate(const DiAny& param)
    {
        DiBaseEditorObj::OnCreate();
        
        mModel = any_cast<DiK2ModelPtr>(param);
        DI_ASSERT(mModel);
        
        mName = mModel->GetName();
        if(!mModel)
        {
            DI_ERROR("Cannot init the DiRefModelObj with a null k2 model pointer");
            return;
        }
        
        mSceneNode->AttachObject(mModel);
    }

    void DiRefModelObj::OnDestroy()
    {
    }

    void DiRefModelObj::OnSelect()
    {
        DiBaseEditorObj::OnSelect();
    }

    DiString DiRefModelObj::GetUICaption()
    {
        return mName;
    }
    
    void DiRefModelObj::SetModel(const DiString& file)
    {
        mSceneNode->DetachObject(mModel);
        mModel = make_shared<DiK2Model>(file);
        mSceneNode->AttachObject(mModel);
    }

    void DiRefModelObj::Update(float dt)
    {
        DiBaseEditorObj::Update(dt);
    }
    
    void DiRefModelObj::InitPropertyTable()
    {
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Reference Model");
        
        g->AddProperty("Name"            , DI_NEW DiStringProperty([&]{ return mName; },
                                                                   [&](DiString& val){ mName = val; }));
        
        DiEditProperty* prop = g->AddProperty("Position", DI_NEW DiVec3Property([&]{ return mSceneNode->GetPosition(); },
                                                                   [&](DiVec3& val){
                                                                        mSceneNode->SetPosition(val);
                                                                        NotifyTransfromUpdate();
                                                                    }));
        mPositionProp = static_cast<DiVec3Property*>(prop->mProperty);
        
        g->AddProperty("Model"           , DI_NEW DiModelProperty( [&]{ return mModel ? mModel->GetName() : DiString::BLANK; },
                                                                   [&](DiString& v){ SetModel(v); }));
        
        g->CreateUI();
        
        mPropGroups.push_back(g);
    }
}