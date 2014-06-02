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

        DiEditProperty(const DiString& caption, DiPropertyBase* prop)
            : mCaption(caption)
            , mProperty(prop)
        {
        }

        ~DiEditProperty()
        {
            DI_DELETE mProperty;
        }

        DiString mCaption;

        DiPropertyBase* mProperty;
    };

    //////////////////////////////////////////////////////////////////////////

    class DiPropertyGroup
    {
    public:

        DiPropertyGroup(const DiString& name)
            :mGroupName(name)
        {
        }

        ~DiPropertyGroup()
        {
            for (auto p : mProperties)
            {
                DI_DELETE p;
            }
        }

        void AddProperty(const DiString& caption, DiPropertyBase* prop)
        {
            mProperties.push_back(DI_NEW DiEditProperty(caption, prop));
        }

        DiString mGroupName;

        DiVector<DiEditProperty*> mProperties;
    };

    using PropertyGroups = DiVector<DiPropertyGroup*>;
}

#endif
