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

        mWireframe->eventMouseButtonClick += MyGUI::newDelegate(this, &CommonToolsControl::notifyButtonPressed);
        mShowBones->eventMouseButtonClick += MyGUI::newDelegate(this, &CommonToolsControl::notifyButtonPressed);
        mShowModel->eventMouseButtonClick += MyGUI::newDelegate(this, &CommonToolsControl::notifyButtonPressed);
        mShowBounds->eventMouseButtonClick += MyGUI::newDelegate(this, &CommonToolsControl::notifyButtonPressed);

        mShowModel->setStateSelected(true);
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
    }

} // namespace tools
