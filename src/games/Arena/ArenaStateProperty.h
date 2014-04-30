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

#ifndef ArenaStateProperty_h__
#define ArenaStateProperty_h__

#include "ArenaPrerequisites.h"
#include "ArenaProperty.h"

namespace Demi
{
    /** state property
     */
    class ArStateProperty : public ArProperty
    {
        DEFINE_PROPERTY_ID(PROPERTY_STATE)

    public:

        ArStateProperty();

        ~ArStateProperty();

    public:

        void            Update(float dt);

        void            AddState(ArCharState stateType, ArStateID id, double life);

        void            RemoveState(ArStateID id);

        void            ModalityChange(ArModalityType mod);

    private:

        typedef DiMap<ArStateID, ArStateInfo> States;

        States          mStates;
    };
}

#endif
