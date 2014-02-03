
#include "GfxPch.h"
#include "AssetManager.h"
#include "ArchiveManager.h"
#include "Archive.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Motion.h"
#include "Scene.h"
#include "PathLib.h"

_IMPLEMENT_SINGLETON(Demi::DiAssetManager);

namespace Demi 
{
    uint32 DiAssetManager::RESOURCE_SYSTEM_NUM_REFERENCE_COUNTS = 1;

    DiAssetManager::DiAssetManager(void)
    {
        mArchiveManager = DI_NEW DiArchiveManager;
    }

    DiAssetManager::~DiAssetManager(void)
    {
        SAFE_DELETE(mArchiveManager);
    }

    DiAssetPtr DiAssetManager::GetAsset( const DiString& path, DiAssetType type, bool ignoreError)
    {
        DiAssetPtr asset = FindAsset(path);

        if(!asset)
            asset = LoadAsset(path,ignoreError);

        if(asset && asset->GetAssetType() != type)
            asset.reset();

        return asset;
    }

    void DiAssetManager::AddSearchPath( const DiString& path )
    {
        if(!path.empty())
        {
            DiString searchPath = path;
            if (!DiString::EndsWith(searchPath, "\\") &&
                !DiString::EndsWith(searchPath, "/"))
            {
                searchPath += "/";
            }
            mSearchPaths.push_back(searchPath);
        }
    }

    void DiAssetManager::ClearSearchPaths( void )
    {
        mSearchPaths.clear();
    }

    DiAssetPtr DiAssetManager::FindAsset( const DiString& name )
    {
        if( mAssets.find(name) != mAssets.end() )
        {
            return mAssets[name];
        }
        return DiAssetPtr();
    }

    DiAssetPtr DiAssetManager::LoadAsset( const DiString& path , bool ignoreError)
    {
        DiAssetPtr asset;
        DiString extension = path.GetFileExtension();
        extension.ToLower();

        if(!extension.empty())
        {
            DiDataStreamPtr buf = OpenArchive(path,ignoreError);

            if(buf)
            {
                if(extension == "mtl")      
                    asset = LoadXMLAsset(buf, path);
                else if(extension == "dds" || extension == "tga") 
                    asset = LoadTextureAsset(buf, path);
                else if (extension == "model")
                    asset = LoadMeshAsset(buf,path);
                else if (extension == "motion")
                    asset = LoadMotionAsset(buf,path);
                else if (extension == "scene")
                    asset = LoadSceneAsset(buf,path);
            }
        }

        if(asset && !asset->LoadingComplete())
        {
            asset.reset();
        }
        if(asset)
        {
            DI_ASSERT(mAssets.find(path) == mAssets.end());
            mAssets[path] = asset;
        }
        return asset;
    }

    DiAssetPtr DiAssetManager::LoadXMLAsset( DiDataStreamPtr data, const DiString& path )
    {
        DiMaterialPtr asset(DI_NEW DiMaterial(path));
        asset->Load(data);
        return asset;
    }

    DiAssetPtr DiAssetManager::LoadTextureAsset( DiDataStreamPtr data, 
        const DiString& path)
    {
        DiTexturePtr asset(DI_NEW DiTexture(path));
        asset->Load(data);
        return asset;
    }

