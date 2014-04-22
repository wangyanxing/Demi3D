
#include "MyGUI_Precompiled.h"
#include "MyGUI_DemiWrapper.h"
#include "MyGUI.h"
#include "RenderPipeline.h"
#include "RenderBatchGroup.h"
#include "AssetManager.h"
#include "MyGUI_DemiPlatform.h"
#include "GfxDriver.h"

namespace MyGUI
{
    DemiWrapper::DemiWrapper()
    {
    }

    DemiWrapper::~DemiWrapper()
    {
        destroyGui();
    }

    void DemiWrapper::init(const std::string& resflename)
    {
        mResourceFileName = resflename;
        Demi::DiRenderBatchGroup* group = DiBase::Driver->GetPipeline()->GetBatchGroup(Demi::BATCH_GUI);
        group->SetExtraProcess([this](){
            mPlatform->getRenderManagerPtr()->drawOneFrame();
        });

        createGui();
    }

    void DemiWrapper::createGui()
    {
        mPlatform = new MyGUI::DemiPlatform();
        mPlatform->initialise();

        setupResources();

        mGUI = new MyGUI::Gui();
        mGUI->initialise(mResourceFileName);
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
}