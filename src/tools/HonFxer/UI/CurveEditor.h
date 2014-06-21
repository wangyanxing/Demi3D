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

#ifndef _CURVE_EDITOR_H_
#define _CURVE_EDITOR_H_

#include "FxerPrerequisites.h"
#include "BaseLayout.h"
#include "DynamicAttribute.h"

namespace Demi
{
	ATTRIBUTE_CLASS_LAYOUT(CurveEditor, "FxCurveEditor.layout");
	class CurveEditor : public wraps::BaseLayout
	{
	public:
        CurveEditor(MyGUI::Widget* _parent = nullptr);
        virtual ~CurveEditor();
        
        typedef MyGUI::delegates::CMultiDelegate1<DiAttributeCurved*> EventHandle_UpdateCurve;

	private:
	//%LE Widget_Declaration list start
        ATTRIBUTE_FIELD_WIDGET_NAME(CurveEditor, mCurveCanvasWidget, "CurveCanvas");
        MyGUI::Widget* mCurveCanvasWidget;
        ATTRIBUTE_FIELD_WIDGET_NAME(CurveEditor, mEditTimeEditBox, "EditTime");
        MyGUI::EditBox* mEditTimeEditBox;
        ATTRIBUTE_FIELD_WIDGET_NAME(CurveEditor, mEditValueEditBox, "EditValue");
        MyGUI::EditBox* mEditValueEditBox;
        ATTRIBUTE_FIELD_WIDGET_NAME(CurveEditor, mRangeMinEditBox, "RangeMin");
        MyGUI::EditBox* mRangeMinEditBox;
        ATTRIBUTE_FIELD_WIDGET_NAME(CurveEditor, mRangeMaxEditBox, "RangeMax");
        MyGUI::EditBox* mRangeMaxEditBox;
        ATTRIBUTE_FIELD_WIDGET_NAME(CurveEditor, mCheckLoopButton, "CheckLoop");
        MyGUI::Button* mCheckLoopButton;
        ATTRIBUTE_FIELD_WIDGET_NAME(CurveEditor, mEditLoopRangeEditBox, "EditLoopRange");
        MyGUI::EditBox* mEditLoopRangeEditBox;
	//%LE Widget_Declaration list end

    private:

        void InitGrid();

        void RefreshNumbers();

        void RefreshCurve();
        
        void RefreshDynAttribute();

        void DeleteNumbersX();

        void DeleteNumbersY();

        void NotifyRangeEditAccept(MyGUI::EditBox* _sender);

        void NotifyRangeLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old);

        void NotifyMousePressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

        void NotifyPointMove(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

        void NotifyPointPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

        void NotifySplineChecked(MyGUI::Widget* _sender);

        void NotifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _button);

        void AddButton(int _left, int _top);

        bool CheckPosition(int _left, int _top);

        DiVec2 GetValue(int _left, int _top);
        
        MyGUI::IntPoint GetButtonPoint(DiVec2 value);

        void RefreshRange();
        
        void NotifyValueUpdate();
        
    public:
        
        DiAttributeCurved& GetAttribute() { return mCurvedAttr; }
        
        void SetAttribute(DiAttributeCurved& rhs);
        
		EventHandle_UpdateCurve eventUpdateCurve;

    protected:

        DiVec2 mValueRange;

        MyGUI::PolygonalSkin* mCurveCanvas{ nullptr };

        MyGUI::PolygonalSkin* mCurveLines{ nullptr };

        DiVector<MyGUI::TextBox*> mNumbersX;

        DiVector<MyGUI::TextBox*> mNumbersY;

        MyGUI::Widget* mCanvasWidget{ nullptr };

        DiVector<MyGUI::Button*> mButtons;

        MyGUI::Button* mSplineButton;
        
        DiAttributeCurved mCurvedAttr;
    };

} // namespace Demi

#endif // _CURVE_EDITOR_H_
