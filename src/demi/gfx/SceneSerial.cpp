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
#include "SceneSerial.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "GfxDriver.h"
#include "DirectionalLight.h"
#include "TerrainMap.h"
#include "WaterMap.h"
#include "MaterialSerial.h"
#include "SceneFormat.h"
#include "CullNode.h"
#include "XMLFile.h"
#include "Model.h"
#include "GfxDriver.h"

namespace Demi
{
    DiSceneSerializer::DiSceneSerializer()
    {
    }

    DiSceneSerializer::~DiSceneSerializer()
    {
    }

    void DiSceneSerializer::ExportScene( const DiScenePtr scene, const DiString& pathName , QuerySceneNodesInterface* sni)
    {
        shared_ptr<DiXMLFile> xmlfile(new DiXMLFile());
        DiXMLElement root = xmlfile->CreateRoot("Scene");
        WriteScene(scene, root);

        DiString path = pathName;
        if (DiString::EndsWith(path,"/",false) && DiString::EndsWith(path,"\\",false))
        {
            path += "\\";
        }

        DiString sceneName = scene->GetName();
        sceneName.StripFileExtension();

        DiString terrainFile = path + sceneName;
        terrainFile += ".terrain";

        DiString sceneFile = path + sceneName;
        sceneFile += ".scene";
        WriteBinaryData(scene,terrainFile);

        if (sni)
        {
            DiVector<DiCullNode*> nodes;
            sni->QueryNodes(nodes);
            DiXMLElement objs = root.CreateChild("Objects");
            size_t count = nodes.size();
            for (size_t i = 0; i < count; i++)
            {
                DiCullNode* node = nodes[i];
                if (node)
                {
                    DiXMLElement o = objs.CreateChild("Object");
                    SaveObject(o,node);
                }
            }
        }

        xmlfile->Save(sceneFile);
    }

    void DiSceneSerializer::ExportScene( const DiString& sceneName, const DiString& path , QuerySceneNodesInterface* sni)
    {
        DiScenePtr md = DiAssetManager::GetInstance().GetAsset<DiScene>(sceneName);
        if (!md)
        {
            DI_ERROR("Cannot export to scene file : %s",sceneName.c_str());
        }
        ExportScene(md,path,sni);
    }

    void DiSceneSerializer::ImportScene( DiDataStreamPtr& stream, DiScene* scene , DiCullNode* parentNode )
    {
        shared_ptr<DiXMLFile> xmlfile(new DiXMLFile());
        xmlfile->Load(stream->GetAsString());
        DiXMLElement root = xmlfile->GetRoot();

        ReadScene(scene, root);

        DiString name = scene->GetName();
        name.StripFileExtension();
        DiString terrainName = name + ".terrain";

        DiDataStreamPtr ds = DiAssetManager::GetInstance().OpenArchive(terrainName);
        if (!ds)
        {
            DI_ERROR("Cannot open scene data.");
            return;
        }

        ReadBinaryData(scene,ds);

        if (parentNode)
        {
            DiXMLElement nd = root.GetChild("Objects");
            if (nd)
            {
                DiXMLElement cd = nd.GetChild();
                while(cd)
                {
                    LoadObject(cd,parentNode);
                    cd = cd.GetNext();
                }
            }
        }
    }

