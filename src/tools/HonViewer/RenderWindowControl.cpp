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
#include "K2ModelView.h"

namespace tools
{
#define MAX_BONE_NAME_NUMS 100

    RenderWindowControl::RenderWindowControl(MyGUI::Widget* _parent) :
        wraps::BaseLayout("RenderWindow.layout", _parent),
        mCanvas(nullptr),
        mKeyFrameBar(nullptr)
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

        for (int i = 0; i < MAX_BONE_NAME_NUMS; ++i)
        {
            MyGUI::TextBox* bonename = mCanvas->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(0, 10, 150, 15), MyGUI::Align::Default);
            bonename->setTextAlign(MyGUI::Align::Center);
            bonename->setTextShadow(true);
            bonename->setTextShadowColour(MyGUI::Colour::Black);
            bonename->setVisible(false);
            mBoneNames.push_back(bonename);
        }

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

    void RenderWindowControl::updateInfo()
    {
        auto model = HonViewerApp::GetViewerApp()->GetModelViewer();
        if (!model)
            return;

        int numVerts = model->GetNumVertices();
        int numFaces = model->GetNumFaces();
        int numSubMods = model->GetNumSubModels();
        int numBones = model->GetNumBones();

        DiString info;
        info.Format("%d\n%d\n%d\n%d", numVerts, numFaces, numSubMods, numBones);
        mInfo->setCaption(info.c_str());
    }

    void RenderWindowControl::updateBoneNames(bool show)
    {
        auto model = HonViewerApp::GetViewerApp()->GetModelViewer();
        if (!model)
            return;
        
        DiK2ModelPtr m = model->GetModel();
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

} // namespace tools
