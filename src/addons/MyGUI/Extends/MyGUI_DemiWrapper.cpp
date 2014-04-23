
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

        PointerManager::getInstance().setVisible(false);

        const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
        root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("Demo of rendering camera view into widget and mesh into widget (you can drag it using mouse).");

        const MyGUI::IntSize& size = MyGUI::RenderManager::getInstance().getViewSize();

        MyGUI::Window* window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(10, size.height - 10 - 230, 300, 230), MyGUI::Align::Default, "Overlapped");
        window->setCaption("Camera view");
        window->setMinSize(MyGUI::IntSize(100, 100));
        MyGUI::Canvas* canvas = window->createWidget<MyGUI::Canvas>("Canvas", MyGUI::IntCoord(0, 0, window->getClientCoord().width, window->getClientCoord().height), MyGUI::Align::Stretch);

        MyGUI::Window* window2 = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(size.width - 10 - 300, 10, 300, 230), MyGUI::Align::Default, "Overlapped");
        window2->setCaption("Model view");
        window2->setMinSize(MyGUI::IntSize(100, 100));
        MyGUI::Canvas* canvas2 = window2->createWidget<MyGUI::Canvas>("Canvas", MyGUI::IntCoord(0, 0, window2->getClientCoord().width, window2->getClientCoord().height), MyGUI::Align::Stretch);
        canvas2->setPointer("hand");

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