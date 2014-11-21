
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
        
        createIcon("ability_abuse.png", DiVec2(480,673),iconSize);
        createIcon("griefing.png", DiVec2(480,767),iconSize);
        createIcon("mentorwings.png", DiVec2(387,673),iconSize);
        createIcon("s2_loss.png", DiVec2(387,767),iconSize);
        createIcon("sbteye.png", DiVec2(294,673),iconSize);
        createIcon("tech_support.png", DiVec2(294,767),iconSize);
    }
    
    void MainPaneControl::createIcon(const DiString& texture, DiVec2 pos, float sizex, float sizey)
    {
        auto image = mMainWidget->createWidget<MyGUI::ImageBox>(MyGUI::WidgetStyle::Overlapped,"ImageBox",
                                                                MyGUI::IntCoord(pos.x, pos.y, sizex, sizey),
                                                                MyGUI::Align::Stretch, "Popup");
        image->setImageTexture(texture.c_str());
    }
    
    void MainPaneControl::createIcon(const DiString& texture, DiVec2 pos, float iconSize)
    {
        auto image = mMainWidget->createWidget<MyGUI::ImageBox>(MyGUI::WidgetStyle::Overlapped,"ImageBox",
                                                                       MyGUI::IntCoord(pos.x, pos.y, iconSize, iconSize),
                                                                       MyGUI::Align::Stretch, "Popup");
        image->setImageTexture(texture.c_str());
    }
    
    void MainPaneControl::initUI()
    {
        initHeroBar();
        
        auto imageHeroBar = mMainWidget->createWidget<MyGUI::ImageBox>(MyGUI::WidgetStyle::Overlapped,"ImageBox",
                                                                           MyGUI::IntCoord(0, 0, 256*1.5, 128*1.5),
                                                                           MyGUI::Align::Stretch, "Popup");
        imageHeroBar->setImageTexture("small_portrait_window.png");
        
        
        auto height = DiBase::Driver->GetMainRenderWindow()->GetHeight();
        auto width = DiBase::Driver->GetMainRenderWindow()->GetWidth();
        
        float miniMapSize = 256*1.02;
        float miniMapBaseSize = 256*0.83;
        auto imageMiniMapBase = mMainWidget->createWidget<MyGUI::ImageBox>(MyGUI::WidgetStyle::Overlapped,"ImageBox",
                                                                       MyGUI::IntCoord(0, height-miniMapBaseSize+3, miniMapBaseSize, miniMapBaseSize),
                                                                       MyGUI::Align::Stretch, "Popup");
        imageMiniMapBase->setImageTexture("minimap.png");
        
        auto imageMiniMap = mMainWidget->createWidget<MyGUI::ImageBox>(MyGUI::WidgetStyle::Overlapped,"ImageBox",
                                                                       MyGUI::IntCoord(0, height-miniMapSize+3, miniMapSize, miniMapSize),
                                                                       MyGUI::Align::Stretch, "Popup");
        imageMiniMap->setImageTexture("bot_left_altview.png");
        
        
        initIcons();
        
        float itemBarSizeX = 512*0.98;
        float itemBarSizeY = 256*0.98;
        auto itemBar = mMainWidget->createWidget<MyGUI::ImageBox>(MyGUI::WidgetStyle::Overlapped,"ImageBox",
                                                                       MyGUI::IntCoord(width-itemBarSizeX, height-itemBarSizeY+3, itemBarSizeX, itemBarSizeY),
                                                                       MyGUI::Align::Stretch, "Popup", "_BackgroundColor");
        itemBar->setImageTexture("bot_right_orders_altview.png");
    }

} // namespace Demi
