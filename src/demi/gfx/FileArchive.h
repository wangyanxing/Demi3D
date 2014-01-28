
/********************************************************************
    File:       FileArchive.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "Archive.h"

namespace Demi 
{
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

    protected:

        void                FindFiles(const DiString& pattern, bool recursive, bool dirs,
                                DiStringVecPtr simpleList, DiFileInfoListPtr detailList) const;
    };
}