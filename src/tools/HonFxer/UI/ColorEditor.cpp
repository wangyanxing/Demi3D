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

#define gridNumX 20
#define gridNumY 20

namespace Demi
{
	ColorEditor::ColorEditor(MyGUI::Widget* _parent)
	{
        initialiseByAttributes(this, _parent);

        MyGUI::PolygonalSkin::CreateLineSets = true;
        MyGUI::Widget* widget = mCurveCanvasWidget->createWidget<MyGUI::Widget>("PolygonalSkin",
            MyGUI::IntCoord(MyGUI::IntPoint(), mCurveCanvasWidget->getSize()), MyGUI::Align::Stretch);
        widget->setColour(MyGUI::Colour::Red);
        MyGUI::ISubWidget* main = widget->getSubWidgetMain();
        mCurveLines = main->castType<MyGUI::PolygonalSkin>();
        
        MyGUI::PolygonalSkin::CreateLineSets = true;
        widget = mCurveCanvasWidget->createWidget<MyGUI::Widget>("PolygonalSkin",
            MyGUI::IntCoord(MyGUI::IntPoint(), mCurveCanvasWidget->getSize()), MyGUI::Align::Stretch);
        widget->setColour(MyGUI::Colour::Red);
        main = widget->getSubWidgetMain();
        mCurveCanvas = main->castType<MyGUI::PolygonalSkin>();
        widget->eventMouseButtonPressed += MyGUI::newDelegate(this, &ColorEditor::NotfyMousePressed);
        MyGUI::PolygonalSkin::CreateLineSets = false;
        mCanvasWidget = widget;

        mCurveCanvas->setWidth(10.0f);
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
        
        auto pos = mCanvasWidget->getAbsolutePosition();
        auto size = mCurveCanvasWidget->getSize();
        auto width = size.width;
        auto height = size.height;
        auto inteveralX = width / (gridNumX + 2);
        auto inteveralY = height / (gridNumY + 2);
        
        AddButton(inteveralX - 1, inteveralY + 3);
        AddButton(inteveralX * (gridNumX + 1) - 1, inteveralY + 3);
    }

	ColorEditor::~ColorEditor()
	{
        DeleteNumbersX();
        DeleteNumbersY();
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
        for (auto i = 1; i <= gridNumX+1; ++i)
        {
            lines.push_back(MyGUI::FloatPoint(i*inteveralX, 3));
            lines.push_back(MyGUI::FloatPoint(i*inteveralX, height-4));

            colors.push_back(i == 1 ? dark : light);
            colors.push_back(i == 1 ? dark : light);
        }

        lines.push_back(MyGUI::FloatPoint(inteveralX - 1, 3));
        lines.push_back(MyGUI::FloatPoint(inteveralX - 1, height - 4));
        colors.push_back(dark);
        colors.push_back(dark);

        for (auto i = 1; i <= gridNumY + 1; ++i)
        {
            lines.push_back(MyGUI::FloatPoint(3, i*inteveralY));
            lines.push_back(MyGUI::FloatPoint(width - 4, i*inteveralY));

            colors.push_back(i == gridNumY + 1 ? dark : light);
            colors.push_back(i == gridNumY + 1 ? dark : light);
        }

        lines.push_back(MyGUI::FloatPoint(3, (gridNumY + 1)*inteveralY + 1));
        lines.push_back(MyGUI::FloatPoint(width - 4, (gridNumY + 1)*inteveralY + 1));
        colors.push_back(dark);
        colors.push_back(dark);

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

    void ColorEditor::RefreshNumbers()
    {
        DeleteNumbersY();

        auto size = mCurveCanvasWidget->getSize();
        auto width = size.width;
        auto height = size.height;

        auto inteveralX = width / (gridNumX + 2);
        auto inteveralY = height / (gridNumY + 2);

        float step = (mValueRange.y - mValueRange.x) / gridNumY;
        
        for (auto i = 0; i <= gridNumY; i += 2)
        {
            MyGUI::TextBox* text = mCurveCanvasWidget->createWidget<MyGUI::TextBox>("TextBox",
                MyGUI::IntCoord(3, i * inteveralY + 5, 30, 15), MyGUI::Align::Default);
            text->setFontHeight(14);
            text->setTextShadow(true);
            text->setTextShadowColour(MyGUI::Colour::Black);
            DiString s;
            s.Format("%.1f", step*(gridNumY - i) + mValueRange.x);
            text->setCaption(s.c_str());
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

        auto pos = _sender->getAbsolutePosition();
        
        if (CheckPosition(_left, _top))
            AddButton(_left - pos.left, _top - pos.top);
    }

    void ColorEditor::DeleteNumbersY()
    {
        for (auto i : mNumbersY)
            mCurveCanvasWidget->_destroyChildWidget(i);
        mNumbersY.clear();
    }

    void ColorEditor::AddButton(int _left, int _top)
    {
        auto btn = mCanvasWidget->createWidget<MyGUI::Button>("RoundButtonSkin",
            MyGUI::IntCoord(_left - 7, _top - 7, 15, 15),
            MyGUI::Align::Left | MyGUI::Align::Top);
        btn->eventMouseDrag += newDelegate(this, &ColorEditor::NotifyPointMove);
        btn->eventMouseButtonPressed += newDelegate(this, &ColorEditor::NotifyPointPressed);

        mButtons.push_back(btn);
        std::sort(mButtons.begin(), mButtons.end(),
            [](MyGUI::Button* a, MyGUI::Button* b) {
            return a->getPosition().left < b->getPosition().left; 
        });

        RefreshCurve();
    }

    void ColorEditor::RefreshCurve()
    {
        if (mButtons.size() < 2)
            return;

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
    }

    void ColorEditor::NotifyPointMove(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        if (_id != MyGUI::MouseButton::Left)
            return;

        if (!CheckPosition(_left, _top))
            return;
        
        auto size = mCurveCanvasWidget->getSize();
        auto inteveralX = size.width / (gridNumX + 2);
        auto inteveralY = size.height / (gridNumY + 2);
    
        auto pos = MyGUI::IntPoint(_left, _top) - _sender->getCroppedParent()->getAbsolutePosition();

        if(_sender == mButtons.front())
            pos.left = inteveralX - 1;
        else if(_sender == mButtons.back())
            pos.left = inteveralX * (gridNumX + 1) - 1;
        
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
        auto pos = mCanvasWidget->getAbsolutePosition();

        auto size = mCurveCanvasWidget->getSize();
        auto width = size.width;
        auto height = size.height;
        auto inteveralX = width / (gridNumX + 2);
        auto inteveralY = height / (gridNumY + 2);

        int posX = _left - pos.left + 1 - inteveralX;
        int posY = height - (_top - pos.top) - 3 - inteveralY;

        if (posX >= 0 && posY >= 0 &&
            posX < inteveralX * gridNumX &&
            posY < inteveralY * gridNumY)
        {
            return true;
        }
        return false;
    }

    void ColorEditor::NotifyPointPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        if (_id != MyGUI::MouseButton::Right)
            return;
        
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
            RefreshCurve();
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
        ret.y = mValueRange.x + ret.y*(mValueRange.y - mValueRange.x);

        return ret;
    }

    void ColorEditor::NotifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _button)
    {
        if (_button == "close")
            mMainWidget->setVisible(false);
    }

} // Demi

#undef gridNumX
#undef gridNumY