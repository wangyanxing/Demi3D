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
#include "PanelGroup.h"

namespace tools
{
    const int height      = 24;
    const int height_step = 26;
    const int width       = 55;
    const int width_step  = 3;

	PanelGroup::PanelGroup() : BasePanelViewItem("")
	{
	}

    void PanelGroup::SetCaption(const DiString& cap)
    {
        mPanelCell->setCaption(cap.c_str());
    }

    void PanelGroup::AddItem(const DiString& caption, DiPropertyBase* prop)
    {
        auto propType = prop->getType();

        switch (propType)
        {
        case Demi::PROPERTY_STRING:
        case Demi::PROPERTY_INT:
        case Demi::PROPERTY_FLOAT:
            AddSimpleInputItem(caption, prop);
            break;
        case Demi::PROPERTY_BOOL:
            AddCheckItem(caption, prop);
            break;
        case Demi::PROPERTY_VEC2:
            break;
        case Demi::PROPERTY_VEC3:
            break;
        case Demi::PROPERTY_VEC4:
            break;
        case Demi::PROPERTY_QUAT:
            break;
        case Demi::PROPERTY_COLOR:
            break;
        case Demi::PROPERTY_DYN_ATTR:
            break;
        default:
            DI_WARNING("Invalid item type: %d", propType);
        }
    }

    void PanelGroup::AddSimpleInputItem(const DiString& caption, DiPropertyBase* prop)
    {
        MyGUI::TextBox* text = mWidgetClient->createWidget<MyGUI::TextBox>("TextBox",
            MyGUI::IntCoord(width_step, mCurrentHeight, width, height), MyGUI::Align::Left | MyGUI::Align::Top);
        text->setTextAlign(MyGUI::Align::Right | MyGUI::Align::VCenter);
        text->setCaption(caption.c_str());

        MyGUI::EditBox* edit = mWidgetClient->createWidget<MyGUI::EditBox>("Edit",
            MyGUI::IntCoord(width_step + width_step + width, mCurrentHeight,
            mWidgetClient->getWidth() - (width_step + width_step + width_step + width), height),
            MyGUI::Align::HStretch | MyGUI::Align::Top);

        mCurrentHeight += height_step;
    }

    void PanelGroup::AddCheckItem(const DiString& caption, DiPropertyBase* prop)
    {

    }

    void PanelGroup::FinishAddItem()
    {
        mPanelCell->setClientHeight(mCurrentHeight, false);
    }
}
