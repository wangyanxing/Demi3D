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
        DiString baseFolder = DiBase::CommandMgr->GetConsoleVar("k2_media_folder")->GetString();
        baseFolder += "/";
        baseFolder += relativePath;
        return baseFolder;
    }

    DiDataStreamPtr DiK2Configs::GetDataStream(const DiString& relPath, bool asBinary)
    {
        DiString full = GetK2MediaPath(relPath);

        // now we just simply using OS's file system
        FILE* fp = fopen(full.c_str(), asBinary?"rb":"r");
        if (!fp)
        {
            DI_WARNING("Cannot open the file: %s", full.c_str());
            return nullptr;
        }
        DiDataStreamPtr data(DI_NEW DiFileHandleDataStream(fp));
        return data;
    }

    DiTexturePtr DiK2Configs::GetTexture(const DiString& relPath)
    {
        DiTexturePtr ret = DiAssetManager::GetInstance().FindAsset<DiTexture>(relPath);
        if (ret)
            return ret;

        DiString full = GetK2MediaPath(relPath);

        DiString tgaFile = full + ".tga";
        
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
        DiString ddsFile = full + ".pvr";
#else
        DiString ddsFile = full + ".dds";
#endif

        FILE* fp = NULL;

        // if the file is a tga image, maybe we should change it to .dds
        if (!DiPathLib::FileExisted(tgaFile))
        {
            // try dds
            fp = fopen(ddsFile.c_str(), "rb");
        }
        else
        {
            // try tga
            fp = fopen(tgaFile.c_str(), "rb");
        }

        if (!fp)
        {
            DI_WARNING("Cannot open the texture file: %s", ddsFile.c_str());
            return nullptr;
        }
        DiDataStreamPtr texData(DI_NEW DiFileHandleDataStream(relPath,fp));

        ret = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>(relPath);
        ret->Load(texData);
        return ret;
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
        // TODO ZIP
        DiString full = GetK2MediaPath(relPath);
        return DiPathLib::FileExisted(full);
    }

    DiString DiK2Configs::GetShader(const DiString& shader)
    {
        static DiStrHash<DiString> table;
        if (table.empty())
        {
            table["mesh_color_cliff"] = CLIFF_SHADER;
        }

        auto i = table.find(shader);
        if (i != table.end())
            return i->second;
        else
            return MESH_SHADER;
    }

    DiString DiK2Configs::MESH_SHADER    = "k2_color";
    DiString DiK2Configs::CLIFF_SHADER   = "k2_cliff";
    DiString DiK2Configs::TERRAIN_SHADER = "k2_terrain";
}