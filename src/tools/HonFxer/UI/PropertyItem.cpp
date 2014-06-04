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
    }

    void DiNumericPropertyItem::RearrangeUI(int height)
    {
        auto pos = mTextBox->getPosition();
        mTextBox->setPosition(pos.left, height);

        pos = mEditBox->getPosition();
        mEditBox->setPosition(pos.left, height);
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
    }

    void DiBoolPropertyItem::RearrangeUI(int height)
    {
        auto pos = mTextBox->getPosition();
        mTextBox->setPosition(pos.left, height);

        pos = mCheckBox->getPosition();
        mCheckBox->setPosition(pos.left, height);
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

        mExpandBtn = widgetClient->createWidget<MyGUI::Button>("ButtonRoundExpand",
            MyGUI::IntCoord(10, currentHeight + 4, 14, 14), MyGUI::Align::Left | MyGUI::Align::Top);
        mExpandBtn->eventMouseButtonPressed += MyGUI::newDelegate(this, &DiVectorPropertyItem::NotfyExpandButtonPressed);

        int elements = GetNumElements();
        for (int i = 0; i < elements; ++i)
        {
            mValueLabel[i] = widgetClient->createWidget<MyGUI::TextBox>("TextBox",
                MyGUI::IntCoord(width_step, currentHeight + (i + 1)*mParentGroup->GetHeightStep(), width, height), MyGUI::Align::Left | MyGUI::Align::Top);
            mValueLabel[i]->setTextAlign(MyGUI::Align::Right | MyGUI::Align::VCenter);
            mValueLabel[i]->setCaption(GetElementName(i).c_str());
            mValueLabel[i]->setVisible(false);

            mValueBox[i] = widgetClient->createWidget<MyGUI::EditBox>("EditBox",
                MyGUI::IntCoord(width_step + width_step + width, currentHeight + (i + 1)*mParentGroup->GetHeightStep(),
                widgetClient->getWidth() - (width_step + width_step + width_step + width), height),
                MyGUI::Align::HStretch | MyGUI::Align::Top);
            mValueBox[i]->setVisible(false);
        }
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
        DiString nameQuat[4] = { "W", "X", "Y", "Z" };
        if (mType == PROPERTY_QUAT)
            return nameQuat[i];
        else
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
    }

    void DiColorPropertyItem::RearrangeUI(int height)
    {
        auto pos = mTextBox->getPosition();
        mTextBox->setPosition(pos.left, height);

        pos = mColorButton->getPosition();
        mColorButton->setPosition(pos.left, height);
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
            mColorButton->setColour(c);
        }

        delete colorPanel;
    }
}