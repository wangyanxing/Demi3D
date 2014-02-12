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
