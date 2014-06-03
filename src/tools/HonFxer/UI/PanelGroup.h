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

namespace tools
{
	class PanelGroup : public wraps::BasePanelViewItem
	{
	public:
		PanelGroup();

        virtual ~PanelGroup(){}

    public:

        void SetCaption(const DiString& cap);

        void AddItem(const DiString& caption, DiPropertyBase* prop);

        void FinishAddItem();

    protected:

        void AddSimpleInputItem(const DiString& caption, DiPropertyBase* prop);

        void AddCheckItem(const DiString& caption, DiPropertyBase* prop);

    protected:

        int mCurrentHeight{ 0 };
	};
}

#endif
