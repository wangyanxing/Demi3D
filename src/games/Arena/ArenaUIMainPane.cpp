
#include "ArenaPch.h"
#include "ArenaUIMainPane.h"

namespace Demi
{
	MainPaneControl::MainPaneControl() :
		wraps::BaseLayout("ArenaMainPane.layout")
	{
        initUI();
	}

	MainPaneControl::~MainPaneControl()
	{
	}

	void MainPaneControl::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

    void MainPaneControl::update()
    {
    }
    
    void MainPaneControl::initHeroBar()
    {
        float iconSize = 128 * 0.656;
        createIcon("flux_icon.png", DiVec2(0,0),iconSize);
        
        createIcon("hpbar.png", DiVec2(93,1),136*1.2,15*1.2);
        createIcon("mpbar.png", DiVec2(93,23),136*1.2,15*1.2);
    }
    
    void MainPaneControl::initIcons()
    {
        auto height = DiBase::Driver->GetMainRenderWindow()->GetHeight();
        auto width = DiBase::Driver->GetMainRenderWindow()->GetWidth();
        
        float iconSize = 128 * 0.7;
        float upperY = height-182;
        float lowerY = height-88;
        
        createIcon("ability_abuse.png", DiVec2(480,upperY),iconSize);
        createIcon("griefing.png", DiVec2(480,lowerY),iconSize);
        createIcon("mentorwings.png", DiVec2(387,upperY),iconSize);
        createIcon("s2_loss.png", DiVec2(387,lowerY),iconSize);
        createIcon("sbteye.png", DiVec2(294,upperY),iconSize);
        createIcon("tech_support.png", DiVec2(294,lowerY),iconSize);
    }
    
    MyGUI::ImageBox* MainPaneControl::createIcon(const DiString& texture,
                                                 DiVec2 pos, float sizex, float sizey, MyGUI::Widget* parent)
    {
        if (!parent) parent = mMainWidget;
        auto image = parent->createWidget<MyGUI::ImageBox>("ImageBox",
                                                                MyGUI::IntCoord(pos.x, pos.y, sizex, sizey),
                                                                MyGUI::Align::Stretch, "Main");
        image->setImageTexture(texture.c_str());
        return image;
    }
    
    MyGUI::ImageBox* MainPaneControl::createIcon(const DiString& texture, DiVec2 pos,
                                                 float iconSize, MyGUI::Widget* parent)
    {
        if (!parent) parent = mMainWidget;
        auto image = parent->createWidget<MyGUI::ImageBox>(MyGUI::WidgetStyle::Overlapped,"ImageBox",
                                                                       MyGUI::IntCoord(pos.x, pos.y, iconSize, iconSize),
                                                                       MyGUI::Align::Stretch, "Main");
        image->setImageTexture(texture.c_str());
        return image;
    }
    
    void MainPaneControl::initUI()
    {
        auto height = DiBase::Driver->GetMainRenderWindow()->GetHeight();
        auto width = DiBase::Driver->GetMainRenderWindow()->GetWidth();
        
        initHeroBar();
        
        auto menuBtn = createIcon("small_logo.png", DiVec2(width-130, -25), 150, 150);
        auto imageHeroBar = createIcon("small_portrait_window.png", DiVec2(0,0), 256*1.5, 128*1.5);
        
        float miniMapSize = 256*1.02;
        float miniMapBaseSize = 256*0.83;
        auto imageMiniMapBase = createIcon("minimap.png", DiVec2(0, height-miniMapBaseSize+3), miniMapBaseSize, miniMapBaseSize);
        
        auto imageMiniMap = createIcon("bot_left_altview.png", DiVec2(0, height-miniMapSize+3), miniMapSize, miniMapSize);
        
        initIcons();
        
        float itemBarSizeX = 512*0.98;
        float itemBarSizeY = 256*0.98;
        auto itemBar = createIcon("bot_right_orders_altview.png", DiVec2(width-itemBarSizeX, height-itemBarSizeY+3), itemBarSizeX, itemBarSizeY);
    }

} // namespace Demi
