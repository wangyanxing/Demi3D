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

#ifndef DiSerializer_h__
#define DiSerializer_h__


namespace Demi
{
    class DiSerializer : public DiBase
    {
    public:

        DiSerializer();
        
        virtual                 ~DiSerializer(){}

    public:

        uint16                   ReadChunk(DiDataStreamPtr& stream);

        void                     ReadBools(DiDataStreamPtr& stream, bool* pDest, size_t count);

        void                     ReadFloats(DiDataStreamPtr& stream, float* pDest, size_t count);

        void                     ReadFloats(DiDataStreamPtr& stream, double* pDest, size_t count);

        void                     ReadShorts(DiDataStreamPtr& stream, uint16* pDest, size_t count);

        void                     ReadInts(DiDataStreamPtr& stream, uint32* pDest, size_t count);

        void                     ReadObject(DiDataStreamPtr& stream, DiVec3& pDest);

        void                     ReadObject(DiDataStreamPtr& stream, DiQuat& pDest);

        DiString                 ReadString(DiDataStreamPtr& stream);
                                 
        DiString                 ReadString(DiDataStreamPtr& stream, size_t numChars);

        void                     WriteFloats(const float* const pfloat, size_t count);

        void                     WriteFloats(const double* const pfloat, size_t count);

        void                     WriteShorts(const uint16* const pShort, size_t count);

        void                     WriteInts(const uint32* const pInt, size_t count); 

        void                     WriteBools(const bool* const pLong, size_t count);

        void                     WriteObject(const DiVec3& vec);

        void                     WriteObject(const DiQuat& q);

        void                     WriteString(const DiString& string);

        void                     WriteData(const void* const buf, size_t size, size_t count);

        virtual void             WriteChunkHeader(uint16 id, size_t size);

        static const int         MSTREAM_OVERHEAD_SIZE;

        uint32                   mCurChunkSize;

        DiDataStreamPtr          mStream;
    };
}

#endif
