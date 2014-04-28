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

#ifndef __KEY_FRAME_BAR_H__
#define __KEY_FRAME_BAR_H__

#include <MyGUI.h>
#include "BaseLayout.h"

namespace tools
{
	class KeyFrameBarControl
	{
	public:
        KeyFrameBarControl(MyGUI::Widget* _parent);
        virtual ~KeyFrameBarControl();

    public:

        void update();

        void updateScales(DiK2Clip* clip = nullptr);

        void notifyChangePosition(MyGUI::ScrollBar* _sender, size_t _position);

        void notifyPlayButtonClick(MyGUI::Widget* _sender);

        void notifyFrameBarButtonClick(MyGUI::Widget* _sender);

    protected:

        void updateScaleNums();

        void generateScaleNums(int num);

        void updateBarButtonPosition();

    protected:

        MyGUI::ScrollBar* mKeyFrameButtonBar;

        MyGUI::ScrollBar* mKeyFrameBar;

        MyGUI::Button*    mPlayButton;

        DiVector<MyGUI::TextBox*> mScaleNumbers;

        MyGUI::TextBox* mCurFrames;

        /// number of current animation's frames
        int mNumFrames;

        int mScale;

        int mScaleActuaFrames;
	};

} // namespace tools

#endif // __RENDER_WINDOW_CONTROL_H__
