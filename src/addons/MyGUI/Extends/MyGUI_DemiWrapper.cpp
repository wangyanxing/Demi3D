
#include "MyGUI_Precompiled.h"
#include "MyGUI_DemiWrapper.h"
#include "MyGUI.h"
#include "RenderPipeline.h"
#include "RenderBatchGroup.h"
#include "AssetManager.h"
#include "MyGUI_DemiPlatform.h"

#include "GfxDriver.h"
#include "RenderWindow.h"

namespace MyGUI
{
    DemiWrapper::DemiWrapper()
    {
    }

    DemiWrapper::~DemiWrapper()
    {
        destroyGui();
        Driver->RemoveDeviceLostListener(this);
    }

    void DemiWrapper::init(const std::string& resflename)
    {
        mResourceFileName = resflename;
        Demi::DiRenderBatchGroup* group = DiBase::Driver->GetPipeline()->GetBatchGroup(Demi::BATCH_GUI);
        group->SetExtraProcess([this](){
            mPlatform->getRenderManagerPtr()->drawOneFrame();
        });

        Driver->AddDeviceLostListener(this);

        createGui();
    }

    void DemiWrapper::createGui()
    {
        mPlatform = new MyGUI::DemiPlatform();
        mPlatform->initialise();

        setupResources();

        mGUI = new MyGUI::Gui();
        mGUI->initialise(mResourceFileName);

        PointerManager::getInstance().setVisible(false);
    }

    void DemiWrapper::destroyGui()
    {
        if (mGUI)
        {
            mGUI->shutdown();
            delete mGUI;
            mGUI = nullptr;
        }

        if (mPlatform)
        {
            mPlatform->shutdown();
            delete mPlatform;
            mPlatform = nullptr;
        }
    }

    void DemiWrapper::setupResources()
    {
    }

    void DemiWrapper::OnDeviceReset()
    {
        uint32 w = Driver->GetMainRenderWindow()->GetWidth();
        uint32 h = Driver->GetMainRenderWindow()->GetHeight();
        mPlatform->getRenderManagerPtr()->setViewSize(w, h);
    }
}