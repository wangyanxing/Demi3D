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

#ifndef DiGrid_h__
#define DiGrid_h__


#include "TransformUnit.h"
#include "RenderUnit.h"

namespace Demi 
{
    class DEMI_DEMO_API DiGridPlane : public Demi::DiTransformUnit, public Demi::DiRenderUnit
    {
    public:

        DiGridPlane(uint32 size, float cellSize);

        DiGridPlane(uint32 size, float cellSize, const DiColor& color1, const DiColor& color2);

        ~DiGridPlane(void);

    public:

        virtual void            GetWorldTransform(Demi::DiMat4* xform) const;

        const DiAABB&           GetBoundingBox(void) const;

        void                    AddToBatchGroup(DiRenderBatchGroup* bg);

        virtual DiString&       GetType();

    protected:

        void                    BuildGrid(uint32 size, float cellSize, const DiColor& color1, const DiColor& color2);
    };

}

#endif
