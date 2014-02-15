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

#ifndef SkyLight_h__
#define SkyLight_h__

#include "light.h"

namespace Demi 
{
    typedef shared_ptr<DiSkyLight> DiSkyLightPtr;
    
    /** Sky light with a sky color and ground color
        this kind of light is more suitable than the directional light
        actually it's a hemisphere light
     */
    class DI_GFX_API DiSkyLight : public DiLight
    {
    public:

        DiSkyLight();

        virtual         ~DiSkyLight();

    public:

        /** The general directional of the light
            when we compute the lighting contribution of this direction, it 
            is usually the same as the traditional direction light
         */
        void            SetDirection(const DiVec3& vec) { mDirection = vec; }

        DiVec3          GetDirection() const { return mDirection; }

        /** Set the ground color
            the ground color will reflected in the oppoisite direction of the light
         */
        void            SetGrondColor(const DiColor& vec) { mGroundColor = vec; }

        DiColor         GetGroundColor() const { return mGroundColor; }

        const DiAABB&   GetBoundingBox(void) const;

        void            Update(DiCamera*);

    protected:

        DiVec3          mDirection;

        DiColor         mGroundColor;
    };
}

#endif // SkyLight_h__