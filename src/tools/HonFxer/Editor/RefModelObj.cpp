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
#include "DebugHelper.h"
#include "HonFxerApp.h"
#include "MainPaneControl.h"
#include "RenderWindowControl.h"

#define MAX_BONE_NAME_NUMS 100

namespace Demi
{
    DiRefModelObj::DiRefModelObj()
    {
        DiMaterialPtr dbgHelperMat = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
        dbgHelperMat->SetDepthCheck(false);
        mSkeleton = make_shared<DiDebugHelper>();
        mSkeleton->SetMaterial(dbgHelperMat);
        mSkeleton->SetVisible(false);
        
        auto* renderWnd = HonFxerApp::GetFxApp(
        )->GetMainPane()->GetWorkspaceControl()->GetRenderWndControl();
        auto canvas = renderWnd->getCanvas();
        
        for (int i = 0; i < MAX_BONE_NAME_NUMS; ++i)
        {
            MyGUI::TextBox* bonename = canvas->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(0, 10, 150, 15), MyGUI::Align::Default);
            bonename->setTextAlign(MyGUI::Align::Center);
            bonename->setTextShadow(true);
            bonename->setTextShadowColour(MyGUI::Colour::Black);
            bonename->setVisible(false);
            mBoneNames.push_back(bonename);
        }
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
        
        mSceneNode->AttachObject(mSkeleton);
    }
    
    void DiRefModelObj::OnCreate(const DiAny& param)
    {
        DiBaseEditorObj::OnCreate();
        
        DiXMLElement nd = any_cast<DiXMLElement>(param);
        Load(nd);
        
        mName = DiEditorManager::Get()->GenerateRefModelName();
        if(!mModel)
        {
            DI_ERROR("Cannot init the DiRefModelObj with a null k2 model pointer");
            return;
        }
        
        mSceneNode->AttachObject(mSkeleton);
    }

    void DiRefModelObj::OnDestroy()
    {
        mSceneNode->DetachObject(mModel);
        mModel.reset();
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
        
        if(mModel->GetAnimation())
            mModel->GetAnimation()->Play(K2PrefabClip::ANIM_IDLE);
        
        DiEditorManager::Get()->TriggerEvent("AttachRefModel");
    }

    void DiRefModelObj::Update(float dt)
    {
        DiBaseEditorObj::Update(dt);
        
        if(mModel)
        {
            mModel->UpdateAnimation(dt);
            UpdateSkeletonHelper();
            UpdateBoneNames(mShowBones);
        }
    }
    
    void DiRefModelObj::PostCreate()
    {
        DiEditorManager::Get()->TriggerEvent("RefModel");
    }
    
    void DiRefModelObj::Load(const DiXMLElement& node)
    {
        DiString model = node.GetAttribute("model");
        DiVec3 pos = node.GetVector3("pos");
        DiQuat rot = node.GetQuaternion("rotation");
        
        SetModel(model);
        SetPosition(pos);
        SetRotation(rot);
    }
    
    void DiRefModelObj::Save(DiXMLElement& node)
    {
        auto nd = node.CreateChild("ReferenceModel");
        nd.SetAttribute("model", mModel ? mModel->GetName() : DiString::BLANK);
        nd.SetVector3("pos", mSceneNode->GetPosition());
        nd.SetQuaternion("rotation", mSceneNode->GetOrientation());
    }
    
    void DiRefModelObj::ShowBones(bool show)
    {
        mShowBones = show;
        mSkeleton->SetVisible(mShowBones);
    }
    
    void DiRefModelObj::UpdateSkeletonHelper()
    {
        if (!mSkeleton || !mSkeleton->GetVisible())
            return;
        
        mSkeleton->Clear();
        auto skel = mModel->GetSkeleton();
        
        uint32 numBons = skel->GetNumBones();
        for (uint32 i = 0; i < numBons; ++i)
        {
            const DiNode* b = skel->GetBone(i);
            DiNode* p = b->GetParent();
            DiVec3 pos = b->GetDerivedPosition();
            DiVec3 posParent = p ? (p->GetDerivedPosition()) : pos;
            mSkeleton->AddLine(pos, posParent, DiColor::Red);
            mSkeleton->AddBoundingBox(pos, 0.2f, DiColor::Yellow);
        }
    }
    
    void DiRefModelObj::UpdateBoneNames(bool show)
    {
        DiK2ModelPtr m = mModel;
        if (!m || !m->GetSkeleton())
            return;
        
        uint32 bones = show ? m->GetSkeleton()->GetNumBones() : 0;
        for (uint32 i = 0; i < bones; ++i)
        {
            if (i >= MAX_BONE_NAME_NUMS)
                break;
            
            mBoneNames[i]->setVisible(true);
            
            DiNode* node = m->GetSkeleton()->GetBone(i);
            
            DiVec3 pos = node->GetDerivedPosition();
            DiCamera* cam = DiBase::Driver->GetSceneManager()->GetCamera();
            DiMat4 p = cam->GetProjectionMatrix();
            DiMat4 v = cam->GetViewMatrix();
            DiMat4 vp = p * v;
            pos = vp * pos;
            pos.x = (pos.x + 1.0f) * 0.5f;
            pos.y = (pos.y + 1.0f) * 0.5f;
            mBoneNames[i]->setRealPosition(pos.x,1.0f-pos.y);
            auto intpos = mBoneNames[i]->getPosition();
            auto intsize = mBoneNames[i]->getSize();
            intpos.left -= intsize.width / 2;
            intpos.top -= intsize.height / 2;
            mBoneNames[i]->setPosition(intpos);
            
            mBoneNames[i]->setCaption(node->GetName().c_str());
        }
        for (uint32 i = bones; i < MAX_BONE_NAME_NUMS; ++i)
        {
            mBoneNames[i]->setVisible(false);
        }
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
        
        g->AddProperty("Show Bones"      , DI_NEW DiBoolProperty( [&]{ return mShowBones; },
                                                                  [&](bool& v){ ShowBones(v); }));
        
        g->CreateUI();
        
        mPropGroups.push_back(g);
    }
}