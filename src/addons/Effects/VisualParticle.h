
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

/****************************************************
  This module was originally from Particle Universe
  Repo: https://github.com/scrawl/particleuniverse
  License: MIT 
****************************************************/

#ifndef VisualParticle_h__
#define VisualParticle_h__

#include "FxPrerequisites.h"
#include "Particle.h"

namespace Demi
{
    class DEMI_FX_API DiIVisualData
    {
    public:
        DiIVisualData(void) {};

        virtual ~DiIVisualData(void) {};

        virtual void SetVisible(bool visible) = 0;
    };
    
    //////////////////////////////////////////////////////////////////////////

    class DEMI_FX_API DiVisualParticle : public DiParticle
    {
    public:
        DiVisualParticle(void);

        virtual         ~DiVisualParticle(void);

    public:

        void            SetOwnDimensions(float newWidth, float newHeight, float newDepth);

        void            SetScaleDimensions(float newWidth, float newHeight, float newDepth);

        virtual void    InitForEmission(void);

        virtual void    InitForExpiration(DiParticleElement* technique, float timeElapsed);

        void            CalculateBoundingSphereRadius(void);

    public:

        DiColor         colour;

        DiColor         originalColour;

        // 2D rotation
        DiRadian        zRotation;

        DiRadian        zRotationSpeed;

        DiQuat          orientation;

        DiQuat          originalOrientation;

        // rotation speed, for orientation calculation
        float           rotationSpeed;

        // rotation axis, for orientation calculation
        DiVec3          rotationAxis;

        bool            ownDimensions;

        float           width;

        float           height;

        float           depth;

        float           originWidth{ 0 };

        float           originHeight{ 0 };

        float           originDepth{ 0 };

        // used for collision detection
        float           radius;

        float           textureAnimationTimeStep;

        float           textureAnimationTimeStepCount;

        uint16          textureCoordsCurrent;

        bool            textureAnimationDirectionUp;

        DiAttributeCurved   scale;
    };
}

#endif // VisualParticle_h__
