
#include "FxerPch.h"
#include "MainWorkspaceControl.h"

namespace tools
{
    WorkSpaceControl::WorkSpaceControl(MyGUI::Widget* _parent) 
        : wraps::BaseLayout("FxMainWorkspaceControl.layout", _parent)
    {
        auto parentHeight = _parent->getClientCoord().height;
        auto parentWidth = _parent->getClientCoord().width;
        auto paneWidth = 250;

        mLeftPanel = _parent->createWidget<MyGUI::ScrollBar>("PanelEmpty", MyGUI::IntCoord(0, 0,
            paneWidth, parentHeight), MyGUI::Align::Left | MyGUI::Align::VStretch);

        mSeparatorHLeft = _parent->createWidget<MyGUI::ScrollBar>("PanelEmpty", MyGUI::IntCoord(paneWidth, 0,
            4, parentHeight), MyGUI::Align::Left | MyGUI::Align::VStretch);

        mMiddlePanel = _parent->createWidget<MyGUI::ScrollBar>("PanelEmpty", MyGUI::IntCoord(paneWidth + 4, 0,
            parentWidth - 4 * 2 - paneWidth * 2, parentHeight), MyGUI::Align::Stretch);

        mSeparatorHRight = _parent->createWidget<MyGUI::ScrollBar>("PanelEmpty", MyGUI::IntCoord(parentWidth - paneWidth - 4, 0,
            4, parentHeight), MyGUI::Align::Right | MyGUI::Align::VStretch);

        mRightPanel = _parent->createWidget<MyGUI::ScrollBar>("PanelEmpty", MyGUI::IntCoord(parentWidth - paneWidth, 0,
            paneWidth, parentHeight), MyGUI::Align::Right | MyGUI::Align::VStretch);

        mResTreeCtrl = new ResTreeControl(mLeftPanel);
        mRenderWndCtrl = new RenderWindowControl(mMiddlePanel);
        mToolsCtrl = new ToolsControl(mRightPanel);

        mSeparatorHLeft->eventMouseButtonPressed += MyGUI::newDelegate(this, &WorkSpaceControl::notifyMouseButtonPressed);
        mSeparatorHLeft->eventMouseDrag += MyGUI::newDelegate(this, &WorkSpaceControl::notifyMouseDragLeft);

        mSeparatorHRight->eventMouseButtonPressed += MyGUI::newDelegate(this, &WorkSpaceControl::notifyMouseButtonPressed);
        mSeparatorHRight->eventMouseDrag += MyGUI::newDelegate(this, &WorkSpaceControl::notifyMouseDragRight);
    }

    WorkSpaceControl::~WorkSpaceControl()
    {
        SAFE_DELETE(mResTreeCtrl);
        SAFE_DELETE(mRenderWndCtrl);
        SAFE_DELETE(mToolsCtrl);

        mSeparatorHLeft->eventMouseButtonPressed -= MyGUI::newDelegate(this, &WorkSpaceControl::notifyMouseButtonPressed);
        mSeparatorHLeft->eventMouseDrag -= MyGUI::newDelegate(this, &WorkSpaceControl::notifyMouseDragLeft);

        mSeparatorHRight->eventMouseButtonPressed -= MyGUI::newDelegate(this, &WorkSpaceControl::notifyMouseButtonPressed);
        mSeparatorHRight->eventMouseDrag -= MyGUI::newDelegate(this, &WorkSpaceControl::notifyMouseDragRight);
    }

    void WorkSpaceControl::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        if (_id == MyGUI::MouseButton::Left)
        {
            mStartLeftPanel = mLeftPanel->getCoord();
            mStartMiddlePanel = mMiddlePanel->getCoord();
            mStartRightPanel = mRightPanel->getCoord();

            mStartSeparatorHLeft = mSeparatorHLeft->getCoord();
            mStartSeparatorHRight = mSeparatorHRight->getCoord();

            mStartMousePosition = MyGUI::InputManager::getInstance().getMousePosition();
        }
    }

    void WorkSpaceControl::notifyMouseDragLeft(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        if (_id == MyGUI::MouseButton::Left)
        {
            MyGUI::IntPoint mousePosition = MyGUI::InputManager::getInstance().getMousePosition();
            int delta = mousePosition.left - mStartMousePosition.left;

            MyGUI::IntCoord leftPanel = mStartLeftPanel;
            MyGUI::IntCoord rightPanel = mStartMiddlePanel;

            MyGUI::IntCoord separatorHPanelLeft  = mStartSeparatorHLeft;

            leftPanel.width += delta;
            separatorHPanelLeft.left += delta;
            rightPanel.left += delta;
            rightPanel.width -= delta;

            int diffLeft = mMinSizeLeft - leftPanel.width;
            if (diffLeft > 0)
            {
                leftPanel.width += diffLeft;
                separatorHPanelLeft.left += diffLeft;
                rightPanel.left += diffLeft;
                rightPanel.width -= diffLeft;
            }

            mLeftPanel->setCoord(leftPanel);
            mMiddlePanel->setCoord(rightPanel);
            mSeparatorHLeft->setCoord(separatorHPanelLeft);
        }
    }

    void WorkSpaceControl::notifyMouseDragRight(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        if (_id == MyGUI::MouseButton::Left)
        {
            MyGUI::IntPoint mousePosition = MyGUI::InputManager::getInstance().getMousePosition();
            int delta = mousePosition.left - mStartMousePosition.left;

            MyGUI::IntCoord leftPanel = mStartMiddlePanel;
            MyGUI::IntCoord rightPanel = mStartRightPanel;

            MyGUI::IntCoord separatorHPanelRight = mStartSeparatorHRight;

            leftPanel.width += delta;
            separatorHPanelRight.left += delta;
            rightPanel.left += delta;
            rightPanel.width -= delta;

            int diffRight = mMinSizeRight - rightPanel.width;
            if (diffRight > 0)
            {
                leftPanel.width -= diffRight;
                separatorHPanelRight.left -= diffRight;
                rightPanel.left -= diffRight;
                rightPanel.width += diffRight;
            }

            mMiddlePanel->setCoord(leftPanel);
            mRightPanel->setCoord(rightPanel);
            mSeparatorHRight->setCoord(separatorHPanelRight);
        }
    }

    void WorkSpaceControl::update()
    {
        mRenderWndCtrl->update();
    }

} // namespace tools