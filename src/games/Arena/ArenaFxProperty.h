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

#ifndef ArenaFxProperty_h__
#define ArenaFxProperty_h__

#include "ArenaPrerequisites.h"
#include "ArenaProperty.h"

namespace Demi
{
    /** state property
     */
    class ArFxProperty : public ArProperty
    {
        DEFINE_PROPERTY_ID(PROPERTY_FX)

    public:

        ArFxProperty();

        ~ArFxProperty();

    public:

        void            Update(float dt);
        
    protected:
        
        void            Init();
        
    protected:
        
        DiVector<DiString> mProjectiles;
    };
}

#endif
