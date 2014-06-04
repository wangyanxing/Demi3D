
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
#include "TokensParser.h"
#include "ParticleElement.h"
#include "ParticleSystem.h"
#include "XMLFile.h"
#include "BoxEmitter.h"
#include "CircleEmitter.h"
#include "LineEmitter.h"
#include "ColorController.h"
#include "LinearForceController.h"
#include "SineForceController.h"
#include "ScaleController.h"
#include "VortexController.h"
#include "GravityController.h"
#include "ScaleController.h"
#include "TextureRotatorController.h"
#include "GeometryRotatorController.h"
#include "RandomiserController.h"
#include "JetController.h"
#include "RandomiserController.h"
#include "PlaneColliderController.h"
#include "BoxColliderController.h"
#include "SphereColliderController.h"
#include "EffectManager.h"
#include "AssetManager.h"
#include "MaterialSerial.h"

namespace Demi
{
    DiFxTokensParser::DiFxTokensParser()
    {
        DI_ASSERT(!sParser);
        sParser = this;

        sStrBillboardType["point"]                  = BBT_POINT;
        sStrBillboardType["orientedCommon"]         = BBT_ORIENTED_COMMON;
        sStrBillboardType["orientedSelf"]           = BBT_ORIENTED_SELF;
        sStrBillboardType["perpendicularCommon"]    = BBT_PERPENDICULAR_COMMON;
        sStrBillboardType["perpendicularSelf"]      = BBT_PERPENDICULAR_SELF;
        sStrBillboardType["orientedShape"]          = BBT_ORIENTED_SHAPE;

        sBillboardTypeStr[BBT_POINT]                = "point";
        sBillboardTypeStr[BBT_ORIENTED_COMMON]      = "orientedCommon";
        sBillboardTypeStr[BBT_ORIENTED_SELF]        = "orientedSelf";
        sBillboardTypeStr[BBT_PERPENDICULAR_COMMON] = "perpendicularCommon";
        sBillboardTypeStr[BBT_PERPENDICULAR_SELF]   = "perpendicularSelf";
        sBillboardTypeStr[BBT_ORIENTED_SHAPE]       = "orientedShape";

        sStrBillboardOrigin["topLeft"]              = BBO_TOP_LEFT;
        sStrBillboardOrigin["topCenter"]            = BBO_TOP_CENTER;
        sStrBillboardOrigin["topRight"]             = BBO_TOP_RIGHT;
        sStrBillboardOrigin["centerLeft"]           = BBO_CENTER_LEFT;
        sStrBillboardOrigin["center"]               = BBO_CENTER;
        sStrBillboardOrigin["centerRight"]          = BBO_CENTER_RIGHT;
        sStrBillboardOrigin["bottomLeft"]           = BBO_BOTTOM_LEFT;
        sStrBillboardOrigin["bottomCenter"]         = BBO_BOTTOM_CENTER;
        sStrBillboardOrigin["bottomRight"]          = BBO_BOTTOM_RIGHT;

        sBillboardOriginStr[BBO_TOP_LEFT]           = "topLeft";
        sBillboardOriginStr[BBO_TOP_CENTER]         = "topCenter";
        sBillboardOriginStr[BBO_TOP_RIGHT]          = "topRight";
        sBillboardOriginStr[BBO_CENTER_LEFT]        = "centerLeft";
        sBillboardOriginStr[BBO_CENTER]             = "center";
        sBillboardOriginStr[BBO_CENTER_RIGHT]       = "centerRight";
        sBillboardOriginStr[BBO_BOTTOM_LEFT]        = "bottomLeft";
        sBillboardOriginStr[BBO_BOTTOM_CENTER]      = "bottomCenter";
        sBillboardOriginStr[BBO_BOTTOM_RIGHT]       = "bottomRight";

        sStrBillboardRotationType["vertex"]         = BBR_VERTEX;
        sStrBillboardRotationType["texcoord"]       = BBR_TEXCOORD;

        sBillboardRotationTypeStr[BBR_VERTEX]       = "vertex";
        sBillboardRotationTypeStr[BBR_TEXCOORD]     = "texcoord";
    }

    DiFxTokensParser::~DiFxTokensParser()
    {
        DI_ASSERT(sParser);
        sParser = nullptr;
    }

    void DiFxTokensParser::WriteBillboardRenderer(DiBillboardRenderer* val, DiXMLElement& node)
    {
        WriteRenderer(val, node);

        if (val->GetBillboardType() != DiBillboardRenderer::DEFAULT_BILLBOARD_TYPE)
            node.SetAttribute("billboardType", sBillboardTypeStr[val->GetBillboardType()]);

        if (val->GetBillboardOrigin() != DiBillboardRenderer::DEFAULT_ORIGIN)
            node.SetAttribute("origin", sBillboardOriginStr[val->GetBillboardOrigin()]);

        if (val->GetBillboardRotationType() != DiBillboardRenderer::DEFAULT_ROTATION_TYPE)
            node.SetAttribute("rotationType", sBillboardRotationTypeStr[val->GetBillboardRotationType()]);

        if (val->GetCommonDirection() != DiBillboardRenderer::DEFAULT_COMMON_DIRECTION)
            node.SetVector3("commonDirection", val->GetCommonDirection());

        if (val->GetCommonUpVector() != DiBillboardRenderer::DEFAULT_COMMON_UP_VECTOR)
            node.SetVector3("commonUpVector", val->GetCommonUpVector());
    }

    void DiFxTokensParser::WriteRenderer(DiParticleRenderer* val, DiXMLElement& node)
    {
        if (val->IsSorted() != DiParticleRenderer::DEFAULT_SORTED)
            node.SetBool("sorted", val->IsSorted());
        
        if (val->GetTextureCoordsRows() != DiParticleRenderer::DEFAULT_TEXTURECOORDS_ROWS)
            node.SetInt("texcoordRows", val->GetTextureCoordsRows());

        if (val->GetTextureCoordsColumns() != DiParticleRenderer::DEFAULT_TEXTURECOORDS_COLUMNS)
            node.SetInt("texcoordColumns", val->GetTextureCoordsColumns());

        auto& uvList = val->GetTextureCoords();
        if (!uvList.empty())
            node.SetAttribute("uvlist", WriteFloatRectVector(uvList));
    }

    void DiFxTokensParser::ReadBillboardRenderer(DiBillboardRenderer* renderer, DiXMLElement& node)
    {
        node.IterateAttributes([renderer, this](const DiString& name, const DiString& value){
            if (name == "billboardType")
                renderer->SetBillboardType(sStrBillboardType[value]);
            else if (name == "origin")
                renderer->SetBillboardOrigin(sStrBillboardOrigin[value]);
            else if (name == "rotationType")
                renderer->SetBillboardRotationType(sStrBillboardRotationType[value]);
            else if (name == "commonDirection")
                renderer->SetCommonDirection(value.AsVector3());
            else if (name == "commonUpVector")
                renderer->SetCommonUpVector(value.AsVector3());
        });
    }

    DiParticleRenderer* DiFxTokensParser::ReadRenderer(DiParticleElement* element, DiXMLElement& node)
    {
        DiString type = node.GetAttribute("type");
        element->SetRenderer(type);
        auto renderer = element->GetRenderer();

        node.IterateAttributes([renderer, this](const DiString& name, const DiString& value){
            if (name == "sorted")
                renderer->SetSorted(value.AsBool());
            else if (name == "texcoordRows")
                renderer->SetTextureCoordsRows(value.AsInt());
            else if (name == "texcoordColumns")
                renderer->SetTextureCoordsColumns(value.AsInt());
            else if (name == "uvlist")
                renderer->AddTextureCoords(ReadFloatRectVector(value));
        });

        if (type == "Billboard")
            ReadBillboardRenderer(static_cast<DiBillboardRenderer*>(renderer), node);

        return renderer;
    }

