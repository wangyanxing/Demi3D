
#include "ArenaPch.h"
#include "ArenaUIMainPane.h"

namespace Demi
{
	MainPaneControl::MainPaneControl() :
		wraps::BaseLayout("ArenaMainPane.layout")
	{
        _this = this;
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
        //float iconSize = 128 * 0.45;
        //createIcon("flux_icon.png", DiVec2(0,0), iconSize, nullptr, "LayoutEditor_MainPane");
        
        createIcon("hpbar.png", DiVec2(61,1),136*0.8,15*0.8, nullptr, "Background");
        createIcon("mpbar.png", DiVec2(61,15),136*0.8,15*0.8, nullptr, "Background");
    }
    
    void MainPaneControl::initIcons()
    {
        auto height = DiBase::Driver->GetMainRenderWindow()->GetHeight();
        //auto width = DiBase::Driver->GetMainRenderWindow()->GetWidth();
        
        float iconSize = 128 * 0.7;
        float upperY = height - 182;
        float lowerY = height - 88;
        
        createIcon("ability_abuse.png", DiVec2(480,upperY),iconSize);
        createIcon("griefing.png", DiVec2(480,lowerY),iconSize);
        createIcon("mentorwings.png", DiVec2(387,upperY),iconSize);
        createIcon("s2_loss.png", DiVec2(387,lowerY),iconSize);
        createIcon("sbteye.png", DiVec2(294,upperY),iconSize);
        createIcon("tech_support.png", DiVec2(294,lowerY),iconSize);
    }
    
    MyGUI::ImageBox* MainPaneControl::createIcon(const DiString& texture,
                                                 DiVec2 pos, float sizex, float sizey, MyGUI::Widget* parent, const DiString& layer)
    {
        if (!parent) parent = mMainWidget;
        auto image = parent->createWidget<MyGUI::ImageBox>("ImageBox",
                                                                MyGUI::IntCoord(pos.x, pos.y, sizex, sizey),
                                                                MyGUI::Align::Stretch, layer.c_str());
        image->setImageTexture(texture.c_str());

        return image;
    }
    
    MyGUI::ImageBox* MainPaneControl::createIcon(const DiString& texture, DiVec2 pos,
                                                 float iconSize, MyGUI::Widget* parent, const DiString& layer)
    {
        if (!parent) parent = mMainWidget;
        auto image = parent->createWidget<MyGUI::ImageBox>(MyGUI::WidgetStyle::Overlapped,"ImageBox",
                                                                       MyGUI::IntCoord(pos.x, pos.y, iconSize, iconSize),
                                                                       MyGUI::Align::Stretch, layer.c_str());
        image->setImageTexture(texture.c_str());
        return image;
    }
    
    void MainPaneControl::initUI()
    {
        auto height = DiBase::Driver->GetMainRenderWindow()->GetHeight();
        auto width = DiBase::Driver->GetMainRenderWindow()->GetWidth();
        
        initHeroBar();
        
        createIcon("small_logo.png", DiVec2(width-80, -20), 100, 100);
        createIcon("small_portrait_window2.png", DiVec2(0,0), 256, 128);
        
        float miniMapSize = 256*0.51;
        //float miniMapBaseSize = 256*0.83;
        //auto imageMiniMapBase = createIcon("minimap.png", DiVec2(0, height-miniMapBaseSize+3), miniMapBaseSize, miniMapBaseSize);
        
        mBottomLeft = createIcon("bot_left_altview2.png", DiVec2(0, height-miniMapSize+3),
                                 miniMapSize, miniMapSize);
        
        //initIcons();
        
        float itemBarSizeX = 512*0.49;
        float itemBarSizeY = 256*0.49;
        mBottomRight = createIcon("bot_right_orders_altview2.png", DiVec2(width-itemBarSizeX, height-itemBarSizeY+3),
                                  itemBarSizeX, itemBarSizeY);
        
        showAdBar(true);
    }
    
    void MainPaneControl::showAdBar(bool val)
    {
        int bannerSize = 53;
        
        auto width = DiBase::Driver->GetMainRenderWindow()->GetWidth();
        auto height = DiBase::Driver->GetMainRenderWindow()->GetHeight();
        
        float itemBarSizeX = 512*0.49;
        float itemBarSizeY = 256*0.49;
        float miniMapSize = 256*0.51;
        
        if(val) {
            mBottomLeft->setPosition(0, height-miniMapSize+3-bannerSize);
            mBottomRight->setPosition(width-itemBarSizeX, height-itemBarSizeY+3-bannerSize);
        } else {
            bannerSize = 0;
            mBottomLeft->setPosition(0, height-miniMapSize+3-bannerSize);
            mBottomRight->setPosition(width-itemBarSizeX, height-itemBarSizeY+3-bannerSize);
        }
    }

    // temp
    MainPaneControl* MainPaneControl::_this = nullptr;
} // namespace Demi
