


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
