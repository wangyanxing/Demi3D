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
#include "ZipArchive.h"
#include "DataStream.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "zzip/zzip.h"
#include "zzip/plugin.h"

namespace Demi
{
    /// Utility method to format out zzip errors
    DiString GetZzipErrorDescription(zzip_error_t zzipError)
    {
        DiString errorMsg;
        switch (zzipError)
        {
            case ZZIP_NO_ERROR:
                break;
            case ZZIP_OUTOFMEM:
                errorMsg = "Out of memory.";
                break;
            case ZZIP_DIR_OPEN:
            case ZZIP_DIR_STAT:
            case ZZIP_DIR_SEEK:
            case ZZIP_DIR_READ:
                errorMsg = "Unable to read zip file.";
                break;
            case ZZIP_UNSUPP_COMPR:
                errorMsg = "Unsupported compression format.";
                break;
            case ZZIP_CORRUPTED:
                errorMsg = "Corrupted archive.";
                break;
            default:
                errorMsg = "Unknown error.";
                break;            
        };
        
        return errorMsg;
    }
    
    DiZipArchive::DiZipArchive( const DiString& name)
        : DiArchive(name, ARCHIVE_FILE),
        mZzipDir(nullptr),
        mPluginIo(nullptr)
    {
    }

    DiZipArchive::~DiZipArchive()
    {
        Unload();
    }
    
    void DiZipArchive::CheckZzipError(int zzipError, const DiString& operation) const
    {
        if (zzipError != ZZIP_NO_ERROR)
        {
            DiString errorMsg = GetZzipErrorDescription(static_cast<zzip_error_t>(zzipError));
            DI_WARNING("zziplib error: %s, operation: %s", errorMsg.c_str(), operation.c_str());
        }
    }
    
    void DiZipArchive::Load()
    {
        if (!mZzipDir)
        {
            DI_INFO("Loading zip archive: %s", mName.c_str());

            zzip_error_t zzipError;
            mZzipDir = zzip_dir_open_ext_io(mName.c_str(), &zzipError, 0, mPluginIo);
            CheckZzipError(zzipError, "Opening archive");
            
            // Cache names
            ZZIP_DIRENT zzipEntry;
            while (zzip_dir_read(mZzipDir, &zzipEntry))
            {
                DiFileInfo info;
				info.archive = this;
                
                info.filename = zzipEntry.d_name;
                info.basename = info.filename.ExtractFileName();
                info.path = info.filename.ExtractDirName();
                
                // Get sizes
                info.compressedSize = static_cast<uint32>(zzipEntry.d_csize);
                info.uncompressedSize = static_cast<uint32>(zzipEntry.st_size);
                
                // folder entries
                if (info.basename.empty())
                {
                    info.filename = info.filename.substr(0, info.filename.length () - 1);
                    info.basename = info.filename.ExtractFileName();
                    info.path = info.filename.ExtractDirName();
                    
                    // Set compressed size to -1 for folders; anyway nobody will check
                    // the compressed size of a folder, and if he does, its useless anyway
                    info.compressedSize = size_t (-1);
                }
			
                mFileList.push_back(info);
                
            }
            
        }
    }
    
    void DiZipArchive::Unload()
    {
        if (mZzipDir)
        {
            zzip_dir_close(mZzipDir);
            mZzipDir = nullptr;
            mFileList.clear();
        }
    }
    
    DiString DiZipArchive::GetFullPath(const DiString& filename)
    {
        return filename;
    }

