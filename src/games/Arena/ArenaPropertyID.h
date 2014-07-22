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

#ifndef ArenaPropertyID_h__
#define ArenaPropertyID_h__

namespace Demi
{
    enum ArPropertyIDType
    {
        /// dummy type
        PROPERTY_NONE = -1,

        PROPERTY_MOVE = 0,
        PROPERTY_MOTION,
        PROPERTY_FX,
        PROPERTY_ATTRIBUTE,
        PROPERTY_STATE,
        PROPERTY_ITEM,
        PROPERTY_SKILL,
        PROPERTY_TASK,
        PROPERTY_APPROACH,
        PROPERTY_PASSIVE_APPROACH,
        PROPERTY_AI,
        PROPERTY_SHAPE,
        PROPERTY_UNIT_SHAPE,

        PROPERTY_ID_NUM,
    };
}

#endif
