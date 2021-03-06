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
    DiAssetManager::DiAssetManager(void)
    {
        mArchiveManager = DI_NEW DiArchiveManager;
        
        //register built-in types
        
        RegisterAssetType("Material","mtl",[](const DiString& name){
            return make_shared<DiMaterial>(name);
        });
        
        RegisterAssetType("Model","model",[](const DiString& name){
            return make_shared<DiMesh>(name);
        });
        
        RegisterAssetType("Motion","motion",[](const DiString& name){
            return make_shared<DiMotion>(name);
        });
        
        RegisterAssetType("Scene","scene",[](const DiString& name){
            return make_shared<DiScene>(name);
        });
        
        DiVector<DiString> imageExts;
        imageExts.push_back("jpg");
        imageExts.push_back("tga");
        imageExts.push_back("dds");
        imageExts.push_back("pvr");
        imageExts.push_back("png");
        
        RegisterAssetType("Texture",imageExts,[](const DiString& name){
            return make_shared<DiTexture>(name);
        });
    }

    DiAssetManager::~DiAssetManager(void)
    {
        SAFE_DELETE(mArchiveManager);
    }

    DiAssetPtr DiAssetManager::GetAsset(const DiString& path, const DiString& type, bool ignoreError)
    {
        DiAssetPtr asset = FindAsset(path);

        if(!asset)
            asset = LoadAsset(path,type,ignoreError);

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
    
    ArchivePtr DiAssetManager::FindDirArchive(const DiString& name)
    {
        auto it = mDirArchives.find(name);
        if( it != mDirArchives.end() )
        {
            return it->second;
        }
        return nullptr;
    }

    DiAssetPtr DiAssetManager::FindAsset( const DiString& name )
    {
        auto it = mAssets.find(name);
        if( it != mAssets.end() )
        {
            return it->second;
        }
        return nullptr;
    }

    DiAssetPtr DiAssetManager::LoadAsset(const DiString& path, const DiString& type, bool ignoreError)
    {
        DiString filepath = path;
        DiAssetPtr asset;
        DiString extension = filepath.GetFileExtension();
        extension.ToLower();
        
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
        if(extension == "dds")
        {
            filepath = filepath.ExtractBaseName() + ".pvr";
        }
#endif

        // if we have an extension loader, use it
        if(!extension.empty() && mExtensionLoaders.find(extension) != mExtensionLoaders.end())
        {
            DiDataStreamPtr buf = OpenArchive(filepath,ignoreError);

            auto it = mExtensionLoaders.find(extension);
            if(buf)
            {
                asset = it->second(filepath);
                asset->Load(buf);
            }
            else
                DI_WARNING("Cannot load the asset :%s", filepath.c_str());
        }
        // otherwise we just call Load() without parameter
        else {
            auto it = mAssetLoaders.find(type);
            if (it != mAssetLoaders.end())
            {
                asset = it->second(filepath);
                asset->Load();
            }
            else
                DI_WARNING("Cannot load the asset :%s", filepath.c_str());
        }

        if(asset && !asset->LoadingComplete())
        {
            asset.reset();
        }
        if(asset)
        {
            mAssets[filepath] = asset;
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

    DiAssetPtr DiAssetManager::CreateManualAsset(const DiString& name, const DiString& type)
    {
        DiAssetPtr asset = FindAsset(name);

        if(!asset)
        {
            auto it = mAssetLoaders.find(type);
            if (it != mAssetLoaders.end())
            {
                asset = it->second(name);
                asset->Load();
            }
            else
            {
                DI_WARNING("Cannot create asset, unregistered asset type: %s", type.c_str());
            }
        }

        if(asset && asset->GetAssetType() != type)
            asset.reset();

        if(asset)
        {
            DI_ASSERT(mAssets.find(name) == mAssets.end());
            mAssets[name] = asset;
        }

        return asset;
    }
    
    ArchivePtr DiAssetManager::GetArchive(const DiString& filename)
    {
        auto it = mArchives.find(filename);
        if (it != mArchives.end())
        {
            ArchivePtr ar = it->second;
            return ar;
        }
        return nullptr;
    }

    bool DiAssetManager::SetBasePath(const DiString& val)
    {
        DiString appPath = DiPathLib::GetApplicationPath();

        DiString path = appPath + val;
        path.SimplifyPath();
        
        mDirArchives.clear();

        DI_LOG("Try to locate the media path: %s", path.c_str());
        // test if this folder is existed
        if (!DiPathLib::FileExisted(path))
        {
            return false;
        }
        
        mBasePath = path + "/";

        DI_INFO("Set media path to: %s", mBasePath.c_str());
        DI_INFO("Searching for sub paths");
        
        if(!path.empty() && !DiString::EndsWith(path, "\\/"))
            path += "/";

        DiVector<ArchivePtr> zips;

        ArchivePtr baseDir = mArchiveManager->Load(mBasePath);
        mRootDirArchive = baseDir;
        auto baseName = baseDir->GetName();
        baseName.Replace(path.c_str(), "");
        mDirArchives[baseName] = baseDir;
        
        DiFileInfoListPtr fl = baseDir->ListFileInfo(true,true);
        for (auto it = fl->begin(); it != fl->end(); ++it)
        {
            ArchivePtr ar = mArchiveManager->Load(it->filename);

            auto curname = ar->GetName();
            curname.Replace(path.c_str(), "");
            mDirArchives[curname] = ar;
            
            auto vec = ar->FindFileInfo("*", false);
            for( auto i = vec->begin(); i != vec->end(); ++i )
            {
                DiString ext = i->basename.GetFileExtension();
                if (stricmp("zip", ext.c_str()) == 0)
                {
                    ArchivePtr a = mArchiveManager->Load(i->filename, ARCHIVE_ZIP);
                    zips.push_back(a);
                }
                else
                {
                    mArchives[i->basename] = ar;
                }
            }
        }

        // search files in zip packs
        for (auto it = zips.begin(); it != zips.end(); ++it)
        {
            auto vec = (*it)->FindFileInfo("*", false);
            for (auto i = vec->begin(); i != vec->end(); ++i)
            {
                mArchives[i->basename] = *it;
            }
        }

        AddSearchPath(mBasePath);
        return true;
    }
    
    DiString DiAssetManager::GetArchivePath(const DiString& filename, bool ignoreError)
    {
        auto it = mArchives.find(filename);
        if (it != mArchives.end())
        {
            ArchivePtr ar = it->second;
            return ar->GetFullPath(filename);
        }
        if (!ignoreError)
        {
            DI_WARNING("Cannot locate the resouce file : %s",filename.c_str());
        }
        return DiString::BLANK;
    }
    
    void DiAssetManager::IterateArchive(const DiString& extension, std::function<void(const DiString& name, ArchivePtr arc)> func)
    {
        for (auto& it : mArchives)
        {
            if(it.first.CheckFileExtension(extension))
            {
                func(it.first, it.second);
            }
        }
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
        ArchivePtr ar = mArchiveManager->Load(path);
        mArchives[name] = ar;
    }

    Demi::DiAssetPtr DiAssetManager::CreateOrReplaceAsset(const DiString& name, const DiString& type)
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
    
    bool DiAssetManager::HasAsset(const DiString& filename)
    {
        return mAssets.find(filename) != mAssets.end();
    }

    bool DiAssetManager::HasArchive( const DiString& filename )
    {
        return mArchives.find(filename) != mArchives.end();
    }
    
    void DiAssetManager::RegisterAssetType(const DiString& type,
        const DiString& extension, const AssetLoaderFunc& loader)
    {
        mAssetLoaders[type] = loader;
        mExtensionLoaders[extension] = loader;
    }
    
    void DiAssetManager::RegisterAssetType(const DiString& type,
        const DiVector<DiString>& extensions, const AssetLoaderFunc& loader)
    {
        mAssetLoaders[type] = loader;
        for (auto i = extensions.begin(); i != extensions.end(); ++i)
            mExtensionLoaders[*i] = loader;
    }

    void DiAssetManager::UnregisterAssetType(const DiString& type)
    {
        auto it = mAssetLoaders.find(type);
        if (it != mAssetLoaders.end())
            mAssetLoaders.erase(it);
    }

    DiFileTree* DiAssetManager::GenerateFileTree(const DiString& pattern)
    {
        DiFileTree* node = nullptr;
        
        if(mRootDirArchive)
        {
            mRootDirArchive->GenerateFileTree(node, pattern);
        }
    
        return node;
    }
    
    void DiAssetManager::RegisterManualLoader(const DiString& type, std::function<DiAssetPtr(const DiString&)> func)
    {
        mManualLoaders[type] = func;
    }
}