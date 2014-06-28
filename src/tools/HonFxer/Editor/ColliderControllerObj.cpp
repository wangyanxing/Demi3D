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

#include "FxerPch.h"
#include "ColliderControllerObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleElementObj.h"
#include "ParticleElement.h"
#include "BoxColliderController.h"
#include "SphereColliderController.h"
#include "PlaneColliderController.h"
#include "ControllerBaseObj.h"
#include "EnumProperties.h"

namespace Demi
{
    DiBaseColliderControllerObj::DiBaseColliderControllerObj()
    {
    }

    DiBaseColliderControllerObj::~DiBaseColliderControllerObj()
    {
    }

    DiString DiBaseColliderControllerObj::GetUICaption()
    {
        return mController->GetName();
    }
    
    DiBaseColliderController* DiBaseColliderControllerObj::GetController()
    {
        return static_cast<DiBaseColliderController*>(mController);
    }

    void DiBaseColliderControllerObj::InitBasePropertyTable()
    {
        DiControllerBaseObj::InitPropertyTable();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Collider Controller");
        
        using InterMode = DiBaseColliderController::IntersectionType;
        using ColliMode = DiBaseColliderController::CollisionType;
        
        g->AddProperty("Intersection Type",   DI_NEW DiEnumProperty([&](){ return make_shared<IntersectionTypeEnum>(GetController()->GetIntersectionType()); },
                                                             [&](DiBaseEnumPropPtr& val){
                                                                 GetController()->SetIntersectionType(val->getEnum<IntersectionTypeEnum,InterMode>());}));
        
        g->AddProperty("Collision Type",   DI_NEW DiEnumProperty([&](){ return make_shared<CollisionTypeEnum>(GetController()->GetCollisionType()); },
                                                                 [&](DiBaseEnumPropPtr& val){
                                                                        GetController()->SetCollisionType(val->getEnum<CollisionTypeEnum,ColliMode>());}));
        
        g->AddProperty("Friction", DI_NEW DiFloatProperty([&](){ return GetController()->GetFriction(); },
                                                          [&](float& val){ GetController()->SetFriction(val);}));
        
        g->AddProperty("Bouncyness", DI_NEW DiFloatProperty([&](){ return GetController()->GetBouncyness(); },
                                                            [&](float& val){ GetController()->SetBouncyness(val);}));
        
        g->CreateUI();

        mPropGroups.push_back(g);
    }
    
    DiBoxColliderController* DiBoxColliderControllerObj::GetBox()
    {
        return static_cast<DiBoxColliderController*>(mController);
    }
    
    void DiBoxColliderControllerObj::InitPropertyTable()
    {
        InitBasePropertyTable();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Box Collider");
        
        g->AddProperty("Width",   DI_NEW DiFloatProperty([&](){ return GetBox()->GetWidth(); },
                                                         [&](float& val){ GetBox()->SetWidth(val);}));

        g->AddProperty("Height",  DI_NEW DiFloatProperty([&](){ return GetBox()->GetHeight(); },
                                                         [&](float& val){ GetBox()->SetHeight(val);}));
        
        g->AddProperty("Depth",   DI_NEW DiFloatProperty([&](){ return GetBox()->GetDepth(); },
                                                         [&](float& val){ GetBox()->SetDepth(val);}));
        
        g->AddProperty("Inner Collision", DI_NEW DiBoolProperty([&](){ return GetBox()->IsInnerCollision(); },
                                                         [&](bool& val){ GetBox()->SetInnerCollision(val);}));
        
        g->CreateUI();
        
        mPropGroups.push_back(g);

    }
    
    DiSphereColliderController* DiSphereColliderControllerObj::GetSphere()
    {
        return static_cast<DiSphereColliderController*>(mController);
    }
 
    void DiSphereColliderControllerObj::InitPropertyTable()
    {
        InitBasePropertyTable();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Sphere Collider");
        
        g->AddProperty("Radius",          DI_NEW DiFloatProperty([&](){ return GetSphere()->GetRadius(); },
                                                                 [&](float& val){ GetSphere()->SetRadius(val);}));
        
        g->AddProperty("Inner Collision", DI_NEW DiBoolProperty([&](){ return GetSphere()->IsInnerCollision(); },
                                                                [&](bool& val){ GetSphere()->SetInnerCollision(val);}));
        
        g->CreateUI();
        
        mPropGroups.push_back(g);

    }
    
    DiPlaneColliderController* DiPlaneColliderControllerObj::GetPlane()
    {
        return static_cast<DiPlaneColliderController*>(mController);
    }
    
    void DiPlaneColliderControllerObj::InitPropertyTable()
    {
        InitBasePropertyTable();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Plane Collider");
        
        g->AddProperty("Normal", DI_NEW DiVec3Property([&](){ return GetPlane()->GetNormal(); },
                                                       [&](DiVec3& val){ GetPlane()->SetNormal(val);}));
        g->CreateUI();
        
        mPropGroups.push_back(g);
        
    }
    
}
