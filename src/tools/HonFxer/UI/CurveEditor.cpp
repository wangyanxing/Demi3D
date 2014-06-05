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
#include "CurveEditor.h"
#include "MyGUI_PolygonalSkin.h"

#define gridNumX 20
#define gridNumY 20

namespace Demi
{
	CurveEditor::CurveEditor(MyGUI::Widget* _parent)
	{
        initialiseByAttributes(this, _parent);

        mEditTimeEditBox->setEditReadOnly(true);
        mEditValueEditBox->setEditReadOnly(true);

        mRangeMinEditBox->eventKeyLostFocus += MyGUI::newDelegate(this, &CurveEditor::NotifyRangeLostFocus);
        mRangeMinEditBox->eventEditSelectAccept += MyGUI::newDelegate(this, &CurveEditor::NotifyRangeEditAccept);
        mRangeMaxEditBox->eventKeyLostFocus += MyGUI::newDelegate(this, &CurveEditor::NotifyRangeLostFocus);
        mRangeMaxEditBox->eventEditSelectAccept += MyGUI::newDelegate(this, &CurveEditor::NotifyRangeEditAccept);

        MyGUI::PolygonalSkin::CreateLineSets = false;
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
        widget->eventMouseButtonPressed += MyGUI::newDelegate(this, &CurveEditor::NotfyMousePressed);
        MyGUI::PolygonalSkin::CreateLineSets = false;
        mCanvasWidget = widget;

        mCurveCanvas->setWidth(5.0f);
        std::vector<MyGUI::FloatPoint> mLinePoints;
        mLinePoints.push_back(MyGUI::FloatPoint(0, 0));
        mLinePoints.push_back(MyGUI::FloatPoint(0, 1));
        mCurveCanvas->setPoints(mLinePoints);
        mLinePoints.clear();
        mCurveCanvas->setPoints(mLinePoints); // todo fix it 

        mCurveLines->_setColour(MyGUI::Colour(0.2f, 0.2f, 0.2f));

        // default range
        mValueRange.x = 0;
        mValueRange.y = 10;
        mRangeMinEditBox->setCaption("0");
        mRangeMaxEditBox->setCaption("10");

        InitGrid();

        RefreshNumbers();

        mSplineButton = mMainWidget->createWidget<MyGUI::Button>("CheckBox",
            MyGUI::IntCoord(15, 10, 95, 25),
            MyGUI::Align::Left | MyGUI::Align::Top);
        mSplineButton->setCaption("Use Spline");
        mSplineButton->eventMouseButtonClick += MyGUI::newDelegate(this, &CurveEditor::NotifySplineChecked);
    }

	CurveEditor::~CurveEditor()
	{
        DeleteNumbersX();
        DeleteNumbersY();
    }

    void CurveEditor::InitGrid()
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

    void CurveEditor::RefreshNumbers()
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

    void CurveEditor::DeleteNumbersX()
    {
        for (auto i : mNumbersX)
            delete i;
        mNumbersX.clear();
    }

    void CurveEditor::NotfyMousePressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        if (_id != MyGUI::MouseButton::Left)
            return;

        auto pos = _sender->getAbsolutePosition();
        if (CheckPosition(_left, _top))
        {
            AddButton(_left - pos.left, _top - pos.top);
        }
    }

    void CurveEditor::DeleteNumbersY()
    {
        for (auto i : mNumbersY)
        {
            mCurveCanvasWidget->_destroyChildWidget(i);
        }
        mNumbersY.clear();
    }

    void CurveEditor::AddButton(int _left, int _top)
    {
        auto btn = mCanvasWidget->createWidget<MyGUI::Button>("RoundButtonSkin",
            MyGUI::IntCoord(_left - 7, _top - 7, 15, 15),
            MyGUI::Align::Left | MyGUI::Align::Top);
        btn->eventMouseDrag += newDelegate(this, &CurveEditor::NotifyPointMove);
        btn->eventMouseButtonPressed += newDelegate(this, &CurveEditor::NotifyPointPressed);

        mButtons.push_back(btn);
        std::sort(mButtons.begin(), mButtons.end(),
            [](MyGUI::Button* a, MyGUI::Button* b) {
            return a->getPosition().left < b->getPosition().left; 
        });

        RefreshCurve();
    }

    void CurveEditor::RefreshCurve()
    {
        if (mButtons.size() < 2)
            return;

        std::vector<MyGUI::FloatPoint> mLinePoints;

        // line
        if (!mSplineButton->getStateSelected())
        {
            for (auto btn : mButtons)
            {
                auto pos = btn->getPosition();
                mLinePoints.push_back(MyGUI::FloatPoint(pos.left + 7, pos.top + 7));
            }
        }
        else
        {
            int nums = 100;
            float step = 1.0f / nums;

            DiSpline spline;
            for (auto btn : mButtons)
            {
                auto pos = btn->getPosition();
                spline.AddPoint(DiVec3(pos.left + 7, pos.top + 7, 0));
            }

            for (int i = 0; i < nums; i++)
            {
                auto v = spline.Interpolate(i*step);
                mLinePoints.push_back(MyGUI::FloatPoint(v.x, v.y));
            }
        }

        mCurveCanvas->setPoints(mLinePoints);
    }

    void CurveEditor::NotifyPointMove(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        if (_id != MyGUI::MouseButton::Left)
            return;

        if (!CheckPosition(_left, _top))
            return;

        auto pos = MyGUI::IntPoint(_left, _top) - _sender->getCroppedParent()->getAbsolutePosition();
        auto value = GetValue(pos.left, pos.top);

        pos.left -= 7;
        pos.top -= 7;
        _sender->setPosition(pos);

        DiString str;
        str.Format("%.3g", value.x);
        mEditTimeEditBox->setCaption(str.c_str());
        str.Format("%.3g", value.y);
        mEditValueEditBox->setCaption(str.c_str());

        std::sort(mButtons.begin(), mButtons.end(),
            [](MyGUI::Button* a, MyGUI::Button* b) {
            return a->getPosition().left < b->getPosition().left;
        });

        RefreshCurve();
    }

    bool CurveEditor::CheckPosition(int _left, int _top)
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

    void CurveEditor::NotifyPointPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        if (_id != MyGUI::MouseButton::Right)
            return;

        auto it = std::find(mButtons.begin(), mButtons.end(), _sender);
        if (it != mButtons.end())
        {
            mCanvasWidget->_destroyChildWidget(*it);
            mButtons.erase(it);
            RefreshCurve();
        }
    }

    void CurveEditor::NotifySplineChecked(MyGUI::Widget* _sender)
    {
        mSplineButton->setStateSelected(!mSplineButton->getStateSelected());
        RefreshCurve();
    }

    DiVec2 CurveEditor::GetValue(int _left, int _top)
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

    void CurveEditor::NotifyRangeEditAccept(MyGUI::EditBox* _sender)
    {
        RefreshRange();
    }

    void CurveEditor::NotifyRangeLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old)
    {
        RefreshRange();
    }

    void CurveEditor::RefreshRange()
    {
        DiString minStr = mRangeMinEditBox->getCaption().asUTF8_c_str();
        DiString maxStr = mRangeMaxEditBox->getCaption().asUTF8_c_str();

        mValueRange.x = minStr.AsFloat();
        mValueRange.y = maxStr.AsFloat();

        RefreshNumbers();
    }

} // Demi

#undef gridNumX
#undef gridNumY