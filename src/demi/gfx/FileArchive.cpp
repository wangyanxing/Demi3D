
#include "GfxPch.h"
#include "FileArchive.h"
#include "DataStream.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <direct.h>
#include <io.h>
#include <Shlwapi.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace Demi 
{
    static bool is_absolute_path(const char* path)
    {
        if (isalpha(uint8(path[0])) && path[1] == ':')
            return true;
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
        std::istream*    baseStream = 0;
        DiIfstream*    roStream = roStream = DI_NEW DiIfstream();
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
            directory = pattern.substr (0, pos1 + 1);
        }

        DiString full_pattern = concatenate_path(mName, pattern);

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
                    simpleList->push_back(directory + tagData.name);
                }
                else if (detailList)
                {
                    DiFileInfo fi;
                    fi.archive = this;
                    fi.filename = directory + tagData.name;
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
                mask.append (pattern.substr (pos1 + 1));
            else
                mask.append (pattern);

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