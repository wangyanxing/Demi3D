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

#ifndef ForceControllerObject_h__
#define ForceControllerObject_h__

#include "FxPrerequisites.h"
#include "FxerPrerequisites.h"
#include "BaseEditorObject.h"
#include "ControllerBaseObj.h"
#include "PropertyTypes.h"

namespace Demi 
{
    class DiBaseForceControllerObj : public DiControllerBaseObj
    {
    public:

        DiBaseForceControllerObj();

        ~DiBaseForceControllerObj();

    public:

        DiString    GetUICaption() override;

        DiString    GetUINodeType() { return "File"; }

    public:

        DiBaseForceController*  GetController();

    protected:

        void        InitBasePropertyTable();
    };
    
    ////////////////////////////////////////////////////////////////////////////////
    
    class DiLinearForceControllerObj final : public DiBaseForceControllerObj
    {
    public:
        
        DiLinearForceControllerObj() = default;
        
        ~DiLinearForceControllerObj() = default;
        
    public:
        
        DiString    GetType() override { return "LinearForceController"; }
        
        DiString    GetControllerType() override { return "LinearForce"; }
        
    public:
        
        DiLinearForceController*  GetLinearForce();
        
    protected:
        
        void        InitPropertyTable();
    };

    ////////////////////////////////////////////////////////////////////////////////

    class DiSineForceControllerObj final : public DiBaseForceControllerObj
    {
    public:
        
        DiSineForceControllerObj() = default;
        
        ~DiSineForceControllerObj() = default;
        
    public:
        
        DiString    GetType() override { return "SineForceController"; }

        DiString    GetControllerType() override { return "SineForce"; }

    public:

        DiSineForceController*  GetSineForce();

    protected:

        void        InitPropertyTable();
    };
}

#endif
