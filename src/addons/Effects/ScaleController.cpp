
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
#include "ScaleController.h"
#include "ParticleElement.h"
#include "ParticleSystem.h"
#include "VisualParticle.h"

namespace Demi
{
    const float DiScaleController::DEFAULT_X_SCALE = 1.0f;
    const float DiScaleController::DEFAULT_Y_SCALE = 1.0f;
    const float DiScaleController::DEFAULT_Z_SCALE = 1.0f;
    const float DiScaleController::DEFAULT_XYZ_SCALE = 1.0f;

    DiScaleController::DiScaleController(void) : 
        DiParticleController(),
        mDynScaleXSet(false),
        mDynScaleYSet(false),
        mDynScaleZSet(false),
        mDynScaleXYZSet(false),
        mSinceStartSystem(false)
    {
        mDynScaleX = DI_NEW DiAttributeFixed();
        mDynScaleY = DI_NEW DiAttributeFixed();
        mDynScaleZ = DI_NEW DiAttributeFixed();
        mDynScaleXYZ = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynScaleX))->SetValue(DEFAULT_X_SCALE);
        (static_cast<DiAttributeFixed*>(mDynScaleY))->SetValue(DEFAULT_Y_SCALE);
        (static_cast<DiAttributeFixed*>(mDynScaleZ))->SetValue(DEFAULT_Z_SCALE);
        (static_cast<DiAttributeFixed*>(mDynScaleXYZ))->SetValue(DEFAULT_XYZ_SCALE);
        mLatestTimeElapsed = 1.0f;
    }
    
    DiScaleController::~DiScaleController(void)
    {
        SAFE_DELETE(mDynScaleX);
        SAFE_DELETE(mDynScaleY);
        SAFE_DELETE(mDynScaleZ);
        SAFE_DELETE(mDynScaleXYZ);
    }
    
    void DiScaleController::Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed)
    {
        if (particle->particleType != DiParticle::PT_VISUAL)
        {
            return;
        }

        DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);
        float ds = 0;
        float width = 0;
        float height = 0;
        float depth = 0;

        if (mDynScaleXYZSet)
        {
            ds = CalculateScale(mDynScaleXYZ, particle) * timeElapsed * CalculateAffectSpecialisationFactor(particle);
            if (visualParticle->width > 0)
            {
                width = visualParticle->width + ds * mControllerScale.x;
            }
            if (visualParticle->height > 0)
            {
                height = visualParticle->height + ds * mControllerScale.y;
            }
            if (visualParticle->depth > 0)
            {
                depth = visualParticle->depth + ds * mControllerScale.z;
            }
            visualParticle->SetOwnDimensions(width, height, depth);
        }
        else
        {
            if (mDynScaleXSet)
            {
                ds = CalculateScale(mDynScaleX, particle) * timeElapsed;
                if (visualParticle->width > 0)
                {
                    width = visualParticle->width + ds * mControllerScale.x;
                }
            }
            if (mDynScaleYSet)
            {
                ds = CalculateScale(mDynScaleY, particle) * timeElapsed;
                if (visualParticle->height > 0)
                {
                    height = visualParticle->height + ds * mControllerScale.y;
                }
            }
            if (mDynScaleZSet)
            {
                ds = CalculateScale(mDynScaleZ, particle) * timeElapsed;
                if (visualParticle->depth > 0)
                {
                    depth = visualParticle->depth + ds * mControllerScale.z;
                }
            }
            visualParticle->SetOwnDimensions(width, height, depth);
        }
    }
    
    void DiScaleController::SetDynScaleX(DiDynamicAttribute* dynScaleX)
    {
        SAFE_DELETE(mDynScaleX);

        mDynScaleX = dynScaleX;
        mDynScaleXSet = true;
    }
    
    void DiScaleController::ResetDynScaleX(bool resetToDefault)
    {
        if (resetToDefault)
        {
            SAFE_DELETE(mDynScaleX);
            mDynScaleX = DI_NEW DiAttributeFixed();
            (static_cast<DiAttributeFixed*>(mDynScaleX))->SetValue(DEFAULT_X_SCALE);
            mDynScaleXSet = false;
        }
        else
        {
            mDynScaleXSet = true;
        }
    }
    
    void DiScaleController::SetDynScaleY(DiDynamicAttribute* dynScaleY)
    {
        SAFE_DELETE(mDynScaleY);
        mDynScaleY = dynScaleY;
        mDynScaleYSet = true;
    }
    
    void DiScaleController::ResetDynScaleY(bool resetToDefault)
    {
        if (resetToDefault)
        {
            SAFE_DELETE(mDynScaleY);
            mDynScaleY = DI_NEW DiAttributeFixed();
            (static_cast<DiAttributeFixed*>(mDynScaleY))->SetValue(DEFAULT_X_SCALE);
            mDynScaleYSet = false;
        }
        else
        {
            mDynScaleYSet = true;
        }
    }
    
    void DiScaleController::SetDynScaleZ(DiDynamicAttribute* dynScaleZ)
    {
        SAFE_DELETE(mDynScaleZ);

        mDynScaleZ = dynScaleZ;
        mDynScaleZSet = true;
    }
    
    void DiScaleController::ResetDynScaleZ(bool resetToDefault)
    {
        if (resetToDefault)
        {
            SAFE_DELETE(mDynScaleZ);
            mDynScaleZ = DI_NEW DiAttributeFixed();
            (static_cast<DiAttributeFixed*>(mDynScaleZ))->SetValue(DEFAULT_X_SCALE);
            mDynScaleYSet = false;
        }
        else
        {
            mDynScaleYSet = true;
        }
    }
    
    void DiScaleController::SetDynScaleXYZ(DiDynamicAttribute* dynScaleXYZ)
    {
        SAFE_DELETE(mDynScaleXYZ);

        mDynScaleXYZ = dynScaleXYZ;
        mDynScaleXYZSet = true;
    }
    
    void DiScaleController::ResetDynScaleXYZ(bool resetToDefault)
    {
        if (resetToDefault)
        {
            SAFE_DELETE(mDynScaleXYZ);
            mDynScaleXYZ = DI_NEW DiAttributeFixed();
            (static_cast<DiAttributeFixed*>(mDynScaleXYZ))->SetValue(DEFAULT_XYZ_SCALE);
            mDynScaleXYZSet = false;
        }
        else
        {
            mDynScaleXYZSet = true;
        }
    }
    
    float DiScaleController::CalculateScale(DiDynamicAttribute* dynScale, DiParticle* particle)
    {
        if (!particle)
            return 0.0f;

        if (mSinceStartSystem)
        {
            return mDynamicAttributeHelper.Calculate(dynScale, mParentElement->GetParentSystem()->GetTimeElapsedSinceStart());
        }
        else
        {
            return mDynamicAttributeHelper.Calculate(dynScale, particle->timeFraction);
        }
    }
    
    void DiScaleController::CopyTo (DiParticleController* affector)
    {
        DiParticleController::CopyTo(affector);

        DiScaleController* scaleAffector = static_cast<DiScaleController*>(affector);
        scaleAffector->SetDynScaleX(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynScaleX()));
        scaleAffector->SetDynScaleY(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynScaleY()));
        scaleAffector->SetDynScaleZ(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynScaleZ()));
        scaleAffector->SetDynScaleXYZ(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynScaleXYZ()));
        scaleAffector->mDynScaleXSet         = mDynScaleXSet;
        scaleAffector->mDynScaleYSet         = mDynScaleYSet;
        scaleAffector->mDynScaleZSet         = mDynScaleZSet;
        scaleAffector->mDynScaleXYZSet    = mDynScaleXYZSet;
        scaleAffector->mSinceStartSystem    = mSinceStartSystem;
    }
}

