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

#include "ViewerPch.h"
#include "KeyFrameBar.h"
#include "K2ModelView.h"
#include "HonViewerApp.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_DemiTexture.h"

namespace tools
{
    static const float FRAME_SCALE_WIDTH = 79.0f;

    KeyFrameBarControl::KeyFrameBarControl(MyGUI::Widget* _parent)
        : mKeyFrameBar(nullptr)
        , mPlayButton(nullptr)
        , mKeyFrameButtonBar(nullptr)
        , mNumFrames(0)
        , mScale(1)
    {
        mKeyFrameBar = _parent->createWidget<MyGUI::ScrollBar>("KeyFrameBar", MyGUI::IntCoord(20, _parent->getClientCoord().height - 25,
            _parent->getClientCoord().width - 20, 20), MyGUI::Align::HStretch | MyGUI::Align::Bottom);
        mKeyFrameButtonBar = _parent->createWidget<MyGUI::ScrollBar>("KeyFrameButtonBar", MyGUI::IntCoord(0, _parent->getClientCoord().height - 50,
            _parent->getClientCoord().width, 21), MyGUI::Align::HStretch | MyGUI::Align::Bottom);
        mKeyFrameButtonBar->setScrollRange(200);
        mKeyFrameButtonBar->setScrollPosition(0);
        mKeyFrameButtonBar->setTrackSize(60);
        mKeyFrameButtonBar->setMinTrackSize(60);
        mKeyFrameButtonBar->eventScrollChangePosition += MyGUI::newDelegate(this, &KeyFrameBarControl::notifyChangePosition);

        mPlayButton = _parent->createWidget<MyGUI::Button>("ButtonRightSkin", MyGUI::IntCoord(3, _parent->getClientCoord().height - 23,
            15, 15), MyGUI::Align::Left | MyGUI::Align::Bottom);
        mKeyFrameBar->setScrollRange(200);
        mKeyFrameBar->setScrollPosition(0);
        mKeyFrameBar->setMinTrackSize(10);
        mKeyFrameBar->setMoveToClick(false);
        mKeyFrameBar->setDisableInput(true);
        mKeyFrameBar->setTrackSize(10);
        mKeyFrameBar->getTrackButton()->setAlpha(0.8f);
        mKeyFrameBar->getTrackButton()->setColour(MyGUI::Colour(1, 0.7f, 0, 0.8f));

        mCurFrames = mKeyFrameButtonBar->getTrackButton()->createWidget<MyGUI::TextBox>("TextBox",
            MyGUI::IntCoord(5, 3, 50, 15), MyGUI::Align::Default, "Background");
        mCurFrames->setTextAlign(MyGUI::Align::Center);
        mCurFrames->setFontHeight(13);
        mCurFrames->setTextShadow(true);
        mCurFrames->setEnabled(false);
        mCurFrames->setTextShadowColour(MyGUI::Colour::Black);

        mKeyFrameButtonBar->getTrackButton()->eventMouseButtonClick += MyGUI::newDelegate(this, &KeyFrameBarControl::notifyFrameBarButtonClick);
        mPlayButton->eventMouseButtonClick += MyGUI::newDelegate(this, &KeyFrameBarControl::notifyPlayButtonClick);

        generateScaleNums(20);
        updateBarButtonPosition();
    }

    KeyFrameBarControl::~KeyFrameBarControl()
    {

    }

    void KeyFrameBarControl::update()
    {
        auto clip = HonViewerApp::GetViewerApp()->GetModelViewer()->GetCurrentClip();
        if (!clip)
        {
            mCurFrames->setCaption("");
            return;
        }

        int cur = clip->mCurFrame;
        mKeyFrameBar->setScrollPosition(cur);
        mKeyFrameButtonBar->setScrollPosition(cur);

        updateBarButtonPosition();

        DiString frameInfo;
        frameInfo.Format("%d/%d",cur+1,mNumFrames);
        mCurFrames->setCaption(frameInfo.c_str());
    }

