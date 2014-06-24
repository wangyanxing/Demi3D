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

#ifndef __PROPERTY_ITEM_H__
#define __PROPERTY_ITEM_H__

#include "FxerPrerequisites.h"
#include "Property.h"

namespace Demi
{
    class DiPropertyItem
    {
    public:

        DiPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type);

        virtual ~DiPropertyItem();

    public:

        virtual void CreateUI(const DiString& caption) = 0;

        virtual int GetHeight();

        virtual void RearrangeUI(int height) = 0;

    protected:

        DiPanelGroup* mParentGroup{ nullptr };

        DiPropertyBase* mProperty{ nullptr };

        DiPropertyType mType{ PROPERTY_MAX };
    };

    //////////////////////////////////////////////////////////////////////////

    class DiNumericPropertyItem : public DiPropertyItem
    {
    public:

        DiNumericPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type);

        virtual ~DiNumericPropertyItem();

    public:

        virtual void CreateUI(const DiString& caption) override;

        virtual void RearrangeUI(int height);

        void RefreshUI();

        void RefreshValue();

        void NotifyEditAccept(MyGUI::EditBox* _sender);

        void NotifyKeyLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old);

    protected:

        MyGUI::TextBox* mTextBox{ nullptr };

        MyGUI::EditBox* mEditBox{ nullptr };
    };
    
    //////////////////////////////////////////////////////////////////////////
    
    class DiEnumPropertyItem : public DiPropertyItem
    {
    public:
        
        DiEnumPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type);
        
        virtual ~DiEnumPropertyItem();
        
    public:
        
        virtual void CreateUI(const DiString& caption) override;
        
        virtual void RearrangeUI(int height);
        
        void RefreshUI();
        
        void RefreshValue();
        
        void NotifyEnumChanged(MyGUI::ComboBox* _sender, size_t _index);
        
    protected:
        
        MyGUI::TextBox* mTextBox{ nullptr };
        
        MyGUI::ComboBox* mEnumBox{ nullptr };
    };
    
    //////////////////////////////////////////////////////////////////////////
    
    class DiColorCurvePropertyItem : public DiPropertyItem
    {
    public:
        
        DiColorCurvePropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type);
        
        virtual ~DiColorCurvePropertyItem();
        
    public:
        
        virtual void CreateUI(const DiString& caption) override;
        
        virtual void RearrangeUI(int height);
        
        void RefreshUI();
        
        void RefreshValue();
        
        void NotifyButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
        
        void NotifyColorUpdated(DiMap<float, DiColor>& colors);
        
    protected:
        
        MyGUI::TextBox* mTextBox{ nullptr };
        
        MyGUI::Button* mCurveButton{ nullptr };
    };
    
    //////////////////////////////////////////////////////////////////////////
    
    class DiTexturePropertyItem : public DiPropertyItem
    {
    public:
        
        DiTexturePropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type);
        
        virtual ~DiTexturePropertyItem();
        
    public:
        
        virtual void CreateUI(const DiString& caption) override;
        
        virtual void RearrangeUI(int height);
        
        void RefreshUI();
        
        void RefreshValue();
        
        void NotifyButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
        
        int GetHeight() override;
        
    protected:
        
        MyGUI::TextBox* mTextBox{ nullptr };
        
        MyGUI::Widget* mImagePane{ nullptr };
        
        MyGUI::ImageBox* mImageBox{ nullptr };
    };

    //////////////////////////////////////////////////////////////////////////

    class DiBoolPropertyItem : public DiPropertyItem
    {
    public:

        DiBoolPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type);

        virtual ~DiBoolPropertyItem();

    public:

        virtual void CreateUI(const DiString& caption) override;

        virtual void RearrangeUI(int height);

        void RefreshUI();

        void RefreshValue();

        void NotifyButtonClick(MyGUI::Widget* _sender);

    protected:

        MyGUI::TextBox* mTextBox{ nullptr };

        MyGUI::Button* mCheckBox{ nullptr };
    };

    //////////////////////////////////////////////////////////////////////////

    class DiVectorPropertyItem : public DiPropertyItem
    {
    public:

        DiVectorPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type);

        virtual ~DiVectorPropertyItem();

    public:

        virtual void CreateUI(const DiString& caption) override;

        void NotfyExpandButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

        int  GetNumElements();

        DiString GetElementName(int i);

        virtual int GetHeight();

        virtual void RearrangeUI(int height);

        void Expand(bool val);

        void RefreshUI();

        void RefreshValue();

        void RefreshSubValue();

        void NotifyEditAccept(MyGUI::EditBox* _sender);

        void NotifyKeyLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old);

    protected:

        MyGUI::Button*  mExpandBtn{ nullptr };

        MyGUI::TextBox* mTextBox{ nullptr };

        MyGUI::EditBox* mEditBox{ nullptr };

        MyGUI::EditBox* mValueBox[4];

        MyGUI::TextBox* mValueLabel[4];

        bool mExpanded{ false };
    };

    //////////////////////////////////////////////////////////////////////////

    class DiColorPropertyItem : public DiPropertyItem
    {
    public:

        DiColorPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type);

        virtual ~DiColorPropertyItem();

    public:

        virtual void CreateUI(const DiString& caption) override;

        virtual void RearrangeUI(int height);

        void NotfyColorButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

        void NotifyEndDialog(Dialog* _sender, bool _result);

    protected:

        void RefreshUI();

        MyGUI::TextBox* mTextBox{ nullptr };

        MyGUI::Button* mColorButton{ nullptr };

        MyGUI::EditBox* mColorValue{ nullptr };
    };

    //////////////////////////////////////////////////////////////////////////

    class DiDynamicPropertyItem : public DiPropertyItem
    {
    public:

        DiDynamicPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type);

        virtual ~DiDynamicPropertyItem();

    public:

        virtual void CreateUI(const DiString& caption) override;

        virtual void RearrangeUI(int height);

        virtual int GetHeight();

        void NotifyTypeChanged(MyGUI::ComboBox* _sender, size_t _index);

        void NotfyCurveButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
        
        void NotfyUpdateCurve(DiAttributeCurved* curve);
        
        void NotifyEditAccept(MyGUI::EditBox* _sender);
        
        void NotifyEditKeyLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old);

        void RefreshValue();
    
        void RefreshUI();

    protected:

        MyGUI::TextBox* mTextBox{ nullptr };

        MyGUI::ComboBox* mDynType{ nullptr };

        MyGUI::EditBox* mFixedValue{ nullptr };

        MyGUI::EditBox* mValueBox[2];

        MyGUI::TextBox* mValueLabel[2];

        MyGUI::Button* mCurveButton{ nullptr };

        DiDynamicAttribute::DynamicAttributeType mType{ DiDynamicAttribute::DAT_FIXED };
        
        DiAttributeCurved mCurrentCurveVal;
        
        DiAttributeFixed mCurrentFixedVal;
        
        DiAttributeRandom mCurrentRandomVal;
    };
}

#endif
