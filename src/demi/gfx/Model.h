
/********************************************************************
    File:       Model.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "TransformUnit.h"
#include "Mesh.h"
#include "Material.h"
#include "XMLElement.h"

namespace Demi 
{
    typedef shared_ptr<DiModel> DiModelPtr;

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

        virtual void      CullingUpdate(DiRenderBatchGroup*,DiCamera*);

        virtual void      AddToBatchGroup(DiRenderBatchGroup* bg);

        DiMeshPtr         GetMesh(){return mMesh;}

        size_t            GetNumSubEntites()const {return mSubModels.size();}

        DiSubModel*       CreateSubModel(DiSubMesh* sm);

        DiSubModel*       GetSubModel(size_t id);

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