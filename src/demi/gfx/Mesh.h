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

#ifndef DiMesh_h__
#define DiMesh_h__


#include "Asset.h"

namespace Demi 
{
    class DI_GFX_API DiMesh : public DiAsset
    {
    public:

        DiMesh(const DiString& name);

        virtual                 ~DiMesh();

        typedef DiVector<DiSubMesh*>                SubMeshList;
        typedef DiVectorIterator<SubMeshList>       SubMeshIterator;
        typedef DiConstVectorIterator<SubMeshList>  ConstSubMeshIterator;

    public:

        virtual const DiString& GetAssetType() const { return TYPE; }

        bool                    LoadingComplete() const;

        bool                    Load(DiDataStreamPtr data);

        bool                    Load();

        DiSubMesh*              CreateSubMesh();

        const DiAABB&           GetBounds() const { return mAABB; }

        void                    SetBounds(const DiAABB& val) { mAABB = val; }

        ConstSubMeshIterator    GetSubMeshs() const;

        SubMeshIterator         GetSubMeshs();

        uint32                  GetNumSubMeshes() const { return (uint32)mSubMeshList.size(); }

        DiSubMesh*              GetSubMesh(int id){return mSubMeshList[id];}
        
        uint32                  GetMaxWeights() const;

    public:

        static DiString         TYPE;

    protected:

        SubMeshList             mSubMeshList;

        DiAABB                  mAABB;
    };
}

#endif
