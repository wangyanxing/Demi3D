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

#ifndef SpotLight_h__
#define SpotLight_h__

#include "light.h"

namespace Demi 
{    
    /** Spot light
     */
    class DI_GFX_API DiSpotLight : public DiLight
    {
    public:

        DiSpotLight();

        virtual             ~DiSpotLight();

    public:

        void                SetRange(const DiRadian& innerAngle, const DiRadian& outerAngle, float falloff = 1.0)
        {
            mSpotInner = innerAngle;
            mSpotOuter = outerAngle;
            mSpotFalloff = falloff;
        }
        
        const DiRadian&     GetInnerAngle(void) const{ return mSpotInner; }

        const DiRadian&     GetOuterAngle(void) const{ return mSpotOuter; }

        float               GetFalloff(void) const{return mSpotFalloff;}
        
		void                SetInnerAngle(const DiRadian& val){mSpotInner = val;}
        
		void                SetOuterAngle(const DiRadian& val){mSpotOuter = val;}
        
		void                SetFalloff(float val){mSpotFalloff = val;}
        
		void                SetNearClipDistance(float nearClip) { mSpotNearClip = nearClip; }
		
		float               GetNearClipDistance() const { return mSpotNearClip; }
		
        const DiAABB&       GetBoundingBox(void) const;

        void                Update(DiCamera*);
        
        void                SetDirection(const DiVec3& vec){mDirection = vec;}
        
        DiVec3              GetDirection() const {return mDirection;}
        
        DiVec3              GetDerivedDirection() const;
        
        DiVec3              GetDerivedPosition() const;

    protected:

        DiRadian            mSpotOuter{ DiDegree(40.0f) };

        DiRadian            mSpotInner{ DiDegree(30.0f) };
        
        float               mSpotFalloff{ 1.0f };
		
        float               mSpotNearClip{ 0.0f };
        
        DiVec3              mDirection;
    };
}

#endif // SkyLight_h__