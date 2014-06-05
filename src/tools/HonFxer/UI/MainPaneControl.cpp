
#include "FxerPch.h"
#include "MainPaneControl.h"
#include "CurveEditor.h"
#include "ColorEditor.h"

namespace tools
{
	MainPaneControl::MainPaneControl() :
		wraps::BaseLayout("FxMainPane.layout"),
		mMainMenuControl(nullptr),
		mMainWorkspaceControl(nullptr)
	{
		assignBase(mMainMenuControl, "MainMenuControl");
		assignBase(mMainWorkspaceControl, "MainWorkspaceControl");

        //mCurveEditor = new CurveEditor();
        mColorEditor = new ColorEditor();
	}

	MainPaneControl::~MainPaneControl()
	{
	}

	void MainPaneControl::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

    void MainPaneControl::Update()
    {
        mMainWorkspaceControl->Update();
    }

} // namespace tools
