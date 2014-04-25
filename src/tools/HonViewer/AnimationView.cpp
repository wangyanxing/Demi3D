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
#include "AnimationView.h"
#include "K2ModelView.h"
#include "HonViewerApp.h"

namespace tools
{
    AnimationView::AnimationView(MyGUI::Widget* _parent) :
		wraps::BaseLayout("AnimationView.layout", _parent)
	{
        mAnimationList = _parent->createWidget<MyGUI::MultiListBox>("MultiListBox", MyGUI::IntCoord(0, 0, 300, 400), MyGUI::Align::Stretch);
        mAnimationList->addColumn("Name");
        mAnimationList->setColumnResizingPolicyAt(0, MyGUI::ResizingPolicy::Fill);
        mAnimationList->addColumn("Frames");
        mAnimationList->setColumnResizingPolicyAt(1, MyGUI::ResizingPolicy::Fill);

        mAnimationList->eventListChangePosition += MyGUI::newDelegate(this, &AnimationView::notifyListChangePosition);
	}

    AnimationView::~AnimationView()
	{
	}

    void AnimationView::UpdateClips()
    {
        mAnimationList->removeAllItems();
        mClips = HonViewerApp::GetViewerApp()->GetModelViewer()->GetClipList();

        for (int i = 0; i < (int)mClips.size(); ++i)
        {
            DiK2Clip* clip = mClips[i];
            mAnimationList->addItem(clip->mName.c_str());
            DiString frames;
            frames.Format("%d", clip->mNumFrames);
            mAnimationList->setSubItemNameAt(1, i, frames.c_str());
        }
    }

    void AnimationView::notifyListChangePosition(MyGUI::MultiListBox* _sender, size_t _index)
    {
        if (_index != MyGUI::ITEM_NONE)
        {
            DiK2Clip* clip = mClips[_index];
            HonViewerApp::GetViewerApp()->GetModelViewer()->PlayClip(clip);
        }
    }

} // namespace tools
