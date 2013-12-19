
#include "Mesh.h"
#include "MeshSerial.h"
#include "SubMesh.h"

namespace Demi 
{
    DiAssetType DiMesh::TYPE = ASSET_MODEL;

    DiMesh::DiMesh( const DiString& name )
        :DiAsset(name),
        mSkeleton(NULL)
    {

    }

    DiMesh::~DiMesh()
    {
        SubMeshList::iterator it;
        SubMeshList::iterator itEnd = mSubMeshList.end();

        for (it = mSubMeshList.begin(); it != itEnd; ++it)
        {
            DI_DELETE (*it);
        }

        mSubMeshList.clear();
    }

    BOOL DiMesh::LoadingComplete() const
    {
        return mSubMeshList.empty() ? FALSE : TRUE;
    }

    BOOL DiMesh::Load( DiDataStreamPtr data )
    {
        DiMeshSerializer ms;
        ms.ImportMesh(data,this);
        return TRUE;
    }

    BOOL DiMesh::Load()
    {
        return TRUE;
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