    using FloatRectVec = DiVector<DiFloatRect*>;

    FloatRectVec DiFxTokensParser::ReadFloatRectVector(const DiString& val)
    {
        DiVector<DiFloatRect*> ret;

        auto tokens = val.Tokenize(",");

        DI_ASSERT(!tokens.empty());

        for (auto& t : tokens)
        {
            DiFloatRect* fr = DI_NEW DiFloatRect();
            *fr = t.AsFloatRect();
            ret.push_back(fr);
        }

        return ret;
    }

    DiString DiFxTokensParser::WriteFloatRectVector(const FloatRectVec& val)
    {
        DiString ret;

        for (size_t i = 0; i < val.size(); ++i)
        {
            ret.AppendFloatRect(*val[i]);
            if (i != val.size() - 1)
                ret += ",";
        }

        return ret;
    }

    void DiFxTokensParser::WriteElement(DiParticleElement* technique, DiXMLElement& node)
    {
        if (!technique->GetName().empty())
            node.SetAttribute("name", technique->GetName());

        if (technique->GetOriginalEnabled() != DiParticleElement::DEFAULT_ENABLED)
            node.SetBool("originalEnabled", technique->GetOriginalEnabled());

        if (technique->position != DiParticleElement::DEFAULT_POSITION)
            node.SetVector3("position", technique->position);

        if (technique->IsKeepLocal() != DiParticleElement::DEFAULT_KEEP_LOCAL)
            node.SetBool("keepLocal", technique->IsKeepLocal());

        if (technique->GetVisualParticleQuota() != DiParticleElement::DEFAULT_VISUAL_PARTICLE_QUOTA)
            node.SetInt("visualParticleQuota", technique->GetVisualParticleQuota());

        if (technique->GetEmittedEmitterQuota() != DiParticleElement::DEFAULT_EMITTED_EMITTER_QUOTA)
            node.SetInt("emittedEmitterQuota", technique->GetEmittedEmitterQuota());

        if (technique->GetEmittedElementQuota() != DiParticleElement::DEFAULT_EMITTED_TECHNIQUE_QUOTA)
            node.SetInt("emittedElementQuota", technique->GetEmittedElementQuota());

        if (technique->GetEmittedControllerQuota() != DiParticleElement::DEFAULT_EMITTED_AFFECTOR_QUOTA)
            node.SetInt("emittedControllerQuota", technique->GetEmittedControllerQuota());

        if (technique->GetEmittedSystemQuota() != DiParticleElement::DEFAULT_EMITTED_SYSTEM_QUOTA)
            node.SetInt("emittedSystemQuota", technique->GetEmittedSystemQuota());

        if (technique->GetLodIndex() != DiParticleElement::DEFAULT_LOD_INDEX) 
            node.SetInt("lodIndex", technique->GetLodIndex());

        if (technique->GetDefaultWidth() != DiParticleElement::DEFAULT_WIDTH) 
            node.SetFloat("defaultWidth", technique->GetDefaultWidth());
        
        if (technique->GetDefaultHeight() != DiParticleElement::DEFAULT_HEIGHT) 
            node.SetFloat("defaultHeight", technique->GetDefaultHeight());
        
        if (technique->GetDefaultDepth() != DiParticleElement::DEFAULT_DEPTH) 
            node.SetFloat("defaultDepth", technique->GetDefaultDepth());
        
        if (technique->GetMaxVelocity() != DiParticleElement::DEFAULT_MAX_VELOCITY) 
            node.SetFloat("maxVelocity", technique->GetMaxVelocity());

        // write material
        DI_ASSERT(technique->GetMaterial());
        auto matNode = node.CreateChild("Material");
        DiMaterialSerializer matseral;
        matseral.SaveMaterial(matNode, technique->GetMaterial().get());

        // write renderer
        DiParticleRenderer* renderer = technique->GetRenderer();
        if (renderer->GetRendererType() == "Billboard")
        {
            auto rendererNode = node.CreateChild("Renderer");
            rendererNode.SetAttribute("type", "Billboard");
            WriteBillboardRenderer((DiBillboardRenderer*)renderer, rendererNode);
        }
        else
        {
            DI_ERROR("No suitable renderer founded!");
        }

        // write emitters
        size_t numEmitters = technique->GetNumEmitters();
        for (size_t i = 0; i < numEmitters; ++i)
        {
            auto emitter = technique->GetEmitter(i);
            auto type = emitter->GetEmitterType();
            auto emitterNode = node.CreateChild("Emitter");

            if (type == "Point")
            {
                emitterNode.SetAttribute("type", "Point");
                WriteBaseEmitter(emitter, emitterNode);
            }
            else if (type == "Box")
            {
                emitterNode.SetAttribute("type", "Box");
                WriteBoxEmitter(emitter, emitterNode);
            }
            else if (type == "Circle")
            {
                emitterNode.SetAttribute("type", "Circle");
                WriteCircleEmitter(emitter, emitterNode);
            }
            else if (type == "Line")
            {
                emitterNode.SetAttribute("type", "Line");
                WriteLineEmitter(emitter, emitterNode);
            }
            else
            {
                DI_ERROR("Invalid emitter type!");
            }
        }

        // write controllers
        size_t numCtrls = technique->GetNumControllers();
        for (size_t i = 0; i < numCtrls; ++i)
        {
            auto controller = technique->GetController(i);
            auto type = controller->GetControllerType();
            auto controllerNode = node.CreateChild("Controller");

            if (type == "Scale")
            {
                controllerNode.SetAttribute("type", "Scale");
                WriteScaleController(controller, controllerNode);
            }
            else if (type == "Color")
            {
                controllerNode.SetAttribute("type", "Color");
                WriteColorController(controller, controllerNode);
            }
            else if (type == "TextureRotator")
            {
                controllerNode.SetAttribute("type", "TextureRotator");
                WriteTextureRotatorController(controller, controllerNode);
            }
            else if (type == "GeometryRotator")
            {
                controllerNode.SetAttribute("type", "GeometryRotator");
                WriteGeometryRotatorController(controller, controllerNode);
            }
            else if (type == "Gravity")
            {
                controllerNode.SetAttribute("type", "Gravity");
                WriteGravityController(controller, controllerNode);
            }
            else if (type == "Jet")
            {
                controllerNode.SetAttribute("type", "Jet");
                WriteJetController(controller, controllerNode);
            }
            else if (type == "Randomiser")
            {
                controllerNode.SetAttribute("type", "Randomiser");
                WriteRandomiserController(controller, controllerNode);
            }
            else if (type == "Vortex")
            {
                controllerNode.SetAttribute("type", "Vortex");
                WriteVortexController(controller, controllerNode);
            }
            else if (type == "LinearForce")
            {
                controllerNode.SetAttribute("type", "LinearForce");
                WriteLinearForceController(controller, controllerNode);
            }
            else if (type == "SineForce")
            {
                controllerNode.SetAttribute("type", "SineForce");
                WriteSinForceController(controller, controllerNode);
            }
            else if (type == "PlaneCollider")
            {
                controllerNode.SetAttribute("type", "PlaneCollider");
                WritePlaneColliderController(controller, controllerNode);
            }
            else if (type == "BoxCollider")
            {
                controllerNode.SetAttribute("type", "BoxCollider");
                WriteBoxColliderController(controller, controllerNode);
            }
            else if (type == "SphereCollider")
            {
                controllerNode.SetAttribute("type", "SphereCollider");
                WriteSphereColliderController(controller, controllerNode);
            }
            else
            {
                DI_ERROR("Invalid controller type!");
            }
        }
    }

