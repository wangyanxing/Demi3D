#ifndef __MAIN_PANE_CONTROL_H__
#define __MAIN_PANE_CONTROL_H__

#include "BaseLayout.h"

namespace Demi
{
	class MainPaneControl :
		public wraps::BaseLayout
	{
	public:
		MainPaneControl();
		virtual ~MainPaneControl();

		void setVisible(bool _value);
        
        virtual void update();

        void initUI();
        
    private:
        
        void initIcons();
        
        void initHeroBar();
        
        MyGUI::ImageBox* createIcon(const DiString& texture, DiVec2 pos, float size, MyGUI::Widget* parent = nullptr);
        MyGUI::ImageBox* createIcon(const DiString& texture, DiVec2 pos, float sizex, float sizey, MyGUI::Widget* parent = nullptr);
	};

} // namespace tools

#endif // __MAIN_PANE_CONTROL_H__
