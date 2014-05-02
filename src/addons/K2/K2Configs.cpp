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

namespace Demi
{
    void DiK2Configs::Init()
    {
        // default hon media folder
        if (!CommandMgr->HasCommand("k2_media_folder"))
        {
            DiString honMediaPath = DiAssetManager::GetInstance().GetBasePath();
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
            honMediaPath += "../media_ios"; // default value
#else
            honMediaPath += "../media_hon"; // default value
#endif
            honMediaPath.SimplifyPath();
#if 1
            CommandMgr->RegisterString("k2_media_folder", honMediaPath, 0);
#else
            CommandMgr->RegisterString("k2_media_folder", "L:/Games/HON_res", 0);
#endif
        }

        // register the asset type
        DiAssetManager::GetInstance().RegisterAssetType(DiK2ModelAsset::TYPE, "", [](const DiString& name){
            return make_shared<DiK2ModelAsset>(name);
        });
    }

    void DiK2Configs::Shutdown()
    {
        DiAssetManager::GetInstance().UnregisterAssetType(DiK2ModelAsset::TYPE);
    }

    DiString DiK2Configs::GetK2MediaPath(const DiString& relativePath)
    {
        if (!RESOURCE_PACK)
        {
            DiString baseFolder = DiBase::CommandMgr->GetConsoleVar("k2_media_folder")->GetString();
            baseFolder += "/";
            baseFolder += relativePath;
            return baseFolder;
        }
        else
        {
            return relativePath;
        }
    }

    DiDataStreamPtr DiK2Configs::GetDataStream(const DiString& relPath, bool asBinary)
    {
        DiString full = GetK2MediaPath(relPath);

        if (RESOURCE_PACK)
        {
            DiDataStreamPtr data = RESOURCE_PACK->Open(full);
            return data;
        }
        else
        {
            // now we just simply using OS's file system
            FILE* fp = fopen(full.c_str(), asBinary ? "rb" : "r");
            if (!fp)
            {
                DI_WARNING("Cannot open the file: %s", full.c_str());
                return nullptr;
            }
            DiDataStreamPtr data(DI_NEW DiFileHandleDataStream(fp));
            return data;
        }
    }

    DiTexturePtr DiK2Configs::GetTexture(const DiString& relPath)
    {
        DiString baseName = relPath.ExtractFileName();
        if (baseName.empty() || baseName[0] == '$')
        {
            return DiTexture::GetDefaultTexture();
        }

        DiTexturePtr ret = DiAssetManager::GetInstance().FindAsset<DiTexture>(relPath);
        if (ret)
            return ret;

        DiString full = GetK2MediaPath(relPath);
        if(full.empty())
        {
            DI_WARNING("Empty texture file!, using default texture instead");
            return DiTexture::GetDefaultTexture();
        }
        else if(full[0] != '\\' && full[0] != '/')
        {
            full = "/"+full;
        }
        
        DiString tgaFile = full + ".tga";

#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
        DiString ddsFile = full + ".pvr";
#else
        DiString ddsFile = full + ".dds";
#endif

        if (TEXTURE_PACK)
        {
            DiDataStreamPtr data;
            
            /// TODO read the descriptor file
            tgaFile = DiK2Configs::TEXTURE_PACK_PREFIX_FOLDER + tgaFile;
            ddsFile = DiK2Configs::TEXTURE_PACK_PREFIX_FOLDER + ddsFile;

            if (TEXTURE_PACK->HasFile(ddsFile))
                data = TEXTURE_PACK->Open(ddsFile);
            else
                data = TEXTURE_PACK->Open(tgaFile);

            if (!data)
            {
                DI_WARNING("Cannot open the texture file: %s, using default texture", ddsFile.c_str());
                return DiTexture::GetDefaultTexture();
            }
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
                fp = fopen(tgaFile.c_str(), "rb");

            if (!fp)
            {
                DI_WARNING("Cannot open the texture file: %s, using default texture", ddsFile.c_str());
                return DiTexture::GetDefaultTexture();
            }
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

    bool DiK2Configs::K2ArchiveExists(const DiString& relPath)
    {
        if (RESOURCE_PACK)
        {
            return RESOURCE_PACK->HasFile(relPath);
        }
        else
        {
            DiString full = GetK2MediaPath(relPath);
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


    DiString DiK2Configs::TEXTURE_PACK_PREFIX_FOLDER;

    DiZipArchive* DiK2Configs::TEXTURE_PACK = nullptr;
    DiZipArchive* DiK2Configs::RESOURCE_PACK = nullptr;

    DiString DiK2Configs::MESH_SHADER    = "k2_color";
    DiString DiK2Configs::CLIFF_SHADER   = "k2_cliff";
    DiString DiK2Configs::TERRAIN_SHADER = "k2_terrain";
    DiString DiK2Configs::WATER_SHADER   = "k2_water";
}