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

#ifndef DiTransGizmoHelper_h__
#define DiTransGizmoHelper_h__

#include "GfxPrerequisites.h"

namespace Demi
{
    /** Transform gizmo class
     */
    class DI_GFX_API DiTransGizmo
    {
    public:
        
        DiTransGizmo(void);

        ~DiTransGizmo(void);

        enum GizmoMode
        {
            GIZMO_HIDE,
            GIZMO_MOVE,
            GIZMO_ROTATE,
            GIZMO_SCALE,
        };
        
    public:

        void            Update();
        
        void            SetGizmoMode(GizmoMode mode);
        
        GizmoMode       GetGizmoMode() { return mMode; }
        
    protected:
        
        void            Create();
        
        void            HideAll();
    
    protected:
        
        DiCullNode*     mBaseNode{ nullptr };
        
        DiCullNode*     mAxesNode{ nullptr };
        
        DiCullNode*     mRotateCircleNode{ nullptr };
        
        DiCullNode*     mScaleNode{ nullptr };
        
        GizmoMode       mMode{GIZMO_HIDE};
        
        DiDebugHelperPtr mCircles;
        
        DiTransAxesPtr  mAxes;
        
        DiSimpleShapePtr mRotateRings[3];
        
        DiCullNode*     mRotateRingNode[3]{nullptr,nullptr,nullptr};
    };
}

#endif
