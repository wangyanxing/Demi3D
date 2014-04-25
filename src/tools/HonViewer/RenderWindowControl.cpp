/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/

#include "ViewerPch.h"
#include "RenderWindowControl.h"
#include "KeyFrameBar.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_DemiTexture.h"
#include "InputManager.h"
#include "HonViewerApp.h"
#include "CameraHelper.h"

namespace tools
{
    RenderWindowControl::RenderWindowControl(MyGUI::Widget* _parent) :
        wraps::BaseLayout("RenderWindow.layout", _parent),
        mCanvas(nullptr),
        mKeyFrameBar(nullptr)
    {
        mCanvas = mMainWidget->createWidget<MyGUI::Canvas>("Canvas",
            MyGUI::IntCoord(0, 0, mMainWidget->getClientCoord().width, mMainWidget->getClientCoord().height-51), MyGUI::Align::Stretch);

        mKeyFrameBar = new KeyFrameBarControl(mMainWidget);

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
        delete mKeyFrameBar;
        mKeyFrameBar = nullptr;
	}

    void RenderWindowControl::onUpdateCanvas(MyGUI::Canvas* canvas, MyGUI::Canvas::Event evt)
    {
        if (evt.widgetResized)
        {
            DiBase::Driver->GetMainRenderWindow()->SetCustomizedCanvasSize(canvas->getWidth(), canvas->getHeight());
            mKeyFrameBar->updateScales();
        }
    }

    void RenderWindowControl::update()
    {
        mKeyFrameBar->update();
    }

    bool RenderWindowControl::mouseInCanvas()
    {
        MyGUI::IntPoint point = MyGUI::InputManager::getInstance().getMousePosition() - mCanvas->getAbsolutePosition();
        if (point.left > 0 && point.top > 0 && 
            point.left < mCanvas->getSize().width &&
            point.top < mCanvas->getSize().height)
        {
            return true;
        }
        return false;
    }

} // namespace tools
