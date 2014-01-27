
#include "MiscPch.h"
#include "DataStream.h"
#include "MathMisc.h"
#include <fstream>

namespace Demi 
{
    template <typename T> DiDataStream& DiDataStream::operator >>(T& val)
    {
        Read(static_cast<void*>(&val), sizeof(T));
        return *this;
    }

    DiString DiDataStream::GetLine()
    {
        char tmpBuf[STREAM_TEMP_SIZE];
        DiString retString;
        size_t readCount;
        while ((readCount = Read(tmpBuf, STREAM_TEMP_SIZE-1)) != 0)
        {
            tmpBuf[readCount] = '\0';

            char* p = strchr(tmpBuf, '\n');
            if (p != 0)
            {
                Skip((long)(p + 1 - tmpBuf - readCount));
                *p = '\0';
            }

            retString += tmpBuf;

            if (p != 0)
            {
                if (retString.length() && retString[retString.length()-1] == '\r')
                {
                    retString.ExtractRange(0,retString.length () - 1);
                }

                break;
            }
        }

        return retString;
    }

    size_t DiDataStream::ReadLine(char* buf, size_t maxCount, const DiString& delim)
    {
        bool trimCR = false;
        if (delim.find_first_of('\n') != DiString::npos)
        {
            trimCR = true;
        }

        char tmpBuf[STREAM_TEMP_SIZE];
        uint32 chunkSize = DiMath::Min((uint32)maxCount, (uint32)STREAM_TEMP_SIZE-1);
        size_t totalCount = 0;
        size_t readCount; 
        while (chunkSize && (readCount = Read(tmpBuf, chunkSize)) != 0)
        {
            tmpBuf[readCount] = '\0';

            size_t pos = strcspn(tmpBuf, delim.c_str());

            if (pos < readCount)
            {
                Skip((long)(pos + 1 - readCount));
            }

            if (buf)
            {
                memcpy(buf+totalCount, tmpBuf, pos);
            }
            totalCount += pos;

            if (pos < readCount)
            {
                if (trimCR && totalCount && buf && buf[totalCount-1] == '\r')
                {
                    --totalCount;
                }

                break;
            }

            chunkSize = DiMath::Min((uint32)(maxCount-totalCount), (uint32)STREAM_TEMP_SIZE-1);
        }

        if(buf)
            buf[totalCount] = '\0';

        return totalCount;
    }

    size_t DiDataStream::SkipLine(const DiString& delim)
    {
        char tmpBuf[STREAM_TEMP_SIZE];
        size_t total = 0;
        size_t readCount;
        
        while ((readCount = Read(tmpBuf, STREAM_TEMP_SIZE-1)) != 0)
        {
            tmpBuf[readCount] = '\0';

            size_t pos = strcspn(tmpBuf, delim.c_str());

            if (pos < readCount)
            {
                Skip((long)(pos + 1 - readCount));

                total += pos + 1;

                break;
            }

            total += readCount;
        }

        return total;
    }

    DiString DiDataStream::GetAsString(void)
    {
        size_t bufSize = (mSize > 0 ? mSize : 4096);
        char* pBuf = (char*)malloc(bufSize);
        Seek(0);
        DiString result;
        while (!Eof())
        {
            size_t nr = Read(pBuf, bufSize);
            result.append(pBuf, (int)nr);
        }
        free(pBuf);
        return result;
    }


    DiMemoryDataStream::DiMemoryDataStream(void* pMem, size_t inSize, bool freeOnClose, bool readOnly)
    : DiDataStream(static_cast<uint16>(readOnly ? READ : (READ | WRITE)))
    {
        mData = mPos = static_cast<BYTE*>(pMem);
        mSize = inSize;
        mEnd = mData + mSize;
        mFreeOnClose = freeOnClose;
        DI_ASSERT(mEnd >= mPos);
    }

    DiMemoryDataStream::DiMemoryDataStream(const DiString& name, void* pMem, size_t inSize, 
                                       bool freeOnClose, bool readOnly)
                                       : DiDataStream(name, static_cast<uint16>(
                                       readOnly ? READ : (READ | WRITE)))
    {
        mData = mPos = static_cast<BYTE*>(pMem);
        mSize = inSize;
        mEnd = mData + mSize;
        mFreeOnClose = freeOnClose;
        DI_ASSERT(mEnd >= mPos);
    }

