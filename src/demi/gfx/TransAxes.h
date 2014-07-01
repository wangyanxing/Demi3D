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

#ifndef DiTransAxesHelper_h__
#define DiTransAxesHelper_h__

#include "GfxPrerequisites.h"
#include "TransformUnit.h"
#include "RenderUnit.h"
#include "GfxDriver.h"

namespace Demi
{
    struct GizmoVert
    {
        GizmoVert() = default;
        
        GizmoVert(const DiVec3& vec, const DiColor& col)
        {
            x = vec.x;
            y = vec.y;
            z = vec.z;
            color = DiBase::Driver->GetNativeColor(col);
        }
        
        GizmoVert(float xx ,float yy, float zz, const DiColor& col)
        {
            x = xx;
            y = yy;
            z = zz;
            color = DiBase::Driver->GetNativeColor(col);
        }
        
        float x{0};
        float y{0};
        float z{0};
        uint32 color{0};
    };
    
    /** Transform axes class
     */
    class DI_GFX_API DiTransAxes : public DiTransformUnit
    {
    public:
        
        DiTransAxes(void);

        ~DiTransAxes(void);
        
        enum PickResult
        {
            PICK_NONE = 0,
            
            PICK_MOVE_X,
            PICK_MOVE_Y,
            PICK_MOVE_Z,
            PICK_MOVE_XY,
            PICK_MOVE_YZ,
            PICK_MOVE_XZ,
            PICK_MOVE_XYZ,
            
            PICK_SCALE_X,
            PICK_SCALE_Y,
            PICK_SCALE_Z,
            PICK_SCALE_XY,
            PICK_SCALE_YZ,
            PICK_SCALE_XZ,
            PICK_SCALE_XYZ,
        };

    public:

        const DiAABB&   GetBoundingBox(void) const;

        void            Update(DiCamera* camera);

        void            AddToBatchGroup(DiRenderBatchGroup* bg);

        DiString&       GetType();
        
        bool            IsShowArraw() {return mShowArraow;}
        
        bool            IsShowScale() {return mShowScale;}
        
        void            SetShowArraw(bool v) {mShowArraow = v;}
        
        void            SetShowScale(bool v) {mShowScale = v;}
        
        void            SetShowScalePlane(int id, bool v) {mShowScalePlane[id] = v;}
        
        void            SetShowScalePlane(bool v);
        
        bool            GetShowScalePlane(int id) {return mShowScalePlane[id];}
        
        PickResult      Pick(const DiRay& ray);
        
        void            Highlight(PickResult pick);
        
    protected:
        
        /** Create the geometry data
         */
        void            Create();
        
        DiRenderUnit*   AddMeshes(const DiVector<GizmoVert>& verts,
                                  const DiVector<uint16>& indices,
                                  DiPrimitiveType primType, DiMaterialPtr mat);
        
        DiRenderUnit*   AddMeshes(const DiVector<GizmoVert>& verts,
                                  DiPrimitiveType primType, DiMaterialPtr mat);

    private:

        /// Bounding box
        DiAABB          mBounds;
        
        DiMaterialPtr   mColorMats[3];
        
        DiRenderUnit*   mLines[3]{ nullptr,nullptr,nullptr };
        
        DiRenderUnit*   mArraw[3]{ nullptr,nullptr,nullptr };
        
        DiRenderUnit*   mScaleBlock[3]{ nullptr,nullptr,nullptr };
        
        DiRenderUnit*   mScaleLines{ nullptr };
        
        DiRenderUnit*   mScalePlane[3]{ nullptr,nullptr,nullptr };
        
        DiVertexDeclaration*    mVertexDecl{ nullptr };
        
        bool            mShowArraow{ true };
        
        bool            mShowScale{ false };
        
        bool            mShowScalePlane[3]{false,false,false};
        
        /// Arraw vertices, for picking
        DiVector<DiVec3> mArrawVertices[3];
        
        DiVector<DiVec3> mScaleBlockVertices[3];
        
        DiVector<DiVec3> mScalePlaneVertices[3];
        
        float            mPickCenterRadius;
    };
}

#endif