    DiParticleElement* DiFxTokensParser::ReadElement(DiParticleSystem* ps, DiXMLElement& node)
    {
        auto element = ps->CreateElement();

        node.IterateAttributes([element](const DiString& name, const DiString& value){
            if (name == "name")
                element->SetName(value);
            else if (name == "originalEnabled")
                element->SetOriginalEnabled(value.AsBool());
            else if (name == "position")
                element->position = value.AsVector3();
            else if (name == "keepLocal")
                element->SetKeepLocal(value.AsBool());
            else if (name == "visualParticleQuota")
                element->SetVisualParticleQuota(value.AsInt());
            else if (name == "emittedEmitterQuota")
                element->SetEmittedEmitterQuota(value.AsInt());
            else if (name == "emittedElementQuota")
                element->SetEmittedElementQuota(value.AsInt());
            else if (name == "emittedControllerQuota")
                element->SetEmittedControllerQuota(value.AsInt());
            else if (name == "emittedSystemQuota")
                element->SetEmittedSystemQuota(value.AsInt());
            else if (name == "lodIndex")
                element->SetLodIndex(value.AsInt());
            else if (name == "defaultWidth")
                element->SetDefaultWidth(value.AsFloat());
            else if (name == "defaultHeight")
                element->SetDefaultHeight(value.AsFloat());
            else if (name == "defaultDepth")
                element->SetDefaultDepth(value.AsFloat());
            else if (name == "maxVelocity")
                element->SetMaxVelocity(value.AsFloat());
        });

        auto child = node.GetChild();
        while (child)
        {
            if (child.CheckName("Renderer"))
                ReadRenderer(element, child);
            else if (child.CheckName("Emitter"))
                ReadEmitter(element, child);
            else if (child.CheckName("Controller"))
                ReadController(element, child);
            else if (child.CheckName("Material"))
            {
                // read material
                DiMaterialSerializer matseral;
                static int elementid = 0;
                DiString name;
                name.Format("_psmat%d", elementid++);
                DiMaterialPtr mat = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiMaterial>(name);
                matseral.ParseMaterial(child, mat.get());
                element->SetMaterialName(name);
            }

            child = child.GetNext();
        }


        return element;
    }


    DiVector<DiParticleSystem*> DiFxTokensParser::LoadEffects(const DiString& file)
    {
        DiVector<DiParticleSystem*> ret;

        auto stream = DiAssetManager::GetInstance().OpenArchive(file);
        shared_ptr<DiXMLFile> xmlfile(new DiXMLFile());
        xmlfile->Load(stream->GetAsString());
        DiXMLElement root = xmlfile->GetRoot();

        if (!root.CheckName("Effects"))
        {
            DI_WARNING("Bad effect file: %s", file.c_str());
        }

        auto child = root.GetChild();
        while (child)
        {
            if (child.CheckName("ParticleSystem"))
                ret.push_back(ReadSystem(child));

            child = child.GetNext();
        }

        return ret;
    }

    void DiFxTokensParser::WriteSystem(DiParticleSystem* system, DiXMLElement& node)
    {
        auto name = system->GetTemplateName();
        if (name.empty())
            name = system->GetName();
        node.SetAttribute("name", name);

        if (system->IsKeepLocal() != DiParticleSystem::DEFAULT_KEEP_LOCAL)
            node.SetBool("keepLocal", system->IsKeepLocal());

        if (system->GetFixedTimeout() != DiParticleSystem::DEFAULT_FIXED_TIMEOUT)
            node.SetFloat("fixedTimeout", system->GetFixedTimeout());
        
        if (system->GetIterationInterval() != DiParticleSystem::DEFAULT_ITERATION_INTERVAL)
            node.SetFloat("iterationInterval", system->GetIterationInterval());
        
        if (system->GetNonVisibleUpdateTimeout() != DiParticleSystem::DEFAULT_NON_VISIBLE_UPDATE_TIMEOUT)
            node.SetFloat("nonVisibleUpdateTimeout", system->GetNonVisibleUpdateTimeout());
        
        if (system->IsSmoothLod() != DiParticleSystem::DEFAULT_SMOOTH_LOD)
            node.SetFloat("isSmoothLod", system->IsSmoothLod());

        if (system->GetFastForwardTime() != DiParticleSystem::DEFAULT_FAST_FORWARD_TIME ||
            system->GetFastForwardInterval() != 0.0f)
        {
            DiString ret;
            ret.Format("%f,%f", system->GetFastForwardTime(), system->GetFastForwardInterval());
            node.SetAttribute("fastForward", ret);
        }

        if (system->GetScaleVelocity() != DiParticleSystem::DEFAULT_SCALE_VELOCITY)
            node.SetFloat("scaleVelocity", system->GetScaleVelocity());
        
        if (system->GetScaleTime() != DiParticleSystem::DEFAULT_SCALE_TIME)
            node.SetFloat("scaleTime", system->GetScaleTime());
        
        if (system->GetScale() != DiParticleSystem::DEFAULT_SCALE)
            node.SetVector3("scale", system->GetScale());
        
        if (system->HasTightBoundingBox() != DiParticleSystem::DEFAULT_TIGHT_BOUNDINGBOX)
            node.SetBool("hasTightBoundingBox", system->HasTightBoundingBox());
        
        // Write LOD distances
        if (!(system->GetLodDistances()).empty())
        {
            DI_WARNING("lod distance hasn't been supported yet~");
        }

        // elements
        size_t elements = system->GetNumElements();
        for (size_t i = 0; i < elements; ++i)
        {
            auto ele = system->GetElement(i);
            auto nd = node.CreateChild("Element");
            WriteElement(ele, nd);
        }
    }

    void DiFxTokensParser::WriteSystem(DiParticleSystem* val, const DiString& filePath)
    {
        shared_ptr<DiXMLFile> xmlfile(new DiXMLFile());
        DiXMLElement root = xmlfile->CreateRoot("Effects");
        auto nd = root.CreateChild("ParticleSystem");
        WriteSystem(val, nd);
        xmlfile->Save(filePath);
    }

    DiParticleSystem* DiFxTokensParser::ReadSystem(DiXMLElement& node)
    {
        DiString name = node.GetAttribute("name");
        if (name.empty())
        {
            DI_WARNING("The name of the particle system cannot be empty!");
            return nullptr;
        }

        auto ps = DiEffectManager::GetInstance().GetParticleSystemTemplate(name);
        if (ps)
        {
            DI_WARNING("The particle system: %s has already existed!", name.c_str());
            return ps;
        }

        // create a new one
        ps = DiEffectManager::GetInstance().CreateParticleSystemTemplate(name);
        DI_LOG("Creating particle system template: %s", name.c_str());

        node.IterateAttributes([ps](const DiString& name, const DiString& value){
            if (name == "keepLocal")
                ps->SetKeepLocal(value.AsBool());
            else if (name == "fixedTimeout")
                ps->SetFixedTimeout(value.AsFloat());
            else if (name == "iterationInterval")
                ps->SetIterationInterval(value.AsFloat());
            else if (name == "nonVisibleUpdateTimeout")
                ps->SetNonVisibleUpdateTimeout(value.AsFloat());
            else if (name == "isSmoothLod")
                ps->SetSmoothLod(value.AsFloat());
            else if (name == "fastForward")
            {
                auto v = value.AsVector2();
                ps->SetFastForward(v.x, v.y);
            }
            else if (name == "scaleVelocity")
                ps->SetScaleVelocity(value.AsFloat());
            else if (name == "scaleTime")
                ps->SetScaleTime(value.AsFloat());
            else if (name == "scale")
                ps->SetScale(value.AsVector3());
            else if (name == "hasTightBoundingBox")
                ps->SetTightBoundingBox(value.AsBool());
        });

        auto child = node.GetChild();
        while (child)
        {
            if (child.CheckName("Element"))
                ReadElement(ps, child);
            child = child.GetNext();
        }

        return ps;
    }