    DiMemoryDataStream::DiMemoryDataStream(DiDataStream& sourceStream, 
                                       bool freeOnClose, bool readOnly)
                                       : DiDataStream(static_cast<uint16>(
                                       readOnly ? READ : (READ | WRITE)))
    {
        mSize = sourceStream.Size();
        if (mSize == 0 && !sourceStream.Eof())
        {
            DiString contents = sourceStream.GetAsString();
            mSize = contents.size();
            mData = (BYTE*)malloc(mSize);
            mPos = mData;
            memcpy(mData, contents.AsCharPtr(), mSize);
            mEnd = mData + mSize;
        }
        else
        {
            mData = (BYTE*)malloc(mSize);
            mPos = mData;
            mEnd = mData + sourceStream.Read(mData, mSize);
            mFreeOnClose = freeOnClose;
        }
        DI_ASSERT(mEnd >= mPos);
    }

    DiMemoryDataStream::DiMemoryDataStream(const DiString& name, DiDataStream& sourceStream, 
                                       bool freeOnClose, bool readOnly)
                                       : DiDataStream(name, static_cast<uint16>(
                                       readOnly ? READ : (READ | WRITE)))
    {
        mSize = sourceStream.Size();
        if (mSize == 0 && !sourceStream.Eof())
        {
            DiString contents = sourceStream.GetAsString();
            mSize = contents.size();
            mData = (BYTE*)malloc(mSize);
            mPos = mData;
            memcpy(mData, contents.AsCharPtr(), mSize);
            mEnd = mData + mSize;
        }
        else
        {
            mData = (BYTE*)malloc(mSize);
            mPos = mData;
            mEnd = mData + sourceStream.Read(mData, mSize);
            mFreeOnClose = freeOnClose;
        }
        DI_ASSERT(mEnd >= mPos);
    }

    DiMemoryDataStream::DiMemoryDataStream(size_t inSize, bool freeOnClose, bool readOnly)
    : DiDataStream(static_cast<uint16>(readOnly ? READ : (READ | WRITE)))
    {
        mSize = inSize;
        mFreeOnClose = freeOnClose;
        mData = (BYTE*)malloc(mSize);
        mPos = mData;
        mEnd = mData + mSize;
        DI_ASSERT(mEnd >= mPos);
    }

    DiMemoryDataStream::DiMemoryDataStream(const DiString& name, size_t inSize, 
                                       bool freeOnClose, bool readOnly)
                                       : DiDataStream(name, static_cast<uint16>(
                                       readOnly ? READ : (READ | WRITE)))
    {
        mSize = inSize;
        mFreeOnClose = freeOnClose;
        mData = (BYTE*)malloc(mSize);
        mPos = mData;
        mEnd = mData + mSize;
        DI_ASSERT(mEnd >= mPos);
    }

    DiMemoryDataStream::~DiMemoryDataStream()
    {
        Close();
    }

    size_t DiMemoryDataStream::Read(void* buf, size_t count)
    {
        size_t cnt = count;
        if (mPos + cnt > mEnd)
            cnt = mEnd - mPos;
        if (cnt == 0)
            return 0;

        DI_ASSERT (cnt<=count);

        memcpy(buf, mPos, cnt);
        mPos += cnt;
        return cnt;
    }

    size_t DiMemoryDataStream::Write(const void* buf, size_t count)
    {
        size_t written = 0;
        if (IsWriteable())
        {
            written = count;
            if (mPos + written > mEnd)
                written = mEnd - mPos;
            if (written == 0)
                return 0;

            memcpy(mPos, buf, written);
            mPos += written;
        }
        return written;
    }

    size_t DiMemoryDataStream::ReadLine(char* buf, size_t maxCount, 
                                      const DiString& delim)
    {
        bool trimCR = false;
        if (delim.find_first_of('\n') != DiString::npos)
        {
            trimCR = true;
        }

        size_t pos = 0;

        while (pos < maxCount && mPos < mEnd)
        {
            if (delim.find(*mPos) != DiString::npos)
            {
                if (trimCR && pos && buf[pos-1] == '\r')
                {
                    --pos;
                }

                ++mPos;
                break;
            }

            buf[pos++] = *mPos++;
        }

        buf[pos] = '\0';

        return pos;
    }

