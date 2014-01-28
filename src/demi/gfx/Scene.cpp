
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

    BOOL DiScene::LoadingComplete() const
    {
        return TRUE;
    }

    BOOL DiScene::Load( DiDataStreamPtr data )
    {
        DiSceneSerializer ss;
        ss.ImportScene(data,this);
        return TRUE;
    }

    BOOL DiScene::Load()
    {
        return TRUE;
    }

}