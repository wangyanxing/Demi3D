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

#include "FxerPch.h"
#include "PropertiesControl.h"
#include "PanelView.h"

namespace tools
{
	PropertiesControl::PropertiesControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("FxPropertiesControl.layout", _parent)
	{
        assignBase(mPanelView, "_Main");

        auto s = _parent->getSize();
        mPanelView->getMainWidget()->setSize(s);
	}

	PropertiesControl::~PropertiesControl()
	{
	}

} // namespace tools
