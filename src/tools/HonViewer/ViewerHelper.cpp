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
#include "ViewerHelper.h"

namespace tools
{
    ViewerHelper::ViewerHelper() :
        Dialog("Helper.layout"),
        mButtonOk(nullptr)
    {
        assignWidget(mButtonOk, "Ok");
       
        mButtonOk->eventMouseButtonClick += MyGUI::newDelegate(this, &ViewerHelper::notifyOk);
       
        mMainWidget->setVisible(false);
    }

    ViewerHelper::~ViewerHelper()
    {
    }

    void ViewerHelper::notifyOk(MyGUI::Widget* _sender)
    {
        eventEndDialog(this, true);
        endModal();
    }
} // tools
