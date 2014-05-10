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
#include "K2Configs.h"
#include "AssetManager.h"
#include "Command.h"
#include "ConsoleVariable.h"
#include "K2Asset.h"
#include "PathLib.h"
#include "Texture.h"
#include "PathLib.h"
#include "ZipArchive.h"
#include "math/euler.h"
#include "XMLFile.h"
#include "XMLElement.h"

namespace Demi
{
    void DiK2Configs::Init()
    {
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        LoadConfig("ConfigWin32.xml");
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX
        LoadConfig("ConfigOSX.xml");
#else
        LoadConfig("ConfigiOS.xml");
#endif

        DiString resPath;
        if (DiBase::CommandMgr->GetConsoleVar("k2_resource_path"))
        {
            resPath = DiBase::CommandMgr->GetConsoleVar("k2_resource_path")->GetString();
        }

        DiString texPath;
        if (DiBase::CommandMgr->GetConsoleVar("k2_texture_path"))
        {
            texPath = DiBase::CommandMgr->GetConsoleVar("k2_texture_path")->GetString();
        }

        // if zip
        if (resPath.CheckFileExtension("zip") || resPath.CheckFileExtension("s2z"))
            SetK2ResourcePack(resPath, texPath);

        // register the asset type
        DiAssetManager::GetInstance().RegisterAssetType(DiK2ModelAsset::TYPE, "", [](const DiString& name){
            return make_shared<DiK2ModelAsset>(name);
        });
    }

    void DiK2Configs::Shutdown()
    {
        DiAssetManager::GetInstance().UnregisterAssetType(DiK2ModelAsset::TYPE);

#ifdef _K2_RECORD_USED_RESOURCE
        DumpUsedRes();
#endif
    }

    DiString DiK2Configs::GetK2MediaPath(const DiString& relativePath, bool texture)
    {
        auto var = DiBase::CommandMgr->GetConsoleVar(texture ? "k2_texture_path" : "k2_resource_path");
        if (!var)
        {
            DI_WARNING("Invalid k2 media path!");
            return DiString::BLANK;
        }

        if (!RESOURCE_PACK)
        {
            DiString baseFolder = var->GetString();
            baseFolder += "/";
            baseFolder += relativePath;
            return baseFolder;
        }
        else
        {
            return relativePath;
        }
    }

    DiDataStreamPtr DiK2Configs::GetDataStream(const DiString& relPath, K2ResourceType type)
    {
#ifdef _K2_RECORD_USED_RESOURCE
        sUsedResources.insert(relPath);
#endif

        DiString full = GetK2MediaPath(relPath, type == K2_RES_TEXTURE);

        if (RESOURCE_PACK)
        {
            DiDataStreamPtr data = RESOURCE_PACK->Open(full);
            return data;
        }
        else
        {
            // now we just simply using OS's file system
            FILE* fp = fopen(full.c_str(), type == K2_RES_XML ? "r" : "rb");
            if (!fp)
            {
                DI_WARNING("Cannot open the file: %s", full.c_str());
                return nullptr;
            }
            DiDataStreamPtr data(DI_NEW DiFileHandleDataStream(fp));
            return data;
        }
    }
    
    DiTexturePtr DiK2Configs::GetSpecialTexture(const DiString& name)
    {
        static DiMap<DiString, DefaultTextureType> sMap;
        if(sMap.empty())
        {
            sMap["$black"] = DEFAULT_BLACK;
            sMap["$white"] = DEFAULT_WHITE;
            sMap["$flat"]  = DEFAULT_FLAT;
        }
        auto i = sMap.find(name);
        if(i != sMap.end())
        {
            return DiTexture::GetDefaultTexture(i->second);
        }
        return DiTexture::GetDefaultTexture();
    }

    DiTexturePtr DiK2Configs::GetTexture(const DiString& relPath)
    {
        DiString baseName = relPath.ExtractFileName();
        if (baseName.empty() || baseName[0] == '$')
        {
            return DiK2Configs::GetSpecialTexture(baseName);
        }

        DiTexturePtr ret = DiAssetManager::GetInstance().FindAsset<DiTexture>(relPath);
        if (ret)
            return ret;

        DiString full = GetK2MediaPath(relPath, true);
        if(full.empty())
        {
            DI_WARNING("Empty texture file!, using default texture instead");
            return DiTexture::GetDefaultTexture();
        }
        
        if (TEXTURE_PACK && full[0] != '\\' && full[0] != '/')
        {
            full = "/"+full;
        }

        DiString tgaExt = ".tga";
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
        DiString compExt = ".pvr";
#else
        DiString compExt = ".dds";
#endif

        DiString tgaFile = full + tgaExt;
        DiString ddsFile = full + compExt;
        bool useTga = false;

        if (TEXTURE_PACK)
        {
            DiDataStreamPtr data;
            
            /// TODO read the descriptor file
            tgaFile = DiK2Configs::TEXTURE_PACK_PREFIX_FOLDER + tgaFile;
            ddsFile = DiK2Configs::TEXTURE_PACK_PREFIX_FOLDER + ddsFile;

            if (TEXTURE_PACK->HasFile(ddsFile))
                data = TEXTURE_PACK->Open(ddsFile);
            else
            {
                data = TEXTURE_PACK->Open(tgaFile);
                useTga = true;
            }

            if (!data)
            {
                DI_WARNING("Cannot open the texture file: %s, using default texture", ddsFile.c_str());
                return DiTexture::GetDefaultTexture();
            }

#ifdef _K2_RECORD_USED_RESOURCE
            sUsedResources.insert(relPath + (useTga ? tgaExt : compExt));
#endif
            ret = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>(relPath);
            ret->Load(data);
            return ret;
        }
        else
        {
            FILE* fp = NULL;

            // try dds
            fp = fopen(ddsFile.c_str(), "rb");

            // try tga
            if (!fp)
            {
                fp = fopen(tgaFile.c_str(), "rb");
                useTga = true;
            }

            if (!fp)
            {
                DI_WARNING("Cannot open the texture file: %s, using default texture", ddsFile.c_str());
                return DiTexture::GetDefaultTexture();
            }

#ifdef _K2_RECORD_USED_RESOURCE
            sUsedResources.insert(relPath + (useTga ? tgaExt : compExt));
#endif
            DiDataStreamPtr texData(DI_NEW DiFileHandleDataStream(relPath, fp));

            ret = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>(relPath);
            ret->Load(texData);
            return ret;
        }
    }

