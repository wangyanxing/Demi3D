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
#include "ResTreeControl.h"

namespace tools
{
    ResTreeControl::ResTreeControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("FxResTreeControl.layout", _parent)
	{
	}

    ResTreeControl::~ResTreeControl()
	{
	}

} // namespace tools
