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
#include "PropertyItem.h"
#include "PanelGroup.h"
#include "ColorPanel.h"
#include "PropertyTypes.h"
#include "MainPaneControl.h"
#include "HonFxerApp.h"
#include "CurveEditor.h"
#include "EnumProperties.h"

namespace Demi
{
    DiPropertyItem::DiPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type)
        : mProperty(prop)
        , mParentGroup(group)
        , mType(type)
    {
    }

    DiPropertyItem::~DiPropertyItem()
    {

    }

    int DiPropertyItem::GetHeight()
    {
        return mParentGroup->GetHeightStep();
    }

    DiNumericPropertyItem::DiNumericPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type)
        :DiPropertyItem(group, prop, type)
    {
    }

    DiNumericPropertyItem::~DiNumericPropertyItem()
    {
    }

    void DiNumericPropertyItem::CreateUI(const DiString& caption)
    {
        auto widgetClient = mParentGroup->GetClientWidget();
        auto currentHeight = mParentGroup->GetCurrentHeight();
        auto width_step = mParentGroup->GetWidthStep();
        auto width = mParentGroup->GetWidth();
        auto height = mParentGroup->GetHeight();

        mTextBox = widgetClient->createWidget<MyGUI::TextBox>("TextBox",
            MyGUI::IntCoord(width_step, currentHeight, width, height), MyGUI::Align::Left | MyGUI::Align::Top);
        mTextBox->setTextAlign(MyGUI::Align::Right | MyGUI::Align::VCenter);
        mTextBox->setCaption(caption.c_str());

        mEditBox = widgetClient->createWidget<MyGUI::EditBox>("EditBox",
            MyGUI::IntCoord(width_step + width_step + width, currentHeight,
            widgetClient->getWidth() - (width_step + width_step + width_step + width), height),
            MyGUI::Align::HStretch | MyGUI::Align::Top);
        mEditBox->eventKeyLostFocus += MyGUI::newDelegate(this, &DiNumericPropertyItem::NotifyKeyLostFocus);
        mEditBox->eventEditSelectAccept += MyGUI::newDelegate(this, &DiNumericPropertyItem::NotifyEditAccept);

        RefreshUI();
    }

    void DiNumericPropertyItem::RearrangeUI(int height)
    {
        auto pos = mTextBox->getPosition();
        mTextBox->setPosition(pos.left, height);

        pos = mEditBox->getPosition();
        mEditBox->setPosition(pos.left, height);
    }

    void DiNumericPropertyItem::RefreshUI()
    {
        DiString str;
        switch (mProperty->getType())
        {
        case PROPERTY_STRING:
            {
                auto prop = dynamic_cast<DiStringProperty*>(mProperty);
                str = *prop;
            }
            break;
        case PROPERTY_INT:
            {
                auto prop = dynamic_cast<DiIntProperty*>(mProperty);
                str.SetInt(*prop);
            }
            break;
        case PROPERTY_FLOAT:
            {
                auto prop = dynamic_cast<DiFloatProperty*>(mProperty);
                str.SetFloat(*prop);
            }
            break;
        default:
            DI_ASSERT(false);
            break;
        }

        mEditBox->setCaption(str.c_str());
    }

    void DiNumericPropertyItem::RefreshValue()
    {
        DiString str = mEditBox->getCaption().asUTF8_c_str();
        switch (mProperty->getType())
        {
        case PROPERTY_STRING:
            {
                auto prop = dynamic_cast<DiStringProperty*>(mProperty);
                *prop = str;
            }
            break;
        case PROPERTY_INT:
            {
                auto prop = dynamic_cast<DiIntProperty*>(mProperty);
                *prop = str.AsInt();
            }
            break;
        case PROPERTY_FLOAT:
            {
                auto prop = dynamic_cast<DiFloatProperty*>(mProperty);
                *prop = str.AsFloat();
            }
            break;
        default:
            DI_ASSERT(false);
            break;
        }
    }

    void DiNumericPropertyItem::NotifyEditAccept(MyGUI::EditBox* _sender)
    {
        RefreshValue();
    }

    void DiNumericPropertyItem::NotifyKeyLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old)
    {
        RefreshValue();
    }

    DiEnumPropertyItem::DiEnumPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type)
        :DiPropertyItem(group, prop, type)
    {
    }
    
    DiEnumPropertyItem::~DiEnumPropertyItem()
    {
    }
    
    void DiEnumPropertyItem::NotifyEnumChanged(MyGUI::ComboBox* _sender, size_t _index)
    {
        
    }
    
    void DiEnumPropertyItem::CreateUI(const DiString& caption)
    {
        auto widgetClient = mParentGroup->GetClientWidget();
        auto currentHeight = mParentGroup->GetCurrentHeight();
        auto width_step = mParentGroup->GetWidthStep();
        auto width = mParentGroup->GetWidth();
        auto height = mParentGroup->GetHeight();
        
        mTextBox = widgetClient->createWidget<MyGUI::TextBox>("TextBox",
                                                              MyGUI::IntCoord(width_step, currentHeight, width, height), MyGUI::Align::Left | MyGUI::Align::Top);
        mTextBox->setTextAlign(MyGUI::Align::Right | MyGUI::Align::VCenter);
        mTextBox->setCaption(caption.c_str());
        
        mEnumBox = widgetClient->createWidget<MyGUI::ComboBox>("ComboBox",
                                                              MyGUI::IntCoord(width_step + width_step + width, currentHeight,
                                                                widgetClient->getWidth() - (width_step + width_step + width_step + width), height),
                                                              MyGUI::Align::HStretch | MyGUI::Align::Top);
        mEnumBox->setComboModeDrop(true);
        mEnumBox->eventComboAccept += MyGUI::newDelegate(this, &DiEnumPropertyItem::NotifyEnumChanged);
        
        auto prop = dynamic_cast<DiEnumProperty*>(mProperty);
        DiBaseEnumPropPtr enumProp = *prop;
        auto& strs = enumProp->GetStrings();
        for(auto& s : strs)
        {
            mEnumBox->addItem(s.c_str());
        }
        
        RefreshUI();
    }
    
    void DiEnumPropertyItem::RearrangeUI(int height)
    {
        auto pos = mTextBox->getPosition();
        mTextBox->setPosition(pos.left, height);
        
        pos = mEnumBox->getPosition();
        mEnumBox->setPosition(pos.left, height);
    }
    
    void DiEnumPropertyItem::RefreshUI()
    {
        auto prop = dynamic_cast<DiEnumProperty*>(mProperty);
        DiBaseEnumPropPtr enumProp = *prop;
        auto val = (size_t)enumProp->enumValue;
        DI_ASSERT(val >= 0 && val < mEnumBox->getItemCount());
        mEnumBox->setIndexSelected(val);
    }
    
    void DiEnumPropertyItem::RefreshValue()
    {
        auto prop = dynamic_cast<DiEnumProperty*>(mProperty);
        DiBaseEnumPropPtr enumProp = *prop;
        enumProp->enumValue = mEnumBox->getIndexSelected();
    }

    DiBoolPropertyItem::DiBoolPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type)
        :DiPropertyItem(group, prop, type)
    {
    }

    DiBoolPropertyItem::~DiBoolPropertyItem()
    {
    }

    void DiBoolPropertyItem::CreateUI(const DiString& caption)
    {
        auto widgetClient = mParentGroup->GetClientWidget();
        auto currentHeight = mParentGroup->GetCurrentHeight();
        auto width_step = mParentGroup->GetWidthStep();
        auto width = mParentGroup->GetWidth();
        auto height = mParentGroup->GetHeight();

        mTextBox = widgetClient->createWidget<MyGUI::TextBox>("TextBox",
            MyGUI::IntCoord(width_step, currentHeight, width, height), MyGUI::Align::Left | MyGUI::Align::Top);
        mTextBox->setTextAlign(MyGUI::Align::Right | MyGUI::Align::VCenter);
        mTextBox->setCaption(caption.c_str());

        mCheckBox = widgetClient->createWidget<MyGUI::Button>("CheckBox",
            MyGUI::IntCoord(width_step + width_step + width, currentHeight,
            widgetClient->getWidth() - (width_step + width_step + width_step + width), height),
            MyGUI::Align::HStretch | MyGUI::Align::Top);
        mCheckBox->eventMouseButtonClick += MyGUI::newDelegate(this, &DiBoolPropertyItem::NotifyButtonClick);

        RefreshUI();
    }

    void DiBoolPropertyItem::RearrangeUI(int height)
    {
        auto pos = mTextBox->getPosition();
        mTextBox->setPosition(pos.left, height);

        pos = mCheckBox->getPosition();
        mCheckBox->setPosition(pos.left, height);
    }

    void DiBoolPropertyItem::RefreshUI()
    {
        DI_ASSERT(mProperty->getType() == PROPERTY_BOOL);
        auto prop = dynamic_cast<DiBoolProperty*>(mProperty);
        bool val = *prop;
        mCheckBox->setStateSelected(val);
    }

    void DiBoolPropertyItem::RefreshValue()
    {
        bool val = mCheckBox->getStateSelected();
        auto prop = dynamic_cast<DiBoolProperty*>(mProperty);
        *prop = val;
    }

    void DiBoolPropertyItem::NotifyButtonClick(MyGUI::Widget* _sender)
    {
        mCheckBox->setStateSelected(!mCheckBox->getStateSelected());
        RefreshValue();
    }

    DiVectorPropertyItem::DiVectorPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type)
        :DiPropertyItem(group, prop, type)
    {
        for (int i = 0; i < 4; i++)
        {
            mValueBox[i] = nullptr;
            mValueLabel[i] = nullptr;
        }
    }

    DiVectorPropertyItem::~DiVectorPropertyItem()
    {

    }

    void DiVectorPropertyItem::CreateUI(const DiString& caption)
    {
        auto widgetClient = mParentGroup->GetClientWidget();
        auto currentHeight = mParentGroup->GetCurrentHeight();
        auto width_step = mParentGroup->GetWidthStep();
        auto width = mParentGroup->GetWidth();
        auto height = mParentGroup->GetHeight();

        mTextBox = widgetClient->createWidget<MyGUI::TextBox>("TextBox",
            MyGUI::IntCoord(width_step, currentHeight, width, height), MyGUI::Align::Left | MyGUI::Align::Top);
        mTextBox->setTextAlign(MyGUI::Align::Right | MyGUI::Align::VCenter);
        mTextBox->setCaption(caption.c_str());

        mEditBox = widgetClient->createWidget<MyGUI::EditBox>("EditBox",
            MyGUI::IntCoord(width_step + width_step + width, currentHeight,
            widgetClient->getWidth() - (width_step + width_step + width_step + width), height),
            MyGUI::Align::HStretch | MyGUI::Align::Top);
        mEditBox->eventKeyLostFocus += MyGUI::newDelegate(this, &DiVectorPropertyItem::NotifyKeyLostFocus);
        mEditBox->eventEditSelectAccept += MyGUI::newDelegate(this, &DiVectorPropertyItem::NotifyEditAccept);

        mExpandBtn = widgetClient->createWidget<MyGUI::Button>("ButtonRoundExpand",
            MyGUI::IntCoord(10, currentHeight + 4, 14, 14), MyGUI::Align::Left | MyGUI::Align::Top);
        mExpandBtn->eventMouseButtonPressed += MyGUI::newDelegate(this, &DiVectorPropertyItem::NotfyExpandButtonPressed);

        int elements = GetNumElements();
        for (int i = 0; i < elements; ++i)
        {
            mValueLabel[i] = widgetClient->createWidget<MyGUI::TextBox>("TextBox",
                MyGUI::IntCoord(width_step, currentHeight + (i + 1)*mParentGroup->GetHeightStep(), width, height),
                MyGUI::Align::Left | MyGUI::Align::Top);
            mValueLabel[i]->setTextAlign(MyGUI::Align::Right | MyGUI::Align::VCenter);
            mValueLabel[i]->setCaption(GetElementName(i).c_str());
            mValueLabel[i]->setVisible(false);

            mValueBox[i] = widgetClient->createWidget<MyGUI::EditBox>("EditBox",
                MyGUI::IntCoord(width_step + width_step + width, currentHeight + (i + 1)*mParentGroup->GetHeightStep(),
                widgetClient->getWidth() - (width_step + width_step + width_step + width), height),
                MyGUI::Align::HStretch | MyGUI::Align::Top);
            mValueBox[i]->setVisible(false);
            mValueBox[i]->eventKeyLostFocus += MyGUI::newDelegate(this, &DiVectorPropertyItem::NotifyKeyLostFocus);
            mValueBox[i]->eventEditSelectAccept += MyGUI::newDelegate(this, &DiVectorPropertyItem::NotifyEditAccept);
        }

        RefreshUI();
    }

    void DiVectorPropertyItem::NotfyExpandButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        Expand(!mExpanded);
    }

    int DiVectorPropertyItem::GetNumElements()
    {
        switch (mType)
        {
        case Demi::PROPERTY_VEC2:
            return 2;
        case Demi::PROPERTY_VEC3:
            return 3;
        case Demi::PROPERTY_VEC4:
        case Demi::PROPERTY_QUAT:
            return 4;
        default:
            DI_WARNING("Invalid type");
            return 0;
        }
    }

    Demi::DiString DiVectorPropertyItem::GetElementName(int i)
    {
        DI_ASSERT(i >= 0 && i < 4);
        DiString name[4] = { "X", "Y", "Z", "W" };
        return name[i];
    }

    int DiVectorPropertyItem::GetHeight()
    {
        if (!mExpanded)
            return mParentGroup->GetHeightStep();
        else
            return mParentGroup->GetHeightStep() * (GetNumElements() + 1);
    }

    void DiVectorPropertyItem::RearrangeUI(int height)
    {
        auto pos = mTextBox->getPosition();
        mTextBox->setPosition(pos.left, height);

        pos = mEditBox->getPosition();
        mEditBox->setPosition(pos.left, height);

        pos = mExpandBtn->getPosition();
        mExpandBtn->setPosition(pos.left, height + 4);

        if (mExpanded)
        {
            int elements = GetNumElements();
            for (int i = 0; i < elements; ++i)
            {
                pos = mValueLabel[i]->getPosition();
                mValueLabel[i]->setPosition(pos.left, height + mParentGroup->GetHeightStep()*(i + 1));

                pos = mValueBox[i]->getPosition();
                mValueBox[i]->setPosition(pos.left, height + mParentGroup->GetHeightStep()*(i + 1));
            }
        }
    }

    void DiVectorPropertyItem::Expand(bool val)
    {
        mExpanded = val;
        mExpandBtn->setStateSelected(mExpanded);

        int elements = GetNumElements();
        for (int i = 0; i < elements; ++i)
        {
            mValueLabel[i]->setVisible(mExpanded);
            mValueBox[i]->setVisible(mExpanded);
        }

        mParentGroup->NotifyRearrangeHeight();
    }

    void DiVectorPropertyItem::RefreshUI()
    {
        DiString str;
        DiString t;
        if (mType == PROPERTY_VEC2)
        {
            auto prop = dynamic_cast<DiVec2Property*>(mProperty);
            DiVec2 v = *prop;
            str.Format("%.3g,%.3g", v.x, v.y);
            
            t.SetFloat(v.x);
            mValueBox[0]->setCaption(t.c_str());
            t.SetFloat(v.y);
            mValueBox[1]->setCaption(t.c_str());
        }
        else if (mType == PROPERTY_VEC3)
        {
            auto prop = dynamic_cast<DiVec3Property*>(mProperty);
            DiVec3 v = *prop;
            str.Format("%.3g,%.3g,%.3g", v.x, v.y, v.z);

            t.SetFloat(v.x);
            mValueBox[0]->setCaption(t.c_str());
            t.SetFloat(v.y);
            mValueBox[1]->setCaption(t.c_str());
            t.SetFloat(v.z);
            mValueBox[2]->setCaption(t.c_str());
        }
        else if (mType == PROPERTY_VEC4)
        {
            auto prop = dynamic_cast<DiVec4Property*>(mProperty);
            DiVec4 v = *prop;
            str.Format("%.3g,%.3g,%.3g,%.3g", v.x, v.y, v.z, v.w);

            t.SetFloat(v.x);
            mValueBox[0]->setCaption(t.c_str());
            t.SetFloat(v.y);
            mValueBox[1]->setCaption(t.c_str());
            t.SetFloat(v.z);
            mValueBox[2]->setCaption(t.c_str());
            t.SetFloat(v.w);
            mValueBox[3]->setCaption(t.c_str());
        }
        else if (mType == PROPERTY_QUAT)
        {
            auto prop = dynamic_cast<DiQuatProperty*>(mProperty);
            DiQuat v = *prop;
            str.Format("%.3g,%.3g,%.3g,%.3g", v.x, v.y, v.z, v.w);

            t.SetFloat(v.x);
            mValueBox[0]->setCaption(t.c_str());
            t.SetFloat(v.y);
            mValueBox[1]->setCaption(t.c_str());
            t.SetFloat(v.z);
            mValueBox[2]->setCaption(t.c_str());
            t.SetFloat(v.w);
            mValueBox[3]->setCaption(t.c_str());
        }

        mEditBox->setCaption(str.c_str());
    }

    void DiVectorPropertyItem::RefreshValue()
    {
        DiString val = mEditBox->getCaption().asUTF8_c_str();

        if (mType == PROPERTY_VEC2)
        {
            auto prop = dynamic_cast<DiVec2Property*>(mProperty);
            DiVec2 v = val.AsVector2();
            *prop = v;
        }
        else if (mType == PROPERTY_VEC3)
        {
            auto prop = dynamic_cast<DiVec3Property*>(mProperty);
            DiVec3 v = val.AsVector3();
            *prop = v;
        }
        else if (mType == PROPERTY_VEC4)
        {
            auto prop = dynamic_cast<DiVec4Property*>(mProperty);
            DiVec4 v = val.AsVector4();
            *prop = v;
        }
        else if (mType == PROPERTY_QUAT)
        {
            auto prop = dynamic_cast<DiQuatProperty*>(mProperty);
            DiQuat v = val.AsQuaternion(false);
            *prop = v;
        }

        RefreshUI();
    }

    void DiVectorPropertyItem::RefreshSubValue()
    {
        float x, y, z, w;
        DiString val;
        
        val = mValueBox[0]->getCaption().asUTF8_c_str();
        x = val.AsFloat();

        if (mType == PROPERTY_VEC2)
        {
            val = mValueBox[1]->getCaption().asUTF8_c_str();
            y = val.AsFloat();

            auto prop = dynamic_cast<DiVec2Property*>(mProperty);
            DiVec2 v(x, y);
            *prop = v;
        }
        else if (mType == PROPERTY_VEC3)
        {
            val = mValueBox[1]->getCaption().asUTF8_c_str();
            y = val.AsFloat();
            val = mValueBox[2]->getCaption().asUTF8_c_str();
            z = val.AsFloat();

            auto prop = dynamic_cast<DiVec3Property*>(mProperty);
            DiVec3 v(x, y, z);
            *prop = v;
        }
        else if (mType == PROPERTY_VEC4)
        {
            val = mValueBox[1]->getCaption().asUTF8_c_str();
            y = val.AsFloat();
            val = mValueBox[2]->getCaption().asUTF8_c_str();
            z = val.AsFloat();
            val = mValueBox[3]->getCaption().asUTF8_c_str();
            w = val.AsFloat();

            auto prop = dynamic_cast<DiVec4Property*>(mProperty);
            DiVec4 v(x, y, z, w);
            *prop = v;
        }
        else if (mType == PROPERTY_QUAT)
        {
            val = mValueBox[1]->getCaption().asUTF8_c_str();
            y = val.AsFloat();
            val = mValueBox[2]->getCaption().asUTF8_c_str();
            z = val.AsFloat();
            val = mValueBox[3]->getCaption().asUTF8_c_str();
            w = val.AsFloat();

            auto prop = dynamic_cast<DiQuatProperty*>(mProperty);
            DiQuat v(w, x, y, z);
            *prop = v;
        }

        RefreshUI();
    }

    void DiVectorPropertyItem::NotifyEditAccept(MyGUI::EditBox* _sender)
    {
        if (_sender == mEditBox)
            RefreshValue();
        else
            RefreshSubValue();
    }

    void DiVectorPropertyItem::NotifyKeyLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old)
    {
        if (_sender == mEditBox)
            RefreshValue();
        else
            RefreshSubValue();
    }

    DiColorPropertyItem::DiColorPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type)
        :DiPropertyItem(group, prop, type)
    {
    }

    DiColorPropertyItem::~DiColorPropertyItem()
    {
    }

    void DiColorPropertyItem::CreateUI(const DiString& caption)
    {
        auto widgetClient = mParentGroup->GetClientWidget();
        auto currentHeight = mParentGroup->GetCurrentHeight();
        auto widthStep = mParentGroup->GetWidthStep();
        auto width = mParentGroup->GetWidth();
        auto height = mParentGroup->GetHeight();
        auto colorBtnWidth = 23;
        auto alphaLabelWidth = 38;

        int curWidth = widthStep;
        mTextBox = widgetClient->createWidget<MyGUI::TextBox>("TextBox",
            MyGUI::IntCoord(curWidth, currentHeight, width, height), MyGUI::Align::Left | MyGUI::Align::Top);
        mTextBox->setTextAlign(MyGUI::Align::Right | MyGUI::Align::VCenter);
        mTextBox->setCaption(caption.c_str());
        curWidth += widthStep + width;

        mColorButton = widgetClient->createWidget<MyGUI::Button>("ColorButton",
            MyGUI::IntCoord(curWidth, currentHeight,
            colorBtnWidth, height),
            MyGUI::Align::Left | MyGUI::Align::Top);
        mColorButton->eventMouseButtonPressed += MyGUI::newDelegate(this, &DiColorPropertyItem::NotfyColorButtonPressed);
        curWidth += widthStep + colorBtnWidth;

        mColorValue = widgetClient->createWidget<MyGUI::EditBox>("EditBox",
            MyGUI::IntCoord(curWidth, currentHeight,
            widgetClient->getWidth() - (curWidth + widthStep), height),
            MyGUI::Align::HStretch | MyGUI::Align::Top);
        mColorValue->setEditReadOnly(true);

        RefreshUI();
    }

    void DiColorPropertyItem::RearrangeUI(int height)
    {
        auto pos = mTextBox->getPosition();
        mTextBox->setPosition(pos.left, height);

        pos = mColorButton->getPosition();
        mColorButton->setPosition(pos.left, height);

        pos = mColorValue->getPosition();
        mColorValue->setPosition(pos.left, height);
    }

    void DiColorPropertyItem::NotfyColorButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        if (_id == MyGUI::MouseButton::Left)
        {
            auto colorPanel = new ColourPanel();
            colorPanel->eventEndDialog = MyGUI::newDelegate(this, &DiColorPropertyItem::NotifyEndDialog);

            DiColorProperty* colorProp = dynamic_cast<DiColorProperty*>(mProperty);
            DI_ASSERT(colorProp);
            DiColor preCol = *colorProp;

            colorPanel->setColour(MyGUI::Colour(preCol.r, preCol.g, preCol.b, preCol.a));
            colorPanel->doModal();
        }
    }

    void DiColorPropertyItem::NotifyEndDialog(Dialog* _sender, bool _result)
    {
        ColourPanel* colorPanel = dynamic_cast<ColourPanel*>(_sender);
        DI_ASSERT(colorPanel);
        colorPanel->endModal();

        if (_result)
        {
            DiColorProperty* colorProp = dynamic_cast<DiColorProperty*>(mProperty);
            DI_ASSERT(colorProp);

            auto c = colorPanel->getColour();
            DiColor col(c.red, c.green, c.blue, c.alpha);
            *colorProp = col;

            RefreshUI();
        }

        delete colorPanel;
    }

    void DiColorPropertyItem::RefreshUI()
    {
        DiColorProperty* colorProp = dynamic_cast<DiColorProperty*>(mProperty);
        DI_ASSERT(colorProp);

        DiColor col = *colorProp;
        MyGUI::Colour c(col.r,col.g,col.b,col.a);

        mColorButton->setColour(c);
        DiString colStr;
        colStr.Format("%.2g,%.2g,%.2g (%.2g)", c.red, c.green, c.blue, c.alpha);
        mColorValue->setCaption(colStr.c_str());
    }

    DiDynamicPropertyItem::DiDynamicPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type)
        :DiPropertyItem(group, prop, type)
    {
        mValueBox[0]   = mValueBox[1]   = nullptr;
        mValueLabel[0] = mValueLabel[1] = nullptr;
    }

    DiDynamicPropertyItem::~DiDynamicPropertyItem()
    {
    }
    
    void DiDynamicPropertyItem::NotfyUpdateCurve(DiAttributeCurved* curve)
    {
        curve->CopyTo(&mCurrentCurveVal);
        
        RefreshValue();
    }

    void DiDynamicPropertyItem::CreateUI(const DiString& caption)
    {
        auto widgetClient = mParentGroup->GetClientWidget();
        auto currentHeight = mParentGroup->GetCurrentHeight();
        auto width_step = mParentGroup->GetWidthStep();
        auto width = mParentGroup->GetWidth();
        auto height = mParentGroup->GetHeight();
        int curWidth = width_step;
        int comboWidth = 80;

        DiDynProperty* dynProp = dynamic_cast<DiDynProperty*>(mProperty);
        DI_ASSERT(dynProp);
        DiDynamicAttribute* attr = *dynProp;
        mType = attr->GetType();

        mTextBox = widgetClient->createWidget<MyGUI::TextBox>("TextBox",
            MyGUI::IntCoord(curWidth, currentHeight, width, height), MyGUI::Align::Left | MyGUI::Align::Top);
        mTextBox->setTextAlign(MyGUI::Align::Right | MyGUI::Align::VCenter);
        mTextBox->setCaption(caption.c_str());
        curWidth += width_step + width;

        mDynType = widgetClient->createWidget<MyGUI::ComboBox>("ComboBox",
            MyGUI::IntCoord(curWidth, currentHeight,
            comboWidth, height),
            MyGUI::Align::Left | MyGUI::Align::Top);
        mDynType->addItem("Fixed");
        mDynType->addItem("Random");
        mDynType->addItem("Curve");
        mDynType->setIndexSelected(0);
        mDynType->setComboModeDrop(true);
        mDynType->eventComboAccept += MyGUI::newDelegate(this, &DiDynamicPropertyItem::NotifyTypeChanged);
        curWidth += width_step + comboWidth;

        mFixedValue = widgetClient->createWidget<MyGUI::EditBox>("EditBox",
            MyGUI::IntCoord(curWidth, currentHeight,
            widgetClient->getWidth() - (curWidth + width_step), height),
            MyGUI::Align::HStretch | MyGUI::Align::Top);
        mFixedValue->setVisible(false);
        mFixedValue->eventKeyLostFocus += MyGUI::newDelegate(this, &DiDynamicPropertyItem::NotifyEditKeyLostFocus);
        mFixedValue->eventEditSelectAccept += MyGUI::newDelegate(this, &DiDynamicPropertyItem::NotifyEditAccept);

        mCurveButton = widgetClient->createWidget<MyGUI::Button>("Button",
            MyGUI::IntCoord(curWidth, currentHeight,
            widgetClient->getWidth() - (curWidth + width_step), height),
            MyGUI::Align::HStretch | MyGUI::Align::Top);
        mCurveButton->setVisible(false);
        mCurveButton->setCaption("...");
        mCurveButton->eventMouseButtonPressed += MyGUI::newDelegate(this, &DiDynamicPropertyItem::NotfyCurveButtonPressed);

        for (size_t i = 0; i < 2; i++)
        {
            mValueLabel[i] = widgetClient->createWidget<MyGUI::TextBox>("TextBox",
                MyGUI::IntCoord(width_step, currentHeight + (i + 1)*mParentGroup->GetHeightStep(), width, height), MyGUI::Align::Left | MyGUI::Align::Top);
            mValueLabel[i]->setTextAlign(MyGUI::Align::Right | MyGUI::Align::VCenter);
            mValueLabel[i]->setCaption(i==0 ? "Min" : "Max");
            mValueLabel[i]->setVisible(false);

            mValueBox[i] = widgetClient->createWidget<MyGUI::EditBox>("EditBox",
                MyGUI::IntCoord(width_step + width_step + width, currentHeight + (i + 1)*mParentGroup->GetHeightStep(),
                widgetClient->getWidth() - (width_step + width_step + width_step + width), height),
                MyGUI::Align::HStretch | MyGUI::Align::Top);
            mValueBox[i]->setVisible(false);
            mValueBox[i]->eventKeyLostFocus += MyGUI::newDelegate(this, &DiDynamicPropertyItem::NotifyEditKeyLostFocus);
            mValueBox[i]->eventEditSelectAccept += MyGUI::newDelegate(this, &DiDynamicPropertyItem::NotifyEditAccept);
        }

        RefreshUI();
    }

    void DiDynamicPropertyItem::RearrangeUI(int height)
    {
        auto pos = mTextBox->getPosition();
        mTextBox->setPosition(pos.left, height);

        pos = mDynType->getPosition();
        mDynType->setPosition(pos.left, height);

        pos = mFixedValue->getPosition();
        mFixedValue->setPosition(pos.left, height);

        for (size_t i = 0; i < 2; i++)
        {
            pos = mValueLabel[i]->getPosition();
            mValueLabel[i]->setPosition(pos.left, height + mParentGroup->GetHeightStep()*(i + 1));

            pos = mValueBox[i]->getPosition();
            mValueBox[i]->setPosition(pos.left, height + mParentGroup->GetHeightStep()*(i + 1));
        }
    }

    int DiDynamicPropertyItem::GetHeight()
    {
        if (mType == DiDynamicAttribute::DAT_FIXED)
            return mParentGroup->GetHeightStep();
        else if (mType == DiDynamicAttribute::DAT_RANDOM)
            return mParentGroup->GetHeightStep() * 3;
        else 
            return mParentGroup->GetHeightStep();
    }

    void DiDynamicPropertyItem::NotifyTypeChanged(MyGUI::ComboBox* _sender, size_t _index)
    {
        mType = DiDynamicAttribute::DynamicAttributeType(_index);
        RefreshUI();
    }

    void DiDynamicPropertyItem::RefreshUI()
    {
        mDynType->setIndexSelected((int)mType);
        
        auto prop = dynamic_cast<DiDynProperty*>(mProperty);
        DiDynamicAttribute* dynAttr = *prop;

        bool randomVis = mType == DiDynamicAttribute::DAT_RANDOM;
        for (size_t i = 0; i < 2; i++)
        {
            mValueLabel[i]->setVisible(randomVis);
            mValueBox[i]->setVisible(randomVis);
        }

        if (mType == DiDynamicAttribute::DAT_RANDOM)
        {
            mFixedValue->setVisible(false);
            mCurveButton->setVisible(false);
            
            DiString temp;
            temp.Format("%g", static_cast<DiAttributeRandom*>(dynAttr)->GetMin());
            mValueBox[0]->setCaption(temp.c_str());
            
            temp.Format("%g", static_cast<DiAttributeRandom*>(dynAttr)->GetMax());
            mValueBox[1]->setCaption(temp.c_str());
        }
        else if (mType == DiDynamicAttribute::DAT_FIXED)
        {
            mFixedValue->setVisible(true);
            mCurveButton->setVisible(false);
            
            DiString temp;
            temp.Format("%g", static_cast<DiAttributeFixed*>(dynAttr)->GetValue());
            mFixedValue->setCaption(temp.c_str());
        }
        else
        {
            mFixedValue->setVisible(false);
            mCurveButton->setVisible(true);
        }

        mParentGroup->NotifyRearrangeHeight();
    }
    
    void DiDynamicPropertyItem::NotifyEditAccept(MyGUI::EditBox* _sender)
    {
        RefreshValue();
    }
    
    void DiDynamicPropertyItem::NotifyEditKeyLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old)
    {
        RefreshValue();
    }

    void DiDynamicPropertyItem::NotfyCurveButtonPressed(MyGUI::Widget* _sender,
                                                        int _left, int _top,
                                                        MyGUI::MouseButton _id)
    {
        DI_ASSERT(mType == DiDynamicAttribute::DAT_CURVED);

        auto prop = dynamic_cast<DiDynProperty*>(mProperty);
        DiDynamicAttribute* dynAttr = *prop;
        
        if(dynAttr->GetType() == DiDynamicAttribute::DAT_FIXED)
        {
            mCurrentCurveVal.RemoveAllControlPoints();
            mCurrentCurveVal.AddControlPoint(0.5f, static_cast<DiAttributeFixed*>(dynAttr)->GetValue());
        }
        else if(dynAttr->GetType() == DiDynamicAttribute::DAT_RANDOM)
        {
            mCurrentCurveVal.RemoveAllControlPoints();
            mCurrentCurveVal.AddControlPoint(0.0f, static_cast<DiAttributeRandom*>(dynAttr)->GetMin());
            mCurrentCurveVal.AddControlPoint(1.0f, static_cast<DiAttributeRandom*>(dynAttr)->GetMax());
        }
        else
        {
            mCurrentCurveVal = *static_cast<DiAttributeCurved*>(dynAttr);
        }
        
        auto curveEditor = HonFxerApp::GetFxApp()->GetMainPane()->getCurveEditor();
        curveEditor->SetAttribute(mCurrentCurveVal);
        curveEditor->eventUpdateCurve += MyGUI::newDelegate(this, &DiDynamicPropertyItem::NotfyUpdateCurve);
        
        HonFxerApp::GetFxApp()->GetMainPane()->showCurveEditor();
    }

    void DiDynamicPropertyItem::RefreshValue()
    {
        auto prop = dynamic_cast<DiDynProperty*>(mProperty);
        DiDynamicAttribute* dynAttr = *prop;
        
        if (mType == DiDynamicAttribute::DAT_RANDOM)
        {
            DiString random0 = mValueBox[0]->getCaption().asUTF8_c_str();
            DiString random1 = mValueBox[1]->getCaption().asUTF8_c_str();
            mCurrentRandomVal.SetMinMax(random0.AsFloat(), random1.AsFloat());
            dynAttr = DI_NEW DiAttributeRandom(mCurrentRandomVal);
        }
        else if (mType == DiDynamicAttribute::DAT_FIXED)
        {
            DiString fixedValue = mFixedValue->getCaption().asUTF8_c_str();
            mCurrentFixedVal.SetValue(fixedValue.AsFloat());
            dynAttr = DI_NEW DiAttributeFixed(mCurrentFixedVal);
        }
        else
        {
            dynAttr = DI_NEW DiAttributeCurved(mCurrentCurveVal);
        }
        *prop = dynAttr;
    }
}
