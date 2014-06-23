
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

        mCurveEditor = new CurveEditor();
        mCurveEditor->getMainWidget()->setVisible(false);

        mColorEditor = new ColorEditor();
        mColorEditor->getMainWidget()->setVisible(false);
	}
    
    void MainPaneControl::showCurveEditor()
    {
        mCurveEditor->getMainWidget()->setVisible(true);
    }
    
    void MainPaneControl::showColorEditor()
    {
        mColorEditor->getMainWidget()->setVisible(true);
    }

	MainPaneControl::~MainPaneControl()
	{
        delete mCurveEditor;
        delete mColorEditor;
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
