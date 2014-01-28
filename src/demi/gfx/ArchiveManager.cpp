
#include "GfxPch.h"
#include "ArchiveManager.h"
#include "Archive.h"
#include "FileArchive.h"


_IMPLEMENT_SINGLETON(Demi::DiArchiveManager);

namespace Demi 
{
    DiArchiveManager::DiArchiveManager()
    {
    }

    DiArchiveManager::~DiArchiveManager()
    {
        for( ArchiveMap::iterator it = mArchives.begin(); it != mArchives.end(); ++it )
        {
            ArchivePtr arch = it->second;
            arch->Unload();
        }
        mArchives.clear();
    }

    ArchivePtr DiArchiveManager::Load( const DiString& filename, DiArchiveType archiveType )
    {
        ArchiveMap::iterator i = mArchives.find(filename);
        ArchivePtr pArch;

        if (i == mArchives.end())
        {
            pArch = CreateArchiveInstance(filename,archiveType);
            pArch->Load();
            mArchives[filename] = pArch;
        }
        else
        {
            pArch = i->second;
        }
        return pArch;
    }

    void DiArchiveManager::Unload( DiArchive* arch )
    {
        Unload(arch->GetName());
    }

    void DiArchiveManager::Unload( const DiString& filename )
    {
        ArchiveMap::iterator i = mArchives.find(filename);

        if (i != mArchives.end())
        {
            i->second->Unload();
            mArchives.erase(i);
        }
    }

    DiArchiveManager::ArchiveMapIterator DiArchiveManager::GetArchiveIterator( void )
    {
        return ArchiveMapIterator(mArchives.begin(), mArchives.end());
    }

    ArchivePtr DiArchiveManager::CreateArchiveInstance( const DiString& filename, DiArchiveType archiveType )
    {
        switch (archiveType)
        {
        case ARCHIVE_FILE:
            return ArchivePtr(DI_NEW DiFileArchive(filename));
            break;
        default:
            DI_ERROR("没有合适的文件系统可用");
            return ArchivePtr();
        }
    }
}