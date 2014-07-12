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

#include "FxerPrerequisites.h"
#include "MyGUI_MouseButton.h"

namespace Demi
{
    /** Transform gizmo class
     */
    class DiTransGizmo
    {
    public:
        
        DiTransGizmo(DiBaseEditorObj* parent);

        ~DiTransGizmo(void);

        enum GizmoMode
        {
            GIZMO_HIDE,
            GIZMO_SELECT,
            GIZMO_MOVE,
            GIZMO_ROTATE,
            GIZMO_SCALE,
        };
        
        enum RotatePick
        {
            PICK_NONE = 0,
            PICK_ROT_X = 1<<7,
            PICK_ROT_Y = 1<<8,
            PICK_ROT_Z = 1<<9,
            PICK_ROT_XYZ = PICK_ROT_X|PICK_ROT_Y|PICK_ROT_Z,
        };
        
    public:

        void            Update();
        
        void            SetGizmoMode(GizmoMode mode);
        
        GizmoMode       GetGizmoMode() { return mMode; }
        
        void            OnMouseMove(int _left, int _top);
        
        void            OnMouseDown(int _left, int _top, MyGUI::MouseButton _id);
        
        void            OnMouseDrag(int _left, int _top, MyGUI::MouseButton _id);
        
        void            OnMouseUp(int _left, int _top, MyGUI::MouseButton _id);
        
        void            Show(bool visible);
        
        bool            IsActive(){return mActive;}
        
        DiCullNode*     GetNode(){ return mBaseNode; }
        
        DiRay           GetMouseRay(int _left, int _top);
        
        DiAABB          GetWorldAABB();
        
    protected:
        
        void            Create();
        
        void            HideAll();
        
        void            GenerateRotRingVerts();
        
        RotatePick      PickRotRings(const DiRay& ray, DiVec3& out);
        
        void            HightlightRotRings(RotatePick pickret);
        
        DiVec3          CalculateDeltaTrans(const DiRay& ray, const DiPlane& plane, int pickResult, const DiVec3& curPos);
        
        DiPlane         FindGizmoPlane(DiRay &pickRay, int TranslationAxis);
    
    protected:
        
        DiCullNode*     mBaseNode{ nullptr };
        
        DiCullNode*     mAxesNode{ nullptr };
        
        DiCullNode*     mRotateCircleNode{ nullptr };
        
        DiCullNode*     mScaleNode{ nullptr };
        
        GizmoMode       mMode{GIZMO_MOVE};
        
        DiDebugHelperPtr mCircles;
        
        DiTransAxesPtr  mAxes;
        
        DiSimpleShapePtr mRotateRings[3];
        
        DiVector<DiVec3> mRotatingRingsVerts;
        
        DiMaterialPtr    mRotatingRingsMat[3];
        
        DiCullNode*     mRotateRingNode[3]{nullptr,nullptr,nullptr};
        
        bool            mActive{ true };
        
        DiVec3          mLastPos{ 0,0,0 };
        
        DiVec3          mLastDerivedPos{ 0,0,0 };
        
        DiVec3          mLastDerivedScale{ 1,1,1 };
        
        DiVec3          mLastTranslationDelta{ 0,0,0 };
        
        DiVec3          mLast3DDelta{ 0,0,0 };
        
        DiPlane         mLastGizmoPlane;
        
        int             mLastPickResult{ 0 };
        
        bool            mPicking{ false };
        
        DiBaseEditorObj* mObject{ nullptr };
    };
}

#endif
