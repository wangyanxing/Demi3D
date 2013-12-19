
#include "MyGUI_Precompiled.h"
#include "MyGUI_DemiWrapper.h"
#include "MyGUI.h"
#include "RenderPipeline.h"
#include "RenderBatchGroup.h"
#include "AssetManager.h"
#include "Callback.h"
#include "MyGUI_DirectXPlatform.h"
#include "D3D9Driver.h"

namespace MyGUI
{
    DemiWrapper::DemiWrapper()
        :mD3D9Device(nullptr)
    {

    }

    DemiWrapper::~DemiWrapper()
    {
        destroyGui();
    }

    void DemiWrapper::init(const std::string& resflename)
    {
        mResourceFileName = resflename;
        Demi::DiD3D9Driver* driver = static_cast<DiD3D9Driver*>(DiBase::Driver);
        mD3D9Device = DiD3D9Driver::Device;

        Demi::DiRenderBatchGroup* group = DiBase::Driver->GetPipeline()->GetBatchGroup(Demi::BATCH_GUI);
        group->SetExtraProcess(Demi::functor(*this, &DemiWrapper::render));

        createGui();
    }

    void DemiWrapper::addResourceLocation(const std::string& _name, bool _recursive)
    {
        mPlatform->getDataManagerPtr()->addResourceLocation(_name, _recursive);
    }

    void DemiWrapper::createGui()
    {
        mPlatform = new MyGUI::DirectXPlatform();
        mPlatform->initialise((IDirect3DDevice9*)mD3D9Device);

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

    void DemiWrapper::render()
    {
        mPlatform->getRenderManagerPtr()->drawOneFrame();
    }

    void DemiWrapper::setupResources()
    {
        DiString basePath = Demi::DiAssetManager::GetInstance().GetBasePath() + "ui";
        addResourceLocation(basePath.c_str(),true);
    }
}