/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/

#include "ViewerPch.h"
#include "RenderWindowControl.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_DemiTexture.h"

namespace tools
{
    RenderWindowControl::RenderWindowControl(MyGUI::Widget* _parent) :
        wraps::BaseLayout("RenderWindow.layout", _parent),
        mCanvas(nullptr)
    {
        mCanvas = mMainWidget->createWidget<MyGUI::Canvas>("Canvas",
            MyGUI::IntCoord(0, 0, mMainWidget->getClientCoord().width, mMainWidget->getClientCoord().height), MyGUI::Align::Stretch);

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

} // namespace tools
