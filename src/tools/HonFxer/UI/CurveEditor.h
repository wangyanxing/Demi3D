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

namespace Demi
{
	ATTRIBUTE_CLASS_LAYOUT(CurveEditor, "FxCurveEditor.layout");
	class CurveEditor : public wraps::BaseLayout
	{
	public:
        CurveEditor(MyGUI::Widget* _parent = nullptr);
        virtual ~CurveEditor();

	private:
	//%LE Widget_Declaration list start
        ATTRIBUTE_FIELD_WIDGET_NAME(CurveEditor, mCurveCanvasWidget, "CurveCanvas");
        MyGUI::Widget* mCurveCanvasWidget;
        ATTRIBUTE_FIELD_WIDGET_NAME(CurveEditor, mCurveTypeComboBox, "CurveType");
        MyGUI::ComboBox* mCurveTypeComboBox;
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

        void DeleteNumbers();

    protected:

        DiVec2 mValueRange;

        MyGUI::PolygonalSkin* mCurveCanvas{ nullptr };

        MyGUI::PolygonalSkin* mCurveLines{ nullptr };

        DiVector<MyGUI::TextBox*> mNumbersX;

        DiVector<MyGUI::TextBox*> mNumbersY;
    };

} // namespace Demi

#endif // _CURVE_EDITOR_H_
