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
        
        void createIcon(const DiString& texture, DiVec2 pos, float size);
        void createIcon(const DiString& texture, DiVec2 pos, float sizex, float sizey);
	};

} // namespace tools

#endif // __MAIN_PANE_CONTROL_H__