    void DiSceneSerializer::WriteScene( const DiScenePtr pScene,DiXMLElement rootNode )
    {
        rootNode.SetAttribute("Name",pScene->GetName());

        DiTerrainDesc& desc = pScene->GetTerrainDesc();

        DiXMLElement terrain = rootNode.CreateChild("Terrain");
        terrain.SetFloat("GridSize",desc.mGridSize);
        terrain.SetInt("SizeX",desc.mSizeX);
        terrain.SetInt("SizeY",desc.mSizeY);
        terrain.SetFloat("TextureScale",desc.DemiureScale);

        DiTerrainDesc::TextureTable& texTable = desc.DemiureTable;
        auto itEnd = texTable.end();
        for (auto it=texTable.begin(); it!=itEnd; ++it)
        {
            DiXMLElement tt = terrain.CreateChild("TextureTable");
            tt.SetInt("ID",it->first);
            tt.SetAttribute("Name",it->second);
        }

        DiSceneManager* sm = Driver->GetSceneManager();
        if (sm)
        {
            DiXMLElement environment = rootNode.CreateChild("Environment");
            environment.SetColor("Ambient",sm->GetAmbientColor());
        }

        DiWaterMap* wm = sm->GetTerrain()->GetWaterMap();
        if (wm)
        {
            DiMaterialPtr mat = wm->GetWaterMaterial();
            if (mat)
            {
                DiXMLElement wn = rootNode.CreateChild("Water");
                
                DiMaterialSerializer mms;
                mms.SaveMaterial(wn.CreateChild("material"), mat.get());
            }
        }
    }

    void DiSceneSerializer::WriteBinaryData( const DiScenePtr pScene, const DiString& fileName )
    {
        FILE* fp = fopen(fileName.c_str(),"wb");
        DiDataStreamPtr stream(DI_NEW DiFileHandleDataStream(fp,DiDataStream::WRITE));

        char sign[4];
        sign[0] = 'M';
        sign[1] = '2';
        sign[2] = 'T';
        sign[3] = 'R';
        stream->Write(sign,4);

        uint16 version = SCENE_SERIAL_VERSION_NUM - 1;
        stream->Write(&version,sizeof(uint16));

        DiSceneBinSerializerImpl* impl = GetImplemention((DiMeshSerialVersion)version);
        if (impl)
        {
            impl->mStream = stream;
            impl->ExportSceneBin(pScene);
        }

        DI_DELETE impl;

    }

    void DiSceneSerializer::ReadBinaryData( DiScene* pDest, DiDataStreamPtr stream )
    {
        DiTerrainDesc& desc = pDest->GetTerrainDesc();
        desc.Release();

        char sign[4];
        stream->Read(sign,4);

        if (sign[0] == 'M' &&
            sign[1] == '2' &&
            sign[2] == 'T' &&
            sign[3] == 'R')
        {
            uint16 version;
            stream->Read(&version,sizeof(uint16));

            if (version >= SCENE_SERIAL_VERSION_NUM)
            {
                DI_WARNING("Unsupported scene file version: %d", version);
                return;
            }

            DiSceneBinSerializerImpl* impl = GetImplemention((DiMeshSerialVersion)version);
            if (impl)
            {
                impl->ImportSceneBin(stream,pDest);
            }

            DI_DELETE impl;
            return;
        }

        
        return;
    }

    void DiSceneSerializer::ReadScene(DiScene* pScene,DiXMLElement rootNode)
    {
        if (!rootNode.CheckName("Scene"))
        {
            DI_ERROR("Invalid scene file.");
            return;
        }

        DiXMLElement terrainNode = rootNode.GetChild("Terrain");
        if (!terrainNode)
        {
            DI_ERROR("Invalid scene file - no terrain node");
            return;
        }

        DiTerrainDesc& desc = pScene->GetTerrainDesc();

        desc.mGridSize = terrainNode.GetFloat("GridSize");
        desc.mSizeX    = terrainNode.GetInt("SizeX");
        desc.mSizeY = terrainNode.GetInt("SizeY");
        desc.DemiureScale = terrainNode.GetFloat("TextureScale");
        
        DiXMLElement child = terrainNode.GetChild();
        while(child)
        {
            if (child.CheckName("TextureTable"))
            {
                DiString name = child.GetAttribute("Name");
                DI_ASSERT(!name.empty());
                int id = child.GetInt("ID");
                
                desc.DemiureTable[id] = name;
            }

            child = child.GetNext();
        }

        DiSceneManager* sm = Driver->GetSceneManager();
        DiXMLElement envNode = rootNode.GetChild("Environment");
        if (envNode && sm)
        {
            DiColor ambient = envNode.GetColor("Ambient");

            sm->SetAmbientColor(ambient);
        }
    }

