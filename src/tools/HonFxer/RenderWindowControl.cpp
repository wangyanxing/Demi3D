/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/

#include "FxerPch.h"
#include "RenderWindowControl.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_DemiTexture.h"
#include "InputManager.h"
#include "HonFxerApp.h"
#include "CameraHelper.h"

namespace tools
{
#define MAX_BONE_NAME_NUMS 100

    RenderWindowControl::RenderWindowControl(MyGUI::Widget* _parent) :
        wraps::BaseLayout("RenderWindow.layout", _parent)
    {
        mCanvas = mMainWidget->createWidget<MyGUI::Canvas>("Canvas",
            MyGUI::IntCoord(0, 0, mMainWidget->getClientCoord().width, mMainWidget->getClientCoord().height-51), MyGUI::Align::Stretch);

        mInfo = mCanvas->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(100, 10, 100, 70), MyGUI::Align::Default);
        mInfo->setTextShadow(true);
        mInfo->setTextShadowColour(MyGUI::Colour::Black);
        mInfoStatic = mCanvas->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(10, 10, 80, 70), MyGUI::Align::Default);
        mInfoStatic->setTextShadow(true);
        mInfoStatic->setTextShadowColour(MyGUI::Colour::Black);
        mInfoStatic->setCaption("Vertices :\nFaces:\nSub Models:\nBones:");
        updateInfo();

        uint32 width = (uint32)mCanvas->getWidth();
        uint32 height = (uint32)mCanvas->getHeight();
        DiBase::Driver->GetMainRenderWindow()->SetCustomizedCanvasSize(width, height);

        MyGUI::RenderManager& render = MyGUI::RenderManager::getInstance();
        DiTexturePtr canvasTexture = DiBase::Driver->GetMainRenderWindow()->GetCanvasTexture();
        MyGUI::DemiTexture* tex = static_cast<MyGUI::DemiTexture*>(render.createTexture(canvasTexture->GetName().c_str()));
        tex->loadFromDemiTexture(canvasTexture->GetName().c_str());

        mCanvas->createTexture(tex->getName());
        mCanvas->requestUpdateCanvas = MyGUI::newDelegate(this, &RenderWindowControl::onUpdateCanvas);
	}

    RenderWindowControl::~RenderWindowControl()
	{
	}

    void RenderWindowControl::onUpdateCanvas(MyGUI::Canvas* canvas, MyGUI::Canvas::Event evt)
    {
        if (evt.widgetResized)
        {
            DiBase::Driver->GetMainRenderWindow()->SetCustomizedCanvasSize(canvas->getWidth(), canvas->getHeight());
        }
    }

    void RenderWindowControl::update()
    {
    }

    bool RenderWindowControl::mouseInCanvas()
    {
        return mCanvas->getRootMouseFocus();
    }

    void RenderWindowControl::updateInfo()
    {
    }

} // namespace tools
