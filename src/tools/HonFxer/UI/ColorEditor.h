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

#ifndef _COLOR_EDITOR_H_
#define _COLOR_EDITOR_H_

#include "FxerPrerequisites.h"
#include "BaseLayout.h"

namespace Demi
{
	ATTRIBUTE_CLASS_LAYOUT(ColorEditor, "FxColorEditor.layout");
	class ColorEditor : public wraps::BaseLayout
	{
	public:
        ColorEditor(MyGUI::Widget* _parent = nullptr);
        virtual ~ColorEditor();

	private:
	//%LE Widget_Declaration list start
        ATTRIBUTE_FIELD_WIDGET_NAME(ColorEditor, mCurveCanvasWidget, "CurveCanvas");
        MyGUI::Widget* mCurveCanvasWidget;
	//%LE Widget_Declaration list end

    private:

        void InitGrid();

        void RefreshNumbers();

        void RefreshCurve();

        void DeleteNumbersX();

        void DeleteNumbersY();

        void NotfyMousePressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

        void NotifyPointMove(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

        void NotifyPointPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

        void NotifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _button);

        void AddButton(int _left, int _top);

        bool CheckPosition(int _left, int _top);

        DiVec2 GetValue(int _left, int _top);

    protected:

        DiVec2 mValueRange{ 0, 1 };

        MyGUI::PolygonalSkin* mCurveCanvas{ nullptr };

        MyGUI::PolygonalSkin* mCurveLines{ nullptr };

        DiVector<MyGUI::TextBox*> mNumbersX;

        DiVector<MyGUI::TextBox*> mNumbersY;

        MyGUI::Widget* mCanvasWidget{ nullptr };

        DiVector<MyGUI::Button*> mButtons;
    };

} // namespace Demi

#endif // _CURVE_EDITOR_H_
