
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

#ifndef PropertyTypes_h__
#define PropertyTypes_h__

#include "Property.h"

namespace Demi
{
    typedef DiProperty<DiString>            DiStringProperty;
    typedef DiProperty<float>               DiFloatProperty;
    typedef DiProperty<int>                 DiIntProperty;
    typedef DiProperty<bool>                DiBoolProperty;
    typedef DiProperty<DiVec2>              DiVec2Property;
    typedef DiProperty<DiVec3>              DiVec3Property;
    typedef DiProperty<DiVec4>              DiVec4Property;
    typedef DiProperty<DiQuat>              DiQuatProperty;
    typedef DiProperty<DiColor>             DiColorProperty;
    typedef DiProperty<DiDynamicAttribute*> DiDynProperty;
}

#endif // PropertyTypes_h__