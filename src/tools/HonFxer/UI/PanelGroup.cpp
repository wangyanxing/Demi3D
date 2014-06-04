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
#include "PropertyItem.h"

namespace Demi
{
	DiPanelGroup::DiPanelGroup() : BasePanelViewItem("")
	{
	}

    DiPanelGroup::~DiPanelGroup()
    {
        for (auto r : mItems)
        {
            DI_DELETE r;
        }
    }

    void DiPanelGroup::SetCaption(const DiString& cap)
    {
        mPanelCell->setCaption(cap.c_str());
    }

    void DiPanelGroup::AddItem(const DiString& caption, DiPropertyBase* prop)
    {
        auto propType = prop->getType();

        DiPropertyItem* item = nullptr;

        switch (propType)
        {
        case Demi::PROPERTY_STRING:
        case Demi::PROPERTY_INT:
        case Demi::PROPERTY_FLOAT:
            item = DI_NEW DiNumericPropertyItem(this, prop, propType);
            break;
        case Demi::PROPERTY_BOOL:
            item = DI_NEW DiBoolPropertyItem(this, prop, propType);
            break;
        case Demi::PROPERTY_VEC2:
        case Demi::PROPERTY_VEC3:
        case Demi::PROPERTY_VEC4:
        case Demi::PROPERTY_QUAT:
            item = DI_NEW DiVectorPropertyItem(this, prop, propType);
            break;
            break;
        case Demi::PROPERTY_COLOR:
            item = DI_NEW DiColorPropertyItem(this, prop, propType);
            break;
        case Demi::PROPERTY_DYN_ATTR:
            item = DI_NEW DiDynamicPropertyItem(this, prop, propType);
            break;
        default:
            DI_WARNING("Invalid item type: %d", propType);
        }

        if (item)
        {
            item->CreateUI(caption);
            mItems.push_back(item);
            mPanelCell->setClientHeight(GetCurrentHeight(), false);
        }
    }

    int DiPanelGroup::GetCurrentHeight()
    {
        int ret = 0;
        for (auto item : mItems)
        {
            ret += item->GetHeight();
        }
        return ret;
    }

    void DiPanelGroup::NotifyRearrangeHeight()
    {
        int height = 0;
        for (auto item : mItems)
        {
            item->RearrangeUI(height);
            height += item->GetHeight();
        }

        mPanelCell->setClientHeight(GetCurrentHeight(), false);
    }

}
