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

#ifndef __FXER_PROPERTIES_CONTROL_H__
#define __FXER_PROPERTIES_CONTROL_H__

#include "FxerPrerequisites.h"
#include "BaseLayout.h"

namespace tools
{
	class PropertiesControl : public wraps::BaseLayout
	{
	public:
		PropertiesControl(MyGUI::Widget* _parent = nullptr);

		virtual ~PropertiesControl();

        PanelView* getPanelView() { return mPanelView; }

    private:
        PanelView* mPanelView{ nullptr };
    };

} // namespace tools

#endif // __TOOLS_CONTROL_H__