    DiSceneBinSerializerImpl* DiSceneSerializer::GetImplemention( DiMeshSerialVersion ver )
    {
        if (ver == SCENE_SERIAL_VERSION_0)
        {
            return DI_NEW DiSceneBinSerializerImpl();
        }
        else 
        {
            return NULL;
        }
    }

    void DiSceneBinSerializerImpl::ExportSceneBin( const DiScenePtr pScene )
    {
        DiTerrainDesc* desc = &pScene->GetTerrainDesc();

        ExportHeightMap(desc);

        ExportVertColor(desc);

        ExportLayerData(desc);

        ExportWaterData(desc);

        ExportFoliageData(desc);

        ExportCliffData(desc);
    }

    void DiSceneBinSerializerImpl::ImportSceneBin( DiDataStreamPtr& stream, DiScene* pDest )
    {
        DiTerrainDesc* desc = &pDest->GetTerrainDesc();
        if (!stream->Eof())
        {
            uint16 streamID = ReadChunk(stream);
            while(!stream->Eof())
            {
                switch(streamID)
                {
                case S_HEIGHT_MAP:
                    ImportHeightMap(desc,stream);
                    break;
                case S_VERT_COLOR:
                    ImportVertColor(desc,stream);
                    break;
                case S_LAYER_DATA:
                    ImportLayerData(desc,stream);
                    break;
                case S_WATER_DATA:
                    ImportWaterData(desc,stream);
                    break;
                case S_FOLIAGE_DATA:
                    ImportFoliageData(desc,stream);
                    break;
                case S_CLIFF_DATA:
                    ImportCliffData(desc,stream);
                    break;
                }

                if (!stream->Eof())
                {
                    streamID = ReadChunk(stream);
                }

            }
            if (!stream->Eof())
            {
                stream->Skip(-MSTREAM_OVERHEAD_SIZE);
            }
        }
    }

    void DiSceneBinSerializerImpl::ExportHeightMap( DiTerrainDesc* desc )
    {
        float* heights = desc->mHeightData;
        if (!heights)
        {
            WriteChunkHeader(S_HEIGHT_MAP, 0);
        }
        else
        {
            int vertNum = desc->GetVertNum();
            // �ļ�ͷ
            WriteChunkHeader(S_HEIGHT_MAP, vertNum * sizeof(float));
            WriteData(heights,sizeof(float) * vertNum,1);
        }
    }

    void DiSceneBinSerializerImpl::ExportVertColor( DiTerrainDesc* desc )
    {
        uint32* colors = desc->mColorData;
        if (!colors)
        {
            // �ļ�ͷ
            WriteChunkHeader(S_VERT_COLOR, 0);
        }
        else
        {
            // �ļ�ͷ
            int vertNum = desc->GetVertNum();
            WriteChunkHeader(S_VERT_COLOR, vertNum * sizeof(uint32));
            WriteData(colors,vertNum * sizeof(uint32),1);
        }
    }

    void DiSceneBinSerializerImpl::ExportLayerData( DiTerrainDesc* desc )
    {
        int gridNum = desc->GetGridNum();
        WriteChunkHeader(S_LAYER_DATA, gridNum * sizeof(BYTE) * TERRAIN_LAYER_NUM);

        for (BYTE i=0; i<TERRAIN_LAYER_NUM; i++)
        {
            BYTE* texData = desc->mLayers[i].DemiureId;
            if (!texData)
            {
                texData = DI_NEW BYTE[gridNum];
                memset(texData,0,sizeof(BYTE) * gridNum);
                WriteData(texData,sizeof(BYTE) * gridNum,1);
                SAFE_ARRAY_DELETE(texData);
            }
            else
            {
                WriteData(texData,sizeof(BYTE) * gridNum,1);
            }
        }
    }