    void DiFxTokensParser::WriteBaseEmitter(DiParticleEmitter* emitter, DiXMLElement& node)
    {
        node.SetAttribute("name", emitter->GetName());

        DiString att;
        if (emitter->GetDynEmissionRate()->GetType() != DiDynamicAttribute::DAT_FIXED ||
            emitter->GetDynEmissionRate()->GetValue() != DiParticleEmitter::DEFAULT_EMISSION_RATE)
        {
            emitter->GetDynEmissionRate()->Write(att);
            node.SetAttribute("emissionRate", att);
        }

        if (emitter->GetDynAngle()->GetType() != DiDynamicAttribute::DAT_FIXED ||
            emitter->GetDynAngle()->GetValue() != DiParticleEmitter::DEFAULT_ANGLE)
        {
            emitter->GetDynAngle()->Write(att);
            node.SetAttribute("angle", att);
        }

        if (emitter->GetDynTotalTimeToLive()->GetType() != DiDynamicAttribute::DAT_FIXED ||
            emitter->GetDynTotalTimeToLive()->GetValue() != DiParticleEmitter::DEFAULT_TIME_TO_LIVE)
        {
            emitter->GetDynTotalTimeToLive()->Write(att);
            node.SetAttribute("totalTimeToLive", att);
        }

        if (emitter->GetDynParticleMass()->GetType() != DiDynamicAttribute::DAT_FIXED ||
            emitter->GetDynParticleMass()->GetValue() != DiParticleEmitter::DEFAULT_MASS)
        {
            emitter->GetDynParticleMass()->Write(att);
            node.SetAttribute("mass", att);
        }

        if (emitter->GetDynVelocity()->GetType() != DiDynamicAttribute::DAT_FIXED ||
            emitter->GetDynVelocity()->GetValue() != DiParticleEmitter::DEFAULT_VELOCITY)
        {
            emitter->GetDynVelocity()->Write(att);
            node.SetAttribute("velocity", att);
        }

        if (emitter->GetDynDuration()->GetType() != DiDynamicAttribute::DAT_FIXED ||
            emitter->GetDynDuration()->GetValue() != DiParticleEmitter::DEFAULT_DURATION)
        {
            emitter->GetDynDuration()->Write(att);
            node.SetAttribute("duration", att);
        }

        if (emitter->GetDynRepeatDelay()->GetType() != DiDynamicAttribute::DAT_FIXED ||
            emitter->GetDynRepeatDelay()->GetValue() != DiParticleEmitter::DEFAULT_REPEAT_DELAY)
        {
            emitter->GetDynRepeatDelay()->Write(att);
            node.SetAttribute("repeatDelay", att);
        }

        if (emitter->GetDynParticleAllDimensions()->GetType() != DiDynamicAttribute::DAT_FIXED ||
            emitter->GetDynParticleAllDimensions()->GetValue() != DiParticleEmitter::DEFAULT_DIMENSIONS)
        {
            emitter->GetDynParticleAllDimensions()->Write(att);
            node.SetAttribute("allDimensions", att);
        }

        if (emitter->GetDynParticleWidth()->GetType() != DiDynamicAttribute::DAT_FIXED ||
            emitter->GetDynParticleWidth()->GetValue() != DiParticleEmitter::DEFAULT_WIDTH)
        {
            emitter->GetDynParticleWidth()->Write(att);
            node.SetAttribute("width", att);
        }

        if (emitter->GetDynParticleHeight()->GetType() != DiDynamicAttribute::DAT_FIXED ||
            emitter->GetDynParticleHeight()->GetValue() != DiParticleEmitter::DEFAULT_HEIGHT)
        {
            emitter->GetDynParticleHeight()->Write(att);
            node.SetAttribute("height", att);
        }

        if (emitter->GetDynParticleDepth()->GetType() != DiDynamicAttribute::DAT_FIXED ||
            emitter->GetDynParticleDepth()->GetValue() != DiParticleEmitter::DEFAULT_DEPTH)
        {
            emitter->GetDynParticleDepth()->Write(att);
            node.SetAttribute("depth", att);
        }

        if (emitter->GetOriginalEnabled() != DiParticleEmitter::DEFAULT_ENABLED)
            node.SetBool("originalEnabled", emitter->GetOriginalEnabled());

        if (emitter->position != DiParticleEmitter::DEFAULT_POSITION)
            node.SetVector3("position", emitter->position);

        if (emitter->IsKeepLocal() != DiParticleEmitter::DEFAULT_KEEP_LOCAL)
            node.SetBool("keepLocal", emitter->IsKeepLocal());

        if (emitter->GetOriginalParticleDirection() != DiParticleEmitter::DEFAULT_DIRECTION)
            node.SetVector3("originalParticleDirection", emitter->GetOriginalParticleDirection());

        if (emitter->GetParticleOrientation() != DiParticleEmitter::DEFAULT_ORIENTATION)
            node.SetQuaternion("orientation", emitter->GetParticleOrientation());

        if (emitter->GetEmitsType() != DiParticleEmitter::DEFAULT_EMITS)
        {
            DiString particle = "particle";
            node.SetAttribute("emitsType", particle);
        }

        if (emitter->GetParticleTextureCoords() != DiParticleEmitter::DEFAULT_TEXTURE_COORDS)
            node.SetInt("particleTextureCoords", emitter->GetParticleTextureCoords());

        if (emitter->GetParticleColour() != DiParticleEmitter::DEFAULT_COLOUR)
            node.SetColor("color", emitter->GetParticleColour());

        if (emitter->IsAutoDirection() != DiParticleEmitter::DEFAULT_AUTO_DIRECTION)
            node.SetBool("autoDirection", emitter->IsAutoDirection());

        if (emitter->IsForceEmission() != DiParticleEmitter::DEFAULT_FORCE_EMISSION)
            node.SetBool("forceEmission", emitter->IsForceEmission());
    }

    void DiFxTokensParser::ReadBaseEmitter(DiParticleEmitter* emitter, DiXMLElement& node)
    {
        node.IterateAttributes([emitter](const DiString& name, const DiString& value){
            if (name == "name")
                emitter->SetName(value);
            else if (name == "emissionRate")
                emitter->SetDynEmissionRate(DiDynamicAttribute::Read(value));
            else if (name == "angle")
                emitter->SetDynAngle(DiDynamicAttribute::Read(value));
            else if (name == "totalTimeToLive")
                emitter->SetDynTotalTimeToLive(DiDynamicAttribute::Read(value));
            else if (name == "mass")
                emitter->SetDynParticleMass(DiDynamicAttribute::Read(value));
            else if (name == "velocity")
                emitter->SetDynVelocity(DiDynamicAttribute::Read(value));
            else if (name == "duration")
                emitter->SetDynDuration(DiDynamicAttribute::Read(value));
            else if (name == "repeatDelay")
                emitter->SetDynRepeatDelay(DiDynamicAttribute::Read(value));
            else if (name == "allDimensions")
                emitter->SetDynParticleAllDimensions(DiDynamicAttribute::Read(value));
            else if (name == "width")
                emitter->SetDynParticleWidth(DiDynamicAttribute::Read(value));
            else if (name == "height")
                emitter->SetDynParticleHeight(DiDynamicAttribute::Read(value));
            else if (name == "depth")
                emitter->SetDynParticleDepth(DiDynamicAttribute::Read(value));
            else if (name == "originalEnabled")
                emitter->SetOriginalEnabled(value.AsBool());
            else if (name == "position")
                emitter->position = value.AsVector3();
            else if (name == "keepLocal")
                emitter->SetKeepLocal(value.AsBool());
            else if (name == "originalParticleDirection")
                emitter->SetParticleDirection(value.AsVector3());
            else if (name == "orientation")
                emitter->SetParticleOrientation(value.AsQuaternion());
            else if (name == "emitsType")
            {
                DiParticle::ParticleType type = DiParticleEmitter::DEFAULT_EMITS;
                emitter->SetEmitsType(type);
            }
            else if (name == "particleTextureCoords")
                emitter->SetParticleTextureCoords(value.AsInt());
            else if (name == "color")
                emitter->SetParticleColour(value.AsColourValue());
            else if (name == "autoDirection")
                emitter->SetAutoDirection(value.AsBool());
            else if (name == "forceEmission")
                emitter->SetForceEmission(value.AsBool());
        });
    }

