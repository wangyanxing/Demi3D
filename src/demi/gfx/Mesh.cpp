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
#include "Mesh.h"
#include "MeshSerial.h"
#include "SubMesh.h"

namespace Demi 
{
    DiString DiMesh::TYPE = "Model";

    DiMesh::DiMesh( const DiString& name )
        :DiAsset(name)
    {
    }

    DiMesh::~DiMesh()
    {
        for (auto it = mSubMeshList.begin(); it != mSubMeshList.end(); ++it)
        {
            DI_DELETE (*it);
        }

        mSubMeshList.clear();
    }

    bool DiMesh::LoadingComplete() const
    {
        return mSubMeshList.empty() ? false : true;
    }

    bool DiMesh::Load( DiDataStreamPtr data )
    {
        DiMeshSerializer ms;
        ms.ImportMesh(data,this);
        return true;
    }

    bool DiMesh::Load()
    {
        return true;
    }

    DiSubMesh* DiMesh::CreateSubMesh()
    {
        DiSubMesh* sm = DI_NEW DiSubMesh(this);
        mSubMeshList.push_back(sm);
        sm->mIndex = mSubMeshList.size();

        return sm;
    }

    DiMesh::ConstSubMeshIterator DiMesh::GetSubMeshs() const
    {
        return ConstSubMeshIterator(mSubMeshList.begin(), mSubMeshList.end());
    }

    DiMesh::SubMeshIterator DiMesh::GetSubMeshs()
    {
        return SubMeshIterator(mSubMeshList.begin(), mSubMeshList.end());
    }
}