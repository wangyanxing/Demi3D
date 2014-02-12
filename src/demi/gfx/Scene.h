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

#ifndef DiScene_h__
#define DiScene_h__


#include "Asset.h"
#include "TerrainDesc.h"
#include "XMLElement.h"

namespace Demi
{
    typedef shared_ptr<DiScene> DiScenePtr;

    class DI_GFX_API DiScene : public DiAsset
    {
    public:

        friend class DiSceneSerializer;

        DiScene(const DiString& name);

        ~DiScene();

    public:

        bool                    LoadingComplete() const;

        bool                    Load(DiDataStreamPtr data);

        bool                    Load();

        DiAssetType             GetAssetType() const{return TYPE;}

        DiTerrainDesc&          GetTerrainDesc() { return mTerrainDesc; }

        DiXMLElement            GetWaterMaterial() const { return mWaterMaterial; }

    public:

        static DiAssetType      TYPE;

    protected:

        DiTerrainDesc           mTerrainDesc;

        DiXMLElement            mWaterMaterial;
        
    };

}

#endif
