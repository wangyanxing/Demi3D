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