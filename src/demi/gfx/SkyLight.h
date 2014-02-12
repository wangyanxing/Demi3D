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
    class DI_GFX_API DiSkyLight : public DiLight
    {
    public:

        friend class DiSceneManager;

    protected:

        DiSkyLight(DiSceneManager* mgr);

        virtual         ~DiSkyLight();

    public:

        void            SetDirection(const DiVec3& vec) { mDirection = vec; }

        DiVec3          GetDirection() const { return mDirection; }

        void            SetGrondColor(const DiColor& vec) { mGroundColor = vec; }

        DiColor         GetGroundColor() const { return mGroundColor; }

        const DiAABB&   GetBoundingBox(void) const;

        DiString&       GetType();

    protected:

        DiVec3          mDirection;

        DiColor         mGroundColor;
    };
}

#endif // SkyLight_h__