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
#include "CommonToolControl.h"
#include "FilesView.h"
#include "K2ModelView.h"
#include "HonViewerApp.h"

namespace tools
{
    CommonToolsControl::CommonToolsControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("CommonTools.layout", _parent)
        
	{
        assignWidget(mWireframe, "CheckWireframe");
        assignWidget(mShowBones, "CheckShowBones");
        assignWidget(mShowModel, "CheckShowModel");
        assignWidget(mShowBounds, "CheckShowBounds");
        assignWidget(mShowGrids, "CheckShowGrids");
        assignWidget(mLoopAnim, "CheckLoopAnim");
        assignWidget(mShowBoneNames, "CheckShowBoneNames");
        assignWidget(mEnableBloom, "CheckEnableBloom");

        mWireframe->eventMouseButtonClick += MyGUI::newDelegate(this, &CommonToolsControl::notifyButtonPressed);
        mShowBones->eventMouseButtonClick += MyGUI::newDelegate(this, &CommonToolsControl::notifyButtonPressed);
        mShowModel->eventMouseButtonClick += MyGUI::newDelegate(this, &CommonToolsControl::notifyButtonPressed);
        mShowBounds->eventMouseButtonClick += MyGUI::newDelegate(this, &CommonToolsControl::notifyButtonPressed);
        mShowGrids->eventMouseButtonClick += MyGUI::newDelegate(this, &CommonToolsControl::notifyButtonPressed);
        mLoopAnim->eventMouseButtonClick += MyGUI::newDelegate(this, &CommonToolsControl::notifyButtonPressed);
        mShowBoneNames->eventMouseButtonClick += MyGUI::newDelegate(this, &CommonToolsControl::notifyButtonPressed);
        mEnableBloom->eventMouseButtonClick += MyGUI::newDelegate(this, &CommonToolsControl::notifyButtonPressed);

        mShowModel->setStateSelected(true);
        mLoopAnim->setStateSelected(true);
    }

    CommonToolsControl::~CommonToolsControl()
	{
	}

    void CommonToolsControl::notifyButtonPressed(MyGUI::Widget* _sender)
    {
        auto viewer = HonViewerApp::GetViewerApp()->GetModelViewer();
        if (_sender == mWireframe)
        {
            bool checked = mWireframe->getStateSelected();
            if (viewer->SetWireframe(!checked))
            {
                mWireframe->setStateSelected(!checked);
            }
        }
        else if (_sender == mShowBones)
        {
            bool checked = mShowBones->getStateSelected();
            if (viewer->ShowBones(!checked))
            {
                mShowBones->setStateSelected(!checked);
            }
        }
        else if (_sender == mShowBoneNames)
        {
            bool checked = mShowBoneNames->getStateSelected();
            if (viewer->ShowBoneNames(!checked))
            {
                mShowBoneNames->setStateSelected(!checked);
            }
        }
        else if (_sender == mShowModel)
        {
            bool checked = mShowModel->getStateSelected();
            if (viewer->ShowModel(!checked))
            {
                mShowModel->setStateSelected(!checked);
            }
        }
        else if (_sender == mShowBounds)
        {
            bool checked = mShowBounds->getStateSelected();
            if (viewer->ShowBounds(!checked))
            {
                mShowBounds->setStateSelected(!checked);
            }
        }
        else if (_sender == mShowGrids)
        {
            bool checked = mShowGrids->getStateSelected();
            if (viewer->ShowGrids(!checked))
            {
                mShowGrids->setStateSelected(!checked);
            }
        }
        else if (_sender == mLoopAnim)
        {
            bool checked = mLoopAnim->getStateSelected();
            if (viewer->SetLoopAnim(!checked))
            {
                mLoopAnim->setStateSelected(!checked);
            }
        }
        else if (_sender == mEnableBloom)
        {
            bool checked = mEnableBloom->getStateSelected();
            DiPostEffectManager* peMgr = DiBase::Driver->GetMainRenderWindow()->GetPostEffectManager();
            DiPostEffect* bloom = peMgr->GetEffect("Bloom");
            if (bloom)
                bloom->SetEnable(!checked);
            mEnableBloom->setStateSelected(!checked);
        }
    }

} // namespace tools
