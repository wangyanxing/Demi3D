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

#ifndef DiArchiveManager_h__
#define DiArchiveManager_h__


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

        ArchivePtr            Load(const DiString& filename, DiArchiveType archiveType = ARCHIVE_FILE);

        void                  Unload(DiArchive* arch);
        
        void                  Unload(const DiString& filename);

        ArchiveMapIterator    GetArchiveIterator(void);

    protected:

        ArchivePtr            CreateArchiveInstance( const DiString& filename, DiArchiveType archiveType );

        ArchiveMap            mArchives;
    };
}

#endif
