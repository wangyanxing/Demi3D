


#ifndef DiSimpleShape_h__
#define DiSimpleShape_h__


#include "RenderUnit.h"
#include "TransformUnit.h"

namespace Demi 
{
    typedef shared_ptr<DiSimpleShape> DiSimpleShapePtr;

    class DI_GFX_API DiSimpleShape : public Demi::DiRenderUnit, public Demi::DiTransformUnit
    {
    public:
        DiSimpleShape();

        DiSimpleShape(const DiString& name);

        ~DiSimpleShape(void);

    public:

        void                    GetWorldTransform(Demi::DiMat4* xform) const;
        
        void                    CreatePlane(float size = 100);

        void                    CreateBox(float size = 100);

        void                    CreateTorus(float radius = 100, float tube = 40, int radialSegments = 8, int tubularSegments = 6);

        void                    CreateSphere(float radius = 100, int segments = 16, int rings = 16);

        void                    CreateWireBox(const Demi::DiAABB& aabb);

        const Demi::DiAABB&     GetBoundingBox(void) const;

        void                    CullingUpdate(Demi::DiRenderBatchGroup*,Demi::DiCamera*);

        void                    AddToBatchGroup(DiRenderBatchGroup* bg);

        void                    CreateColorBox( float size, const DiColor& color );

        virtual DiString&       GetType();

    protected:

        DiAABB                  mAABB;
    };

}


#endif
