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

#include "K2Pch.h"
#include "K2WorldSerial.h"
#include "K2World.h"
#include "K2MapLoader.h"
#include "K2Configs.h"

#include "K2TerrainMap.h"
#include "K2TerrainDesc.h"
#include "XMLFile.h"
#include "XMLElement.h"

namespace Demi
{
    void DiK2WorldSerial::Load(const DiString& path, DiK2World* world)
    {
        DI_LOG("Loading k2 world: %s", path.c_str());

        DiTerrainDescPtr terrainDesc = make_shared<DiTerrainDesc>();

        DiK2HeightMap heightMap;
        DiDataStreamPtr dataHeightmap = DiK2Configs::GetDataStream(path + "/heightmap", true);
        heightMap.Load(dataHeightmap);

        terrainDesc->mTextureIDMap = DI_NEW DiK2TileMap();
        DiDataStreamPtr dataTilemap = DiK2Configs::GetDataStream(path + "/tilematerialmap", true);
        terrainDesc->mTextureIDMap->Load(dataTilemap);

        terrainDesc->mColorMap = DI_NEW DiK2VertexColorMap();
        DiDataStreamPtr dataColmap = DiK2Configs::GetDataStream(path + "/vertexcolormap", true);
        terrainDesc->mColorMap->Load(dataColmap);

        LoadTextureList(path, terrainDesc);

        terrainDesc->mSizeX = (heightMap.GetWidth() - 1) / CHUNK_GRID_SIZE;
        terrainDesc->mSizeY = (heightMap.GetHeight() - 1) / CHUNK_GRID_SIZE;
        terrainDesc->mHeightData = heightMap.GetBuffer();

        world->mTerrain = make_shared<DiTerrain>();
        world->mTerrain->Load(terrainDesc);
    }

    void DiK2WorldSerial::LoadTextureList(const DiString& path, DiTerrainDescPtr terrainDesc)
    {
        DiDataStreamPtr texListData = DiK2Configs::GetDataStream(path + "/texturelist", false);

        shared_ptr<DiXMLFile> xmlfile(DI_NEW DiXMLFile());
        xmlfile->Load(texListData->GetAsString());

        DiXMLElement root = xmlfile->GetRoot();
        if (root.GetName() != "texturelist")
        {
            DI_WARNING("Invalid K2 texturelist script");
            return;
        }

        terrainDesc->mTextureTable.clear();
        DiXMLElement child = root.GetChild();
        while (child)
        {
            if (child.CheckName("texture"))
            {
                int id = child.GetInt("id");
                DiString name = child.GetAttribute("name");

                DI_LOG("K2 terrain tile[%d]: %s", id, name.c_str());

                if (name == "$checker" || name == "$flat_dull")
                    name.clear(); // dummy texture

                name.TrimLeft("/");
                name = name.ExtractBaseName();

                terrainDesc->mTextureTable[id] = name;
            }
            child = child.GetNext();
        }
    }
}