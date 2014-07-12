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
#include "PropertyTable.h"
#include "MainWorkspaceControl.h"
#include "MainPaneControl.h"
#include "ToolsControl.h"
#include "PropertiesControl.h"
#include "HonFxerApp.h"
#include "PanelView.h"
#include "PropertyItem.h"

namespace Demi
{
    DiEditProperty::DiEditProperty(const DiString& caption, DiPropertyBase* prop) : mCaption(caption)
        , mProperty(prop)
    {
    }

    DiEditProperty::~DiEditProperty()
    {
        DI_DELETE mProperty;
    }

    DiPropertyGroup::DiPropertyGroup(const DiString& name) :mGroupName(name)
    {
    }

    DiPropertyGroup::~DiPropertyGroup()
    {
        auto toolsCtrl = HonFxerApp::GetFxApp()->GetMainPane()->GetWorkspaceControl()->GetToolsControl();
        auto propCtrl = toolsCtrl->getPropertiesCtrl();
        propCtrl->getPanelView()->RemoveGroup(mUIGroup);
        delete mUIGroup;
        mUIGroup = nullptr;
        
        for (auto p : mProperties)
        {
            DI_DELETE p;
        }
    }

    DiEditProperty* DiPropertyGroup::AddProperty(const DiString& caption, DiPropertyBase* prop)
    {
        DiEditProperty* ret = DI_NEW DiEditProperty(caption, prop);
        mProperties.push_back(ret);
        return ret;
    }
    
    void DiPropertyGroup::UpdateUI(const DiString& caption)
    {
        auto& items = mPropItems[caption];
        for(auto i : items)
        {
            i->RefreshUI();
        }
    }

    void DiPropertyGroup::CreateUI()
    {
        auto toolsCtrl = HonFxerApp::GetFxApp()->GetMainPane()->GetWorkspaceControl()->GetToolsControl();
        auto propCtrl = toolsCtrl->getPropertiesCtrl();
        mUIGroup = propCtrl->getPanelView()->AddGroup();
        mUIGroup->SetCaption(mGroupName);
        mUIGroup->setVisible(false);
        propCtrl->getPanelView()->markFirstInit(false);
        
        for (auto p : mProperties)
        {
            auto item = mUIGroup->AddItem(p->mCaption, p->mProperty);
            mPropItems[p->mCaption].push_back(item);
        }
    }
}