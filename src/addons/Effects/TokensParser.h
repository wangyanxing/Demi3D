
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

#ifndef FxTokensParser_h__
#define FxTokensParser_h__

#include "FxPrerequisites.h"

#include "ParticleEmitter.h"
#include "BillboardRender.h"

namespace Demi
{
    class DEMI_FX_API DiFxTokensParser
    {
    public:

        DiFxTokensParser();

        ~DiFxTokensParser();

        static DiFxTokensParser* Get() { return sParser;  }

    public:

        /// write one particle system
        void    WriteSystem(DiParticleSystem* val, const DiString& filePath);

        DiVector<DiParticleSystem*>  LoadEffects(const DiString& file);

    public:

        void    WriteSystem(DiParticleSystem* val, DiXMLElement& node);

        void    WriteElement(DiParticleElement* val, DiXMLElement& node);

        DiParticleElement*   ReadElement(DiParticleSystem* val, DiXMLElement& node);

        DiParticleSystem*    ReadSystem(DiXMLElement& node);

    public:

        void    WriteRenderer(DiParticleRenderer* val, DiXMLElement& node);

        void    WriteBillboardRenderer(DiBillboardRenderer* val, DiXMLElement& node);

        DiParticleRenderer*    ReadRenderer(DiParticleElement* val, DiXMLElement& node);

        void    ReadBillboardRenderer(DiBillboardRenderer* val, DiXMLElement& node);

    public:

        void    WriteBaseEmitter(DiParticleEmitter* val, DiXMLElement& node);

        void    ReadBaseEmitter(DiParticleEmitter* val, DiXMLElement& node);

        void    WriteBoxEmitter(DiParticleEmitter* val, DiXMLElement& node);

        void    ReadBoxEmitter(DiParticleEmitter* val, DiXMLElement& node);

        void    WriteCircleEmitter(DiParticleEmitter* val, DiXMLElement& node);

        void    ReadCircleEmitter(DiParticleEmitter* val, DiXMLElement& node);

        void    WriteLineEmitter(DiParticleEmitter* val, DiXMLElement& node);

        void    ReadLineEmitter(DiParticleEmitter* val, DiXMLElement& node);

        DiParticleEmitter*  ReadEmitter(DiParticleElement* val, DiXMLElement& node);

    public:

        void    WriteBaseController(DiParticleController* val, DiXMLElement& node);

        void    ReadBaseController(DiParticleController* val, DiXMLElement& node);

        void    WriteBaseForceController(DiParticleController* val, DiXMLElement& node);

        void    ReadBaseForceController(DiParticleController* val, DiXMLElement& node);

        void    WriteLinearForceController(DiParticleController* val, DiXMLElement& node);

        void    ReadLinearForceController(DiParticleController* val, DiXMLElement& node);

        void    WriteSinForceController(DiParticleController* val, DiXMLElement& node);

        void    ReadSinForceController(DiParticleController* val, DiXMLElement& node);

        void    WriteGravityController(DiParticleController* val, DiXMLElement& node);

        void    ReadGravityController(DiParticleController* val, DiXMLElement& node);

        void    WriteJetController(DiParticleController* val, DiXMLElement& node);

        void    ReadJetController(DiParticleController* val, DiXMLElement& node);

        void    WriteRandomiserController(DiParticleController* val, DiXMLElement& node);

        void    ReadRandomiserController(DiParticleController* val, DiXMLElement& node);

        void    WriteVortexController(DiParticleController* val, DiXMLElement& node);

        void    ReadVortexController(DiParticleController* val, DiXMLElement& node);

        void    WriteTextureRotatorController(DiParticleController* val, DiXMLElement& node);

        void    ReadTextureRotatorController(DiParticleController* val, DiXMLElement& node);

        void    WriteBaseColliderController(DiParticleController* val, DiXMLElement& node);

        void    ReadBaseColliderController(DiParticleController* val, DiXMLElement& node);

        void    WritePlaneColliderController(DiParticleController* val, DiXMLElement& node);

        void    ReadPlaneColliderController(DiParticleController* val, DiXMLElement& node);

        void    WriteBoxColliderController(DiParticleController* val, DiXMLElement& node);

        void    ReadBoxColliderController(DiParticleController* val, DiXMLElement& node);

        void    WriteSphereColliderController(DiParticleController* val, DiXMLElement& node);

        void    ReadSphereColliderController(DiParticleController* val, DiXMLElement& node);

        void    WriteGeometryRotatorController(DiParticleController* val, DiXMLElement& node);

        void    ReadGeometryRotatorController(DiParticleController* val, DiXMLElement& node);

        void    WriteColorController(DiParticleController* val, DiXMLElement& node);

        void    ReadColorController(DiParticleController* val, DiXMLElement& node);

        void    WriteScaleController(DiParticleController* val, DiXMLElement& node);

        void    ReadScaleController(DiParticleController* val, DiXMLElement& node);

        DiParticleController* ReadController(DiParticleElement* val, DiXMLElement& node);

    protected:

        DiVector<DiFloatRect*>  ReadFloatRectVector(const DiString& val);

        DiString                WriteFloatRectVector(const DiVector<DiFloatRect*>& val);

    protected:

        static DiFxTokensParser*               sParser;

        DiMap<DiString, BillboardType>         sStrBillboardType;
        DiMap<BillboardType, DiString>         sBillboardTypeStr;

        DiMap<DiString, BillboardOrigin>       sStrBillboardOrigin;
        DiMap<BillboardOrigin, DiString>       sBillboardOriginStr;

        DiMap<DiString, BillboardRotationType> sStrBillboardRotationType;
        DiMap<BillboardRotationType, DiString> sBillboardRotationTypeStr;
    };
}

#endif // PointEmitter_h__
