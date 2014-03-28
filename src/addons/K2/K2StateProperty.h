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

#ifndef DiK2StateProperty_h__
#define DiK2StateProperty_h__

#include "K2Prerequisites.h"
#include "K2Property.h"

namespace Demi
{
    /** state property
     */
    class DEMI_K2_API DiK2StateProperty : public DiK2Property
    {
        DEFINE_PROPERTY_ID(PROPERTY_STATE)

    public:

        DiK2StateProperty();

        ~DiK2StateProperty();

    public:

        void            Update(float dt);

        void            AddState(K2CharState stateType, K2StateID id, double life);

        void            RemoveState(K2StateID id);

        void            ModalityChange(K2ModalityType mod);

    private:

        typedef DiMap<K2StateID, K2StateInfo> States;

        States          mStates;
    };
}

#endif