    void DiSceneBinSerializerImpl::ExportWaterData( DiTerrainDesc* desc )
    {
        // ˮ
        int waterMapSize = WATER_MAP_SIZE*WATER_MAP_SIZE*sizeof(BYTE);
        int heightsSize = desc->mSizeX*desc->mSizeY*sizeof(float);
        int headSize = waterMapSize + sizeof(int) + heightsSize;

        if(!desc->mWaterMap)
        {
            waterMapSize = 0;
        }

        WriteChunkHeader(S_WATER_DATA, headSize);

        WriteData(&waterMapSize,sizeof(int),1);
        if (waterMapSize != 0)
        {
            WriteData(desc->mWaterMap,waterMapSize,1);
        }

        float* ht = desc->mWaterHeightMap;
        if (!ht)
        {
            ht = DI_NEW float[desc->mSizeX*desc->mSizeY];
            memset(ht,0,sizeof(float) * desc->mSizeX*desc->mSizeY);
            WriteData(ht,sizeof(float) * desc->mSizeX*desc->mSizeY,1);
            SAFE_ARRAY_DELETE(ht);
        }
        else
        {
            WriteData(ht,sizeof(float) * desc->mSizeX*desc->mSizeY,1);
        }
    }

    void DiSceneBinSerializerImpl::ExportFoliageData( DiTerrainDesc* desc )
    {
        // ֲ��
        WriteChunkHeader(S_FOLIAGE_DATA, 0);

        int layerNum = (int)desc->mFoliageLayers.size();
        WriteData(&layerNum,sizeof(int),1);

        for (int i=0; i < layerNum; i++)
        {
            DiFoliageLayerDesc* layerdesc = desc->mFoliageLayers[i];
            layerdesc->Save(mStream);
        }
    }

    void DiSceneBinSerializerImpl::ExportCliffData( DiTerrainDesc* desc )
    {
        WriteChunkHeader(S_CLIFF_DATA, 0);

        // cliff
        char* cliffs = desc->mCliffData;
        uint32 cliffVertNum = desc->GetCliffVertNum();    
        if (!cliffs)
        {
            cliffs = DI_NEW char[cliffVertNum];
            memset(cliffs, CLIFF_A, sizeof(char) * cliffVertNum);
            mStream->Write(cliffs, sizeof(char) * cliffVertNum);
            SAFE_ARRAY_DELETE(cliffs);
        }
        else
        {
            mStream->Write(cliffs,sizeof(char) * cliffVertNum);
        }
    }

    void DiSceneBinSerializerImpl::ImportHeightMap( DiTerrainDesc* desc, DiDataStreamPtr ds )
    {
        int vertNum = desc->GetVertNum();
        desc->mHeightData = DI_NEW float[vertNum];
        if (mCurChunkSize != 0)
        {
            ds->Read(desc->mHeightData,sizeof(float) * vertNum);
        }
        else
        {
            memset(desc->mHeightData,0,sizeof(float)*vertNum);
        }
    }

    void DiSceneBinSerializerImpl::ImportVertColor( DiTerrainDesc* desc, DiDataStreamPtr ds )
    {
        int vertNum = desc->GetVertNum();
        desc->mColorData = DI_NEW uint32[vertNum];

        if (mCurChunkSize != 0)
        {
            ds->Read(desc->mColorData,sizeof(uint32) * vertNum);
        }
        else
        {
            memset(desc->mColorData,0,sizeof(uint32)*vertNum);
        }
    }

    void DiSceneBinSerializerImpl::ImportLayerData( DiTerrainDesc* desc, DiDataStreamPtr ds )
    {
        int gridNum = desc->GetGridNum();
        
        desc->mLayers[0].DemiureId = DI_NEW BYTE[gridNum];
        desc->mLayers[1].DemiureId = DI_NEW BYTE[gridNum];

        ds->Read(desc->mLayers[0].DemiureId,sizeof(BYTE) * gridNum);
        ds->Read(desc->mLayers[1].DemiureId,sizeof(BYTE) * gridNum);
    }

