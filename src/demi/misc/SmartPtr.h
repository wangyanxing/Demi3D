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

#ifndef SmartPtr_h_
#define SmartPtr_h_
                     
#include <memory>

#define TR1_PREFIX std

namespace Demi
{
    using TR1_PREFIX::shared_ptr;
    using TR1_PREFIX::weak_ptr;
    using TR1_PREFIX::enable_shared_from_this;
    using TR1_PREFIX::make_shared;
}

#endif