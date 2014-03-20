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

#ifndef DiModel_h__
#define DiModel_h__


#include "TransformUnit.h"
#include "Mesh.h"
#include "Material.h"
#include "XMLElement.h"

namespace Demi 
{
    class DI_GFX_API DiModel : public DiTransformUnit
    {
    public:

        DiModel(const DiString& name);

        DiModel(const DiString& name,
                const DiString& modelName);

        DiModel(const DiString& name,DiMeshPtr model);

        virtual ~DiModel();

        typedef DiVector<DiSubModel*> SubModelList;

        typedef DiVectorIterator<SubModelList> SubModelIterator;

    public:

        const DiAABB&     GetBoundingBox(void) const;

        virtual void      AddToBatchGroup(DiRenderBatchGroup* bg);

        DiMeshPtr         GetMesh(){return mMesh;}

        uint32            GetNumSubModels()const {return mSubModels.size();}

        DiSubModel*       CreateSubModel(DiSubMesh* sm);

        DiSubModel*       GetSubModel(uint32 id);

        void              DestroySubModels();

        SubModelIterator  GetSubModels();

        void              SetMaterial(const DiString& name);

        void              SetMaterial(DiMaterialPtr mat);

        virtual DiString& GetType();

        virtual void      Save(DiXMLElement node);

        virtual void      Load(DiXMLElement node);

    protected:

        virtual void      Init();

        void              InitSubModels();

    protected:

        DiMeshPtr         mMesh;

        SubModelList      mSubModels;
    };
}

#endif
