
/********************************************************************
    File:       Archive.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi 
{
    struct DiFileInfo 
    {
        const DiArchive* archive;
        DiString         filename;
        DiString         basename;
    };

    typedef DiVector<DiFileInfo>        DiFileInfoList;
    typedef shared_ptr<DiFileInfoList>    DiFileInfoListPtr;
    typedef shared_ptr<StringVec>        DiStringVecPtr;

    //////////////////////////////////////////////////////////////////////////
    class DiArchive
    {
    public:
    
        DiArchive( const DiString& name, DiArchiveType archType )
            : mName(name), mType(archType){}

        virtual                        ~DiArchive() {}

        const DiString&                GetName() const {return mName;}

        DiArchiveType                  GetType() const {return mType;}

        virtual    void                Load() = 0;

        virtual    void                Unload() = 0;

        virtual DiDataStreamPtr        Open(const DiString& filename) const = 0;

        virtual DiStringVecPtr         List(bool recursive = true, bool dirs = false) = 0;

        virtual DiFileInfoListPtr      ListFileInfo(bool recursive = true, bool dirs = false) = 0;

        virtual DiStringVecPtr         Find(const DiString& pattern, bool recursive = true,
                                            bool dirs = false) = 0;

        virtual DiFileInfoListPtr      FindFileInfo(const DiString& pattern, 
                                            bool recursive = true, bool dirs = false) const = 0;

        virtual bool                   Exists(const DiString& filename) = 0;

    protected:

        DiString                       mName; 

        DiArchiveType                  mType;
    };

    typedef shared_ptr<DiArchive>      ArchivePtr;
}