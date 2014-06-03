/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __BASE_PANEL_VIEW_ITEM_H__
#define __BASE_PANEL_VIEW_ITEM_H__

#include <MyGUI.h>
#include "BaseLayout.h"
#include "BasePanelViewCell.h"

namespace wraps
{
    class MYGUI_EXPORT BasePanelViewItem : public wraps::BaseLayout
	{
	public:
		BasePanelViewItem(const std::string& _layout);

		void _initialise(BasePanelViewCell* _cell);

		void _shutdown();

		virtual void notifyChangeWidth(int _width)
		{
		}

		virtual void setVisible(bool _visible);

		bool getVisible()
		{
			return mPanelCell->getVisible();
		}

		BasePanelViewCell* getPanelCell()
		{
			return mPanelCell;
		}

	protected:
		virtual void initialise() { }
		virtual void shutdown() { }

	protected:
		BasePanelViewCell* mPanelCell;
		MyGUI::Widget* mWidgetClient;
		std::string mLayout;
	};

} // namespace wraps

#endif // __BASE_PANEL_VIEW_ITEM_H__