    void DiFxTokensParser::WriteCircleEmitter(DiParticleEmitter* val, DiXMLElement& node)
    {
        DiCircleEmitter* emitter = static_cast<DiCircleEmitter*>(val);

        WriteBaseEmitter(emitter, node);

        if (emitter->GetRadius() != DiCircleEmitter::DEFAULT_RADIUS) 
            node.SetFloat("radius", emitter->GetRadius());

        if (emitter->GetStep() != DiCircleEmitter::DEFAULT_STEP)
            node.SetFloat("step", emitter->GetStep());
        
        if (emitter->GetCircleAngle() != DiCircleEmitter::DEFAULT_ANGLE)
            node.SetFloat("circleAngle", emitter->GetCircleAngle());
        
        if (emitter->IsRandom() != DiCircleEmitter::DEFAULT_RANDOM)
            node.SetFloat("isRandom", emitter->IsRandom());
        
        if (emitter->GetNormal() != DiCircleEmitter::DEFAULT_NORMAL)
            node.SetVector3("normal", emitter->GetNormal());
    }

    void DiFxTokensParser::ReadCircleEmitter(DiParticleEmitter* val, DiXMLElement& node)
    {
        ReadBaseEmitter(val, node);

        DiCircleEmitter* emitter = static_cast<DiCircleEmitter*>(val);

        node.IterateAttributes([emitter](const DiString& name, const DiString& value){
            if (name == "radius")
                emitter->SetRadius(value.AsFloat());
            else if (name == "step")
                emitter->SetStep(value.AsFloat());
            else if (name == "circleAngle")
                emitter->SetCircleAngle(value.AsFloat());
            else if (name == "isRandom")
                emitter->SetRandom(value.AsFloat());
            else if (name == "normal")
                emitter->SetNormal(value.AsVector3());
        });
    }

    void DiFxTokensParser::WriteBoxEmitter(DiParticleEmitter* val, DiXMLElement& node)
    {
        DiBoxEmitter* emitter = static_cast<DiBoxEmitter*>(val);

        WriteBaseEmitter(emitter, node);

        if (emitter->GetWidth() != DiBoxEmitter::DEFAULT_WIDTH)
            node.SetFloat("boxWidth", emitter->GetWidth());
        
        if (emitter->GetHeight() != DiBoxEmitter::DEFAULT_HEIGHT)
            node.SetFloat("boxHeight", emitter->GetHeight());

        if (emitter->GetDepth() != DiBoxEmitter::DEFAULT_DEPTH)
            node.SetFloat("boxDepth", emitter->GetDepth());
    }

    void DiFxTokensParser::ReadBoxEmitter(DiParticleEmitter* val, DiXMLElement& node)
    {
        ReadBaseEmitter(val, node);

        DiBoxEmitter* emitter = static_cast<DiBoxEmitter*>(val);

        node.IterateAttributes([emitter](const DiString& name, const DiString& value){
            if (name == "boxWidth")
                emitter->SetWidth(value.AsFloat());
            else if (name == "boxHeight")
                emitter->SetHeight(value.AsFloat());
            else if (name == "boxDepth")
                emitter->SetDepth(value.AsFloat());
        });
    }

    void DiFxTokensParser::WriteLineEmitter(DiParticleEmitter* val, DiXMLElement& node)
    {
        DiLineEmitter* emitter = static_cast<DiLineEmitter*>(val);

        WriteBaseEmitter(emitter, node);

        if (emitter->GetEnd() != DiLineEmitter::DEFAULT_END)
            node.SetVector3("end", emitter->GetEnd());
        
        if (emitter->GetMinIncrement() != DiLineEmitter::DEFAULT_MIN_INCREMENT)
            node.SetFloat("minIncrement", emitter->GetMinIncrement());
        
        if (emitter->GetMaxIncrement() != DiLineEmitter::DEFAULT_MAX_INCREMENT)
            node.SetFloat("maxIncrement", emitter->GetMaxIncrement());

        if (emitter->GetMaxDeviation() != DiLineEmitter::DEFAULT_MAX_DEVIATION)
            node.SetFloat("maxDeviation", emitter->GetMaxDeviation());
    }

    void DiFxTokensParser::ReadLineEmitter(DiParticleEmitter* val, DiXMLElement& node)
    {
        ReadBaseEmitter(val, node);

        DiLineEmitter* emitter = static_cast<DiLineEmitter*>(val);

        node.IterateAttributes([emitter](const DiString& name, const DiString& value){
            if (name == "end")
                emitter->SetEnd(value.AsVector3());
            else if (name == "minIncrement")
                emitter->SetMinIncrement(value.AsFloat());
            else if (name == "maxIncrement")
                emitter->SetMinIncrement(value.AsFloat());
            else if (name == "maxDeviation")
                emitter->SetMaxDeviation(value.AsFloat());
        });
    }

    void DiFxTokensParser::WriteBaseForceController(DiParticleController* val, DiXMLElement& node)
    {
        DiBaseForceController* controller = static_cast<DiBaseForceController*>(val);
        WriteBaseController(controller, node);

        if (controller->GetForceVector() != DiBaseForceController::DEFAULT_FORCE_VECTOR)
            node.SetVector3("vector", controller->GetForceVector());

        if (controller->GetForceApplication() != DiBaseForceController::DEFAULT_FORCE_APPL)
        {
            DiString application = "add";
            if (controller->GetForceApplication() == DiBaseForceController::FA_AVERAGE)
                application = "average";

            node.SetAttribute("application", application);
        }
    }

    void DiFxTokensParser::ReadBaseForceController(DiParticleController* val, DiXMLElement& node)
    {
        DiBaseForceController* ctrl = static_cast<DiBaseForceController*>(val);

        ReadBaseController(ctrl, node);

        node.IterateAttributes([&](const DiString& name, const DiString& value) {
            if (name == "vector")
                ctrl->SetForceVector(value.AsVector3());
            else if (name == "application")
            {
                auto t = DiBaseForceController::FA_ADD;
                if (value == "average")
                    t = DiBaseForceController::FA_AVERAGE;
                ctrl->SetForceApplication(t);
            }
        });
    }

    void DiFxTokensParser::WriteLinearForceController(DiParticleController* val, DiXMLElement& node)
    {
        WriteBaseForceController(val, node);
    }

    void DiFxTokensParser::ReadLinearForceController(DiParticleController* val, DiXMLElement& node)
    {
        ReadBaseForceController(val, node);
    }

    void DiFxTokensParser::WriteSinForceController(DiParticleController* val, DiXMLElement& node)
    {
        DiSineForceController* controller = static_cast<DiSineForceController*>(val);

        WriteBaseForceController(controller, node);
        
        if (controller->GetFrequencyMin() != DiSineForceController::DEFAULT_FREQ_MIN)
            node.SetFloat("freqMin", controller->GetFrequencyMin());

        if (controller->GetFrequencyMax() != DiSineForceController::DEFAULT_FREQ_MAX)
            node.SetFloat("freqMax", controller->GetFrequencyMax());
    }