    DiQuat DiK2Configs::ConvertAngles(const DiVec3& eulerrot)
    {
        DiQuat q;
        DiVec3 euler(DiDegree(eulerrot.x).valueRadians(),
            DiDegree(eulerrot.y).valueRadians(), DiDegree(eulerrot.z).valueRadians());
        DiEuler::ToQuat(q, euler, DiEuler::YXZ);
        return q;
    }

    bool DiK2Configs::K2ArchiveExists(const DiString& relPath, bool texture)
    {
        if (RESOURCE_PACK)
        {
            if (texture && TEXTURE_PACK)
                return TEXTURE_PACK->HasFile(relPath);
            else
                return RESOURCE_PACK->HasFile(relPath);
        }
        else
        {
            DiString full = GetK2MediaPath(relPath, texture);
            return DiPathLib::FileExisted(full);
        }
    }

    DiString DiK2Configs::GetShader(const DiString& shader)
    {
        static DiStrHash<DiString> table;
        if (table.empty())
        {
            table["mesh_color_cliff"] = CLIFF_SHADER;
            table["meshc_color_water5"] = WATER_SHADER;
        }

        auto i = table.find(shader);
        if (i != table.end())
            return i->second;
        else
            return MESH_SHADER;
    }

    void DiK2Configs::SetK2ResourcePack(const DiString& resPack, const DiString& texturePack)
    {
        DiTimer timer;

        DiK2Configs::RESOURCE_PACK = DI_NEW DiZipArchive(resPack);
        DiK2Configs::TEXTURE_PACK = DI_NEW DiZipArchive(texturePack);

        DiK2Configs::RESOURCE_PACK->Load();
        DiK2Configs::TEXTURE_PACK->Load();

        double loadingTime = timer.GetElapse();
        DI_LOG("Zip files loading time: %f", loadingTime);
    }

    void DiK2Configs::SetK2ResourcePack(const DiString& resPack)
    {
        DiTimer timer;

        DiK2Configs::RESOURCE_PACK = DI_NEW DiZipArchive(resPack);
        DiK2Configs::TEXTURE_PACK = RESOURCE_PACK;

        DiK2Configs::RESOURCE_PACK->Load();

        double loadingTime = timer.GetElapse();
        DI_LOG("Zip files loading time: %f", loadingTime);
    }

    void DiK2Configs::ReleaseK2Packs()
    {
        SAFE_DELETE(DiK2Configs::RESOURCE_PACK);
        SAFE_DELETE(DiK2Configs::TEXTURE_PACK);
    }

#ifdef _K2_RECORD_USED_RESOURCE
    DiSet<DiString> DiK2Configs::sUsedResources;

    void DiK2Configs::DumpUsedRes()
    {
        DiString path = DiPathLib::GetApplicationPath();
        path += "used_k2_res.txt";

        FILE* fp = fopen(path.c_str(), "w+");
        for (auto i = sUsedResources.begin(); i != sUsedResources.end(); ++i)
        {
            fprintf(fp, "%s\n", i->c_str());
        }
        fclose(fp);
    }
#endif

    void DiK2Configs::LoadConfig(const DiString& config)
    {
        auto file = DiAssetManager::GetInstance().OpenArchive(config, true);
        if (!file)
        {
            DI_WARNING("Failed to load the config: %s", config.c_str());
            return;
        }

        shared_ptr<DiXMLFile> xmlfile(new DiXMLFile());
        xmlfile->Load(file->GetAsString());
        DiXMLElement node = xmlfile->GetRoot();
        DiXMLElement child = node.GetChild();
        while (child)
        {
            DiString name = child.GetName();
            if (name == "Property")
            {
                DiString key = child.GetAttribute("key");
                DiString val = child.GetAttribute("value");
                CommandMgr->RegisterString(key, val, 0);
            }

            child = child.GetNext();
        }
    }

    DiString DiK2Configs::TEXTURE_PACK_PREFIX_FOLDER;

    DiZipArchive* DiK2Configs::TEXTURE_PACK  = nullptr;
    DiZipArchive* DiK2Configs::RESOURCE_PACK = nullptr;

    DiString DiK2Configs::MESH_SHADER    = "k2_color";
    DiString DiK2Configs::CLIFF_SHADER   = "k2_cliff";
    DiString DiK2Configs::TERRAIN_SHADER = "k2_terrain";
    DiString DiK2Configs::WATER_SHADER   = "k2_water";
}