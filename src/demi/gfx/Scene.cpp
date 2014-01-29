
#include "GfxPch.h"
#include "Scene.h"
#include "TerrainMap.h"
#include "SceneSerial.h"
#include "CullNode.h"

namespace Demi
{
    DiAssetType DiScene::TYPE = ASSET_SCENE;

    DiScene::DiScene( const DiString& name )
        :DiAsset(name)
    {

    }

    DiScene::~DiScene()
    {

    }

    bool DiScene::LoadingComplete() const
    {
        return true;
    }

    bool DiScene::Load( DiDataStreamPtr data )
    {
        DiSceneSerializer ss;
        ss.ImportScene(data,this);
        return true;
    }

    bool DiScene::Load()
    {
        return true;
    }

}