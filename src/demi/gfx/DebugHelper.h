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

namespace Demi
{
    /// Debug rendering line.
    struct DebugLine
    {
        DebugLine(){}

        DebugLine(const DiVec3& startPos, const DiVec3& endPos, const DiColor& lineColor) :
            start(startPos),
            end(endPos),
            color(lineColor)
        {
        }

        /// start/end position
        DiVec3 start;
        DiVec3 end;

        DiColor color;
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

        DiString&       GetType();

    private:

        DiVector<DebugLine>     mLines;
    };
}

#endif