    void DiSceneBinSerializerImpl::ImportWaterData( DiTerrainDesc* desc, DiDataStreamPtr ds )
    {
        int waterMapSize = 0;
        ds->Read(&waterMapSize,sizeof(int));

        if (waterMapSize != 0)
        {
            desc->mWaterMap = DI_NEW BYTE[waterMapSize];
            desc->mWaterHeightMap = DI_NEW float[desc->mSizeX*desc->mSizeY];
            ds->Read(desc->mWaterMap,waterMapSize);
            ds->Read(desc->mWaterHeightMap,desc->mSizeX*desc->mSizeY*sizeof(float));
        }
    }

    void DiSceneBinSerializerImpl::ImportFoliageData( DiTerrainDesc* desc, DiDataStreamPtr ds )
    {
        int foliageSize = 0;
        ds->Read(&foliageSize,sizeof(int));
        for (int i=0; i < foliageSize; i++)
        {
            DiFoliageLayerDesc* layerdesc = DI_NEW DiFoliageLayerDesc;
            layerdesc->Load(ds);
            desc->mFoliageLayers.push_back(layerdesc);
        }
    }

    void DiSceneBinSerializerImpl::ImportCliffData( DiTerrainDesc* desc, DiDataStreamPtr ds )
    {
        uint32 cliffVertNew = desc->GetCliffVertNum();
        desc->mCliffData = DI_NEW char[cliffVertNew];
        ds->Read(desc->mCliffData, sizeof(char) * cliffVertNew);
    }

    void DiSceneSerializer::SaveObject( DiXMLElement node,DiCullNode* cn )
    {
        node.SetAttribute("name",cn->GetName());

        node.SetVector3("pos",cn->GetDerivedPosition());
        node.SetQuaternion("rot",cn->GetDerivedOrientation());
        if (cn->GetDerivedScale() != DiVec3::UNIT_SCALE)
            node.SetVector3("scale",cn->GetDerivedScale());

        DiCullNode::ObjectIterator ci = cn->GetAttachedObjectIterator();
        while (ci.HasMoreElements())
        {
            DiTransUnitPtr tu = ci.GetNext();
            DiXMLElement tuo = node.CreateChild("Unit");
            tu->Save(tuo);
        }
    }

    void DiSceneSerializer::LoadObject(DiXMLElement node,DiCullNode* parentNode)
    {
        DiString name = node.GetAttribute("name");
        DiCullNode* cullnode = parentNode->CreateChild(name);

        DiVec3 pos = node.GetVector3("pos");
        DiQuat rot = node.GetQuaternion("rot");        
        DiVec3 scale = DiVec3::UNIT_SCALE;
        if (node.HasAttribute("scale"))
            scale = node.GetVector3("scale");

        cullnode->SetDerivedPosition(pos);
        cullnode->SetDerivedOrientation(rot);
        cullnode->SetScale(scale);

        DiXMLElement cd = node.GetChild();
        int count = 0;
        while(cd);
        {
            DiString mtname;
            mtname.Format("%s_%d",name.c_str(),count++);

            DiString tag = cd.GetName();
            if (tag == "Unit")
            {
                DiString type = cd.GetAttribute("type");

                DiTransformUnit* tunit = CreateTransformUnit(type,mtname);
                if (tunit)
                    tunit->Load(cd);
                else
                    DI_WARNING("Cannot create transform unit : %s",type.c_str());
            }

            cd = cd.GetNext();
        }
    }

    DiTransformUnit* DiSceneSerializer::CreateTransformUnit( const DiString& type ,
        const DiString& name)
    {
        if (type == "Model")
            return DI_NEW DiModel(name);
        
        return NULL;
    }
}