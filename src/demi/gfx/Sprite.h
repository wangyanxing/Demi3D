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

#ifndef DiSprite_h__
#define DiSprite_h__


#include "RenderUnit.h"
#include "TransformUnit.h"

namespace Demi
{
    struct DI_GFX_API DiQuadElement
    {
        DiQuadElement(const DiColor& col);

        DiQuadElement();

        float       left;
        float       top;
        float       right;
        float       bottom;

        float       leftUv;
        float       topUv;
        float       rightUv;
        float       bottomUv;

        ARGB        topLeftColor;
        ARGB        topRightColor;
        ARGB        bottomLeftColor;
        ARGB        bottomRightColor;

        bool        absCoord;
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_GFX_API DiSprite : public DiRenderUnit, public DiTransformUnit
    {
    public:

        DiSprite();

        ~DiSprite();

    public:

        void                    GetWorldTransform(DiMat4* xform) const;

        const DiAABB&            GetBoundingBox(void) const;

        void                    CullingUpdate(DiRenderBatchGroup*,DiCamera*);

        void                    AddToBatchGroup(DiRenderBatchGroup* bg);

        void                    Update(DiCamera* camera);

        void                    AddQuad(const DiQuadElement& quad);

        void                    ClearQuads(){mQuads.clear();}

        void                    UpdateGeometry(DiRenderTarget* rt);

        virtual DiString&       GetType();

    protected:

        DiVector<DiQuadElement>    mQuads;
    };
}

#endif
