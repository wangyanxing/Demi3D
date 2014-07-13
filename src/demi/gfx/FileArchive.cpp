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
#include "FileArchive.h"
#include "DataStream.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <regex>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#   include <direct.h>
#   include <io.h>
#   include <Shlwapi.h>
#else
#   include <sys/param.h>
#   include <dirent.h>
#   include <unistd.h>
#   include <fnmatch.h>

struct _finddata_t
{
    char *name;
    int attrib;
    unsigned long size;
};

struct _find_search_t
{
    char *pattern;
    char *curfn;
    char *directory;
    int dirlen;
    DIR *dirfd;
};

#define _A_NORMAL 0x00  /* Normalfile-Noread/writerestrictions */
#define _A_RDONLY 0x01  /* Read only file */
#define _A_HIDDEN 0x02  /* Hidden file */
#define _A_SYSTEM 0x04  /* System file */
#define _A_ARCH   0x20  /* Archive file */
#define _A_SUBDIR 0x10  /* Subdirectory */

int _findclose(intptr_t id);
int _findnext(intptr_t id, struct _finddata_t *data);

intptr_t _findfirst(const char *pattern, struct _finddata_t *data)
{
    _find_search_t *fs = new _find_search_t;
    fs->curfn = NULL;
    fs->pattern = NULL;

    // Separate the mask from directory name
    const char *mask = strrchr(pattern, '/');
    if (mask)
    {
        fs->dirlen = mask - pattern;
        mask++;
        fs->directory = (char *)malloc(fs->dirlen + 1);
        memcpy(fs->directory, pattern, fs->dirlen);
        fs->directory[fs->dirlen] = 0;
    }
    else
    {
        mask = pattern;
        fs->directory = strdup(".");
        fs->dirlen = 1;
    }

    fs->dirfd = opendir(fs->directory);
    if (!fs->dirfd)
    {
        _findclose((intptr_t)fs);
        return -1;
    }

    /* Hack for "*.*" -> "*' from DOS/Windows */
    if (strcmp(mask, "*.*") == 0)
        mask += 2;
    fs->pattern = strdup(mask);

    /* Get the first entry */
    if (_findnext((intptr_t)fs, data) < 0)
    {
        _findclose((intptr_t)fs);
        return -1;
    }

    return (intptr_t)fs;
}

int _findnext(intptr_t id, struct _finddata_t *data)
{
    _find_search_t *fs = (_find_search_t *)id;

    /* Loop until we run out of entries or find the next one */
    dirent *entry;
    for (;;)
    {
        if (!(entry = readdir(fs->dirfd)))
            return -1;

        /* See if the filename matches our pattern */
        if (fnmatch(fs->pattern, entry->d_name, 0) == 0)
            break;
    }

    if (fs->curfn)
        free(fs->curfn);
    data->name = fs->curfn = strdup(entry->d_name);

    size_t namelen = strlen(entry->d_name);
    char *xfn = new char[fs->dirlen + 1 + namelen + 1];
    sprintf(xfn, "%s/%s", fs->directory, entry->d_name);

    /* stat the file to get if it's a subdir and to find its length */
    struct stat stat_buf;
    if (stat(xfn, &stat_buf))
    {
        // Hmm strange, imitate a zero-length file then
        data->attrib = _A_NORMAL;
        data->size = 0;
    }
    else
    {
        if (S_ISDIR(stat_buf.st_mode))
            data->attrib = _A_SUBDIR;
        else
            /* Default type to a normal file */
            data->attrib = _A_NORMAL;

        data->size = (unsigned long)stat_buf.st_size;
    }

    delete[] xfn;

    /* Files starting with a dot are hidden files in Unix */
    if (data->name[0] == '.')
        data->attrib |= _A_HIDDEN;

    return 0;
}

int _findclose(intptr_t id)
{
    int ret;
    _find_search_t *fs = (_find_search_t *)id;

    ret = fs->dirfd ? closedir(fs->dirfd) : 0;
    free(fs->pattern);
    free(fs->directory);
    if (fs->curfn)
        free(fs->curfn);
    delete fs;

    return ret;
}

#endif

namespace Demi 
{
    static bool is_absolute_path(const char* path)
    {
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        if (isalpha(uint8(path[0])) && path[1] == ':')
            return true;
#endif
        return path[0] == '/' || path[0] == '\\';
    }