    void DiFxTokensParser::ReadSinForceController(DiParticleController* val, DiXMLElement& node)
    {
        DiSineForceController* controller = static_cast<DiSineForceController*>(val);

        ReadBaseForceController(controller, node);

        node.IterateAttributes([controller](const DiString& name, const DiString& value) {
            if (name == "freqMin")
                controller->SetFrequencyMin(value.AsFloat());
            else if (name == "freqMax")
                controller->SetFrequencyMax(value.AsFloat());
        });
    }

    void DiFxTokensParser::WriteBaseController(DiParticleController* affector, DiXMLElement& node)
    {
        node.SetAttribute("name", affector->GetName());

        // Write base attributes
        if (affector->GetOriginalEnabled() != DiParticleController::DEFAULT_ENABLED)
            node.SetBool("originalEnabled", affector->GetOriginalEnabled());

        if (affector->position != DiParticleController::DEFAULT_POSITION)
            node.SetVector3("position", affector->position);

        if (affector->mass != DiParticleController::DEFAULT_MASS)
            node.SetFloat("mass", affector->mass);

        DiString affectSpecialisation = "default";

        if (affector->GetControllerSpecialisation() == DiParticleController::AFSP_TTL_INCREASE)
        {
            affectSpecialisation = "ttlIncrease";
        }
        else if (affector->GetControllerSpecialisation() == DiParticleController::AFSP_TTL_DECREASE)
        {
            affectSpecialisation = "ttlDecrease";
        }

        if (affector->GetControllerSpecialisation() != DiParticleController::DEFAULT_SPECIALISATION)
            node.SetAttribute("specialisation", affectSpecialisation);

        const auto& excludedEmitters = affector->GetEmittersToExclude();

        if (!excludedEmitters.empty())
        {
            DiString emitterList;
            for (auto it = excludedEmitters.begin(); it != excludedEmitters.end(); ++it)
            {
                if (it != excludedEmitters.begin())
                    emitterList += ",";

                emitterList += *it;
            }

            node.SetAttribute("excludedEmitters", emitterList);
        }
    }

    DiParticleController* DiFxTokensParser::ReadController(DiParticleElement* ele, DiXMLElement& controllerNode)
    {
        DiString type = controllerNode.GetAttribute("type");
        DiParticleController* controller = ele->CreateController(type);

        if (type == "Scale")
            ReadScaleController(controller, controllerNode);
        else if (type == "Color")
            ReadColorController(controller, controllerNode);
        else if (type == "TextureRotator")
            ReadTextureRotatorController(controller, controllerNode);
        else if (type == "GeometryRotator")
            ReadGeometryRotatorController(controller, controllerNode);
        else if (type == "Gravity")
            ReadGravityController(controller, controllerNode);
        else if (type == "Jet")
            ReadJetController(controller, controllerNode);
        else if (type == "Randomiser")
            ReadRandomiserController(controller, controllerNode);
        else if (type == "Vortex")
            ReadVortexController(controller, controllerNode);
        else if (type == "LinearForce")
            ReadLinearForceController(controller, controllerNode);
        else if (type == "SineForce")
            ReadSinForceController(controller, controllerNode);
        else if (type == "PlaneCollider")
            ReadPlaneColliderController(controller, controllerNode);
        else if (type == "BoxCollider")
            ReadBoxColliderController(controller, controllerNode);
        else if (type == "SphereCollider")
            ReadSphereColliderController(controller, controllerNode);
        else
        {
            DI_ERROR("Invalid controller type!");
        }

        return controller;
    }

    void DiFxTokensParser::WriteGravityController(DiParticleController* val, DiXMLElement& node)
    {
        DiGravityController* controller = static_cast<DiGravityController*>(val);

        WriteBaseController(controller, node);

        if (controller->GetGravity() != DiGravityController::DEFAULT_GRAVITY)
            node.SetFloat("gravity", controller->GetGravity());
    }

    void DiFxTokensParser::ReadGravityController(DiParticleController* val, DiXMLElement& node)
    {
        DiGravityController* ctrl = static_cast<DiGravityController*>(val);

        ReadBaseController(ctrl, node);

        node.IterateAttributes([&](const DiString& name, const DiString& value) {
            if (name == "gravity")
                ctrl->SetGravity(value.AsFloat());
        });
    }

    void DiFxTokensParser::WriteJetController(DiParticleController* val, DiXMLElement& node)
    {
        DiJetController* controller = static_cast<DiJetController*>(val);

        WriteBaseController(controller, node);

        // Write own attributes
        DiDynamicAttributeFactory dynamicAttributeFactory;
        if (dynamicAttributeFactory.GetDefaultValue(controller->GetDynAcceleration()) != DiJetController::DEFAULT_ACCELERATION)
        {
            DiString att;
            controller->GetDynAcceleration()->Write(att);
            node.SetAttribute("acceleration", att);
        }
    }

    void DiFxTokensParser::ReadJetController(DiParticleController* val, DiXMLElement& node)
    {
        DiJetController* ctrl = static_cast<DiJetController*>(val);

        ReadBaseController(ctrl, node);

        node.IterateAttributes([&](const DiString& name, const DiString& value) {
            if (name == "acceleration")
                ctrl->SetDynAcceleration(DiDynamicAttribute::Read(value));
        });
    }

    void DiFxTokensParser::WriteRandomiserController(DiParticleController* val, DiXMLElement& node)
    {
        DiRandomiserController* affector = static_cast<DiRandomiserController*>(val);

        WriteBaseController(affector, node);

        if (affector->GetMaxDeviationX() != DiRandomiserController::DEFAULT_MAX_DEVIATION.x)
            node.SetFloat("maxDeviationX", affector->GetMaxDeviationX());

        if (affector->GetMaxDeviationY() != DiRandomiserController::DEFAULT_MAX_DEVIATION.y)
            node.SetFloat("maxDeviationY", affector->GetMaxDeviationY());
        
        if (affector->GetMaxDeviationZ() != DiRandomiserController::DEFAULT_MAX_DEVIATION.z)
            node.SetFloat("maxDeviationZ", affector->GetMaxDeviationZ());
       
        if (affector->GetTimeStep() != DiRandomiserController::DEFAULT_TIME_STEP)
            node.SetFloat("timeStep", affector->GetTimeStep());
       
        if (affector->IsRandomDirection() != DiRandomiserController::DEFAULT_RANDOM_DIRECTION)
            node.SetBool("randomDirection", affector->IsRandomDirection());
    }

    void DiFxTokensParser::ReadRandomiserController(DiParticleController* val, DiXMLElement& node)
    {
        DiRandomiserController* ctrl = static_cast<DiRandomiserController*>(val);

        ReadBaseController(ctrl, node);

        node.IterateAttributes([&](const DiString& name, const DiString& value) {
            if (name == "maxDeviationX")
                ctrl->SetMaxDeviationX(value.AsFloat());
            else if (name == "maxDeviationY")
                ctrl->SetMaxDeviationY(value.AsFloat());
            else if (name == "maxDeviationZ")
                ctrl->SetMaxDeviationZ(value.AsFloat());
            else if (name == "timeStep")
                ctrl->SetTimeStep(value.AsFloat());
            else if (name == "randomDirection")
                ctrl->SetRandomDirection(value.AsBool());
        });
    }

