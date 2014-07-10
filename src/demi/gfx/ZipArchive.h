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

#ifndef DiZipArchive_h__
#define DiZipArchive_h__

#include "Archive.h"

class ZipArchive;
typedef std::shared_ptr<ZipArchive> _ZipArchivePtr;

typedef struct zzip_dir       ZZIP_DIR;
typedef struct zzip_file      ZZIP_FILE;
typedef union _zzip_plugin_io zzip_plugin_io_handlers;

namespace Demi
{
    /** Zip file system
     */
    class DI_GFX_API DiZipArchive : public DiArchive
    {
    public:

        DiZipArchive(const DiString& name);

        virtual             ~DiZipArchive();

    public:

        void                Load();

        void                Unload();

        DiDataStreamPtr     Open(const DiString& filename) const;

        /// List files recursively
        DiFileTree*         GenerateFileTree(const DiString& pattern);

        DiStringVecPtr      List(bool recursive = true, bool dirs = false);

        DiFileInfoListPtr   ListFileInfo(bool recursive = true, bool dirs = false);

        DiStringVecPtr      Find(const DiString& pattern, bool recursive = true,
                                bool dirs = false);

        DiFileInfoListPtr   FindFileInfo(const DiString& pattern, 
                                bool recursive = true, bool dirs = false) const;

        bool                HasFile(const DiString& file);
        
        DiString            GetFullPath(const DiString& filename);

    protected:

        /// File list (since zziplib seems to only allow scanning of dir tree once)
        DiFileInfoList      mFileList;
        
        _ZipArchivePtr      mZipPtr;
    };
}

#endif
