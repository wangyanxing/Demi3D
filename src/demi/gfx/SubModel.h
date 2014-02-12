


#ifndef DiSubModel_h__
#define DiSubModel_h__


#include "RenderUnit.h"
#include "Any.h"

namespace Demi 
{
    class DiSubModel : public DiRenderUnit, public DiAnyInterface
    {
    public:

        DiSubModel(DiModel* parent, DiSubMesh* model);

        ~DiSubModel();

    public:

        void            GetWorldTransform(DiMat4* xform) const;

        DiSubMesh*      GetSubMesh(){return mMesh;}

    protected:

        void            InitFromSubMesh();

    protected:

        DiModel*        mParent;

        DiSubMesh*      mMesh;
    };
}

#endif
