
/********************************************************************
    File:       Scene.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

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

        BOOL                    LoadingComplete() const;

        BOOL                    Load(DiDataStreamPtr data);

        BOOL                    Load();

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