
#include "FxerPch.h"
#include "MainWorkspaceControl.h"

namespace tools
{
	MainWorkspaceControl::MainWorkspaceControl(MyGUI::Widget* _parent) :
		SeparatorPartControl("MainWorkspaceControl.layout", _parent),
        mToolsCtrl(nullptr),
        mRenderWndCtrl(nullptr)
	{
        assignBase(mToolsCtrl, "ToolsControl");
        assignBase(mRenderWndCtrl, "WorkspaceControl");
    }

	MainWorkspaceControl::~MainWorkspaceControl()
	{
	}

    void MainWorkspaceControl::update()
    {
        mRenderWndCtrl->update();
    }

} // namespace tools