    DiAssetPtr DiAssetManager::CreateManualAsset( const DiString& name, DiAssetType type )
    {
        DiAssetPtr asset = FindAsset(name);

        if(!asset)
        {
            if (type == ASSET_MATERIAL)
            {
                asset = DiMaterialPtr(DI_NEW DiMaterial(name));
                asset->Load();
            }
            else if (type == ASSET_TEXTURE)
            {
                asset = DiTexturePtr(DI_NEW DiTexture(name));
                asset->Load();
            }
            else if (type == ASSET_MODEL)
            {
                asset = DiMeshPtr(DI_NEW DiMesh(name));
                asset->Load();
            }
            else if (type == ASSET_MOTION)
            {
                asset = DiMotionPtr(DI_NEW DiMotion(name));
                asset->Load();
            }
            else if (type == ASSET_SCENE)
            {
                asset = DiScenePtr(DI_NEW DiScene(name));
                asset->Load();
            }
            else 
            {
                DI_WARNING("Cannot recognize the asset type : %d", (int)type);
            }
        }

        if(asset && asset->GetAssetType() != type)
        {
            asset.reset();
        }

        if(asset)
        {
            DI_ASSERT(mAssets.find(name) == mAssets.end());
            mAssets[name] = asset;
        }

        return asset;
    }

    void DiAssetManager::SetBasePath( const DiString& val )
    {
        DiString appPath = DiPathLib::GetApplicationPath();

        mBasePath = appPath + val;
        mBasePath.SimplifyPath();
        mBasePath += "/";

        DI_INFO("Set media path to: %s", mBasePath.c_str());
        DI_INFO("Searching for sub paths");

        ArchivePtr baseDir = mArchiveManager->Load(mBasePath, ARCHIVE_FILE);
        DiFileInfoListPtr fl = baseDir->ListFileInfo(true,true);
        for (auto it = fl->begin(); it != fl->end(); ++it)
        {
            ArchivePtr ar = mArchiveManager->Load(mBasePath+it->filename,ARCHIVE_FILE);

            DiStringVecPtr vec = ar->Find("*",false);
            for( auto i = vec->begin(); i != vec->end(); ++i )
            {
                mArchives[*i] = ar;
            }
        }
        AddSearchPath(mBasePath);
    }

    DiDataStreamPtr DiAssetManager::OpenArchive( const DiString& filename , bool ignoreError)
    {
        auto it = mArchives.find(filename);
        if (it != mArchives.end())
        {
            ArchivePtr ar = it->second;
            return ar->Open(filename);
        }
        if (!ignoreError)
        {
            DI_WARNING("Cannot locate the resouce file : %s",filename.c_str());
        }
        return DiDataStreamPtr();
    }

    Demi::DiAssetPtr DiAssetManager::LoadMeshAsset( DiDataStreamPtr data, const DiString& path )
    {
        DiMeshPtr asset(DI_NEW DiMesh(path));
        asset->Load(data);
        return asset;
    }

    Demi::DiAssetPtr DiAssetManager::LoadMotionAsset( DiDataStreamPtr data, const DiString& path )
    {
        DiMotionPtr asset(DI_NEW DiMotion(path));
        asset->Load(data);
        return asset;
    }

    void DiAssetManager::DestroyAsset( const DiString& name )
    {
        auto it = mAssets.find(name);

        if( it != mAssets.end() )
        {
            mAssets.erase(it);
        }
    }

    bool DiAssetManager::AssetExists( const DiString& name)
    {
        return mAssets.find(name) != mAssets.end();
    }

    void DiAssetManager::DestroyAllAssets()
    {
        mAssets.clear();
    }

    void DiAssetManager::DeclareAsset( const DiString& fullName )
    {
        DiString name = fullName.ExtractFileName();
        DiString path = fullName.ExtractDirName();
        ArchivePtr ar = mArchiveManager->Load(path,ARCHIVE_FILE);
        mArchives[name] = ar;
    }

    Demi::DiAssetPtr DiAssetManager::CreateOrReplaceAsset( const DiString& name, DiAssetType type )
    {
        DestroyAsset(name);
        return CreateManualAsset(name,type);
    }

    Demi::DiAssetPtr DiAssetManager::LoadSceneAsset( DiDataStreamPtr data, const DiString& path )
    {
        DiScenePtr asset(DI_NEW DiScene(path));
        asset->Load(data);
        return asset;
    }

    bool DiAssetManager::HasArchive( const DiString& filename )
    {
        return mArchives.find(filename) != mArchives.end();
    }
}