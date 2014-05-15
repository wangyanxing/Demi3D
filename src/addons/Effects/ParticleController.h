
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

#ifndef ParticleController_h__
#define ParticleController_h__

#include "FxPrerequisites.h"
#include "Particle.h"

namespace Demi
{
    class DEMI_FX_API DiParticleController : public DiParticle
    {
    public:
        // 控制器的控制方式，用于计算部分控制器的附加参数
        // 只有几个控制器才会涉及到这个枚举值，比如重力球/缩放/线行力
        enum ControlSpecial
        {
            AFSP_DEFAULT,         //参数恒定，1.0
            AFSP_TTL_INCREASE,    //参数随时间递增
            AFSP_TTL_DECREASE     //参数随时间递减
        };

        static const bool            DEFAULT_ENABLED;
        static const DiVec3          DEFAULT_POSITION;
        static const ControlSpecial  DEFAULT_SPECIALISATION;

    public:

        DiParticleController(void);

        virtual                     ~DiParticleController(void) {}

    public:

        const ControlSpecial&       GetControllerSpecialisation(void) const {return mControllerSpecialisation;}

        void                        SetControllerSpecialisation(const ControlSpecial& affectSpecialisation);

        const DiString&             GetControllerType(void) const {return mControllerType;}

        void                        SetControllerType(const DiString& affectorType) {mControllerType = affectorType;}

        const DiString&             GetName(void) const {return mName;}

        void                        SetName(const DiString& name) {mName = name;}

        DiParticleElement*          GetParentElement(void) const {return mParentElement;}

        void                        SetParentElement(DiParticleElement* parentTechnique);

        virtual void                Prepare(DiParticleElement* particleTechnique){}

        virtual void                Unprepare(DiParticleElement* particleTechnique){}

        virtual void                NotifyStart (void);
                                    
        virtual void                NotifyStop (void){}
                                    
        virtual void                NotifyPause (void){}
                                    
        virtual void                NotifyResume (void){}

        virtual void                NotifyRescaled(const DiVec3& scale);

        virtual void                PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed){}

        virtual void                FirstParticle(DiParticleElement* particleTechnique, 
                                        DiParticle* particle, 
                                        float timeElapsed) {}

        virtual void                InitForEmission(void);

        virtual void                InitForExpiration(DiParticleElement* technique, float timeElapsed);

        virtual void                InitParticleForEmission(DiParticle* particle) {}

        // 控制粒子的入口函数，在Control调用之前调用
        void                        ProcessParticle(DiParticleElement* particleTechnique, 
                                        DiParticle* particle, 
                                        float timeElapsed, 
                                        bool firstParticle);

        virtual void                PostProcessParticles(DiParticleElement* technique, float timeElapsed){}

        // 控制器的核心功能：控制一个粒子
        virtual void                Control(DiParticleElement* particleTechnique, 
                                        DiParticle* particle, 
                                        float timeElapsed) = 0;

        // 排除的发射器，即不受此控制器影响
        void                        AddEmitterToExclude(const DiString& emitterName);

        void                        RemoveEmitterToExclude(const DiString& emitterName);

        void                        RemoveAllEmittersToExclude(void);

        const DiList<DiString>&     GetEmittersToExclude(void) const;

        bool                        HasEmitterToExclude(const DiString& emitterName);

        virtual void                CopyTo(DiParticleController* affector);

        virtual void                CopyParentTo(DiParticleController* affector);

        const DiVec3&               GetDerivedPosition(void);

        // 根据ControlSpecial枚举值计算附加参数
        float                       CalculateAffectSpecialisationFactor (const DiParticle* particle);

    protected:
        DiParticleElement*          mParentElement;

        DiString                    mControllerType;

        DiString                    mName;

        ControlSpecial              mControllerSpecialisation;

        DiList<DiString>            mExcludedEmitters;

        // 控制器层的缩放值
        DiVec3                      mControllerScale;
    };
}

#endif // ParticleController_h__