    static DiString concatenate_path(const DiString& base, const DiString& name)
    {
        if (base.empty() || is_absolute_path(name.c_str()))
            return name;
        else
            return base + "/" + name;
    }

    static bool is_reserved_dir (const char *fn)
    {
        return (fn [0] == '.' && (fn [1] == 0 || (fn [1] == '.' && fn [2] == 0)));
    }

    //////////////////////////////////////////////////////////////////////////

    DiFileArchive::DiFileArchive( const DiString& name)
        : DiArchive(name, ARCHIVE_FILE)
    {
    }

    DiFileArchive::~DiFileArchive()
    {
    }
    
    DiString DiFileArchive::GetFullPath(const DiString& filename)
    {
        return concatenate_path(mName, filename);
    }

    DiDataStreamPtr DiFileArchive::Open( const DiString& filename) const
    {
        DiString full_path = concatenate_path(mName, filename);
        struct stat tagStat;
        int ret = stat(full_path.c_str(), &tagStat);
        
        if (ret != 0)
        {
            DI_WARNING("Cannot open the file:%s",filename.c_str());
            return DiDataStreamPtr(nullptr);
        }

        std::ios::openmode mode = std::ios::in | std::ios::binary;
        std::istream* baseStream = nullptr;
        DiIfstream*   roStream = roStream = DI_NEW DiIfstream();
        roStream->open(full_path.c_str(), mode);
        baseStream = roStream;

        if (baseStream->fail())
        {
            delete roStream;
            DI_WARNING("Failed to open the file:%s",filename.c_str());
            return DiDataStreamPtr();
        }

        DiDataStreamPtr stream(DI_NEW DiFileStreamDataStream(filename,
            roStream, (size_t)tagStat.st_size, true));

        return stream;
    }

    Demi::DiStringVecPtr DiFileArchive::List( bool recursive /*= true*/,
                                        bool dirs /*= false*/ )
    {
        DiStringVecPtr ret(DI_NEW StringVec);
        FindFiles("*", recursive, dirs, ret, DiFileInfoListPtr());
        return ret;
    }

    Demi::DiFileInfoListPtr DiFileArchive::ListFileInfo( bool recursive /*= true*/, 
                                                bool dirs /*= false*/ )
    {
        DiFileInfoListPtr ret(DI_NEW DiFileInfoList);
        FindFiles("*", recursive, dirs, DiStringVecPtr(), ret);
        return ret;
    }

    Demi::DiStringVecPtr DiFileArchive::Find( const DiString& pattern, 
        bool recursive /*= true*/, bool dirs /*= false*/ )
    {
        DiStringVecPtr ret(DI_NEW StringVec);
        FindFiles(pattern, recursive, dirs, ret, DiFileInfoListPtr());
        return ret;
    }
    
    void _recurGenFiles(DiFileTree* node, const DiString& filePattern, const DiString& name, const DiString& searchName)
    {
        long lHandle, res;
        struct _finddata_t tagData;
        
        DiString pattern = "*.*";
        DiString full_pattern = concatenate_path(name, pattern);
        
        DiMap<DiString, DiFileTree*> nodeTable;
        
        lHandle = _findfirst(full_pattern.c_str(), &tagData);
        res = 0;
        while (lHandle != -1 && res != -1)
        {
            if ((tagData.attrib & _A_HIDDEN) == 0 )
            {
                std::regex regexpattern(filePattern.c_str(),std::regex_constants::extended);
                
                bool pass = (tagData.attrib & _A_SUBDIR);
                
                if(!pass)
                {
                    DiString curn = tagData.name;
                    curn.ToLower();
                    pass = std::regex_search(curn.c_str(), regexpattern);
                }
                
                if (pass)
                {
                    auto nd = node->AddChild(tagData.name);
                    nd->folder = tagData.attrib & _A_SUBDIR;
                    nodeTable[tagData.name] = nd;
                }
            }
            res = _findnext( lHandle, &tagData );
        }
        
        if(lHandle != -1)
            _findclose(lHandle);
        
        DiString base_dir = name;
        
        base_dir.append ("/*");
        
        DiString mask ("/");
        mask.append(pattern);
        
        lHandle = _findfirst(base_dir.c_str (), &tagData);
        res = 0;
        while (lHandle != -1 && res != -1)
        {
            if ((tagData.attrib & _A_SUBDIR) &&
                ( (tagData.attrib & _A_HIDDEN) == 0 ) &&
                !is_reserved_dir (tagData.name))
            {
                auto nd = nodeTable.find(tagData.name);
                if(nd != nodeTable.end())
                {
                    base_dir = name;
                    base_dir.append(tagData.name).append (mask);
                    
                    DiString n = name;
                    n += tagData.name;
                    n += "/";
                    _recurGenFiles(nd->second, filePattern, n, base_dir);
                }
                
            }
            res = _findnext( lHandle, &tagData );
        }
        if(lHandle != -1)
        {
            _findclose(lHandle);
        }
    }
    
