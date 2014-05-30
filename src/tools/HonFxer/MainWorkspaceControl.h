#ifndef __MAIN_WORKSPACE_CONTROL_H__
#define __MAIN_WORKSPACE_CONTROL_H__

#include "SeparatorPartControl.h"
#include "ToolsControl.h"
#include "ResTreeControl.h"
#include "RenderWindowControl.h"

namespace tools
{
    class WorkSpaceControl : public wraps::BaseLayout
    {
    public:
        WorkSpaceControl(MyGUI::Widget* _parent);

        virtual ~WorkSpaceControl();

    public:

        void update();

    private:
        void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

        void notifyMouseDragLeft(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

        void notifyMouseDragRight(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

    private:
        MyGUI::Widget* mLeftPanel{ nullptr };

        MyGUI::Widget* mMiddlePanel{ nullptr };
        
        MyGUI::Widget* mRightPanel{ nullptr };
        
        MyGUI::Widget* mSeparatorHLeft{ nullptr };

        MyGUI::Widget* mSeparatorHRight{ nullptr };

        int mMinSizeLeft{ 200 };
        
        int mMinSizeRight{ 200 };

        MyGUI::IntCoord mStartLeftPanel;

        MyGUI::IntCoord mStartMiddlePanel;
        
        MyGUI::IntCoord mStartRightPanel;

        MyGUI::IntCoord mStartSeparatorHLeft;

        MyGUI::IntCoord mStartSeparatorHRight;

        MyGUI::IntPoint mStartMousePosition;

        ResTreeControl* mResTreeCtrl{ nullptr };

        RenderWindowControl* mRenderWndCtrl{ nullptr };

        ToolsControl* mToolsCtrl{ nullptr };
    };

} // namespace tools

#endif // __MAIN_WORKSPACE_CONTROL_H__