    void KeyFrameBarControl::updateScales()
    {
        auto sizeInPixel = mKeyFrameBar->getSize();
        mScaleActuaFrames = int((sizeInPixel.width / FRAME_SCALE_WIDTH) * 10);

        auto clip = HonViewerApp::GetViewerApp()->GetModelViewer()->GetCurrentClip();
        if (!clip)
            return;
        
        mNumFrames = clip->mNumFrames;

        // TODO: may overflow
        mScale = 1;
        while (mScale*mScaleActuaFrames < mNumFrames)
            mScale *= 2;

        mKeyFrameBar->setScrollRange(mScaleActuaFrames * mScale);
        mKeyFrameButtonBar->setScrollRange(mScaleActuaFrames * mScale);

        updateBarButtonPosition();
        updateScaleNums();
    }

    void KeyFrameBarControl::updateScaleNums()
    {
        if ((size_t)mScaleActuaFrames > mScaleNumbers.size() * 10)
        {
            generateScaleNums(mScaleNumbers.size());
        }

        for (size_t i = 0; i < mScaleNumbers.size(); i+=mScale)
        {
            if (i * 10 < (size_t)mScaleActuaFrames*mScale)
            {
                mScaleNumbers[i]->setVisible(true);
                mScaleNumbers[i]->setPosition(FRAME_SCALE_WIDTH*i/mScale, 9);
            }
            else
            {
                mScaleNumbers[i]->setVisible(false);
            }
        }
    }

    void KeyFrameBarControl::generateScaleNums(int num)
    {
        // reserve 20
        for (int i = 0; i < num; ++i)
        {
            MyGUI::TextBox* text = mKeyFrameBar->createWidget<MyGUI::TextBox>("TextBox",
                MyGUI::IntCoord(0, 0, 30, 15), MyGUI::Align::Default);
            text->setFontHeight(13);
            text->setTextShadow(true);
            text->setTextShadowColour(MyGUI::Colour::Black);
            DiString caption;
            caption.Format("%d", (mScaleNumbers.size()) * 10);
            text->setCaption(caption.c_str());
            text->setVisible(false);

            mScaleNumbers.push_back(text);
        }
    }

    void KeyFrameBarControl::notifyChangePosition(MyGUI::ScrollBar* _sender, size_t _position)
    {
        mKeyFrameBar->setScrollPosition(_position);
        updateBarButtonPosition();

        if (HonViewerApp::GetViewerApp()->GetModelViewer()->PauseClip())
        {
            auto clip = HonViewerApp::GetViewerApp()->GetModelViewer()->GetCurrentClip();
            if (!clip)
                return;
            _position = DiMath::Clamp<size_t>(_position, 0, mNumFrames - 1);
            clip->mCurFrame = _position;
            clip->mTime = clip->mCurFrame / clip->mFPS;
            clip->mInterpFactor = 0;
        }
    }

    void KeyFrameBarControl::updateBarButtonPosition()
    {
        MyGUI::Button* btnTrack = mKeyFrameButtonBar->getTrackButton();
        MyGUI::Button* btnStart = mKeyFrameButtonBar->getTrackStartButton();
        MyGUI::Button* btnEnd = mKeyFrameButtonBar->getTrackEndButton();
        if (btnStart && btnEnd)
        {
            MyGUI::IntPoint sPos = btnStart->getPosition();
            btnStart->setPosition(MyGUI::IntPoint(btnTrack->getPosition().left - btnStart->getSize().width, sPos.top));
            btnEnd->setPosition(MyGUI::IntPoint(btnTrack->getPosition().left + mKeyFrameButtonBar->getTrackSize()+6, sPos.top));
        }
    }

    void KeyFrameBarControl::notifyPlayButtonClick(MyGUI::Widget* _sender)
    {
        HonViewerApp::GetViewerApp()->GetModelViewer()->PauseClip(
            !HonViewerApp::GetViewerApp()->GetModelViewer()->PauseClip());
    }

    void KeyFrameBarControl::notifyFrameBarButtonClick(MyGUI::Widget* _sender)
    {
        HonViewerApp::GetViewerApp()->GetModelViewer()->PauseClip(true);
    }

} // namespace tools
