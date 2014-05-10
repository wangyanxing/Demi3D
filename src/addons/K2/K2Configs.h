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

#ifndef DiK2Configs_h__
#define DiK2Configs_h__

#include "K2Prerequisites.h"

//#define _K2_RECORD_USED_RESOURCE

namespace Demi
{

    enum K2ResourceType
    {
        K2_RES_XML,
        K2_RES_BINARY,
        K2_RES_TEXTURE
    };

    /** Global configuration manager
        and some misc functions
     */
    class DEMI_K2_API DiK2Configs : public DiBase
    {
    public:

        static void Init();

        static void Shutdown();

        static void LoadConfig(const DiString& config);

        static DiString GetK2MediaPath(const DiString& relativePath, bool texture);

        /** Get the real data stream by a relative path
            e.g. we can use "heroes/aluna/model.mdf" as the argument
         */
        static DiDataStreamPtr GetDataStream(const DiString& relPath, K2ResourceType type);

        static bool K2ArchiveExists(const DiString& relPath, bool texture);

        /** Load and get the texture file by a relative path
            we don't need extension here 
         */
        static DiTexturePtr GetTexture(const DiString& relPath);
        
        /** Some textures are begin with '$'
         */
        static DiTexturePtr GetSpecialTexture(const DiString& name);

        static DiQuat ConvertAngles(const DiVec3& eulerrot);
        
        /** Map the k2's shader to our shader file
         */
        static DiString GetShader(const DiString& shader);
        
        /** Directly use the pack files
        */
        static void SetK2ResourcePack(const DiString& resPack, const DiString& texturePack);

        static void SetK2ResourcePack(const DiString& resPack);

        static void ReleaseK2Packs();

        static DiString TERRAIN_SHADER;

        static DiString CLIFF_SHADER;
        
        static DiString WATER_SHADER;
        
        static DiString MESH_SHADER;

        static DiZipArchive* RESOURCE_PACK;

        static DiZipArchive* TEXTURE_PACK;

        static DiString TEXTURE_PACK_PREFIX_FOLDER;

#ifdef _K2_RECORD_USED_RESOURCE
        static DiSet<DiString> sUsedResources;

        static void DumpUsedRes();
#endif
    };
}

#endif