    void DiFxTokensParser::WriteVortexController(DiParticleController* val, DiXMLElement& node)
    {
        DiVortexController* affector = static_cast<DiVortexController*>(val);

        WriteBaseController(affector, node);

        if (affector->GetRotationVector() != DiVortexController::DEFAULT_ROTATION_VECTOR)
            node.SetVector3("rotationVector", affector->GetRotationVector());

        DiDynamicAttributeFactory dynamicAttributeFactory;
        if (dynamicAttributeFactory.GetDefaultValue(affector->GetRotationSpeed()) != DiVortexController::DEFAULT_ROTATION_SPEED)
        {
            DiString att;
            affector->GetRotationSpeed()->Write(att);
            node.SetAttribute("rotationSpeed", att);
        }
    }

    void DiFxTokensParser::ReadVortexController(DiParticleController* val, DiXMLElement& node)
    {
        DiVortexController* ctrl = static_cast<DiVortexController*>(val);

        ReadBaseController(ctrl, node);

        node.IterateAttributes([&](const DiString& name, const DiString& value) {
            if (name == "rotationVector")
                ctrl->SetRotationVector(value.AsVector3());
            else if (name == "rotationSpeed")
                ctrl->SetRotationSpeed(DiDynamicAttribute::Read(value));
        });
    }

    void DiFxTokensParser::WriteTextureRotatorController(DiParticleController* val, DiXMLElement& node)
    {
        DiTextureRotatorController* affector = static_cast<DiTextureRotatorController*>(val);

        WriteBaseController(affector, node);

        if (affector->UseOwnRotationSpeed() != DiTextureRotatorController::DEFAULT_USE_OWN_SPEED)
            node.SetBool("ownRotationSpeed", affector->UseOwnRotationSpeed());

        DiDynamicAttributeFactory dynamicAttributeFactory;
        if (dynamicAttributeFactory.GetDefaultValue(affector->GetRotation()) != DiTextureRotatorController::DEFAULT_ROTATION)
        {
            DiString att;
            affector->GetRotation()->Write(att);
            node.SetAttribute("rotation", att);
        }
        if (dynamicAttributeFactory.GetDefaultValue(affector->GetRotationSpeed()) != DiTextureRotatorController::DEFAULT_ROTATION_SPEED)
        {
            DiString att;
            affector->GetRotationSpeed()->Write(att);
            node.SetAttribute("rotationSpeed", att);
        }
    }

    void DiFxTokensParser::ReadTextureRotatorController(DiParticleController* val, DiXMLElement& node)
    {
        DiTextureRotatorController* ctrl = static_cast<DiTextureRotatorController*>(val);

        ReadBaseController(ctrl, node);

        node.IterateAttributes([&](const DiString& name, const DiString& value) {
            if (name == "ownRotationSpeed")
                ctrl->SetUseOwnRotationSpeed(value.AsBool());
            else if (name == "rotation")
                ctrl->SetRotation(DiDynamicAttribute::Read(value));
            else if (name == "rotationSpeed")
                ctrl->SetRotationSpeed(DiDynamicAttribute::Read(value));
        });
    }

    void DiFxTokensParser::WriteBaseColliderController(DiParticleController* val, DiXMLElement& node)
    {
        DiBaseColliderController* affector = static_cast<DiBaseColliderController*>(val);

        WriteBaseController(affector, node);

        // Write own attributes
        if (affector->GetFriction() != DiBaseColliderController::DEFAULT_FRICTION)
            node.SetFloat("friction", affector->GetFriction());

        if (affector->GetBouncyness() != DiBaseColliderController::DEFAULT_BOUNCYNESS)
            node.SetFloat("bouncyness", affector->GetBouncyness());
        
        if (affector->GetIntersectionType() != DiBaseColliderController::DEFAULT_INTERSECTION_TYPE)
        {
            DiString intersectionType = "point";
            if (affector->GetIntersectionType() == DiBaseColliderController::IT_BOX)
                intersectionType = "box";
            node.SetAttribute("intersectionType", intersectionType);
        }

        if (affector->GetCollisionType() != DiBaseColliderController::DEFAULT_COLLISION_TYPE)
        {
            DiString collisionType = "bounce";
            if (affector->GetCollisionType() == DiBaseColliderController::CT_FLOW)
                collisionType = "flow";
            else if (affector->GetCollisionType() == DiBaseColliderController::CT_NONE)
                collisionType = "none";
            node.SetAttribute("collisionType", collisionType);
        }
    }

    void DiFxTokensParser::ReadBaseColliderController(DiParticleController* val, DiXMLElement& node)
    {
        DiBaseColliderController* ctrl = static_cast<DiBaseColliderController*>(val);

        ReadBaseController(ctrl, node);

        node.IterateAttributes([&](const DiString& name, const DiString& value) {
            if (name == "friction")
                ctrl->SetFriction(value.AsFloat());
            else if (name == "bouncyness")
                ctrl->SetBouncyness(value.AsFloat());
            else if (name == "intersectionType")
            {
                auto t = DiBaseColliderController::IT_POINT;
                if (value == "box")
                    t = DiBaseColliderController::IT_BOX;
                ctrl->SetIntersectionType(t);
            }
            else if (name == "collisionType")
            {
                auto t = DiBaseColliderController::CT_BOUNCE;
                if (value == "flow")
                    t = DiBaseColliderController::CT_FLOW;
                else if (value == "none")
                    t = DiBaseColliderController::CT_NONE;
                ctrl->SetCollisionType(t);
            }
        });
    }

    void DiFxTokensParser::WritePlaneColliderController(DiParticleController* val, DiXMLElement& node)
    {
        DiPlaneColliderController* affector = static_cast<DiPlaneColliderController*>(val);

        WriteBaseColliderController(affector, node);

        if (affector->GetNormal() != DiPlaneColliderController::DEFAULT_NORMAL)
            node.SetVector3("normal", affector->GetNormal());
    }

    void DiFxTokensParser::ReadPlaneColliderController(DiParticleController* val, DiXMLElement& node)
    {
        DiPlaneColliderController* affector = static_cast<DiPlaneColliderController*>(val);

        ReadBaseColliderController(affector, node);

        node.IterateAttributes([&](const DiString& name, const DiString& value) {
            if (name == "normal")
                affector->SetNormal(value.AsVector3());
        });
    }

    void DiFxTokensParser::WriteBoxColliderController(DiParticleController* val, DiXMLElement& node)
    {
        DiBoxColliderController* affector = static_cast<DiBoxColliderController*>(val);

        WriteBaseColliderController(affector, node);

        if (affector->GetWidth() != DiBoxColliderController::DEFAULT_WIDTH)
            node.SetFloat("width", affector->GetWidth());

        if (affector->GetHeight() != DiBoxColliderController::DEFAULT_HEIGHT)
            node.SetFloat("height", affector->GetHeight());

        if (affector->GetDepth() != DiBoxColliderController::DEFAULT_DEPTH)
            node.SetFloat("depth", affector->GetDepth());

        if (affector->IsInnerCollision() != false)
            node.SetBool("isInnerCollision", affector->IsInnerCollision());
    }

    void DiFxTokensParser::ReadBoxColliderController(DiParticleController* val, DiXMLElement& node)
    {
        DiBoxColliderController* affector = static_cast<DiBoxColliderController*>(val);

        ReadBaseColliderController(affector, node);

        node.IterateAttributes([&](const DiString& name, const DiString& value) {
            if (name == "width")
                affector->SetWidth(value.AsFloat());
            else if (name == "height")
                affector->SetHeight(value.AsFloat());
            else if (name == "depth")
                affector->SetDepth(value.AsFloat());
            else if (name == "isInnerCollision")
                affector->SetInnerCollision(value.AsBool());
        });
    }

    void DiFxTokensParser::WriteSphereColliderController(DiParticleController* val, DiXMLElement& node)
    {
        DiSphereColliderController* affector = static_cast<DiSphereColliderController*>(val);

        WriteBaseColliderController(affector, node);

        if (affector->GetRadius() != DiSphereColliderController::DEFAULT_RADIUS)
            node.SetFloat("radius", affector->GetRadius());

        if (affector->IsInnerCollision() != false)
            node.SetBool("isInnerCollision", affector->IsInnerCollision());
    }

