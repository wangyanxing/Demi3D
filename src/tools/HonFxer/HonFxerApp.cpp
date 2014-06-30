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
#include "HonFxerApp.h"
#include "MyGUI_FilterNoneSkin.h"
#include "MyGUI_RTTLayer.h"
#include "MyGUI_TreeControl.h"
#include "MyGUI_TreeControlItem.h"
#include "MessageBoxManager.h"
#include "DialogManager.h"
#include "Localise.h"
#include "PathLib.h"
#include "ColorManager.h"
#include "ZipArchive.h"
#include "SettingsManager.h"
#include "CommandManager.h"
#include "MainPaneControl.h"
#include "EditorManager.h"
#include "SetResLocationWindow.h"
#include "SetGameLocationWindow.h"
#include "EditorToolTip.h"

#include "EffectManager.h"

#include "K2Model.h"
#include "K2Clip.h"
#include "K2Configs.h"
#include "K2Plugin.h"
#include "ParticleSystem.h"
#include "ParticleElement.h"
#include "ParticleEmitter.h"
#include "BoxEmitter.h"
#include "ColorController.h"
#include "TextureRotatorController.h"
#include "ScaleController.h"
#include "TokensParser.h"
#include "TransGizmo.h"

DiTransGizmo* gizmo;

DiRay getRay(const OIS::MouseEvent& event)
{
    DiRenderWindow* rw = DiBase::Driver->GetMainRenderWindow();
    
    float screenPosX = float(event.state.X.abs) / float(rw->GetWidth());
    float screenPosY = float(event.state.Y.abs) / float(rw->GetHeight());
    
    screenPosX = DiMath::Clamp<float>(screenPosX, 0, 1);
    screenPosY = DiMath::Clamp<float>(screenPosY, 0, 1);
    
    auto camera = DiBase::Driver->GetSceneManager()->GetCamera();
    DiRay ray = camera->GetCameraToViewportRay(screenPosX, screenPosY);
    return ray;
}


namespace Demi
{
    
    void InitFx_Repeater01()
    {
        DiSceneManager* sm = DiBase::Driver->GetSceneManager();
        
#if 1
        // effect
        auto _ps = DiEffectManager::GetInstance().CreateParticleSystemTemplate("Fx_repeater1");
        std::shared_ptr<DiTransformUnit> ps(_ps);
        DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
        cullnode->AttachObject(ps);
        _ps->Start();
        {
            DiParticleElement* element = _ps->CreateElement();
            element->SetRenderer("Billboard");
            auto emitter = element->CreateEmitter("Box");
            
            auto mat = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR | SHADER_FLAG_USE_MAP);
            mat->GetShaderParameter()->WriteTexture2D("map", "glow_01.dds");
            mat->SetBlendMode(BLEND_ADD);
            mat->SetDepthWrite(false);
            element->SetMaterialName(mat->GetName());
            
            ((DiBoxEmitter*)emitter)->SetWidth(50);
            ((DiBoxEmitter*)emitter)->SetHeight(70);
            ((DiBoxEmitter*)emitter)->SetDepth(50);
            emitter->position = DiVec3(0, 165, 5);
            
            auto rt = DI_NEW DiAttributeFixed();
            rt->SetValue(30);
            emitter->SetDynEmissionRate(rt);
            
            auto spd = DI_NEW DiAttributeFixed();
            spd->SetValue(20);
            emitter->SetDynVelocity(spd);
            
            auto ttl = DI_NEW DiAttributeRandom();
            ttl->SetMinMax(1, 2);
            emitter->SetDynTotalTimeToLive(ttl);
            
            auto sz = DI_NEW DiAttributeRandom();
            sz->SetMinMax(20, 40);
            emitter->SetDynParticleAllDimensions(sz);
            
            DiColorController* colorCtrl = (DiColorController*)element->CreateController("Color");
            colorCtrl->AddColour(0, DiColor::Black);
            colorCtrl->AddColour(0.5f, DiColor(0.25f, 1, 0.5f));
            colorCtrl->AddColour(1, DiColor::Black);
        }
        {
            DiParticleElement* element = _ps->CreateElement();
            element->SetRenderer("Billboard");
            auto emitter = element->CreateEmitter("Box");
            
            auto mat = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR | SHADER_FLAG_USE_MAP);
            auto texture = mat->GetShaderParameter()->WriteTexture2D("map", "mysticenergy2.dds");
            mat->SetBlendMode(BLEND_ADD);
            mat->SetDepthWrite(false);
            element->SetMaterialName(mat->GetName());
            texture->SetAddressing(AM_CLAMP);
            
