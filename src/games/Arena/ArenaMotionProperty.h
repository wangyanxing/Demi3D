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

#ifndef ArenaMotionProperty_h__
#define ArenaMotionProperty_h__

#include "ArenaPrerequisites.h"
#include "ArenaProperty.h"

namespace Demi
{
    /** state property
     */
    class ArMotionProperty : public ArProperty
    {
        DEFINE_PROPERTY_ID(PROPERTY_MOTION)

    public:

        ArMotionProperty();

        ~ArMotionProperty();

    public:

        void            Update(float dt);

        void            ModalityChange(ArModalityType mod);

        void            PlayClip(K2PrefabClip::Clips clip);

    private:

        ArModalityType  mModality { MODALITY_STAND };
    };
}

#endif
