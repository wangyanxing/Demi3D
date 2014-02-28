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

#ifndef DiSimpleShape_h__
#define DiSimpleShape_h__


#include "RenderUnit.h"
#include "TransformUnit.h"

namespace Demi 
{
    class DI_GFX_API DiSimpleShape : public Demi::DiRenderUnit, public Demi::DiTransformUnit
    {
    public:
        DiSimpleShape();

        DiSimpleShape(const DiString& name);

        ~DiSimpleShape(void);

    public:

        void                    GetWorldTransform(Demi::DiMat4* xform) const;
        
        void                    CreatePlane(float sizeX = 100, float sizeY = 100);

        void                    CreateBox(float size = 100);

        void                    CreateTorus(float radius = 100, float tube = 40,
                                            int radialSegments = 8, int tubularSegments = 6);

        void                    CreateSphere(float radius = 100, int segments = 16, int rings = 16);

        const Demi::DiAABB&     GetBoundingBox(void) const;

        void                    AddToBatchGroup(DiRenderBatchGroup* bg);

        void                    CreateColorBox( float size, const DiColor& color );

        virtual DiString&       GetType();

    protected:

        DiAABB                  mAABB;
    };

}


#endif