    void DiFxTokensParser::ReadSphereColliderController(DiParticleController* val, DiXMLElement& node)
    {
        DiSphereColliderController* affector = static_cast<DiSphereColliderController*>(val);

        ReadBaseColliderController(affector, node);

        node.IterateAttributes([&](const DiString& name, const DiString& value) {
            if (name == "radius")
                affector->SetRadius(value.AsFloat());
            else if (name == "isInnerCollision")
                affector->SetInnerCollision(value.AsBool());
        });
    }

    void DiFxTokensParser::WriteGeometryRotatorController(DiParticleController* val, DiXMLElement& node)
    {
        DiGeometryRotatorController* affector = static_cast<DiGeometryRotatorController*>(val);

        WriteBaseController(affector, node);

        if (affector->UseOwnRotationSpeed() != DiGeometryRotatorController::DEFAULT_USE_OWN)
            node.SetBool("speed", affector->UseOwnRotationSpeed());

        if (affector->GetRotationAxis() != DiGeometryRotatorController::DEFAULT_ROTATION_AXIS)
            node.SetVector3("rotationAxis", affector->GetRotationAxis());

        DiDynamicAttributeFactory dynamicAttributeFactory;
        if (dynamicAttributeFactory.GetDefaultValue(affector->GetRotationSpeed()) !=
            DiGeometryRotatorController::DEFAULT_ROTATION_SPEED)
        {
            DiString att;
            affector->GetRotationSpeed()->Write(att);
            node.SetAttribute("rotationSpeed", att);
        }
    }

    void DiFxTokensParser::ReadGeometryRotatorController(DiParticleController* val, DiXMLElement& node)
    {
        DiGeometryRotatorController* ctrl = static_cast<DiGeometryRotatorController*>(val);

        ReadBaseController(ctrl, node);

        node.IterateAttributes([&](const DiString& name, const DiString& value) {
            if (name == "speed")
                ctrl->SetUseOwnRotationSpeed(value.AsBool());
            else if (name == "rotationAxis")
                ctrl->SetRotationAxis(value.AsVector3());
            else if (name == "rotationSpeed")
                ctrl->SetRotationSpeed(DiDynamicAttribute::Read(value));
        });
    }

    void DiFxTokensParser::WriteColorController(DiParticleController* val, DiXMLElement& node)
    {
        DiColorController* affector = static_cast<DiColorController*>(val);

        WriteBaseController(affector, node);

        DiString times;
        auto& colorMap = affector->GetTimeAndColour();
        for (auto it = colorMap.begin(); it != colorMap.end(); ++it)
        {
            auto time = it->first;
            if (it != colorMap.begin())
                times += ",";
            times.AppendFloat(time);
        }
        node.SetAttribute("times", times);

        DiString colors;
        for (auto it = colorMap.begin(); it != colorMap.end(); ++it)
        {
            auto colour = it->second;
            if (it != colorMap.begin())
                colors += ",";
            colors.AppendColourValue(colour);
        }
        node.SetAttribute("colors", colors);

        if (affector->GetColourOperation() != DiColorController::DEFAULT_COLOUR_OPERATION)
        {
            DiString colourOperation = "multiply";
            if (affector->GetColourOperation() == DiColorController::CAO_SET) 
                colourOperation = "set";
            node.SetAttribute("operation", colourOperation);
        }
    }

    void DiFxTokensParser::ReadColorController(DiParticleController* val, DiXMLElement& node)
    {
        DiColorController* ctrl = static_cast<DiColorController*>(val);

        ReadBaseController(ctrl, node);

        DiVector<float> times;
        DiVector<DiColor> colors;

        node.IterateAttributes([&](const DiString& name, const DiString& value) {
            if (name == "times")
            {
                auto tokens = value.Tokenize(",");
                for (auto& t : tokens)
                    times.push_back(t.AsFloat());
            }
            else if (name == "colors")
            {
                auto tokens = value.Tokenize(",");
                for (auto& t : tokens)
                    colors.push_back(t.AsColourValue());
            }
            else if (name == "operation")
            {
                auto op = DiColorController::CAO_MULTIPLY;
                if (value == "set")
                    op = DiColorController::CAO_SET;
                ctrl->SetColourOperation(op);
            }
        });

        DI_ASSERT(times.size() == colors.size());
        for (size_t i = 0; i < times.size(); ++i)
            ctrl->AddColour(times[i], colors[i]);
    }

    void DiFxTokensParser::WriteScaleController(DiParticleController* val, DiXMLElement& node)
    {
        DiScaleController* affector = static_cast<DiScaleController*>(val);

        WriteBaseController(affector, node);

        if (affector->IsSinceStartSystem())
            node.SetBool("sinceStartSystem", affector->IsSinceStartSystem());

        if (affector->GetBeginScale() != DiScaleController::DEFAULT_SCALE)
            node.SetVector2("beginScale", affector->GetBeginScale());

        if (affector->GetMedianScale() != DiScaleController::DEFAULT_SCALE)
            node.SetVector2("medScale", affector->GetMedianScale());

        if (affector->GetEndScale() != DiScaleController::DEFAULT_SCALE)
            node.SetVector2("endScale", affector->GetEndScale());
    }

    void DiFxTokensParser::ReadScaleController(DiParticleController* val, DiXMLElement& node)
    {
        DiScaleController* ctrl = static_cast<DiScaleController*>(val);
        
        ReadBaseController(ctrl, node);

        node.IterateAttributes([ctrl](const DiString& name, const DiString& value) {
            if (name == "sinceStartSystem")
                ctrl->SetSinceStartSystem(value.AsBool());
            else if (name == "beginScale")
                ctrl->SetBeginScale(value.AsVector2());
            else if (name == "medScale")
                ctrl->SetMedianScale(value.AsVector2());
            else if (name == "endScale")
                ctrl->SetEndScale(value.AsVector2());
        });
    }

    DiParticleEmitter* DiFxTokensParser::ReadEmitter(DiParticleElement* val, DiXMLElement& node)
    {
        DiString type = node.GetAttribute("type");
        DiParticleEmitter* emitter = val->CreateEmitter(type);

        if (type == "Point")
            ReadBaseEmitter(emitter, node);
        else if (type == "Circle")
            ReadCircleEmitter(emitter, node);
        else if (type == "Box")
            ReadBoxEmitter(emitter, node);
        else if (type == "Line")
            ReadLineEmitter(emitter, node);

        return emitter;
    }

    void DiFxTokensParser::ReadBaseController(DiParticleController* val, DiXMLElement& node)
    {
        node.IterateAttributes([val](const DiString& name, const DiString& value) {
            if (name == "name")
                val->SetName(value);
            else if (name == "originalEnabled")
                val->SetOriginalEnabled(value.AsBool());
            else if (name == "position")
                val->position = value.AsVector3();
            else if (name == "mass")
                val->mass = value.AsFloat();
            else if (name == "mass")
                val->mass = value.AsFloat();
            else if (name == "specialisation")
            {
                auto v = DiParticleController::AFSP_DEFAULT;
                if (value == "ttlIncrease")
                    v = DiParticleController::AFSP_TTL_INCREASE;
                else if (value == "ttlDecrease")
                    v = DiParticleController::AFSP_TTL_DECREASE;
                val->SetControllerSpecialisation(v);
            }
            else if (name == "excludedEmitters")
            {
                auto tokens = value.Tokenize(",");
                for (auto& t : tokens)
                    val->AddEmitterToExclude(t);
            }
        });
    }

    DiFxTokensParser* DiFxTokensParser::sParser = nullptr;
}