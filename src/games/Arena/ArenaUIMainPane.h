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
        
        // temp
        static MainPaneControl* _this;
        static MainPaneControl* get() {
            return _this;
        }

		void setVisible(bool _value);
        
        virtual void update();

        void initUI();
        
        void showAdBar(bool val);
        
    private:
        
        void initIcons();
        
        void initHeroBar();
        
        MyGUI::ImageBox* createIcon(const DiString& texture, DiVec2 pos, float size, MyGUI::Widget* parent = nullptr, const DiString& layer = "Statistic");
        MyGUI::ImageBox* createIcon(const DiString& texture, DiVec2 pos, float sizex, float sizey, MyGUI::Widget* parent = nullptr, const DiString& layer = "Statistic");
        
        MyGUI::ImageBox* mBottomLeft{ nullptr };
        MyGUI::ImageBox* mBottomRight{ nullptr };
	};

} // namespace tools

#endif // __MAIN_PANE_CONTROL_H__