    void DiFileArchive::GenerateFileTree(DiFileTree*& node, const DiString& pattern)
    {
        if(!node)
        {
            node = DI_NEW DiFileTree();
            node->folder = true;
        }
        
        auto name = mName;
        name.SimplifyPath();
        
        node->fileName = name.ExtractFileName();
        
        _recurGenFiles(node, pattern, mName, mName);
        
        return node;
    }

    Demi::DiFileInfoListPtr DiFileArchive::FindFileInfo( const DiString& pattern, 
        bool recursive /*= true*/, bool dirs /*= false*/ ) const
    {
        DiFileInfoListPtr ret(DI_NEW DiFileInfoList);
        FindFiles(pattern, recursive, dirs, DiStringVecPtr(), ret);
        return ret;
    }

    void DiFileArchive::FindFiles( const DiString& pattern, bool recursive, 
            bool dirs, DiStringVecPtr simpleList, DiFileInfoListPtr detailList ) const
    {
        long lHandle, res;
        struct _finddata_t tagData;

        int pos1 = pattern.rfind ('/');
        int pos2 = pattern.rfind ('\\');
        if (pos1 == pattern.npos || ((pos2 != pattern.npos) && (pos1 < pos2)))
        {
            pos1 = pos2;
        }
        DiString directory;
        if (pos1 != pattern.npos)
        {
            directory = pattern.substr(0, pos1 + 1);
        }

        DiString full_pattern = concatenate_path(mName, pattern);
         
        DI_INFO("Searching path: %s", full_pattern.c_str());
        
        lHandle = _findfirst(full_pattern.c_str(), &tagData);
        res = 0;
        while (lHandle != -1 && res != -1)
        {
            if ((dirs == ((tagData.attrib & _A_SUBDIR) != 0)) &&
                ( (tagData.attrib & _A_HIDDEN) == 0 ) &&
                (!dirs || !is_reserved_dir (tagData.name)))
            {
                if (simpleList)
                {
                    simpleList->push_back(directory);
                    simpleList->back() += tagData.name;
                }
                else if (detailList)
                {
                    DiFileInfo fi;
                    fi.archive = this;
                    fi.path = concatenate_path(mName, directory);
                    fi.path.SimplifyPath();
                    fi.filename = concatenate_path(fi.path, tagData.name);
                    fi.basename = tagData.name;
                    detailList->push_back(fi);
                }
            }
            res = _findnext( lHandle, &tagData );
        }
        if(lHandle != -1)
            _findclose(lHandle);

        if (recursive)
        {
            DiString base_dir = mName;
            if (!directory.empty ())
            {
                base_dir = concatenate_path(mName, directory);
                base_dir.ExtractRange(0,base_dir.length () - 1);
            }
            base_dir.append ("/*");

            DiString mask ("/");
            if (pos1 != pattern.npos)
                mask.append(pattern.substr(pos1 + 1));
            else
                mask.append(pattern);

            lHandle = _findfirst(base_dir.c_str (), &tagData);
            res = 0;
            while (lHandle != -1 && res != -1)
            {
                if ((tagData.attrib & _A_SUBDIR) &&
                    ( (tagData.attrib & _A_HIDDEN) == 0 ) &&
                    !is_reserved_dir (tagData.name))
                {
                    base_dir = directory;
                    base_dir.append (tagData.name).append (mask);
                    FindFiles(base_dir, recursive, dirs, simpleList, detailList);
                }
                res = _findnext( lHandle, &tagData );
            }
            if(lHandle != -1)
            {
                _findclose(lHandle);
            }
        }
    }
}