    size_t DiMemoryDataStream::SkipLine(const DiString& delim)
    {
        size_t pos = 0;

        while (mPos < mEnd)
        {
            ++pos;
            if (delim.find(*mPos++) != DiString::npos)
            {
                break;
            }
        }

        return pos;

    }

    void DiMemoryDataStream::Skip(long count)
    {
        size_t newpos = (size_t)( ( mPos - mData ) + count );
        DI_ASSERT( mData + newpos <= mEnd );        

        mPos = mData + newpos;
    }

    void DiMemoryDataStream::Seek( size_t pos )
    {
        DI_ASSERT( mData + pos <= mEnd );
        mPos = mData + pos;
    }

    size_t DiMemoryDataStream::Tell(void) const
    {
        return mPos - mData;
    }

    bool DiMemoryDataStream::Eof(void) const
    {
        return mPos >= mEnd;
    }

    void DiMemoryDataStream::Close(void)    
    {
        if (mFreeOnClose && mData)
        {
            free(mData);
            mData = 0;
        }
    }

    DiFileStreamDataStream::DiFileStreamDataStream(DiIfstream* s, bool freeOnClose)
        : DiDataStream(), 
        mInStream(s),
        mFStreamRO(s), 
        mFStream(0), 
        mFreeOnClose(freeOnClose)
    {
        mInStream->seekg(0, std::ios_base::end);
        mSize = (size_t)mInStream->tellg();
        mInStream->seekg(0, std::ios_base::beg);
        DetermineAccess();
    }

    DiFileStreamDataStream::DiFileStreamDataStream(const DiString& name, 
                                               DiIfstream* s, bool freeOnClose)
                                               : DiDataStream(name), mInStream(s),
                                               mFStreamRO(s), mFStream(0),
                                               mFreeOnClose(freeOnClose)
    {
        mInStream->seekg(0, std::ios_base::end);
        mSize = (size_t)mInStream->tellg();
        mInStream->seekg(0, std::ios_base::beg);
        DetermineAccess();
    }

    DiFileStreamDataStream::DiFileStreamDataStream(const DiString& name, 
                                               DiIfstream* s, size_t inSize, bool freeOnClose)
                                               : DiDataStream(name), mInStream(s),
                                               mFStreamRO(s), mFStream(0),
                                               mFreeOnClose(freeOnClose)
    {
        mSize = inSize;
        DetermineAccess();
    }
    
    DiFileStreamDataStream::DiFileStreamDataStream(std::fstream* s, bool freeOnClose)
    : DiDataStream(false), mInStream(s), mFStreamRO(0), mFStream(s), mFreeOnClose(freeOnClose)
    {
        mInStream->seekg(0, std::ios_base::end);
        mSize = (size_t)mInStream->tellg();
        mInStream->seekg(0, std::ios_base::beg);
        DetermineAccess();

    }

    DiFileStreamDataStream::DiFileStreamDataStream(const DiString& name, 
                                               std::fstream* s, bool freeOnClose)
                                               : DiDataStream(name, false), 
                                               mInStream(s), 
                                               mFStreamRO(0),
                                               mFStream(s),
                                               mFreeOnClose(freeOnClose)
    {
        mInStream->seekg(0, std::ios_base::end);
        mSize = (size_t)mInStream->tellg();
        mInStream->seekg(0, std::ios_base::beg);
        DetermineAccess();
    }

    DiFileStreamDataStream::DiFileStreamDataStream(const DiString& name, 
                                               std::fstream* s, size_t inSize, bool freeOnClose)
                                               : DiDataStream(name, false), 
                                               mInStream(s),
                                               mFStreamRO(0),
                                               mFStream(s), 
                                               mFreeOnClose(freeOnClose)
    {
        mSize = inSize;
        DetermineAccess();
    }
    
    void DiFileStreamDataStream::DetermineAccess()
    {
        mAccess = 0;
        if (mInStream)
            mAccess |= READ;
        if (mFStream)
            mAccess |= WRITE;
    }

    DiFileStreamDataStream::~DiFileStreamDataStream()
    {
        Close();
    }

    size_t DiFileStreamDataStream::Read(void* buf, size_t count)
    {
        mInStream->read(static_cast<char*>(buf),
            static_cast<std::streamsize>(count));
        return (size_t)mInStream->gcount();
    }

