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

namespace Demi
{
	CurveEditor::CurveEditor(MyGUI::Widget* _parent)
	{
		initialiseByAttributes(this, _parent);

        MyGUI::Widget* widget = mCurveCanvasWidget->createWidget<MyGUI::Widget>("PolygonalSkin", 
            MyGUI::IntCoord(MyGUI::IntPoint(), mCurveCanvasWidget->getSize()), MyGUI::Align::Stretch);
        widget->setColour(MyGUI::Colour::Red);
        MyGUI::ISubWidget* main = widget->getSubWidgetMain();
        mCurveCanvas = main->castType<MyGUI::PolygonalSkin>();

        widget = mCurveCanvasWidget->createWidget<MyGUI::Widget>("PolygonalSkin",
            MyGUI::IntCoord(MyGUI::IntPoint(), mCurveCanvasWidget->getSize()), MyGUI::Align::Stretch);
        widget->setColour(MyGUI::Colour::Red);
        main = widget->getSubWidgetMain();
        mCurveLines = main->castType<MyGUI::PolygonalSkin>();

        std::vector<MyGUI::FloatPoint> mLinePoints;
        mLinePoints.push_back(MyGUI::FloatPoint(10, 80));
        mLinePoints.push_back(MyGUI::FloatPoint(14, 80));
        mLinePoints.push_back(MyGUI::FloatPoint(14, 150));
        mLinePoints.push_back(MyGUI::FloatPoint(147, 150));

        mCurveCanvas->setWidth(3.0f);
        mCurveCanvas->setPoints(mLinePoints);

        mLinePoints.clear();
        mLinePoints.push_back(MyGUI::FloatPoint(50, 80));
        mLinePoints.push_back(MyGUI::FloatPoint(50, 190));
        mLinePoints.push_back(MyGUI::FloatPoint(190, 60));

        mCurveLines->setWidth(1.0f);
        mCurveLines->_setColour(MyGUI::Colour::White);
        mCurveLines->setPoints(mLinePoints);
    }

	CurveEditor::~CurveEditor()
	{
	}

    void CurveEditor::InitGrid()
    {
        auto size = mCurveCanvasWidget->getSize();
        auto width = size.width;
        auto height= size.height;

        auto gridNumX = 12;

        for (auto i = 0; i < gridNumX; ++i)
        {

        }
    }

} // Demi
