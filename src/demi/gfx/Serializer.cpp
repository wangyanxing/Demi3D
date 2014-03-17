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
#include "Serializer.h"

namespace Demi
{
    DiSerializer::DiSerializer()
        :mCurChunkSize(0)
    {

    }

    uint16 DiSerializer::ReadChunk( DiDataStreamPtr& stream )
    {
        unsigned short id;
        ReadShorts(stream, &id, 1);

        ReadInts(stream, &mCurChunkSize, 1);
        return id;
    }

    void DiSerializer::ReadBools( DiDataStreamPtr& stream, bool* pDest, size_t count )
    {
        stream->Read(pDest, sizeof(bool) * count);
    }

    void DiSerializer::ReadFloats( DiDataStreamPtr& stream, float* pDest, size_t count )
    {
        stream->Read(pDest, sizeof(float) * count);
    }

    void DiSerializer::ReadFloats( DiDataStreamPtr& stream, double* pDest, size_t count )
    {
        float* tmp = DI_NEW float[count];
        float* ptmp = tmp;
        stream->Read(tmp, sizeof(float) * count);

        while(count--)
        {
            *pDest++ = *ptmp++;
        }

        DI_DELETE[] tmp;
    }

    void DiSerializer::ReadShorts( DiDataStreamPtr& stream, uint16* pDest, size_t count )
    {
        stream->Read(pDest, sizeof(unsigned short) * count);
    }

    void DiSerializer::ReadInts( DiDataStreamPtr& stream, uint32* pDest, size_t count )
    {
        stream->Read(pDest, sizeof(unsigned int) * count);
    }
    
    int DiSerializer::ReadInt(DiDataStreamPtr& stream)
    {
        int ret = 0;
        stream->Read(&ret, sizeof(int));
        return ret;
    }
    
    float DiSerializer::ReadFloat(DiDataStreamPtr& stream)
    {
        float ret = 0;
        stream->Read(&ret, sizeof(float));
        return ret;
    }

    void DiSerializer::ReadObject( DiDataStreamPtr& stream, DiVec3& pDest )
    {
        ReadFloats(stream, pDest.ptr(), 3);
    }

    void DiSerializer::ReadObject( DiDataStreamPtr& stream, DiQuat& pDest )
    {
        float tmp[4];
        ReadFloats(stream, tmp, 4);
        pDest.x = tmp[0];
        pDest.y = tmp[1];
        pDest.z = tmp[2];
        pDest.w = tmp[3];
    }

    DiString DiSerializer::ReadString( DiDataStreamPtr& stream )
    {
        return stream->GetLine();
    }

    DiString DiSerializer::ReadString( DiDataStreamPtr& stream, size_t numChars )
    {
        assert (numChars <= 255);
        char str[255];
        stream->Read(str, numChars);
        str[numChars] = '\0';
        return str;
    }

    void DiSerializer::WriteChunkHeader( uint16 id, size_t size )
    {
        WriteShorts(&id, 1);
        uint32 uint32size = static_cast<uint32>(size);
        WriteInts(&uint32size, 1);
    }

    void DiSerializer::WriteFloats( const float* const pfloat, size_t count )
    {
        WriteData(pfloat, sizeof(float), count);
    }

    void DiSerializer::WriteFloats( const double* const pDouble, size_t count )
    {
        float* tmp = DI_NEW float[count];
        for (unsigned int i = 0; i < count; ++i)
        {
            tmp[i] = static_cast<float>(pDouble[i]);
        }
        WriteData(tmp, sizeof(float), count);
        DI_DELETE[] tmp;
    }

    void DiSerializer::WriteShorts( const uint16* const pShort, size_t count )
    {
        WriteData(pShort, sizeof(unsigned short), count);
    }

    void DiSerializer::WriteInts( const uint32* const pInt, size_t count )
    {
        WriteData(pInt, sizeof(unsigned int), count);
    }

    void DiSerializer::WriteBools( const bool* const pBool, size_t count )
    {
        WriteData(pBool, sizeof(bool), count);
    }

    void DiSerializer::WriteObject( const DiVec3& vec )
    {
        WriteFloats(vec.ptr(), 3);
    }

    void DiSerializer::WriteObject( const DiQuat& q )
    {
        float tmp[4] = { q.x, q.y, q.z, q.w };
        WriteFloats(tmp, 4);
    }

    void DiSerializer::WriteString( const DiString& string )
    {
        mStream->Write(string.c_str(), string.length());
        char terminator = '\n';
        mStream->Write(&terminator, 1);
    }

    void DiSerializer::WriteData( const void* const buf, size_t size, size_t count )
    {
        mStream->Write(buf, size * count);
    }

    const int DiSerializer::MSTREAM_OVERHEAD_SIZE = sizeof(uint16) + sizeof(uint32);

}