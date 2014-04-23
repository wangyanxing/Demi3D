
#ifndef __RENDER_WINDOW_CONTROL_H__
#define __RENDER_WINDOW_CONTROL_H__

#include <MyGUI.h>
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

    protected:

        MyGUI::Canvas* mCanvas;
	};

} // namespace tools

#endif // __RENDER_WINDOW_CONTROL_H__
