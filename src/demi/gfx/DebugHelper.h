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

#ifndef DiDebugHelper_h__
#define DiDebugHelper_h__

#include "GfxPrerequisites.h"
#include "TransformUnit.h"
#include "RenderUnit.h"
#include "GfxDriver.h"

namespace Demi
{
    /** Debug rendering line.
     */
    struct DebugLine
    {
        DebugLine(){}

        DebugLine(const DiVec3& startPos, const DiVec3& endPos, const DiColor& lineColor) :
            point0(startPos),
            point1(endPos)
        {
            color0 = DiBase::Driver->GetNativeColor(lineColor);
            color1 = color0;
        }

        DiVec3 point0;
        ARGB   color0;
        
        DiVec3 point1;
        ARGB   color1;
    };


    /** Debug helper class
     */
    class DI_GFX_API DiDebugHelper : public DiTransformUnit, public DiRenderUnit
    {
    public:
        
        DiDebugHelper(void);

        ~DiDebugHelper(void);

    public:

        void            GetWorldTransform(DiMat4* xform) const;

        const DiAABB&   GetBoundingBox(void) const;

        void            Update(DiCamera* camera);

        void            AddToBatchGroup(DiRenderBatchGroup* bg);

        /** Add a line and mart the dirty flag
         */
        void            AddLine(const DiVec3& startPos, const DiVec3& endPos, const DiColor& lineColor);
        
        /** Add a bounding box
         */
        void            AddBoundingBox(const DiAABB& bounds, const DiColor& lineColor);

        void            AddBoundingBox(const DiVec3& pos, float size, const DiColor& lineColor);

        /** Add a skeleton
         */
        void            AddSkeleton(const DiSkeletonInstance* skeleton, const DiColor& lineColor);
        
        /** Add a frustum
         */
        void            AddFrustum(const DiFrustum* frustum, const DiColor& lineColor);
        
        void            Clear();

        DiString&       GetType();
        
    protected:
        
        /** Flush the geometry data
         */
        void            Flush();

    private:

        DiVector<DebugLine> mLines;
        
        /// Check if we need to update the geometry data
        bool            mDirty;
        
        /// Size of the vertex buffers
        uint32          mVbSize;
        
        /// Bounding box
        DiAABB          mBounds;
    };
}

#endif
