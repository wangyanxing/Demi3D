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

#ifndef __MAIN_PANE_CONTROL_H__
#define __MAIN_PANE_CONTROL_H__

#include "FxerPrerequisites.h"
#include "BaseLayout.h"
#include "MainMenuControl.h"
#include "MainWorkspaceControl.h"

namespace tools
{
	class MainPaneControl : public wraps::BaseLayout
	{
	public:
		MainPaneControl();

		virtual ~MainPaneControl();

		void setVisible(bool _value);

        WorkSpaceControl* GetWorkspaceControl(){ return mMainWorkspaceControl; }

        virtual void Update();

	private:
        MainMenuControl* mMainMenuControl{ nullptr };

        WorkSpaceControl* mMainWorkspaceControl{ nullptr };

        CurveEditor* mCurveEditor{ nullptr };
        
        ColorEditor* mColorEditor{ nullptr };
	};

} // namespace tools

#endif // __MAIN_PANE_CONTROL_H__
