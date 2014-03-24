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
#include "K2Model.h"
#include "K2TerrainMap.h"
#include "K2TerrainDesc.h"

#include "XMLFile.h"
#include "XMLElement.h"
#include "CullNode.h"

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

        LoadWorldConfig(path, terrainDesc, world);

        world->mTerrain = make_shared<DiTerrain>();
        world->mTerrain->Load(terrainDesc);

        // load entities
        LoadEntityList(path, world);
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

    void DiK2WorldSerial::LoadEntityList(const DiString& path, DiK2World* world)
    {
        DiDataStreamPtr texListData = DiK2Configs::GetDataStream(path + "/entitylist", false);

        DI_LOG("Loading K2 entity lists");

        shared_ptr<DiXMLFile> xmlfile(DI_NEW DiXMLFile());
        xmlfile->Load(texListData->GetAsString());

        DiXMLElement root = xmlfile->GetRoot();
        if (root.GetName() != "entitylist")
        {
            DI_WARNING("Invalid K2 entitylist script");
            return;
        }

        DiXMLElement child = root.GetChild();
        while (child)
        {
            if (child.CheckName("entity"))
            {
                int id = child.GetInt("id");
                DiString model = child.GetAttribute("model");
                model.TrimLeft("/");
                DiString type = child.GetAttribute("type");

                DiVec3 angles = child.GetVector3("angles");
                std::swap(angles.y, angles.z);
                //angles.y *= -1;

                DiQuat rot = DiK2Configs::ConvertAngles(angles);
                DiVec3 pos = child.GetVector3("position");
                std::swap(pos.y, pos.z);

                DiVec2 worldSize = world->GetTerrain()->GetWorldSize();
                pos.x = worldSize.x - pos.x;
                pos.x -= worldSize.x / 2;
                pos.z -= worldSize.y / 2;

                if (type == "Prop_Cliff")
                {
                    //rot = DiQuat(DiRadian(DiDegree(-angles.y)), DiVec3::UNIT_Y);
                }

                float scale = child.GetFloat("scale");

                DiK2Model* k2md = world->AddModel(model, type);
                k2md->GetNode()->SetPosition(pos);
                k2md->GetNode()->SetOrientation(rot);
                k2md->GetNode()->SetScale(scale);

                uint32 size = world->GetNumModels();
                DI_DEBUG("Model[%d] %s", size-1, type.c_str());
            }
            child = child.GetNext();
        }
    }

    void DiK2WorldSerial::LoadWorldConfig(const DiString& path, DiTerrainDescPtr terrainDesc, DiK2World* world)
    {
        DiDataStreamPtr texListData = DiK2Configs::GetDataStream(path + "/worldconfig", false);

        DI_LOG("Loading K2 worldconfig");

        shared_ptr<DiXMLFile> xmlfile(DI_NEW DiXMLFile());
        xmlfile->Load(texListData->GetAsString());

        DiXMLElement root = xmlfile->GetRoot();
        if (root.GetName() != "world")
        {
            DI_WARNING("Invalid K2 worldconfig script");
            return;
        }

        world->mName = root.GetAttribute("name");
        terrainDesc->mGridSize = root.GetFloat("scale");
        terrainDesc->mTextureScale = root.GetFloat("texturescale");
        terrainDesc->mCliffSpace = root.GetUint("cliffsize");
    }
}