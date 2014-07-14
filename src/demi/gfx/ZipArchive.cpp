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

#include "ZipArchiveEntry.h"
#include "ZipFile.h"

namespace Demi
{
    DiZipArchive::DiZipArchive( const DiString& name)
        : DiArchive(name, ARCHIVE_FILE)
    {
    }

    DiZipArchive::~DiZipArchive()
    {
        Unload();
    }
    
    void DiZipArchive::Load()
    {
        DI_INFO("Loading zip archive: %s", mName.c_str());
        
        try
        {
            mZipPtr = ZipFile::Open(mName.c_str());
        }
        catch(...)
        {
            mZipPtr = nullptr;
            DI_WARNING("Failed to load the zip archive");
            return;
        }
        
        size_t entries = mZipPtr->GetEntriesCount();
        for (size_t i = 0; i < entries; ++i)
        {
            DiFileInfo info;
            info.archive = this;
            
            auto entry = mZipPtr->GetEntry((int)i);
            
            info.filename = entry->GetFullName().c_str();
            info.basename = info.filename.ExtractFileName();
            info.path = info.filename.ExtractDirName();
            
            // Get sizes
            info.compressedSize = static_cast<uint32>(entry->GetCompressedSize());
            info.uncompressedSize = static_cast<uint32>(entry->GetSize());
            
            if(entry->IsDirectory())
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
    
    void DiZipArchive::Unload()
    {
        mZipPtr.reset();
    }
    
    DiString DiZipArchive::GetFullPath(const DiString& filename)
    {
        return filename;
    }

    DiDataStreamPtr DiZipArchive::Open( const DiString& filename) const
    {
        auto entry = mZipPtr->GetEntry(filename.c_str());
        if(!entry)
        {
            DI_WARNING("Cannot open the zip entry: %s", filename.c_str());
            return nullptr;
        }
        
        if(entry->IsDirectory())
        {
            DI_WARNING("Cannot open the dir zip entry: %s", filename.c_str());
            return nullptr;
        }
        
        std::istream* data = entry->GetDecompressionStream();
        if(!data)
        {
            DI_WARNING("Cannot open the zip entry, null data stream: %s", filename.c_str());
            return nullptr;
        }
        
        size_t size = entry->GetSize();
        char* p = DI_NEW char[size];
        data->read(p, size);
        
        entry->CloseDecompressionStream();
        return DiDataStreamPtr(DI_NEW DiMemoryDataStream(filename, p, size, true, false));
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

    void DiZipArchive::GenerateFileTree(DiFileTree*& node, const DiString& pattern)
    {
        bool wildCard = pattern.find("*") != DiString::npos;

        if(!node)
        {
            node = DI_NEW DiFileTree();
            node->folder = true;
        }
        
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
        return mZipPtr->GetEntry(file.c_str()) != nullptr;
    }
}