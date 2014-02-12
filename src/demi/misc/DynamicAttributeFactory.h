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

#ifndef DiDynamicAttributeFactory_h__
#define DiDynamicAttributeFactory_h__


#include "DynamicAttribute.h"

namespace Demi
{
    class DI_MISC_API DiDynamicAttributeFactory
    {
    public:

        DiDynamicAttributeFactory(void) {};
        virtual ~DiDynamicAttributeFactory(void) {};

        template <class T>
        T* CreateDynamicAttribute(void)
        {
            return DI_NEW T();
        };

        DiDynamicAttribute* CloneDynamicAttribute(DiDynamicAttribute* dynamicAttribute)
        {
            if (dynamicAttribute)
            {
                switch(dynamicAttribute->GetType())
                {
                    case DiDynamicAttribute::DAT_FIXED:
                    {
                        DiAttributeFixed* dynAttr = CreateDynamicAttribute<DiAttributeFixed>();
                        dynamicAttribute->CopyTo(dynAttr);
                        return dynAttr;    
                    }
                    break;

                    case DiDynamicAttribute::DAT_RANDOM:
                    {
                        DiAttributeRandom* dynAttr = CreateDynamicAttribute<DiAttributeRandom>();
                        dynamicAttribute->CopyTo(dynAttr);
                        return dynAttr;
                    }
                    break;

                    case DiDynamicAttribute::DAT_CURVED:
                    {
                        DiAttributeCurved* dynAttr = CreateDynamicAttribute<DiAttributeCurved>();
                        dynamicAttribute->CopyTo(dynAttr);
                        return dynAttr;
                    }
                    break;

                }
            }

            return 0;
        }
        
        void DestroyDynamicAttribute (DiDynamicAttribute* dynamicAttribute)
        {
            DI_DELETE dynamicAttribute;
        }

        float GetDefaultValue(const DiDynamicAttribute* dynAttribute)
        {
            if (!dynAttribute)
                return 0;

            switch (dynAttribute->GetType())
            {
                case DiDynamicAttribute::DAT_FIXED:
                    {
                        const DiAttributeFixed* dynFixed = static_cast<const DiAttributeFixed*>(dynAttribute);
                        DiAttributeFixed d(*dynFixed);
                        return d.GetValue();
                    }
                    break;

                case DiDynamicAttribute::DAT_RANDOM:
                    {
                        const DiAttributeRandom* dynRandom = static_cast<const DiAttributeRandom*>(dynAttribute);
                        DiAttributeRandom d(*dynRandom);
                        return d.GetValue();
                    }
                    break;

                case DiDynamicAttribute::DAT_CURVED:
                    {
                        const DiAttributeCurved* dynCurved = static_cast<const DiAttributeCurved*>(dynAttribute);
                        DiAttributeCurved d(*dynCurved);
                        return d.GetValue();
                    }
                    break;
            }

            return 0;
        }
    };
}

#endif