            ((DiBoxEmitter*)emitter)->SetWidth(50);
            ((DiBoxEmitter*)emitter)->SetHeight(70);
            ((DiBoxEmitter*)emitter)->SetDepth(50);
            emitter->position = DiVec3(0, 165, 5);
            
            auto rt = DI_NEW DiAttributeFixed();
            rt->SetValue(6);
            emitter->SetDynEmissionRate(rt);
            
            auto spd = DI_NEW DiAttributeFixed();
            spd->SetValue(20);
            emitter->SetDynVelocity(spd);
            
            auto ttl = DI_NEW DiAttributeRandom();
            ttl->SetMinMax(1, 2);
            emitter->SetDynTotalTimeToLive(ttl);
            
            auto sz = DI_NEW DiAttributeRandom();
            sz->SetMinMax(20, 40);
            emitter->SetDynParticleAllDimensions(sz);
            
            DiColorController* colorCtrl = (DiColorController*)element->CreateController("Color");
            colorCtrl->AddColour(0, DiColor::Black);
            colorCtrl->AddColour(0.5f, DiColor(0.25f, 1, 0.5f));
            colorCtrl->AddColour(1, DiColor::Black);
            
            DiTextureRotatorController* texrotCtrl = (DiTextureRotatorController*)element->CreateController("TextureRotator");
            texrotCtrl->SetUseOwnRotationSpeed(true);
            auto rotspeed = DI_NEW DiAttributeRandom();
            rotspeed->SetMinMax(-100, 100);
            texrotCtrl->SetRotationSpeed(rotspeed);
            auto rot = DI_NEW DiAttributeRandom();
            rot->SetMinMax(0, 300);
            texrotCtrl->SetRotation(rot);
        }
        
        //DiFxTokensParser parser;
        //parser.WriteSystem(_ps, "D:/Demi3D_release/ps.xml");
#else
        DiFxTokensParser parser;
        auto pss = parser.LoadEffects("ps1.effect");
        auto _ps = pss[0];
        g_fxs.push_back(_ps);
        std::shared_ptr<DiTransformUnit> ps(_ps);
        DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
        cullnode->AttachObject(ps);
        _ps->Start();
