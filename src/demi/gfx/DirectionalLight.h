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

#ifndef DiDirectionalLight_h__
#define DiDirectionalLight_h__


#include "light.h"

namespace Demi
{
    typedef shared_ptr<DiDirLight> DiDirLightPtr;
    
    /** Directional light
        only contains direction and color
     */
    class DI_GFX_API DiDirLight : public DiLight
    {
    public:

        DiDirLight();

        virtual         ~DiDirLight();

    public:

        void            Update(DiCamera*);

        void            SetDirection(const DiVec3& vec) { mDirection = vec; }

        DiVec3          GetDirection() const {return mDirection;}

        const DiAABB&   GetBoundingBox(void) const;

    protected:

        DiVec3          mDirection;
        
    };
}

#endif
