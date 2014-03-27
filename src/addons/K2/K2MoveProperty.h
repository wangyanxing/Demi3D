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

#ifndef DiK2MoveProperty_h__
#define DiK2MoveProperty_h__

#include "K2Prerequisites.h"
#include "K2Property.h"

namespace Demi
{
    class DEMI_K2_API DiK2MoveProperty : public DiK2Property
    {
        DEFINE_PROPERTY_ID(PROPERTY_MOVE)

    public:

        DiK2MoveProperty();

        ~DiK2MoveProperty();

    public:

        typedef DiVector<DiK2Pos> WorldPosList;

        void            update(float dt);

        void            stop(bool bPassive = false);

        void            turnTo(const DiK2Pos& pos);

        WorldPosList    moveTo(const DiK2Pos& position, float fRange = 0.0f, bool bPassive = false);

    private:

    };
}

#endif