    size_t DiFileStreamDataStream::Write(const void* buf, size_t count)
    {
        size_t written = 0;
        if (IsWriteable() && mFStream)
        {
            mFStream->write(static_cast<const char*>(buf), 
                static_cast<std::streamsize>(count));
            written = count;
        }
        return written;
    }

    size_t DiFileStreamDataStream::ReadLine(char* buf, size_t maxCount, 
                                          const DiString& delim)
    {
        if (delim.empty())
        {
            DI_ASSERT_FAIL;
        }
        
        bool trimCR = false;
        if (delim[0] == '\n') 
        {
            trimCR = true;
        }
        mInStream->getline(buf,
            static_cast<std::streamsize>(maxCount+1), delim[0]);
        size_t ret = (size_t)mInStream->gcount();

        if (mInStream->eof()) 
        {
        }
        else if (mInStream->fail())
        {
            if (ret == maxCount)
            {
                mInStream->clear();
            }
            else
            {
                DI_ASSERT_FAIL;
            }
        }
        else 
        {
            --ret;
        }

        if (trimCR && buf[ret-1] == '\r')
        {
            --ret;
            buf[ret] = '\0';
        }
        return ret;
    }

    void DiFileStreamDataStream::Skip(long count)
    {
        mInStream->clear();
        mInStream->seekg(static_cast<std::ifstream::pos_type>(count), std::ios::cur);
    }

    void DiFileStreamDataStream::Seek( size_t pos )
    {
        mInStream->clear();
        mInStream->seekg(static_cast<std::streamoff>(pos), std::ios::beg);
    }

    size_t DiFileStreamDataStream::Tell(void) const
    {
        mInStream->clear();
        return (size_t)mInStream->tellg();
    }

    bool DiFileStreamDataStream::Eof(void) const
    {
        return mInStream->eof();
    }

    void DiFileStreamDataStream::Close(void)
    {
        if (mInStream)
        {
            if (mFStreamRO)
            {
                mFStreamRO->close();
            }
            if (mFStream)
            {
                mFStream->flush();
                mFStream->close();
            }

            if (mFreeOnClose)
            {
                if (mFStreamRO)
                {    
                    delete mFStreamRO;
                }
                if (mFStream)
                {
                    delete mFStream;
                }
                mInStream = 0;
                mFStreamRO = 0; 
                mFStream = 0; 
            }
        }
    }

    DiFileHandleDataStream::DiFileHandleDataStream(FILE* handle, uint16 accessMode)
    : DiDataStream(accessMode), mFileHandle(handle)
    {
        fseek(mFileHandle, 0, SEEK_END);
        mSize = ftell(mFileHandle);
        fseek(mFileHandle, 0, SEEK_SET);
    }

    DiFileHandleDataStream::DiFileHandleDataStream(const DiString& name, FILE* handle, uint16 accessMode)
    : DiDataStream(name, accessMode), mFileHandle(handle)
    {
        fseek(mFileHandle, 0, SEEK_END);
        mSize = ftell(mFileHandle);
        fseek(mFileHandle, 0, SEEK_SET);
    }

    DiFileHandleDataStream::~DiFileHandleDataStream()
    {
        Close();
    }

    size_t DiFileHandleDataStream::Read(void* buf, size_t count)
    {
        return fread(buf, 1, count, mFileHandle);
    }

    size_t DiFileHandleDataStream::Write(const void* buf, size_t count)
    {
        if (!IsWriteable())
            return 0;
        else
            return fwrite(buf, 1, count, mFileHandle);
    }

    void DiFileHandleDataStream::Skip(long count)
    {
        fseek(mFileHandle, count, SEEK_CUR);
    }

    void DiFileHandleDataStream::Seek( size_t pos )
    {
        fseek(mFileHandle, static_cast<long>(pos), SEEK_SET);
    }

    size_t DiFileHandleDataStream::Tell(void) const
    {
        return ftell( mFileHandle );
    }

    bool DiFileHandleDataStream::Eof(void) const
    {
        return feof(mFileHandle) != 0;
    }

    void DiFileHandleDataStream::Close(void)
    {
        if (mFileHandle != 0)
        {
            fclose(mFileHandle);
            mFileHandle = 0;
        }
    }
}