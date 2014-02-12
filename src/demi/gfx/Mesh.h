


#ifndef DiMesh_h__
#define DiMesh_h__


#include "Asset.h"

namespace Demi 
{
    typedef shared_ptr<DiMesh> DiMeshPtr;

    class DI_GFX_API DiMesh : public DiAsset
    {
    public:

        DiMesh(const DiString& name);

        virtual                 ~DiMesh();

        typedef DiVector<DiSubMesh*>                SubMeshList;
        typedef DiVectorIterator<SubMeshList>        SubMeshIterator;
        typedef DiConstVectorIterator<SubMeshList>    ConstSubMeshIterator;

    public:

        DiAssetType             GetAssetType() const {return TYPE;}

        bool                    LoadingComplete() const;

        bool                    Load(DiDataStreamPtr data);

        bool                    Load();

        DiSubMesh*              CreateSubMesh();

        const DiAABB&           GetBounds() const { return mAABB; }

        void                    SetBounds(const DiAABB& val) { mAABB = val; }

        ConstSubMeshIterator    GetSubMeshs() const;

        SubMeshIterator         GetSubMeshs();

        DiSubMesh*              GetSubMesh(int id){return mSubMeshList[id];}

    public:

        static DiAssetType      TYPE;

    protected:

        SubMeshList             mSubMeshList;

        DiAABB                  mAABB;

        DiSkeleton*             mSkeleton;
    };
}

#endif