    DiDataStreamPtr DiZipArchive::Open( const DiString& filename) const
    {
        DiString lookUpFileName = filename;
        
        // Format not used here (always binary)
        ZZIP_FILE* zzipFile =
        zzip_file_open(mZzipDir, lookUpFileName.c_str(), ZZIP_ONLYZIP | ZZIP_CASELESS);
        if (!zzipFile) // Try if we find the file
        {
            const DiFileInfoListPtr fileNfo = FindFileInfo(lookUpFileName, true);
            if (fileNfo->size() == 1) // If there are more files with the same do not open anyone
            {
                DiFileInfo info = fileNfo->at(0);
                lookUpFileName = info.filename;
                zzipFile = zzip_file_open(mZzipDir, lookUpFileName.c_str(), ZZIP_ONLYZIP | ZZIP_CASELESS); // When an error happens here we will catch it below
            }
        }
        
        if (!zzipFile)
		{
            int zerr = zzip_error(mZzipDir);
            DiString zzDesc = GetZzipErrorDescription((zzip_error_t)zerr);
            DI_WARNING("Unable to open zip archive: %s. Error: %s", lookUpFileName.c_str(), zzDesc.c_str());
			return nullptr;
		}
        
		// Get uncompressed size too
		ZZIP_STAT zstat;
		zzip_dir_stat(mZzipDir, lookUpFileName.c_str(), &zstat, ZZIP_CASEINSENSITIVE);
        
        // Construct & return stream
        return DiDataStreamPtr(DI_NEW DiZipDataStream(lookUpFileName, zzipFile, static_cast<size_t>(zstat.st_size)));
    }

    DiStringVecPtr DiZipArchive::List( bool recursive /*= true*/,
                                        bool dirs /*= false*/ )
    {
        DiStringVecPtr ret = DiStringVecPtr(DI_NEW StringVec());
        for (auto i = mFileList.begin(); i != mFileList.end(); ++i)
            if ((dirs == (i->compressedSize == size_t (-1))) &&
                (recursive || i->path.empty()))
                ret->push_back(i->filename);
        
        return ret;
    }

    DiFileTree* DiZipArchive::GenerateFileTree(const DiString& pattern)
    {
        bool wildCard = pattern.find("*") != DiString::npos;

        DiFileTree* node = DI_NEW DiFileTree();
        node->folder = true;
        for (auto i = mFileList.begin(); i != mFileList.end(); ++i)
        {
            if (false == (i->compressedSize == size_t(-1)) && i->basename[0]!='.')
            {
                if (!DiString::MatchPatternEx(i->basename, pattern))
                    continue;

                DiVector<DiString> tokens = i->filename.Tokenize("/");
                DiFileTree* cur = node;
                for (size_t t = 0; t < tokens.size(); ++t)
                {
                    if (t < tokens.size() - 1)
                    {
                        cur = cur->AddChild(tokens[t]);
                        cur->folder = true;
                    }
                    else
                    {
                        if (wildCard)
                        {
                            if (DiString::MatchPatternEx(tokens[t], pattern, false))
                            {
                                cur = cur->AddChild(tokens[t]);
                                cur->folder = false;
                            }
                        }
                        else
                        {
                            if (tokens[t] == pattern)
                            {
                                cur = cur->AddChild(tokens[t]);
                                cur->folder = false;
                            }
                        }
                    }
                }
            }
        }
        return node;
    }

    DiFileInfoListPtr DiZipArchive::ListFileInfo( bool recursive /*= true*/,
                                                bool dirs /*= false*/ )
    {
        DiFileInfoListPtr fil(DI_NEW DiFileInfoList);
        for (auto i = mFileList.begin(); i != mFileList.end(); ++i)
            if ((dirs == (i->compressedSize == size_t (-1))) &&
                (recursive || i->path.empty()))
                fil->push_back(*i);
        
        return fil;
    }

    DiStringVecPtr DiZipArchive::Find( const DiString& pattern,
        bool recursive /*= true*/, bool dirs /*= false*/ )
    {
        DiStringVecPtr ret(DI_NEW StringVec);
        // If pattern contains a directory name, do a full match
        bool full_match = (pattern.find ('/') != DiString::npos) ||
            (pattern.find ('\\') != DiString::npos);
		bool wildCard = pattern.find("*") != DiString::npos;
        
        for (auto i = mFileList.begin(); i != mFileList.end(); ++i)
            if ((dirs == (i->compressedSize == size_t (-1))) &&
                (recursive || full_match || wildCard))
                // Check basename matches pattern (zip is case insensitive)
                if (DiString::MatchPatternEx(full_match ? i->filename : i->basename, pattern, false))
                    ret->push_back(i->filename);
        
        return ret;
    }