#endif
        
        DiEditorManager::Get()->LoadParticleSystem(_ps);
    }
    
    HonFxerApp::HonFxerApp()
        : DemiDemo(DemoConfig("Hon Fxer",1400, 800))
    {
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32 && !DEMI_DEBUG
        ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
    }

    HonFxerApp::~HonFxerApp()
    {
    }

    void HonFxerApp::Update()
    {
        DemiDemo::Update();
        
        gizmo->Update();

        if (mMainPane)
            mMainPane->Update();

        if (Driver)
            DiEditorManager::Get()->Update();
    }

    void HonFxerApp::Close()
    {
        DI_DELETE(DiEditorManager::Get());
        SAFE_DELETE(mMainPane);
        SAFE_DELETE(mSetResLocWindow);
        SAFE_DELETE(mSetGameLocWindow);

        DialogManager::getInstance().shutdown();
        delete DialogManager::getInstancePtr();

        MessageBoxManager::getInstance().shutdown();
        delete MessageBoxManager::getInstancePtr();

        SettingsManager::getInstance().shutdown();
        delete SettingsManager::getInstancePtr();

        ColourManager::getInstance().shutdown();
        delete ColourManager::getInstancePtr();

        CommandManager::getInstance().shutdown();
        delete CommandManager::getInstancePtr();

        MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::FilterNone>("BasisSkin");
        MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::RTTLayer>("Layer");

        DI_UNINSTALL_PLUGIN(DiK2);
        DI_UNINSTALL_PLUGIN(DiFx);
        DemiDemo::Close();
    }

    void HonFxerApp::OpenImpl()
    {
        DemiDemo::OpenImpl();
        DI_INSTALL_PLUGIN(DiFx);
        DI_INSTALL_PLUGIN(DiK2);

        Driver->GetMainRenderWindow()->SetForceRenderToCanvas(true);
        
        DiPostEffectManager* peMgr = DiBase::Driver->GetMainRenderWindow()->GetPostEffectManager();
        peMgr->SetManualOutputTarget(DiBase::Driver->GetMainRenderWindow()->GetSceneCanvas());

        DiBase::Driver->GetMainRenderWindow()->GetRenderBuffer()->SetClearColor(DiColor(0.2f, 0.2f, 0.2f));
        DiBase::Driver->GetMainRenderWindow()->GetSceneCanvas()->SetClearColor(DiColor(0.2f, 0.2f, 0.2f));

        DiSceneManager* sm = DiBase::Driver->GetSceneManager();
        sm->SetAmbientColor(DiColor(0.3f, 0.3f, 0.3f));
        DiDirLightPtr dirlight;
        dirlight = make_shared<DiDirLight>();
        DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
        dirNode->AttachObject(dirlight);
        dirlight->SetColor(DiColor());
        dirlight->SetDirection(DiVec3(0, -0.3f, -0.4).normalisedCopy());

        //////////////////////////////////////////////////////////////////////////

        MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::RTTLayer>("Layer");
        MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::FilterNone>("BasisSkin");
        
        MyGUI::ResourceManager::getInstance().load("FxEditorLayers.xml");
        MyGUI::ResourceManager::getInstance().load("FxToolbar.xml");

        new CommandManager();
        CommandManager::getInstance().initialise();

        DiString userSettings = DiPathLib::GetApplicationPath() + "FxSettings.xml";
        new SettingsManager();
        SettingsManager::getInstance().initialise(userSettings.c_str());

        std::string mLocale = "English";
        MyGUI::LanguageManager::getInstance().setCurrentLanguage(mLocale);

        new MessageBoxManager();
        MessageBoxManager::getInstance().initialise();

        new ColourManager();
        ColourManager::getInstance().initialise();

        MyGUI::ResourceManager::getInstance().load("FxInitialise.xml");

        MyGUI::FactoryManager& factory = MyGUI::FactoryManager::getInstance();
        factory.registerFactory<MyGUI::TreeControl>("Widget");
        factory.registerFactory<MyGUI::TreeControlItem>("Widget");
        MyGUI::ResourceManager::getInstance().load("TreeControlSkin.xml");
        MyGUI::ResourceManager::getInstance().load("TreeItemIcons.xml");

        CommandManager::getInstance().registerCommand("Command_Quit", MyGUI::newDelegate(this, &HonFxerApp::Command_QuitApp));
        CommandManager::getInstance().registerCommand("Command_Export", MyGUI::newDelegate(this, &HonFxerApp::Command_Export));
        CommandManager::getInstance().registerCommand("Command_ResLocation", MyGUI::newDelegate(this, &HonFxerApp::Command_ResLocation));
        CommandManager::getInstance().registerCommand("Command_GameLocation", MyGUI::newDelegate(this, &HonFxerApp::Command_GameLocation));
        CommandManager::getInstance().registerCommand("Command_ViewHelp", MyGUI::newDelegate(this, &HonFxerApp::Command_ViewHelp));

        mMainPane = new MainPaneControl();

        new DialogManager();
        DialogManager::getInstance().initialise();

        DI_NEW DiEditorManager();

#if 0
        DiBase::CommandMgr->ExecuteCommand("selectLast");
        
        DiBase::CommandMgr->ExecuteCommand("createChild ParticleSystem");
        DiBase::CommandMgr->ExecuteCommand("selectLast");
        
        DiBase::CommandMgr->ExecuteCommand("createChild ParticleElement");
        DiBase::CommandMgr->ExecuteCommand("selectLast");
        
        DiBase::CommandMgr->ExecuteCommand("createChild PointEmitter");
        DiBase::CommandMgr->ExecuteCommand("selectLast");
#endif
        
        mSetResLocWindow = new SetResLocWindow();
        mSetResLocWindow->eventEndDialog = MyGUI::newDelegate(this, &HonFxerApp::NotifySetResLocWindowEndDialog);
        
        mSetGameLocWindow = new SetGameLocWindow();
        mSetGameLocWindow->eventEndDialog = MyGUI::newDelegate(this, &HonFxerApp::NotifySetGameLocWindowEndDialog);
        
        InitFx_Repeater01();
        
        gizmo = DI_NEW DiTransGizmo();
        gizmo->SetGizmoMode(Demi::DiTransGizmo::GIZMO_MOVE);
        
        DemiDemo::GetApp()->GetInputManager()->registerMousePressEvent("_input",
            [&](const OIS::MouseEvent& evt, OIS::MouseButtonID id){
                if(id == OIS::MB_Left)
                {
                    auto ray = getRay(evt);
                    gizmo->RayPick(ray);
                }
            });
    }

    HonFxerApp* HonFxerApp::GetFxApp()
    {
        return (HonFxerApp*)sTheApp;
    }

    void HonFxerApp::mouseMoved(const OIS::MouseEvent& evt)
    {
        bool incanvas = mMainPane->GetWorkspaceControl()->GetRenderWndControl()->mouseInCanvas();
        if (incanvas)
        {
            DemiDemo::mouseMoved(evt);
        }
    }

    void HonFxerApp::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        bool incanvas = mMainPane->GetWorkspaceControl()->GetRenderWndControl()->mouseInCanvas();
        if (incanvas)
        {
            DemiDemo::mousePressed(evt,id);
        }
    }

    void HonFxerApp::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        bool incanvas = mMainPane->GetWorkspaceControl()->GetRenderWndControl()->mouseInCanvas();
        if (incanvas)
        {
            DemiDemo::mouseReleased(evt, id);
        }
    }

    void HonFxerApp::Command_QuitApp(const MyGUI::UString& _commandName, bool& _result)
    {
        if (DialogManager::getInstance().getAnyDialog())
            DialogManager::getInstance().endTopDialog();
        else
        {
            if (MessageBoxManager::getInstance().hasAny())
                MessageBoxManager::getInstance().endTop(MyGUI::MessageBoxStyle::Cancel);
            else
                mQuit = true;
        }

        _result = true;
    }

    void HonFxerApp::Command_GameLocation(const MyGUI::UString& _commandName, bool& _result)
    {
        if (!CheckCommand())
            return;

        mSetGameLocWindow->doModal();

        _result = true;
    }

    void HonFxerApp::keyPressed(const OIS::KeyEvent &arg)
    {
        mCameraHelper->OnKeyDown(arg);
    }

    void HonFxerApp::Command_Export(const MyGUI::UString& _commandName, bool& _result)
    {
        MyGUI::Message::createMessageBox("Message", "Sorry",
            "This feature hasn't been implemented yet.", 
            MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconInfo);
    }

    void HonFxerApp::Command_ResLocation(const MyGUI::UString& _commandName, bool& _result)
    {
        if (!CheckCommand())
            return;

        mSetResLocWindow->doModal();

        _result = true;
    }

    void HonFxerApp::NotifySetResLocWindowEndDialog(Dialog* _dialog, bool _result)
    {
        MYGUI_ASSERT(mSetResLocWindow == _dialog, "mSetResLocWindow == _sender");

        if (_result)
            mSetResLocWindow->saveSettings();
    
        mSetResLocWindow->endModal();
    }

    bool HonFxerApp::CheckCommand()
    {
        if (DialogManager::getInstance().getAnyDialog())
            return false;

        if (MessageBoxManager::getInstance().hasAny())
            return false;

        return true;
    }

    void HonFxerApp::NotifySetGameLocWindowEndDialog(Dialog* _dialog, bool _result)
    {
        MYGUI_ASSERT(mSetGameLocWindow == _dialog, "mSetGameLocWindow == _sender");

        if (_result)
            mSetGameLocWindow->saveSettings();

        mSetGameLocWindow->endModal();
    }

    void HonFxerApp::SetResourceLocation(const DiString& resPack, const DiString& texPack)
    {
        if (!texPack.empty())
            DiEditorManager::Get()->SetK2ResourcePack(resPack, texPack);
        else
            DiEditorManager::Get()->SetK2ResourcePack(resPack);
    }

    void HonFxerApp::Command_ViewHelp(const MyGUI::UString& _commandName, bool& _result)
    {
        if (!CheckCommand())
            return;

        _result = true;
    }
}