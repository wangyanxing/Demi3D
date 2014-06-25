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

#ifndef __PANEL_GROUP_H__
#define __PANEL_GROUP_H__

#include "FxerPrerequisites.h"
#include "BasePanelViewItem.h"
#include "Property.h"

namespace Demi
{
	class DiPanelGroup : public wraps::BasePanelViewItem
	{
	public:
		DiPanelGroup();

        virtual             ~DiPanelGroup();

    public:

        void                SetCaption(const DiString& cap);

        DiPropertyItem*     AddItem(const DiString& caption, DiPropertyBase* prop);

        MyGUI::Widget*      GetClientWidget() { return mWidgetClient; }

        void                NotifyRearrangeHeight();

        int                 GetHeight() { return mHeight; }

        int                 GetHeightStep() { return mHeightStep; }

        int                 GetWidth() { return mWidth; }

        int                 GetWidthStep() { return mWidthStep; }

        int                 GetCurrentHeight();

    protected:

        DiVector<DiPropertyItem*>   mItems;
                           
        int mHeight        { 23 };
                           
        int mHeightStep    { 26 };
                           
        int mWidth         { 125 };
                           
        int mWidthStep     { 8 };
	};
}

#endif
