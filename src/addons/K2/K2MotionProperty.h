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

#ifndef DiK2MotionProperty_h__
#define DiK2MotionProperty_h__

#include "K2Prerequisites.h"
#include "K2Property.h"

namespace Demi
{
    /** state property
     */
    class DEMI_K2_API DiK2MotionProperty : public DiK2Property
    {
        DEFINE_PROPERTY_ID(PROPERTY_MOTION)

    public:

        DiK2MotionProperty();

        ~DiK2MotionProperty();

    public:

        void            Update(float dt);

        void            ModalityChange(K2ModalityType mod);

    private:

        K2ModalityType  mModality;
    };
}

#endif
