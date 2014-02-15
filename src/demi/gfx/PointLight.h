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

#ifndef PointLight_h__
#define PointLight_h__

#include "light.h"

namespace Demi 
{
    typedef shared_ptr<DiPointLight> DiPointLightPtr;
    
    /** Point light, contains the basic attenuation parameters
        it should be culled and pushed to the final pipeline
     */
    class DI_GFX_API DiPointLight : public DiLight
    {
    public:

        DiPointLight();

        virtual         ~DiPointLight();

    public:

        /** Calculate and return the bounding box
         */
        const DiAABB&    GetBoundingBox(void) const;

        /** Attenuation parameters of the point light
         */
        void             SetAttenuation(float end, float begin = 0)
        {
            mAttenuationBegin = begin;
            mAttenuationEnd = end;
        }

        /** Calls only when this point light is visible
         */
        void             Update(DiCamera*);

        float            GetAttenuationBegin() const { return mAttenuationBegin; }

        float            GetAttenuationEnd() const { return mAttenuationEnd; }

    protected:

        float            mAttenuationBegin;

        float            mAttenuationEnd;

        mutable DiAABB   mAABB;
    };

}

#endif // PointLight_h__