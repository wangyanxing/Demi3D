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
#include "GfxPch.h"
#include "Model.h"
#include "AssetManager.h"
#include "SubModel.h"
#include "CullNode.h"
#include "GfxDriver.h"
#include "RenderPipeline.h"

namespace Demi 
{
    DiModel::DiModel( const DiString& name,
        const DiString& modelName)
        :DiTransformUnit(name)
    {
        mMesh = DiAssetManager::GetInstance().GetAsset<DiMesh>(modelName,true);
        Init();
    }

    DiModel::DiModel( const DiString& name,DiMeshPtr model )
        :DiTransformUnit(name),
        mMesh(model)
    {
        Init();
    }

    DiModel::DiModel( const DiString& name )
        :DiTransformUnit(name)
    {
    }

    DiModel::~DiModel()
    {
        DestroySubModels();
    }

    const DiAABB& DiModel::GetBoundingBox( void ) const
    {
        if (!mMesh)
            return DiAABB::BOX_NULL;
        else
            return mMesh->GetBounds();
    }

    void DiModel::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        for (auto it = mSubModels.begin(); it != mSubModels.end(); ++it)
            bg->AddRenderUnit(*it);
    }

    void DiModel::InitSubModels()
    {
        DI_ASSERT(mSubModels.empty());

        DiMesh::SubMeshIterator it = mMesh->GetSubMeshs();
        while(it.HasMoreElements())
        {
            DiSubMesh* sm = it.GetNext();
            CreateSubModel(sm);
        }
    }

    DiSubModel* DiModel::CreateSubModel( DiSubMesh* sm )
    {
        DiSubModel* se = DI_NEW DiSubModel(this,sm);
        mSubModels.push_back(se);
        return se;
    }

    void DiModel::DestroySubModels()
    {
        for (auto it = mSubModels.begin(); it != mSubModels.end(); ++it)
        {
            DI_DELETE *it;
        }
        mSubModels.clear();
    }

    DiModel::SubModelIterator DiModel::GetSubModels()
    {
        return SubModelIterator(mSubModels.begin(),mSubModels.end());
    }

    void DiModel::SetMaterial( const DiString& name )
    {
        for (auto it = mSubModels.begin(); it != mSubModels.end(); ++it)
        {
            (*it)->SetMaterial(name);
        }
    }

    void DiModel::SetMaterial(DiMaterialPtr mat)
    {
        for (auto it = mSubModels.begin(); it != mSubModels.end(); ++it)
        {
            (*it)->SetMaterial(mat);
        }
    }

    void DiModel::Init()
    {
        if (mMesh)
        {
            InitSubModels();
        }

        if( mParentNode )
        {
            mParentNode->NeedUpdate();
        }
    }

    DiSubModel* DiModel::GetSubModel( size_t id )
    {
        if (id >= mSubModels.size())
        {
            return NULL;
        }
        return mSubModels[id];
    }

    DiString& DiModel::GetType()
    {
        static DiString type = "Model";
        return type;
    }

    void DiModel::Save( DiXMLElement node )
    {
        DiTransformUnit::Save(node);
        if (mMesh)
            node.SetAttribute("Model",mMesh->GetName());
    }

    void DiModel::Load( DiXMLElement node )
    {
        DiTransformUnit::Load(node);

        if (node.HasAttribute("Model"))
        {
            DiString modelName = node.GetAttribute("Model");
            mMesh = DiAssetManager::GetInstance().GetAsset<DiMesh>(modelName,true);
            Init();
        }
    }

}