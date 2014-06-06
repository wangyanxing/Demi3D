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
        
        ATTRIBUTE_FIELD_WIDGET_NAME(ColorEditor, mColourRect, "widget_ColourRect");
		MyGUI::ImageBox* mColourRect;
        
        ATTRIBUTE_FIELD_WIDGET_NAME(ColorEditor, mImageColourPicker, "image_Picker");
		MyGUI::ImageBox* mImageColourPicker;
        
        ATTRIBUTE_FIELD_WIDGET_NAME(ColorEditor, mScrollRange, "scroll_Range");
		MyGUI::ScrollBar* mScrollRange;
        
        ATTRIBUTE_FIELD_WIDGET_NAME(ColorEditor, mEditRed, "edit_Red");
		MyGUI::EditBox* mEditRed;
        
		ATTRIBUTE_FIELD_WIDGET_NAME(ColorEditor, mEditGreen, "edit_Green");
		MyGUI::EditBox* mEditGreen;
        
		ATTRIBUTE_FIELD_WIDGET_NAME(ColorEditor, mEditBlue, "edit_Blue");
		MyGUI::EditBox* mEditBlue;
	//%LE Widget_Declaration list end

    private:
        
        void InitColorPane();

        void InitGrid();

        void RefreshNumbers();

        void RefreshCurve();

        void DeleteNumbersX();

        void DeleteNumbersY();
        
        void CreateTexture();

        void NotfyMousePressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
        
        void NotifyEndColorPickerDialog(Dialog* _sender, bool _result);

        void NotifyPointMove(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

        void NotifyPointPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

        void NotifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _button);
        
        void NotifyColPaneMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
        
		void NotifyColPaneMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
        
        void NotifyScrollChangePosition(MyGUI::ScrollBar* _sender, size_t _position);

        void AddButton(int _left, int _top);

        bool CheckPosition(int _left, int _top);

        DiVec2 GetValue(int _left, int _top);
        
        void UpdateFirst();
        
		void UpdateTexture(const MyGUI::Colour& _colour);
        
		void UpdateBackTexture();
		
        void DestroyTexture();
        
		void UpdateFromPoint(const MyGUI::IntPoint& _point);
		
        void UpdateFromColour(const MyGUI::Colour& _colour);
        
        float& ByIndex(MyGUI::Colour& _colour, size_t _index);
        
        void SetColour(const MyGUI::Colour& _colour);
        
        MyGUI::Colour GetSaturate(const MyGUI::Colour& _colour) const;
        
        MyGUI::Colour GetColorAtPoint(int _left, int _top) const;
        
        MyGUI::Colour GetColorAtPoint(int _left) const;
        
        void HighlightButton(MyGUI::Button*);
        
        void UpdateButtonColor(const MyGUI::Colour& value);

    protected:

        DiVec2 mValueRange{ 0, 1 };

        MyGUI::PolygonalSkin* mCurveCanvas{ nullptr };

        MyGUI::PolygonalSkin* mCurveLines{ nullptr };

        DiVector<MyGUI::TextBox*> mNumbersX;

        DiVector<MyGUI::TextBox*> mNumbersY;

        MyGUI::Widget* mCanvasWidget{ nullptr };

        DiVector<MyGUI::Button*> mButtons;
        
        MyGUI::Widget* mBackground{ nullptr };
        
        MyGUI::ITexture* mTexture;
        
		MyGUI::UString mTextureName;
        
        MyGUI::ITexture* mBackTexture;
        
		MyGUI::UString mBackTextureName;
        
        MyGUI::Colour mCurrentColour;
        
		MyGUI::Colour mBaseColour;
        
		std::vector<MyGUI::Colour> mColourRange;
        
        MyGUI::ImageBox* mBackgroundColourRect{ nullptr };
        
        MyGUI::Button* mHighlighted{ nullptr };
    };

} // namespace Demi

#endif // _CURVE_EDITOR_H_
