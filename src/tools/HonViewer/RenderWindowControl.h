
#ifndef __RENDER_WINDOW_CONTROL_H__
#define __RENDER_WINDOW_CONTROL_H__

#include "ViewerPrerequisites.h"
#include "BaseLayout.h"

namespace tools
{
	class RenderWindowControl : public wraps::BaseLayout
	{
	public:
        RenderWindowControl(MyGUI::Widget* _parent);
        virtual ~RenderWindowControl();

    public:
        void onUpdateCanvas(MyGUI::Canvas*, MyGUI::Canvas::Event evt);

        KeyFrameBarControl* getKeyFrameBar(){ return mKeyFrameBar; }

        virtual void update();

        bool mouseInCanvas();

        void updateInfo();

        void updateBoneNames(bool show);

    protected:

        MyGUI::Canvas* mCanvas;

        KeyFrameBarControl* mKeyFrameBar;

        MyGUI::TextBox* mInfo;

        MyGUI::TextBox* mInfoStatic;

        DiVector<MyGUI::TextBox*> mBoneNames;
	};

} // namespace tools

#endif // __RENDER_WINDOW_CONTROL_H__
