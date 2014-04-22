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

#ifndef DiFileArchive_h__
#define DiFileArchive_h__


#include "Archive.h"

namespace Demi 
{
    /** Ordinary file system
     */
    class DiFileArchive : public DiArchive
    {
    public:

        DiFileArchive(const DiString& name);

        virtual             ~DiFileArchive();

    public:

        void                Load(){}

        void                Unload(){}

        DiDataStreamPtr     Open(const DiString& filename) const;

        DiStringVecPtr      List(bool recursive = true, bool dirs = false);

        DiFileInfoListPtr   ListFileInfo(bool recursive = true, bool dirs = false);

        DiStringVecPtr      Find(const DiString& pattern, bool recursive = true,
                                bool dirs = false);

        DiFileInfoListPtr   FindFileInfo(const DiString& pattern, 
                                bool recursive = true, bool dirs = false) const;
        
        DiString            GetFullPath(const DiString& filename);

    protected:

        void                FindFiles(const DiString& pattern, bool recursive, bool dirs,
                                DiStringVecPtr simpleList, DiFileInfoListPtr detailList) const;
    };
}

#endif
