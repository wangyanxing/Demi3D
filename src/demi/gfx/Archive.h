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

#ifndef DiArchive_h__
#define DiArchive_h__


namespace Demi 
{
    struct DiFileInfo 
    {
        const DiArchive* archive;
        DiString         filename;
        DiString         basename;
        DiString         path;
        uint32           compressedSize;
        uint32           uncompressedSize;
    };

    typedef DiVector<DiFileInfo>       DiFileInfoList;
    typedef shared_ptr<DiFileInfoList> DiFileInfoListPtr;

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
        
        virtual    DiString            GetFullPath(const DiString& filename) = 0;

        virtual DiDataStreamPtr        Open(const DiString& filename) const = 0;

        virtual DiStringVecPtr         List(bool recursive = true, bool dirs = false) = 0;

        virtual DiFileInfoListPtr      ListFileInfo(bool recursive = true, bool dirs = false) = 0;

        virtual DiStringVecPtr         Find(const DiString& pattern, bool recursive = true,
                                            bool dirs = false) = 0;

        virtual DiFileInfoListPtr      FindFileInfo(const DiString& pattern, 
                                            bool recursive = true, bool dirs = false) const = 0;

    protected:

        DiString                       mName; 

        DiArchiveType                  mType;
    };
}

#endif
