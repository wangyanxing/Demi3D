
/********************************************************************
    File:       DataStream.h
    Creator:    demiwangya
*********************************************************************/

#ifndef DataStream_h__
#define DataStream_h__

#include "MiscPrerequisites.h"
#include <istream>

namespace Demi 
{
    template <size_t cacheSize>
    class DiStaticCache
    {
    public:
        DiStaticCache()
        {
            mValidBytes = 0;
            mPos = 0;
        }

        size_t CacheData(const void* buf, size_t count)
        {
            DI_ASSERT(Avail() == 0);

            if (count < cacheSize)
            {
                if (count + mValidBytes <= cacheSize)
                {
                    memcpy(mBuffer + mValidBytes, buf, count);
                    mValidBytes += count;
                }
                else
                {
                    size_t begOff = count - (cacheSize - mValidBytes);
                    memmove(mBuffer, mBuffer + begOff, mValidBytes - begOff);
                    memcpy(mBuffer + cacheSize - count, buf, count);
                    mValidBytes = cacheSize;
                }
                mPos = mValidBytes;
                return count;
            }
            else
            {
                memcpy(mBuffer, (const char*)buf + count - cacheSize, cacheSize);
                mValidBytes = mPos = cacheSize;
                return cacheSize;
            }
        }

        size_t Read(void* buf, size_t count)
        {
            size_t rb = Avail();
            rb = (rb < count) ? rb : count;
            memcpy(buf, mBuffer + mPos, rb);
            mPos += rb;
            return rb;
        }

        bool Rewind(size_t count)
        {
            if (mPos < count)
            {
                Clear();
                return false;
            }
            else
            {
                mPos -= count;
                return true;
            }
        }

        bool Ff(size_t count)
        {
            if (Avail() < count)
            {
                Clear();
                return false;
            }
            else
            {
                mPos += count;
                return true;
            }
        }

        size_t Avail() const
        {
            return mValidBytes - mPos;
        }

        void Clear()
        {
            mValidBytes = 0;
            mPos = 0;
        }

    protected:

        char    mBuffer[cacheSize];

        size_t    mValidBytes;

        size_t    mPos;

    };

    //////////////////////////////////////////////////////////////////////////
    class DI_MISC_API DiDataStream
    {
    public:
        enum AccessMode
        {
            READ = 1, 
            WRITE = 2
        };
    protected:

        DiString        mName;        

        size_t            mSize;

        uint16            mAccess;

#define STREAM_TEMP_SIZE 128
    public:

        DiDataStream(uint16 accessMode = READ) : mSize(0), mAccess(accessMode) {}

        DiDataStream(const DiString& name, uint16 accessMode = READ) 
            : mName(name), mSize(0), mAccess(accessMode) {}

        virtual                    ~DiDataStream() {}

    public:

        const DiString&            GetName(void) { return mName; }

        uint16                    GetAccessMode() const { return mAccess; }

        virtual bool             IsReadable() const { return (mAccess & READ) != 0; }

        virtual bool             IsWriteable() const { return (mAccess & WRITE) != 0; }

        virtual size_t            Read(void* buf, size_t count) = 0;

        virtual size_t            Write(const void* buf, size_t count)
        {
            (void)buf;
            (void)count;
            return 0;
        }

        template<typename T> DiDataStream& operator>>(T& val);


        virtual size_t            ReadLine(char* buf, size_t maxCount, const DiString& delim = "\n");

        virtual DiString        GetLine( );

        virtual DiString        GetAsString(void);

        virtual size_t            SkipLine(const DiString& delim = "\n");

        virtual void            Skip(long count) = 0;

        virtual void            Seek( size_t pos ) = 0;

        virtual size_t            Tell(void) const = 0;

        virtual bool            Eof(void) const = 0;

        size_t                    Size(void) const { return mSize; }

        virtual void            Close(void) = 0;
    };

    class DI_MISC_API DiMemoryDataStream : public DiDataStream
    {
    protected:
        BYTE*    mData;
        BYTE*    mPos;
        BYTE*    mEnd;
        bool    mFreeOnClose;            
    public:

        DiMemoryDataStream(void* pMem, size_t size, bool freeOnClose = false, bool readOnly = false);

        DiMemoryDataStream(const DiString& name, void* pMem, size_t size, 
            bool freeOnClose = false, bool readOnly = false);

        DiMemoryDataStream(DiDataStream& sourceStream, 
            bool freeOnClose = true, bool readOnly = false);

        DiMemoryDataStream(const DiString& name, DiDataStream& sourceStream, 
            bool freeOnClose = true, bool readOnly = false);

        DiMemoryDataStream(size_t size, bool freeOnClose = true, bool readOnly = false);

        DiMemoryDataStream(const DiString& name, size_t size, 
            bool freeOnClose = true, bool readOnly = false);

        ~DiMemoryDataStream();

        BYTE*         GetPtr(void) { return mData; }

        BYTE*         GetCurrentPtr(void) { return mPos; }

        size_t        Read(void* buf, size_t count);

        size_t        Write(const void* buf, size_t count);

        size_t        ReadLine(char* buf, size_t maxCount, const DiString& delim = "\n");

        size_t        SkipLine(const DiString& delim = "\n");

        void          Skip(long count);

        void          Seek(size_t pos);

        size_t        Tell(void) const;

        bool          Eof(void) const;

        void          Close(void);

        void          setFreeOnClose(bool free) { mFreeOnClose = free; }
    };

    class DI_MISC_API DiFileStreamDataStream : public DiDataStream
    {
    public:

        DiFileStreamDataStream(DiIfstream* s, 
            bool freeOnClose = true);

        DiFileStreamDataStream(std::fstream* s, 
            bool freeOnClose = true);

        DiFileStreamDataStream(const DiString& name, 
            DiIfstream* s, 
            bool freeOnClose = true);

        DiFileStreamDataStream(const DiString& name, 
            std::fstream* s, 
            bool freeOnClose = true);

        DiFileStreamDataStream(const DiString& name, 
            DiIfstream* s, 
            size_t size, 
            bool freeOnClose = true);

        DiFileStreamDataStream(const DiString& name, 
            std::fstream* s, 
            size_t size, 
            bool freeOnClose = true);

        ~DiFileStreamDataStream();

        size_t            Read(void* buf, size_t count);

        size_t            Write(const void* buf, size_t count);

        size_t            ReadLine(char* buf, size_t maxCount, const DiString& delim = "\n");

        void              Skip(long count);

        void              Seek( size_t pos );

        size_t            Tell(void) const;

        bool              Eof(void) const;

        void              Close(void);

    protected:

        std::istream*     mInStream;

        DiIfstream*       mFStreamRO;

        std::fstream*     mFStream;

        bool              mFreeOnClose;    

        void              DetermineAccess();

    };

    class DI_MISC_API DiFileHandleDataStream : public DiDataStream
    {
    protected:
        FILE* mFileHandle;

    public:
        DiFileHandleDataStream(FILE* handle, uint16 accessMode = READ);

        DiFileHandleDataStream(const DiString& name, FILE* handle, uint16 accessMode = READ);

        ~DiFileHandleDataStream();

        size_t         Read(void* buf, size_t count);

        size_t         Write(const void* buf, size_t count);

        void           Skip(long count);

        void           Seek( size_t pos );

        size_t         Tell(void) const;

        bool           Eof(void) const;

        void           Close(void);

    };

    typedef shared_ptr<DiDataStream> DiDataStreamPtr;

}
#endif // DataStream_h__