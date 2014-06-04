/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/

#include "FxerPch.h"
#include "PanelView.h"

namespace tools
{
    Demi::DiPanelGroup* PanelView::AddGroup()
    {
        Demi::DiPanelGroup* g = new Demi::DiPanelGroup();
        addItem(g);
        return g;
    }
}