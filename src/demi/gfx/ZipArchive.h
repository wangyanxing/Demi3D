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

typedef struct zzip_dir		ZZIP_DIR;
typedef struct zzip_file	ZZIP_FILE;
typedef union _zzip_plugin_io zzip_plugin_io_handlers;

namespace Demi
{
    /** Zip file system
     */
    class DiZipArchive : public DiArchive
    {
    public:

        DiZipArchive(const DiString& name);

        virtual             ~DiZipArchive();

    public:

        void                Load();

        void                Unload();

        DiDataStreamPtr     Open(const DiString& filename) const;

        DiStringVecPtr      List(bool recursive = true, bool dirs = false);

        DiFileInfoListPtr   ListFileInfo(bool recursive = true, bool dirs = false);

        DiStringVecPtr      Find(const DiString& pattern, bool recursive = true,
                                bool dirs = false);

        DiFileInfoListPtr   FindFileInfo(const DiString& pattern, 
                                bool recursive = true, bool dirs = false) const;

    protected:

        /// Handle any errors from zzip
        void                CheckZzipError(int zzipError, const DiString& operation) const;
        
        /// Handle to root zip file
        ZZIP_DIR*           mZzipDir;

        /// File list (since zziplib seems to only allow scanning of dir tree once)
        DiFileInfoList      mFileList;
        
        /// A pointer to file io alternative implementation
        zzip_plugin_io_handlers* mPluginIo;
    };
    
    
    /** Specialisation of DataStream to handle streaming data from zip archives. */
    class DI_GFX_API DiZipDataStream : public DiDataStream
    {
    protected:
        ZZIP_FILE* mZzipFile;
		
		DiStaticCache<256> mCache;
    public:
        
        DiZipDataStream(ZZIP_FILE* zzipFile, size_t uncompressedSize);
        
        DiZipDataStream(const DiString& name, ZZIP_FILE* zzipFile, size_t uncompressedSize);
        
		~DiZipDataStream();
        
        size_t Read(void* buf, size_t count);
        
		size_t Write(void* buf, size_t count);
        
        void Skip(long count);
        
        void Seek( size_t pos );
        
        size_t Tell(void) const;
        
        bool Eof(void) const;
        
        void Close(void);
    };
}

#endif
