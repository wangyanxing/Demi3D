#ifndef __TOOLS_CONTROL_H__
#define __TOOLS_CONTROL_H__

#include "BaseLayout/BaseLayout.h"

namespace tools
{

	class ToolsControl :
		public wraps::BaseLayout
	{
	public:
		ToolsControl(MyGUI::Widget* _parent = nullptr);
		virtual ~ToolsControl();

	private:
	};

} // namespace tools

#endif // __TOOLS_CONTROL_H__
