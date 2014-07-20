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

#ifndef ColliderControllerObject_h__
#define ColliderControllerObject_h__

#include "K2Prerequisites.h"
#include "FxerPrerequisites.h"
#include "BaseEditorObject.h"
#include "ControllerBaseObj.h"
#include "PropertyTypes.h"

namespace Demi 
{
    class DiBaseColliderControllerObj : public DiControllerBaseObj
    {
    public:

        DiBaseColliderControllerObj();

        ~DiBaseColliderControllerObj();

    public:

        DiString    GetUICaption() override;

        DiString    GetUINodeType() { return "File"; }

    public:

        DiBaseColliderController*  GetController();

    protected:

        void        InitBasePropertyTable();
    };
    
    ////////////////////////////////////////////////////////////////////////////////
    
    class DiBoxColliderControllerObj final : public DiBaseColliderControllerObj
    {
    public:
        
        DiBoxColliderControllerObj() = default;
        
        ~DiBoxColliderControllerObj() = default;
        
    public:
        
        DiString    GetType() override { return "BoxColliderController"; }
        
        DiString    GetControllerType() override { return "BoxCollider"; }
        
    public:
        
        DiBoxColliderController*  GetBox();

    protected:
        
        void        InitPropertyTable();
    };

    ////////////////////////////////////////////////////////////////////////////////

    class DiPlaneColliderControllerObj final : public DiBaseColliderControllerObj
    {
    public:
        
        DiPlaneColliderControllerObj() = default;
        
        ~DiPlaneColliderControllerObj() = default;
        
    public:
        
        DiString    GetType() override { return "PlaneColliderController"; }

        DiString    GetControllerType() override { return "PlaneCollider"; }

    public:

        DiPlaneColliderController*  GetPlane();

    protected:

        void        InitPropertyTable();
    };

    ////////////////////////////////////////////////////////////////////////////////

    class DiSphereColliderControllerObj final : public DiBaseColliderControllerObj
    {
    public:
        
        DiSphereColliderControllerObj() = default;
        
        ~DiSphereColliderControllerObj() = default;
        
    public:
        
        DiString    GetType() override { return "SphereColliderController"; }

        DiString    GetControllerType() override { return "SphereCollider"; }

    public:

        DiSphereColliderController*  GetSphere();

    protected:

        void        InitPropertyTable();
    };
}

#endif
