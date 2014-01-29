
/********************************************************************
    File:       AssetManager.h
    Creator:    demiwangya
*********************************************************************/

#ifndef AssetManager_h__
#define AssetManager_h__

#include "GfxPrerequisites.h"
#include "Singleton.h"
#include "Asset.h"

namespace Demi 
{
    class DI_GFX_API DiAssetManager : public DiSingleton<DiAssetManager>
    {
    public:

        DiAssetManager();

        virtual ~DiAssetManager();

        _DECLARE_SINGLETON(DiAssetManager);

    public:

        static uint32                RESOURCE_SYSTEM_NUM_REFERENCE_COUNTS;

    public:

        DiAssetPtr                    GetAsset(const DiString& path, 
                                                DiAssetType type, bool ignoreError=false);

        template <class TAsset>
        shared_ptr<TAsset>            GetAsset(const DiString& path, bool ignoreError=false)
        {
            return std::dynamic_pointer_cast<TAsset>(GetAsset(path,TAsset::TYPE,ignoreError));
        }
        

        DiAssetPtr                    CreateManualAsset(const DiString& name, 
                                                DiAssetType type);

        template <class TAsset>
        shared_ptr<TAsset>            CreateManualAsset(const DiString& name)
        {
            return std::dynamic_pointer_cast<TAsset>(CreateManualAsset(name,TAsset::TYPE));
        }

        DiAssetPtr                    CreateOrReplaceAsset(const DiString& name, 
                                                DiAssetType type);

        template <class TAsset>
        shared_ptr<TAsset>            CreateOrReplaceAsset(const DiString& name)
        {
            return std::dynamic_pointer_cast<TAsset>(CreateOrReplaceAsset(name,TAsset::TYPE));
        }

        bool                          AssetExists(const DiString& name);

        void                          DeclareAsset(const DiString& fullName);
                                      
        void                          DestroyAsset(const DiString& name);
                                      
        void                          DestroyAllAssets();

        void                          AddSearchPath(const DiString& path);

        void                          ClearSearchPaths(void);

        const DiString&               GetBasePath() const { return mBasePath; }

        void                          SetBasePath(const DiString& val);
                                      
        static bool                   SearchForPath(const char* path, char* buffer, 
                                        int bufferSize, int maxRecursion);

        DiDataStreamPtr               OpenArchive(const DiString& filename, bool ignoreError=false);

        bool                          HasArchive(const DiString& filename);

        DiAssetPtr                    FindAsset(const DiString& name);

        template <class TAsset>
        shared_ptr<TAsset>            FindAsset(const DiString& name)
        {
            return std::dynamic_pointer_cast<TAsset>(FindAsset(name));
        }


    protected:

        DiAssetPtr                   LoadAsset(const DiString& path, bool ignoreError = false);

        DiAssetPtr                   LoadXMLAsset(DiDataStreamPtr data, const DiString& path);

        DiAssetPtr                   LoadTextureAsset(DiDataStreamPtr data, const DiString& path);

        DiAssetPtr                   LoadMeshAsset(DiDataStreamPtr data, const DiString& path);

        DiAssetPtr                   LoadMotionAsset(DiDataStreamPtr data, const DiString& path);

        DiAssetPtr                   LoadSceneAsset(DiDataStreamPtr data, const DiString& path);

    protected:

        typedef DiStrHash<DiAssetPtr>    AssetsTable;
        typedef DiStrHash<ArchivePtr>    ArchiveTable;

        DiVector<DiString>           mSearchPaths;

        AssetsTable                  mAssets;

        ArchiveTable                 mArchives;
                                     
        DiString                     mBasePath;

        DiArchiveManager*            mArchiveManager;
    };
}

#endif // AssetManager_h__