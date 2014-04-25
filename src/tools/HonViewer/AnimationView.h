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

#ifndef __ANIMATION_VIEW_H__
#define __ANIMATION_VIEW_H__

#include "ViewerPrerequisites.h"
#include "BaseLayout.h"

namespace tools
{
	class AnimationView : public wraps::BaseLayout
	{
	public:
        AnimationView(MyGUI::Widget* _parent = nullptr);
        virtual ~AnimationView();

    public:

        void UpdateClips();

        void notifyListChangePosition(MyGUI::MultiListBox* _sender, size_t _index);

	private:

        DiVector<DiK2Clip*> mClips;

        MyGUI::MultiListBox* mAnimationList;
	};

} // namespace tools

#endif // __TOOLS_CONTROL_H__