    Demi::DiFileInfoListPtr DiZipArchive::FindFileInfo( const DiString& pattern,
        bool recursive /*= true*/, bool dirs /*= false*/ ) const
    {
        DiFileInfoListPtr ret(DI_NEW DiFileInfoList);
        // If pattern contains a directory name, do a full match
        bool full_match = (pattern.find ('/') != DiString::npos) ||
        (pattern.find ('\\') != DiString::npos);
		bool wildCard = pattern.find("*") != DiString::npos;
        
        for (auto i = mFileList.begin(); i != mFileList.end(); ++i)
            if ((dirs == (i->compressedSize == size_t (-1))) &&
                (recursive || full_match || wildCard))
                // Check name matches pattern (zip is case insensitive)
                if (DiString::MatchPatternEx(full_match ? i->filename : i->basename, pattern, false))
                    ret->push_back(*i);
        
        return ret;
    }

    bool DiZipArchive::HasFile(const DiString& file)
    {
        ZZIP_FILE* zzipFile =
            zzip_file_open(mZzipDir, file.c_str(), ZZIP_ONLYZIP | ZZIP_CASELESS);
        return zzipFile != NULL;
    }

    DiZipDataStream::DiZipDataStream(ZZIP_FILE* zzipFile, size_t uncompressedSize)
    : mZzipFile(zzipFile)
    {
		mSize = uncompressedSize;
    }
  
    DiZipDataStream::DiZipDataStream(const DiString& name, ZZIP_FILE* zzipFile, size_t uncompressedSize)
        :DiDataStream(name), mZzipFile(zzipFile)
    {
		mSize = uncompressedSize;
    }

	DiZipDataStream::~DiZipDataStream()
	{
		Close();
	}
 
    size_t DiZipDataStream::Read(void* buf, size_t count)
    {
		size_t was_avail = mCache.Read(buf, count);
		zzip_ssize_t r = 0;
		if (was_avail < count)
		{
			r = zzip_file_read(mZzipFile, (char*)buf + was_avail, count - was_avail);
			if (r < 0)
            {
				ZZIP_DIR *dir = zzip_dirhandle(mZzipFile);
				DiString msg = zzip_strerror_of(dir);
                DI_WARNING("Error from zziplib: %s", msg.c_str());
			}
			mCache.CacheData((char*)buf + was_avail, (size_t)r);
		}
		return was_avail + (size_t)r;
    }

	size_t DiZipDataStream::Write(void* buf, size_t count)
	{
		// not supported
		return 0;
	}
 
    void DiZipDataStream::Skip(long count)
    {
        long was_avail = static_cast<long>(mCache.Avail());
		if (count > 0)
		{
			if (!mCache.Ff(count))
				zzip_seek(mZzipFile, static_cast<zzip_off_t>(count - was_avail), SEEK_CUR);
		}
		else if (count < 0)
		{
			if (!mCache.Rewind((size_t)(-count)))
				zzip_seek(mZzipFile, static_cast<zzip_off_t>(count + was_avail), SEEK_CUR);
		}
    }
 
    void DiZipDataStream::Seek( size_t pos )
    {
		zzip_off_t newPos = static_cast<zzip_off_t>(pos);
		zzip_off_t prevPos = static_cast<zzip_off_t>(Tell());
		if (prevPos < 0)
		{
			// seek set after invalid pos
			mCache.Clear();
			zzip_seek(mZzipFile, newPos, SEEK_SET);
		}
		else
		{
			// everything is going all right, relative seek
			Skip((long)(newPos - prevPos));
		}
    }

    size_t DiZipDataStream::Tell(void) const
    {
		zzip_off_t pos = zzip_tell(mZzipFile);
		if (pos<0)
			return (size_t)(-1);
		return static_cast<size_t>(pos) - mCache.Avail();
    }

    bool DiZipDataStream::Eof(void) const
    {
        return (Tell() >= mSize);
    }

    void DiZipDataStream::Close(void)
    {
		if (mZzipFile != 0)
		{
			zzip_file_close(mZzipFile);
			mZzipFile = 0;
		}
		mCache.Clear();
    }
}