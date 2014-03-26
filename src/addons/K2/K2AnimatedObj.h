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

#ifndef DiK2AnimatedObj_h__
#define DiK2AnimatedObj_h__

#include "K2Prerequisites.h"
#include "K2RenderObjects.h"

namespace Demi
{
    class DEMI_K2_API DiK2AnimatedObj : public DiK2RenderObject
    {
    public:

        DiK2AnimatedObj();

        virtual ~DiK2AnimatedObj();

    public:

        DiK2ModelPtr    LoadModel(const DiString& mdf);

    };
}

#endif
