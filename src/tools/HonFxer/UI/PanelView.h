/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef __PANEL_VIEW_H__
#define __PANEL_VIEW_H__

#include "K2Prerequisites.h"
#include "BasePanelView.h"
#include "PanelViewCell.h"
#include "PanelGroup.h"

namespace tools
{
	class PanelView : public wraps::BasePanelView<PanelViewCell>
	{
	public:
		PanelView(MyGUI::Widget* _parent) :
			wraps::BasePanelView<PanelViewCell>("", _parent)
		{
		}

    public:

        Demi::DiPanelGroup* AddGroup();
        
        void RemoveGroup(Demi::DiPanelGroup* group);
	};

    

} // namespace demo

#endif // __PANEL_VIEW_H__
