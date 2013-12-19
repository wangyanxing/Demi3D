
/********************************************************************
    File:       Grid.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "TransformUnit.h"
#include "RenderUnit.h"

namespace Demi 
{
    class DEMI_EXT_API DiGridPlane : public Demi::DiTransformUnit, public Demi::DiRenderUnit
    {
    public:

        DiGridPlane(UINT size, float cellSize);

        DiGridPlane(UINT size, float cellSize, const DiColor& color1, const DiColor& color2);

        ~DiGridPlane(void);

    public:

        virtual void            GetWorldTransform(Demi::DiMat4* xform) const;

        const Demi::DiAABB&     GetBoundingBox(void) const;

        virtual    void         CullingUpdate(Demi::DiRenderBatchGroup*, Demi::DiCamera*);

        void                    AddToBatchGroup(DiRenderBatchGroup* bg);

        virtual DiString&       GetType();

    protected:

        void                    BuildGrid(UINT size, float cellSize, const DiColor& color1, const DiColor& color2);
    };

}