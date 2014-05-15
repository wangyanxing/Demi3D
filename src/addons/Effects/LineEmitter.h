
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

#ifndef LineEmitter_h__
#define LineEmitter_h__

#include "FxPrerequisites.h"
#include "ParticleEmitter.h"
#include "ParticleEmitterFactory.h"

namespace Demi
{
    class DEMI_FX_API DiLineEmitter : public DiParticleEmitter
    {
    public:
        
        static const DiVec3 DEFAULT_END;
        static const float     DEFAULT_MIN_INCREMENT;
        static const float     DEFAULT_MAX_INCREMENT;
        static const float     DEFAULT_MAX_DEVIATION;

        DiLineEmitter(void);

        virtual                    ~DiLineEmitter(void) {}

    public:

        virtual unsigned short    CalculateRequestedParticles(float timeElapsed);

        float                    GetMaxDeviation(void) const;

        void                    SetMaxDeviation(float maxDeviation);

        float                    GetMaxIncrement(void) const;

        void                    SetMaxIncrement(float maxIncrement);

        float                    GetMinIncrement(void) const;

        void                    SetMinIncrement(float minIncrement);

        const DiVec3&            GetEnd(void) const;

        void                    SetEnd(const DiVec3& end);

        virtual void             NotifyRescaled(const DiVec3& scale);

        virtual void             InitParticlePosition(DiParticle* particle);

        virtual void             InitParticleDirection(DiParticle* particle);

        virtual void             CopyTo (DiParticleEmitter* emitter);
        
        void                    NotifyStart();

    protected:

        // line的末尾点，起始点是发射器的位置
        DiVec3                    mEnd;

        // line的垂直向量
        DiVec3                    mPerpendicular;

        // 发射偏移
        float                     mMaxDeviation;

        float                     mMinIncrement;
        float                     mMaxIncrement;
        float                     mIncrement;
        float                     mLength;
        bool                    mIncrementsLeft;
        
        // 内部标记，第一个粒子是否已经发射
        bool                    mFirst;

        DiVec3                    mScaledEnd;
        float                     mScaledMaxDeviation;
        float                     mScaledMinIncrement;
        float                     mScaledMaxIncrement;
        float                     mScaledLength;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_FX_API DiLineEmitterFactory : public DiParticleEmitterFactory
    {
    public:
        DiLineEmitterFactory(void) {}

        virtual                ~DiLineEmitterFactory(void) {}

        DiString            GetEmitterType(void) const
        {
            static DiString type = "Line";
            return type;
        }

        DiParticleEmitter*    CreateEmitter(void)
        {
            return CreateEmitterImpl<DiLineEmitter>();
        }
    };
}
#endif // LineEmitter_h__

