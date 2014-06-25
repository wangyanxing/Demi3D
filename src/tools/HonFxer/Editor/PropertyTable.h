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

#ifndef PropertyTable_h__
#define PropertyTable_h__

#include "FxerPrerequisites.h"
#include "MemoryConfig.h"
#include "Property.h"

namespace Demi 
{
    class DiEditProperty
    {
    public:

        DiEditProperty(const DiString& caption, DiPropertyBase* prop);

        ~DiEditProperty();

        DiString mCaption;

        DiPropertyBase* mProperty;
    };

    //////////////////////////////////////////////////////////////////////////

    class DiPropertyGroup
    {
    public:

        DiPropertyGroup(const DiString& name);

        ~DiPropertyGroup();

        DiEditProperty* AddProperty(const DiString& caption, DiPropertyBase* prop);

        void CreateUI();

        DiString mGroupName;

        DiVector<DiEditProperty*> mProperties;
        
        Demi::DiPanelGroup* mUIGroup{nullptr};
    };

    using PropertyGroups = DiVector<DiPropertyGroup*>;
}

#endif
