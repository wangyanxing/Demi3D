/**********************************************************************
This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/

#include "FxerPch.h"
#include "ColorEditor.h"
#include "MyGUI_PolygonalSkin.h"
#include "ColorPanel.h"

#define gridNumX 20
#define gridNumY 20

namespace Demi
{
	ColorEditor::ColorEditor(MyGUI::Widget* _parent)
	{
        initialiseByAttributes(this, _parent);
        
        auto pos = mCurveCanvasWidget->getAbsolutePosition();
        auto size = mCurveCanvasWidget->getSize();
        auto width = size.width;
        auto height = size.height;
        auto inteveralX = width / (gridNumX + 2);
        auto inteveralY = height / (gridNumY + 2);
    
        mBackground = mCurveCanvasWidget->createWidget<MyGUI::Widget>(MyGUI::WidgetStyle::Overlapped, "BackgroundTile",
                                                                      MyGUI::IntCoord(inteveralX*1.5f,inteveralY,
                                                                                      inteveralX * gridNumX, inteveralY * gridNumY),
                                                                      MyGUI::Align::Stretch, "Popup", "_Background");
        
        mBackgroundColourRect = mBackground->createWidget<MyGUI::ImageBox>(MyGUI::WidgetStyle::Overlapped,"ImageBox",
                                                                        MyGUI::IntCoord(MyGUI::IntPoint(), mBackground->getSize()),
                                                                           MyGUI::Align::Stretch, "Popup", "_BackgroundColor");
        mBackgroundColourRect->setEnabled(false);
        
        CreateTexture();
        
        MyGUI::Widget* widget = mCurveCanvasWidget->createWidget<MyGUI::Widget>(MyGUI::WidgetStyle::Overlapped,"PolygonalSkin",
            MyGUI::IntCoord(MyGUI::IntPoint(), mCurveCanvasWidget->getSize()), MyGUI::Align::Stretch, "Popup", "_CurveLines");
        widget->setColour(MyGUI::Colour::Red);
        widget->eventMouseButtonPressed += MyGUI::newDelegate(this, &ColorEditor::NotfyMousePressed);
        widget->setEnabled(false);
        MyGUI::ISubWidget* main = widget->getSubWidgetMain();
        mCurveLines = main->castType<MyGUI::PolygonalSkin>();

        widget = mBackground->createWidget<MyGUI::Widget>(MyGUI::WidgetStyle::Overlapped,"PolygonalSkin",
            MyGUI::IntCoord(MyGUI::IntPoint(), mBackground->getSize()), MyGUI::Align::Stretch, "Popup", "_CurveCanvas");
        widget->setColour(MyGUI::Colour::Red);
        main = widget->getSubWidgetMain();
        mCurveCanvas = main->castType<MyGUI::PolygonalSkin>();
        
        mCanvasWidget = widget;
        mCanvasWidget->eventMouseButtonPressed += MyGUI::newDelegate(this, &ColorEditor::NotfyMousePressed);
        
        std::vector<MyGUI::FloatPoint> mLinePoints;
        mLinePoints.push_back(MyGUI::FloatPoint(0, 0));
        mLinePoints.push_back(MyGUI::FloatPoint(0, 1));
        mCurveCanvas->setPoints(mLinePoints,true);
        mLinePoints.clear();
        mCurveCanvas->setPoints(mLinePoints,true); // todo fix it

        mCurveLines->_setColour(MyGUI::Colour(0.2f, 0.2f, 0.2f));

        InitGrid();

        RefreshNumbers();

        MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
        if (window != nullptr) 
            window->eventWindowButtonPressed += newDelegate(this, &ColorEditor::NotifyWindowButtonPressed);
        
        InitColorPane();
        
        ResetButtons();
    }

	ColorEditor::~ColorEditor()
	{
        DeleteNumbersX();
        DeleteNumbersY();
        DestroyTexture();
    }
    
    void ColorEditor::ResetButtons()
    {
        for (auto btn : mButtons)
            mCanvasWidget->_destroyChildWidget(btn);
        mButtons.clear();
        
        AddButton(0, 0);
        AddButton(mBackground->getSize().width, 0);
    }
    
    void ColorEditor::SetColors(const ColorEditor::ColorMap& colors)
    {
        ResetButtons();
        
        auto size = mBackground->getSize();
        auto width = size.width;
        auto height = size.height;
        
        for (auto i = colors.begin(); i != colors.end(); ++i)
        {
            MyGUI::Button* btn = nullptr;
            
            if(DiMath::RealEqual(i->first, 0.0f))
            {
                mButtons.front()->setPosition(MyGUI::IntPoint(-7, (1.0f - i->second.a) * height - 7));
                btn = mButtons.front();
            }
            else if(DiMath::RealEqual(i->first, 1.0f))
            {
                mButtons.back()->setPosition(MyGUI::IntPoint(width - 7, (1.0f - i->second.a) * height - 7));
                btn = mButtons.back();
            }
            else
            {
                btn = AddButton(width * i->first, (1.0f - i->second.a) * height);
            }
            
            MyGUI::Colour color(i->second.r,i->second.g,i->second.b,i->second.a);
            btn->setUserData(MyGUI::Any(color));
        }
        
        RefreshCurve();
    }
    
    ColorEditor::ColorMap ColorEditor::GetColors()
    {
        ColorMap ret;
        auto size = mBackground->getSize();
        auto width = size.width;
        
        for (size_t i = 0; i < mButtons.size(); ++i)
        {
            auto pos = mButtons[i]->getPosition();
            float timePos = (float)(pos.left+7) / (float)width;
            auto col = *(mButtons[i]->getUserData<MyGUI::Colour>());
            auto diCol = DiColor(col.blue, col.green, col.red, col.alpha);
            ret[timePos] = diCol;
        }
        
        return ret;
    }

    void ColorEditor::InitGrid()
    {
        auto size = mCurveCanvasWidget->getSize();
        auto width = size.width;
        auto height= size.height;

        auto inteveralX = width / (gridNumX + 2);
        auto inteveralY = height / (gridNumY + 2);

        MyGUI::Colour dark(0.1f, 0.1f, 0.1f);
        MyGUI::Colour light(0.2f, 0.2f, 0.2f);

        std::vector<MyGUI::Colour> colors;
        std::vector<MyGUI::FloatPoint> lines;
        
        lines.push_back(MyGUI::FloatPoint(mBackground->getPosition().left, 3));
        lines.push_back(MyGUI::FloatPoint(mBackground->getPosition().left, height - 4));
        colors.push_back(light);
        colors.push_back(light);

        lines.push_back(MyGUI::FloatPoint(3, (gridNumY + 1)*inteveralY + 1));
        lines.push_back(MyGUI::FloatPoint(width - 4, (gridNumY + 1)*inteveralY + 1));
        colors.push_back(light);
        colors.push_back(light);

        mCurveLines->setPoints(lines, true);
        mCurveLines->setColors(colors);

        for (auto i = 1; i <= gridNumX + 1; i += 2)
        {
            MyGUI::TextBox* text = mCurveCanvasWidget->createWidget<MyGUI::TextBox>("TextBox",
                MyGUI::IntCoord(i * inteveralX + 2, height - 19, 30, 15), MyGUI::Align::Default);
            text->setFontHeight(14);
            text->setTextShadow(true);
            text->setTextShadowColour(MyGUI::Colour::Black);
            DiString s;
            s.Format("%.1f", (i - 1) / (float)gridNumX);
            text->setCaption(s.c_str());
            mNumbersX.push_back(text);
        }
    }
    
    void ColorEditor::InitColorPane()
    {
        mColourRect->eventMouseButtonPressed += MyGUI::newDelegate(this, &ColorEditor::NotifyColPaneMouseButtonPressed);
		mColourRect->eventMouseDrag += MyGUI::newDelegate(this, &ColorEditor::NotifyColPaneMouseDrag);
		mImageColourPicker->eventMouseDrag += MyGUI::newDelegate(this, &ColorEditor::NotifyColPaneMouseDrag);
		mScrollRange->eventScrollChangePosition += MyGUI::newDelegate(this, &ColorEditor::NotifyScrollChangePosition);
        
        mColourRange.push_back(MyGUI::Colour(1, 0, 0));
		mColourRange.push_back(MyGUI::Colour(1, 0, 1));
		mColourRange.push_back(MyGUI::Colour(0, 0, 1));
		mColourRange.push_back(MyGUI::Colour(0, 1, 1));
		mColourRange.push_back(MyGUI::Colour(0, 1, 0));
		mColourRange.push_back(MyGUI::Colour(1, 1, 0));
		mColourRange.push_back(mColourRange[0]);
        
        UpdateFirst();
        
        SetColour(MyGUI::Colour::White);
    }
    
    void ColorEditor::CreateTexture()
	{
        mTextureName = MyGUI::utility::toString((size_t)this, "_ColorEditorGradient");
        mBackTextureName = MyGUI::utility::toString((size_t)this, "_ColorEditorBackground");
        
		MyGUI::uint size = 32;
		mTexture = MyGUI::RenderManager::getInstance().createTexture(mTextureName);
		mTexture->createManual(size, size,
                               MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Write,
                               MyGUI::PixelFormat::R8G8B8A8);
		mColourRect->setImageTexture(mTextureName);
        
        mBackTexture = MyGUI::RenderManager::getInstance().createTexture(mBackTextureName);
		mBackTexture->createManual(mBackground->getSize().width, mBackground->getSize().height,
                               MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Write,
                               MyGUI::PixelFormat::R8G8B8A8);
		mBackgroundColourRect->setImageTexture(mBackTextureName);
	}
    
    void ColorEditor::NotifyScrollChangePosition(MyGUI::ScrollBar* _sender, size_t _position)
    {
        float sector_size = (float)mScrollRange->getScrollRange() / 6.0f;
		float sector_current = (float)_position / sector_size;
        
		// current sector
		size_t current = (size_t)sector_current;
		DI_ASSERT(current < 6);

		float offfset = (sector_current - (float)current);
        
		const MyGUI::Colour& from = mColourRange[current];
		const MyGUI::Colour& to = mColourRange[current + 1];
        
		mBaseColour.red = from.red + offfset * (to.red - from.red);
		mBaseColour.green = from.green + offfset * (to.green - from.green);
		mBaseColour.blue = from.blue + offfset * (to.blue - from.blue);
        
		UpdateTexture(mBaseColour);
        
		MyGUI::IntPoint point(
                              mImageColourPicker->getLeft() + (mImageColourPicker->getWidth() / 2),
                              mImageColourPicker->getTop() + (mImageColourPicker->getHeight() / 2));
        
		UpdateFromPoint(point);

    }

    void ColorEditor::RefreshNumbers()
    {
        DeleteNumbersY();

        auto size = mCurveCanvasWidget->getSize();
        auto height = size.height;
        
        {
            MyGUI::TextBox* text = mCurveCanvasWidget->createWidget<MyGUI::TextBox>("TextBox",
                MyGUI::IntCoord(6, 5, 20, 15), MyGUI::Align::Default);
            text->setFontHeight(14);
            text->setTextShadow(true);
            text->setTextShadowColour(MyGUI::Colour::Black);
            text->setCaption("1.0");
            mNumbersY.push_back(text);
        }
        {
            MyGUI::TextBox* text = mCurveCanvasWidget->createWidget<MyGUI::TextBox>("TextBox",
                MyGUI::IntCoord(6, height-35, 20, 15), MyGUI::Align::Default);
            text->setFontHeight(14);
            text->setTextShadow(true);
            text->setTextShadowColour(MyGUI::Colour::Black);
            text->setCaption("0.0");
            mNumbersY.push_back(text);
        }
    }

    void ColorEditor::DeleteNumbersX()
    {
        for (auto i : mNumbersX)
            mCurveCanvasWidget->_destroyChildWidget(i);
        mNumbersX.clear();
    }

    void ColorEditor::NotfyMousePressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        if (_id != MyGUI::MouseButton::Left)
            return;

        auto pos = mCanvasWidget->getAbsolutePosition();
        
        if (CheckPosition(_left - pos.left, _top - pos.top))
            AddButton(_left - pos.left, _top - pos.top);
    }

    void ColorEditor::DeleteNumbersY()
    {
        for (auto i : mNumbersY)
            mCurveCanvasWidget->_destroyChildWidget(i);
        mNumbersY.clear();
    }
    
    void ColorEditor::UpdateButtonColor(const MyGUI::Colour& value)
    {
        if(!mHighlighted)
            return;
        
        MyGUI::Colour v = value;
        mHighlighted->setUserData(v);
        
        UpdateBackTexture();
    }
    
    void ColorEditor::HighlightButton(MyGUI::Button* btn)
    {
        for (auto b : mButtons)
        {
            b->setColour(MyGUI::Colour::White);
            btn->setStateSelected(false);
        }
        btn->setColour(MyGUI::Colour::Red);
        btn->setStateSelected(true);
        mHighlighted = btn;
        
        MyGUI::Colour buttonCol = *(btn->getUserData<MyGUI::Colour>());
        SetColour(buttonCol);
    }

    MyGUI::Button* ColorEditor::AddButton(int _left, int _top)
    {
        auto btn = mCanvasWidget->createWidget<MyGUI::Button>("RoundButtonSkin",
            MyGUI::IntCoord(_left - 7, _top - 7, 15, 15),
            MyGUI::Align::Left | MyGUI::Align::Top);
        
        btn->eventMouseDrag += newDelegate(this, &ColorEditor::NotifyPointMove);
        btn->eventMouseButtonPressed += newDelegate(this, &ColorEditor::NotifyPointPressed);
        mButtons.push_back(btn);
        
        MyGUI::Colour color; // white
        if(mButtons.size() > 2)
            color = GetColorAtPoint(_left, _top);
        
        btn->setUserData(MyGUI::Any(color));

        std::sort(mButtons.begin(), mButtons.end(),
            [](MyGUI::Button* a, MyGUI::Button* b) {
            return a->getPosition().left < b->getPosition().left; 
        });

        if(mButtons.size() > 2)
            HighlightButton(btn);
        RefreshCurve();
        
        return btn;
    }

    void ColorEditor::RefreshCurve()
    {
        if (mButtons.size() < 2)
        {
            return;
        }

        std::vector<MyGUI::FloatPoint> linePoints;
        std::vector<MyGUI::Colour> colors;
        
        // line
        for (size_t i = 0; i < mButtons.size() - 1; ++i)
        {
            auto pos0 = mButtons[i]->getPosition();
            auto pos1 = mButtons[i+1]->getPosition();
            
            linePoints.push_back(MyGUI::FloatPoint(pos0.left + 7, pos0.top + 7));
            colors.push_back(MyGUI::Colour::White);
            
            linePoints.push_back(MyGUI::FloatPoint(pos1.left + 7, pos1.top + 7));
            colors.push_back(MyGUI::Colour::White);
        }
        
        mCurveCanvas->setPoints(linePoints,true);
        mCurveCanvas->setColors(colors);
        
        UpdateBackTexture();
    }

    void ColorEditor::NotifyPointMove(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        if (_id != MyGUI::MouseButton::Left)
            return;
        
        auto size = mBackground->getSize();
        auto inteveralX = size.width / (gridNumX + 2);
        auto inteveralY = size.height / (gridNumY + 2);
    
        auto pos = MyGUI::IntPoint(_left, _top) - _sender->getCroppedParent()->getAbsolutePosition();
        if (!CheckPosition(pos.left, pos.top))
            return;

        if(_sender == mButtons.front())
            pos.left = 0;
        else if(_sender == mButtons.back())
            pos.left = size.width;
        
        pos.left -= 7;
        pos.top -= 7;
        
        _sender->setPosition(pos);

        std::sort(mButtons.begin(), mButtons.end(),
            [](MyGUI::Button* a, MyGUI::Button* b) {
            return a->getPosition().left < b->getPosition().left;
        });

        RefreshCurve();
    }

    bool ColorEditor::CheckPosition(int _left, int _top)
    {
        auto size = mBackground->getSize();
        auto width = size.width;
        auto height = size.height;

        if (_left >= 0 && _top >= 0 &&
            _left < width &&
            _top < height)
        {
            return true;
        }
        return false;
    }

    void ColorEditor::NotifyPointPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        if (_id == MyGUI::MouseButton::Right)
        {
            if (mButtons.size() <= 2)
                return;
        
            DI_ASSERT(mButtons.size() >= 2);
            if(_sender == mButtons.front() ||
               _sender == mButtons.back())
                return;

            auto it = std::find(mButtons.begin(), mButtons.end(), _sender);
            if (it != mButtons.end())
            {
                mCanvasWidget->_destroyChildWidget(*it);
                mButtons.erase(it);
                mHighlighted = nullptr;
                RefreshCurve();
            }
        }
        else if(_id == MyGUI::MouseButton::Left)
        {
            HighlightButton(dynamic_cast<MyGUI::Button*>(_sender));
        }
    }

    DiVec2 ColorEditor::GetValue(int _left, int _top)
    {
        DiVec2 ret;

        auto size = mCurveCanvasWidget->getSize();
        auto width = size.width;
        auto height = size.height;
        auto inteveralX = width / (gridNumX + 2);
        auto inteveralY = height / (gridNumY + 2);

        int posX = _left + 1 - inteveralX;
        int posY = height - _top - 3 - inteveralY;

        float rangeX = width - inteveralX * 2 - 4;
        float rangeY = height - inteveralY * 2 - 4;

        ret.x = posX / rangeX;
        ret.y = posY / rangeY;
        ret.y = ret.y;

        return ret;
    }
    
    void ColorEditor::NotifyEndColorPickerDialog(Dialog* _sender, bool _result)
    {
        ColourPanel* colorPanel = dynamic_cast<ColourPanel*>(_sender);
        DI_ASSERT(colorPanel);
        colorPanel->endModal();
        
        if (_result)
        {
            auto c = colorPanel->getColour();
            DiColor col(c.red, c.green, c.blue, c.alpha);
        }
        
        delete colorPanel;
    }

    void ColorEditor::NotifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _button)
    {
        if (_button == "close")
        {
            mMainWidget->setVisible(false);
            eventUpdateColors.clear();
        }
    }
    
    void ColorEditor::UpdateFirst()
    {
        NotifyScrollChangePosition(nullptr, mScrollRange->getScrollPosition());
        
		NotifyColPaneMouseDrag(nullptr,
                        mImageColourPicker->getAbsoluteLeft() + (mColourRect->getWidth() / 2),
                        mImageColourPicker->getAbsoluteTop() + (mColourRect->getHeight() / 2),
                        MyGUI::MouseButton::Left);
    }
    
    void ColorEditor::UpdateBackTexture()
    {
        auto size = mBackground->getSize();
        
        MyGUI::uint8* pDest = static_cast<MyGUI::uint8*>(mBackTexture->lock(MyGUI::TextureUsage::Write));
        
        for (auto i = 0; i < size.width; ++i)
        {
            MyGUI::Colour final = GetColorAtPoint(i);
            
            MyGUI::uint8* curt = pDest;
            for (auto j = 0; j < size.height; ++j)
            {
                MyGUI::uint8* temp = curt;
                *temp++ = MyGUI::uint8(final.blue  * 255); // B
				*temp++ = MyGUI::uint8(final.green * 255); // G
				*temp++ = MyGUI::uint8(final.red   * 255); // R
				*temp++ = MyGUI::uint8(final.alpha * 255); // A
                
                curt += 4 * size.width;
            }
            pDest += 4;
        }
        
        mBackTexture->unlock();
        
        auto updatecolors = GetColors();
        eventUpdateColors(updatecolors);
    }
    
    void ColorEditor::UpdateTexture(const MyGUI::Colour& _colour)
    {
        size_t size = 32;
        
		MyGUI::uint8* pDest = static_cast<MyGUI::uint8*>(mTexture->lock(MyGUI::TextureUsage::Write));
        
		for (size_t j = 0; j < size; j++)
			for (size_t i = 0; i < size; i++)
			{
				float x = (float)i / size;
				float y = (float)j / size;
				*pDest++ = MyGUI::uint8((1. - y) * (_colour.blue  * x + (1. - x)) * 255); // B
				*pDest++ = MyGUI::uint8((1. - y) * (_colour.green * x + (1. - x)) * 255); // G
				*pDest++ = MyGUI::uint8((1. - y) * (_colour.red   * x + (1. - x)) * 255); // R
				*pDest++ = 255; // A
			}
        
		// Unlock the pixel buffer
		mTexture->unlock();
        
        auto updatecolors = GetColors();
        eventUpdateColors(updatecolors);
    }
    
    void ColorEditor::DestroyTexture()
    {
        MyGUI::RenderManager::getInstance().destroyTexture( mTexture );
		mTexture = nullptr;
        
        MyGUI::RenderManager::getInstance().destroyTexture( mBackTexture );
		mBackTexture = nullptr;
    }
    
    void ColorEditor::UpdateFromPoint(const MyGUI::IntPoint& _point)
    {
        // get colour by cursor position Altren 09.2008
		float x = 1.0f * _point.left / mColourRect->getWidth();
		float y = 1.0f * _point.top / mColourRect->getHeight();
		if (x > 1) x = 1;
		else if (x < 0) x = 0;
		if (y > 1) y = 1;
		else if (y < 0) y = 0;
        
		mCurrentColour.red = (1 - y) * (mBaseColour.red * x + MyGUI::Colour::White.red * (1 - x));
		mCurrentColour.green = (1 - y) * (mBaseColour.green * x + MyGUI::Colour::White.green * (1 - x));
		mCurrentColour.blue = (1 - y) * (mBaseColour.blue * x + MyGUI::Colour::White.blue * (1 - x));
        
		mEditRed->setCaption(MyGUI::utility::toString((int)(mCurrentColour.red * 255)));
		mEditGreen->setCaption(MyGUI::utility::toString((int)(mCurrentColour.green * 255)));
		mEditBlue->setCaption(MyGUI::utility::toString((int)(mCurrentColour.blue * 255)));
        
        UpdateButtonColor(mCurrentColour);
    }
    
    MyGUI::Colour ColorEditor::GetColorAtPoint(int _left) const
    {
        auto size = mBackground->getSize();
        float timeFraction = (float)_left / size.width;
        
        auto it = mButtons.begin();
        for (; it != mButtons.end(); ++it)
        {
            float btnFraction = ((*it)->getPosition().left+7) / (float)mBackground->getSize().width;
            if (timeFraction < btnFraction)
            {
                if (it == mButtons.begin())
                    break;
                else
                {
                    --it;
                    break;
                }
            }
        }
        
        auto it2 = it;
        it2++;
        
        MyGUI::Colour col1 = *((*it)->getUserData<MyGUI::Colour>());
        MyGUI::Colour col2 = *((*it2)->getUserData<MyGUI::Colour>());
        float alpha1 = ((*it)->getPosition().top+7) / (float)mBackground->getSize().height;
        float alpha2 = ((*it2)->getPosition().top+7) / (float)mBackground->getSize().height;
        col1.alpha = 1.0f - alpha1;
        col2.alpha = 1.0f - alpha2;
        float fra1 = ((*it)->getPosition().left+7) / (float)mBackground->getSize().width;
        float fra2 = ((*it2)->getPosition().left+7) / (float)mBackground->getSize().width;
        
        MyGUI::Colour final;
        if (it2 != mButtons.end())
            final = col1 + ((col2 - col1) * ((timeFraction - fra1)/(fra2 - fra1)));
        else
            final = col1;
        
        DiMath::Clamp(final.red, 0.0f, 1.0f);
        DiMath::Clamp(final.green, 0.0f, 1.0f);
        DiMath::Clamp(final.blue, 0.0f, 1.0f);
        
        return final;

    }
    
    MyGUI::Colour ColorEditor::GetColorAtPoint(int _left, int _top) const
    {
        auto final = GetColorAtPoint(_left);
        
        final.alpha = 1.0f - (_top+7) / (float)mBackground->getSize().height;
        DiMath::Clamp(final.alpha, 0.0f, 1.0f);
        
        return final;
    }
    
    MyGUI::Colour ColorEditor::GetSaturate(const MyGUI::Colour& _colour) const
	{
		MyGUI::Colour colour = _colour;
		if (colour.red < 0)
			colour.red = 0;
		else if (colour.red > 1)
			colour.red = 1;
        
		if (colour.green < 0)
			colour.green = 0;
		else if (colour.green > 1)
			colour.green = 1;
        
		if (colour.blue < 0)
			colour.blue = 0;
		else if (colour.blue > 1)
			colour.blue = 1;
        
		return colour;
	}
    
    void ColorEditor::SetColour(const MyGUI::Colour& _colour)
    {
        MyGUI::Colour colour = GetSaturate(_colour);
		mEditRed->setCaption(MyGUI::utility::toString((int)(colour.red * 255)));
		mEditGreen->setCaption(MyGUI::utility::toString((int)(colour.green * 255)));
		mEditBlue->setCaption(MyGUI::utility::toString((int)(colour.blue * 255)));
        
		UpdateFromColour(colour);
    }
    
    float& ColorEditor::ByIndex(MyGUI::Colour& _colour, size_t _index)
    {
        if (_index == 0) return _colour.red;
		else if (_index == 1) return _colour.green;
		else if (_index == 2) return _colour.blue;
		else return _colour.alpha;
    }
    
    void ColorEditor::UpdateFromColour(const MyGUI::Colour& _colour)
    {
        mCurrentColour = _colour;
        mCurrentColour.alpha = 1;
        
		std::vector<float> vec;
		vec.push_back(mCurrentColour.red);
		vec.push_back(mCurrentColour.green);
		vec.push_back(mCurrentColour.blue);
		std::sort(vec.begin(), vec.end());
        
		MyGUI::IntPoint point((int)((1 - vec[0] / vec[2]) * mColourRect->getWidth()),
                              (int)((1 - vec[2]) * mColourRect->getHeight()));
        
		mImageColourPicker->setPosition(point.left - (mImageColourPicker->getWidth() / 2), point.top - (mImageColourPicker->getHeight() / 2));
        
		int iMax = (mCurrentColour.red == vec[2]) ? 0 : (mCurrentColour.green == vec[2]) ? 1 : 2;
		int iMin = (mCurrentColour.red == vec[0]) ? 0 : (mCurrentColour.green == vec[0]) ? 1 : 2;
		int iAvg = 3 - iMax - iMin;
        
		if (iMin == iMax) // if gray colour - set base red
		{
			iMax = 0;
			iMin = 1;
			iAvg = 2;
			ByIndex(mBaseColour, iMin) = 0.;
			ByIndex(mBaseColour, iAvg) = 0.;
			ByIndex(mBaseColour, iMax) = 1.;
		}
		else
		{
			ByIndex(mBaseColour, iMin) = 0.;
			ByIndex(mBaseColour, iAvg) = (vec[1] - vec[0]) / (vec[2] - vec[0]);
			ByIndex(mBaseColour, iMax) = 1.;
		}
        
		int i;
		for (i = 0; i < 6; ++i)
		{
			if ((fabs(ByIndex(mColourRange[i], iMin) - ByIndex(mBaseColour, iMin)) < 0.001) &&
				(fabs(ByIndex(mColourRange[i], iMax) - ByIndex(mBaseColour, iMax)) < 0.001) &&
				(fabs(ByIndex(mColourRange[i+1], iMin) - ByIndex(mBaseColour, iMin)) < 0.001) &&
				(fabs(ByIndex(mColourRange[i+1], iMax) - ByIndex(mBaseColour, iMax)) < 0.001))
				break;
		}
        
		float sector_size = (float)mScrollRange->getScrollRange() / 6.0f;
		size_t current = i;
        
		float offset = ByIndex(mBaseColour, iAvg);
		if (ByIndex(mColourRange[i+1], iAvg) < ByIndex(mColourRange[i], iAvg))
            offset = 1 - ByIndex(mBaseColour, iAvg);
        
		size_t pos = size_t((current + offset) * sector_size);
        
		mScrollRange->setScrollPosition(pos);
        
		mBaseColour.red = mColourRange[i].red + offset * (mColourRange[i+1].red - mColourRange[i].red);
		mBaseColour.green = mColourRange[i].green + offset * (mColourRange[i+1].green - mColourRange[i].green);
		mBaseColour.blue = mColourRange[i].blue + offset * (mColourRange[i+1].blue - mColourRange[i].blue);
        
		UpdateTexture(mBaseColour);
        
        UpdateButtonColor(mCurrentColour);
    }
    
    void ColorEditor::NotifyColPaneMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        if (_id != MyGUI::MouseButton::Left)
			return;
        
		MyGUI::Widget* parent = mImageColourPicker->getParent();
		MyGUI::IntPoint point(_left - parent->getAbsoluteLeft(), _top - parent->getAbsoluteTop());
        
		if (point.left < 0) point.left = 0;
		if (point.top < 0) point.top = 0;
		if (point.left > mColourRect->getWidth()) point.left = mColourRect->getWidth();
		if (point.top > mColourRect->getHeight()) point.top = mColourRect->getHeight();
        
		mImageColourPicker->setPosition(point.left - (mImageColourPicker->getWidth() / 2), point.top - (mImageColourPicker->getHeight() / 2));
        
		UpdateFromPoint(point);
    }
    
    void ColorEditor::NotifyColPaneMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        if (_id == MyGUI::MouseButton::Left)
			NotifyColPaneMouseDrag(nullptr, _left, _top, _id);
    }

} // Demi

#undef gridNumX
#undef gridNumY