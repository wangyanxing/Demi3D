
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

#include "FxPch.h"
#include "VisualParticle.h"
#include "ParticleEmitter.h"
#include "ParticleElement.h"

namespace Demi
{
    DiVisualParticle::DiVisualParticle(void) : 
        DiParticle(),
        colour(DiColor::White),
        originalColour(DiColor::White),
        zRotation(DiRadian(0)),
        zRotationSpeed(DiRadian(0)),
        orientation(DiQuat::IDENTITY),
        originalOrientation(DiQuat::IDENTITY),
        rotationSpeed(0.0f),
        rotationAxis(DiVec3::UNIT_Z),
        ownDimensions(false),
        width(1.0f),
        height(1.0f),
        depth(1.0f),
        radius(0.87f),
        textureAnimationTimeStep(0.1f),
        textureAnimationTimeStepCount(0.0f),
        textureCoordsCurrent(0),
        textureAnimationDirectionUp(true)
    {
        particleType = PT_VISUAL;
        mMarkedForEmission  = true;
    }
    
    void DiVisualParticle::SetOwnDimensions(float newWidth, float newHeight, float newDepth)
    {
        ownDimensions = true;
        if (newWidth)
        {
            originWidth = newWidth;
            width = newWidth;
        }
        if (newHeight)
        {
            originHeight = newHeight;
            height = newHeight;
        }
        if (newDepth)
        {
            originDepth = newDepth;
            depth = newDepth;
        }
        CalculateBoundingSphereRadius();
        parentEmitter->GetParentElement()->NotifyParticleResized();
    }

    void DiVisualParticle::SetScaleDimensions(float newWidth, float newHeight, float newDepth)
    {
        ownDimensions = true;
        if (newWidth)
        {
            width = newWidth;
        }
        if (newHeight)
        {
            height = newHeight;
        }
        if (newDepth)
        {
            depth = newDepth;
        }
        CalculateBoundingSphereRadius();
        parentEmitter->GetParentElement()->NotifyParticleResized();
    }
    
    void DiVisualParticle::InitForEmission(void)
    {
        DiParticle::InitForEmission();
        textureAnimationTimeStep = 0.1f;
        textureAnimationTimeStepCount = 0.0f;
        textureCoordsCurrent = 0;
        textureAnimationDirectionUp = true;
    }
    
    void DiVisualParticle::InitForExpiration(DiParticleElement* technique, float timeElapsed)
    {
        DiParticle::InitForExpiration(technique, timeElapsed);
        if (visualData)
        {
            visualData->SetVisible(false);
        }
    }
    
    inline void DiVisualParticle::CalculateBoundingSphereRadius(void)
    {
        //radius = 0.5 * DiMath::Sqrt(width*width + height*height + depth*depth);
        radius = 0.5f * std::max(depth, std::max(width, height)); // ½üËÆÖµ
    }

    DiVisualParticle::~DiVisualParticle(void)
    {
    }
}

