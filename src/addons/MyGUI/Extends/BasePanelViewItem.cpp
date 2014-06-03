/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "MyGUI_Precompiled.h"
#include "BasePanelViewItem.h"

namespace wraps
{
    BasePanelViewItem::BasePanelViewItem(const std::string& _layout) :
        BaseLayout("", nullptr),
        mPanelCell(nullptr),
        mWidgetClient(nullptr),
        mLayout(_layout)
    {
    }

    void BasePanelViewItem::_initialise(BasePanelViewCell* _cell)
    {
        mPanelCell = _cell;
        mWidgetClient = mPanelCell->getClient();

        if (!mLayout.empty())
        {
            BaseLayout::initialise(mLayout, mWidgetClient);
            mMainWidget->setCoord(0, 0, mWidgetClient->getWidth(), mMainWidget->getHeight());
            mPanelCell->setClientHeight(mMainWidget->getHeight(), false);
        }

        initialise();
    }

    void BasePanelViewItem::_shutdown()
    {
        shutdown();

        if (!mLayout.empty())
        {
            BaseLayout::shutdown();
        }

        mPanelCell = 0;
        mWidgetClient = nullptr;
    }

    void BasePanelViewItem::setVisible(bool _visible)
    {
        mPanelCell->setVisible(_visible);
        mPanelCell->eventUpdatePanel(mPanelCell);
    }

} // namespace wraps

