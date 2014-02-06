
/********************************************************************
    File:       ArchiveManager.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GfxPrerequisites.h" 
#include "Singleton.h"
#include "Archive.h"

namespace Demi
{
    class DI_GFX_API DiArchiveManager : public DiSingleton<DiArchiveManager>
    {
    public:

        typedef DiMap<DiString, ArchivePtr> ArchiveMap;

        typedef DiMapIterator<ArchiveMap> ArchiveMapIterator;

    public:

        DiArchiveManager();

        ~DiArchiveManager();

        _DECLARE_SINGLETON(DiArchiveManager)

    public:

        ArchivePtr            Load( const DiString& filename, DiArchiveType archiveType);

        void                  Unload(DiArchive* arch);
        
        void                  Unload(const DiString& filename);

        ArchiveMapIterator    GetArchiveIterator(void);

    protected:

        ArchivePtr            CreateArchiveInstance( const DiString& filename, DiArchiveType archiveType );

        ArchiveMap            mArchives;
    };
}