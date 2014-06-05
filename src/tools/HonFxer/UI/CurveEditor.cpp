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
        MyGUI::PolygonalSkin::CreateLineSets = false;

        std::vector<MyGUI::FloatPoint> mLinePoints;
        mLinePoints.push_back(MyGUI::FloatPoint(10, 380));
        mLinePoints.push_back(MyGUI::FloatPoint(14, 80));
        mLinePoints.push_back(MyGUI::FloatPoint(214, 150));
        mLinePoints.push_back(MyGUI::FloatPoint(147, 150));

        mCurveCanvas->setWidth(5.0f);
        mCurveCanvas->setPoints(mLinePoints);

        mLinePoints.clear();
        mLinePoints.push_back(MyGUI::FloatPoint(0, 0));
        mLinePoints.push_back(MyGUI::FloatPoint(100, 100));

        mCurveLines->_setColour(MyGUI::Colour(0.2f, 0.2f, 0.2f));

        // default range
        mValueRange.x = 0;
        mValueRange.y = 10;

        InitGrid();

        RefreshNumbers();
    }

	CurveEditor::~CurveEditor()
	{
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

            if (i == 1)
            {
                colors.push_back(dark);
                colors.push_back(dark);
            }
            else
            {
                colors.push_back(light);
                colors.push_back(light);
            }
        }
        lines.push_back(MyGUI::FloatPoint(inteveralX - 1, 3));
        lines.push_back(MyGUI::FloatPoint(inteveralX - 1, height - 4));
        colors.push_back(dark);
        colors.push_back(dark);

        for (auto i = 1; i <= gridNumY + 1; ++i)
        {
            lines.push_back(MyGUI::FloatPoint(3, i*inteveralY));
            lines.push_back(MyGUI::FloatPoint(width - 4, i*inteveralY));

            if (i == gridNumY + 1)
            {
                colors.push_back(dark);
                colors.push_back(dark);
            }
            else
            {
                colors.push_back(light);
                colors.push_back(light);
            }
        }

        lines.push_back(MyGUI::FloatPoint(3, (gridNumY + 1)*inteveralY + 1));
        lines.push_back(MyGUI::FloatPoint(width - 4, (gridNumY + 1)*inteveralY + 1));
        colors.push_back(dark);
        colors.push_back(dark);

        mCurveLines->setPoints(lines, true);
        mCurveLines->setColors(colors);
    }

    void CurveEditor::RefreshNumbers()
    {
        DeleteNumbers();

        auto size = mCurveCanvasWidget->getSize();
        auto width = size.width;
        auto height = size.height;

        auto inteveralX = width / (gridNumX + 2);
        auto inteveralY = height / (gridNumY + 2);

        float range = mValueRange.y - mValueRange.x;
        
        for (auto i = 0; i <= gridNumY; i += 2)
        {
            MyGUI::TextBox* text = mCurveCanvasWidget->createWidget<MyGUI::TextBox>("TextBox",
                MyGUI::IntCoord(3, i * inteveralY + 5, 30, 15), MyGUI::Align::Default);
            text->setFontHeight(14);
            text->setTextShadow(true);
            text->setTextShadowColour(MyGUI::Colour::Black);
            text->setCaption("0.0");
            mNumbersY.push_back(text);
        }

        for (auto i = 1; i <= gridNumX+1; i += 2)
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

    void CurveEditor::DeleteNumbers()
    {
        for (auto i : mNumbersX)
            delete i;
        for (auto i : mNumbersY)
            delete i;
        mNumbersX.clear();
        mNumbersY.clear();
    }

} // Demi

#undef gridNumX
#undef gridNumY