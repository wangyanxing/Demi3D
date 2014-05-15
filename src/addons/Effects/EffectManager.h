
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

#ifndef EffectManager_h__
#define EffectManager_h__

#include "FxPrerequisites.h"
#include "DynamicAttribute.h"
#include "Singleton.h"

namespace Demi
{
    class DEMI_FX_API DiEffectManager : public DiBase
                                      , public DiSingleton<DiEffectManager>
    {
    friend class DiParticleSystemFactory;

    public:
        DiEffectManager ();

        ~DiEffectManager (void);

        _DECLARE_SINGLETON(DiEffectManager)

    public:

        void                        InitFactories();

        void                        ReleaseFactories();

        void                        DestroyAllParticleSystemTemplates(void);

        void                        AddEmitterFactory(DiParticleEmitterFactory* factory);

        DiParticleEmitterFactory*   GetEmitterFactory(const DiString& emitterType);

        void                        RemoveEmitterFactory(const DiString& emitterType);

        void                        DestroyEmitterFactory(const DiString& emitterType);

        DiParticleEmitter*          CreateEmitter(const DiString& emitterType);

        DiParticleEmitter*          CloneEmitter(DiParticleEmitter* emitter);

        void                        DestroyEmitter(DiParticleEmitter* emitter);

        void                        AddControllerFactory(DiParticleControllerFactory* factory);

        DiParticleControllerFactory*GetControllerFactory(const DiString& affectorType);

        void                        RemoveControllerFactory(const DiString& affectorType);

        void                        DestroyControllerFactory(const DiString& affectorType);

        DiParticleController*       CreateController(const DiString& affectorType);

        DiParticleController*       CloneController(DiParticleController* affector);

        void                        DestroyController(DiParticleController* affector);

        DiParticleElement*          CreateElement(void);

        DiParticleElement*          CloneElement(DiParticleElement* technique);

        void                        DestroyElement(DiParticleElement* technique);

        void                        AddRendererFactory(DiParticleRendererFactory* factory);

        DiParticleRendererFactory*  GetRendererFactory(const DiString& rendererType);

        void                        RemoveRendererFactory(const DiString& affectorType);

        void                        DestroyRendererFactory(const DiString& rendererType);

        DiParticleRenderer*         CreateRenderer(const DiString& rendererType);

        DiParticleRenderer*         CloneRenderer(DiParticleRenderer* renderer);

        void                        DestroyRenderer(DiParticleRenderer* renderer);

        // 创建粒子系统模板，一个模板可以创建出多个特效
        // 模板最终表现为文件
        DiParticleSystem*           CreateParticleSystemTemplate(const DiString& name);

        void                        ReplaceParticleSystemTemplate(const DiString& name, DiParticleSystem* system);

        const DiString&             GetLastCreatedTemplateName(void);

        void                        AddParticleSystemTemplate(const DiString& name, DiParticleSystem* systemTemplate);

        DiParticleSystem*           GetParticleSystemTemplate(const DiString& templateName);

        void                        DestroyParticleSystemTemplate(const DiString& templateName);

        void                        ParticleSystemTemplateNames(DiVector<DiString>& v);

        // 由模板名来创建特效
        DiParticleSystem*           CreateParticleSystem(const DiString& name,
            const DiString& templateName);

        // 创建一个默认的特效
        DiParticleSystem*           CreateParticleSystem(const DiString& name);

        DiParticleSystem*           GetParticleSystem(const DiString& name);

        void                        DestroyParticleSystem(DiParticleSystem* particleSystem);

        void                        DestroyParticleSystem(const DiString& particleSystemName);

        void                        DestroyAllParticleSystems();

        DiDynamicAttribute*         CreateDynamicAttribute(DiDynamicAttribute::DynamicAttributeType type);

        void                        Update();
        
    protected:
        
        DiParticleSystem*           CreateSystemImpl(const DiString& name);

        DiParticleSystem*           CreateSystemImpl(const DiString& name, const DiString& templateName);

        void                        DestroySystemImpl(DiParticleSystem* particleSystem);

    protected:

        typedef DiMap<DiString, DiParticleEmitterFactory*> EmitterFactoryMap;
        EmitterFactoryMap           mEmitterFactories;

        typedef DiMap<DiString, DiParticleControllerFactory*> ControllerFactoryMap;
        ControllerFactoryMap        mControllerFactories;

        typedef DiMap<DiString, DiParticleRendererFactory*> RendererFactoryMap;
        RendererFactoryMap          mRendererFactories;

        typedef DiMap<DiString, DiParticleSystem*> ParticleSystemTemplateMap;
        ParticleSystemTemplateMap   mParticleSystemTemplates;

        typedef DiMap<DiString, DiParticleSystem*> ParticleSystemMap;
        ParticleSystemMap           mParticleSystems;

        DiParticleSystemFactory*    mParticleSystemFactory;

        DiString                    mLastCreatedTemplateName;

        DiSceneManager*             mSceneManager;
    };
}

#endif // EffectManager_h__
