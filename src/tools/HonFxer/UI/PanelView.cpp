/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/

#include "FxerPch.h"
#include "PanelView.h"

namespace tools
{
    PanelGroup* PanelView::AddGroup()
    {
        PanelGroup* g = new PanelGroup();
        addItem(g);
        return g;
    }
}