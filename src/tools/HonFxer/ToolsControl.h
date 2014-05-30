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

#ifndef __FXER_TOOLS_CONTROL_H__
#define __FXER_TOOLS_CONTROL_H__

#include "FxerPrerequisites.h"
#include "BaseLayout.h"

namespace tools
{
	class ToolsControl : public wraps::BaseLayout
	{
	public:
		ToolsControl(MyGUI::Widget* _parent = nullptr);

		virtual ~ToolsControl();

	private:

	};

} // namespace tools

#endif // __TOOLS_CONTROL_H__
