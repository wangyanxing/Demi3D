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
#include "PVRTC.h"
#include "DataStream.h"
#include "Image.h"
#include "Texture.h"

#define FOURCC(c0, c1, c2, c3) (c0 | (c1 << 8) | (c2 << 16) | (c3 << 24))
#define PVR_TEXTURE_FLAG_TYPE_MASK    0xff

namespace Demi
{
    
#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   pragma pack (push, 1)
#else
#   pragma pack (1)
#endif
    
    const uint32 PVR2_MAGIC = FOURCC('P', 'V', 'R', '!');
    const uint32 PVR3_MAGIC = FOURCC('P', 'V', 'R', 3);
    
    enum
    {
        kPVRTextureFlagTypePVRTC_2 = 24,
        kPVRTextureFlagTypePVRTC_4
    };
    
    enum
    {
        kPVRTC1_PF_2BPP_RGB,
        kPVRTC1_PF_2BPP_RGBA,
        kPVRTC1_PF_4BPP_RGB,
        kPVRTC1_PF_4BPP_RGBA,
        kPVRTC2_PF_2BPP,
        kPVRTC2_PF_4BPP
    };
    
    typedef struct _PVRTCTexHeaderV2
    {
        uint32 headerLength;
        uint32 height;
        uint32 width;
        uint32 numMipmaps;
        uint32 flags;
        uint32 dataLength;
        uint32 bpp;
        uint32 bitmaskRed;
        uint32 bitmaskGreen;
        uint32 bitmaskBlue;
        uint32 bitmaskAlpha;
        uint32 pvrTag;
        uint32 numSurfs;
    } PVRTCTexHeaderV2;
    
    typedef struct _PVRTCTexHeaderV3
    {
        uint32  version;         //Version of the file header, used to identify it.
        uint32  flags;           //Various format flags.
        uint64  pixelFormat;     //The pixel format, 8cc value storing the 4 channel identifiers and their respective sizes.
        uint32  colourSpace;     //The Colour Space of the texture, currently either linear RGB or sRGB.
        uint32  channelType;     //Variable type that the channel is stored in. Supports signed/unsigned int/short/byte or float for now.
        uint32  height;          //Height of the texture.
        uint32  width;           //Width of the texture.
        uint32  depth;           //Depth of the texture. (Z-slices)
        uint32  numSurfaces;     //Number of members in a Texture Array.
        uint32  numFaces;        //Number of faces in a Cube Map. Maybe be a value other than 6.
        uint32  mipMapCount;     //Number of MIP Maps in the texture - NB: Includes top level.
        uint32  metaDataSize;    //Size of the accompanying meta data.
    } PVRTCTexHeaderV3;
    
    typedef struct _PVRTCMetaData
    {
        uint32 DevFOURCC;
        uint32 u32Key;
        uint32 u32DataSize;
        uint8* Data;
    } PVRTCMetadata;
    
#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   pragma pack (pop)
#else
#   pragma pack ()
#endif

    DiPVRTC::DiPVRTC()
        :mWidth(0)
        ,mHeight(0)
        ,mFormat(DiPixelFormat::PIXEL_FORMAT_MAX)
        ,mMipmaps(0)
        ,mNumFaces(1)
    {
        
    }
    
    DiPVRTC::~DiPVRTC()
    {
        
    }
    
    bool DiPVRTC::CheckHead(const DiString& head)
    {
        if(head == "PVR!" || head == "PVR\x03")
            return true;
        return false;
    }
    
    DiPixelBox DiPVRTC::GetPixelBox(uint32 face, uint32 mipmap)
    {
        if(mipmap > mMipmaps)
        {
            DI_WARNING("Mipmap index out of range");
        }
        if(face >= mNumFaces)
        {
            DI_WARNING("Face index out of range");
        }
        
        // Calculate mipmap offset and size
        uint8 *offset = mData->GetPtr();
        
        // Figure out the offsets
        uint32 width = mWidth;
        uint32 height = mHeight;
        uint32 finalWidth = 0, finalHeight = 0;
        
        uint32 mipOffset = 0;
        uint32 lastLevelSize = 0;
        
        for(uint32 mip = 0; mip < mMipmaps; ++mip)
        {
            mipOffset += lastLevelSize * mNumFaces;
            lastLevelSize = DiPixelBox::ComputeImageByteSize(width, height, mFormat);
            if (mip == mipmap)
            {
                mipOffset += face * lastLevelSize;
                finalWidth = width;
                finalHeight = height;
                break;
            }
            
            /// Half size in each dimension
            if(width!=1) width /= 2;
            if(height!=1) height /= 2;
        }
        offset += mipOffset;

        // Return subface as pixelbox
        DiPixelBox src(finalWidth, finalHeight, mFormat, offset);
        return src;

    }
    
    void DiPVRTC::FlipEndian(void * pData, size_t size, size_t count) const
    {
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
        for(unsigned int index = 0; index < count; index++)
        {
            FlipEndian((void *)((long)pData + (index * size)), size);
        }
#endif
    }
    
    void DiPVRTC::FlipEndian(void * pData, size_t size) const
    {
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
        for(unsigned int byteIndex = 0; byteIndex < size/2; byteIndex++)
        {
            char swapByte = *(char *)((long)pData + byteIndex);
            *(char *)((long)pData + byteIndex) = *(char *)((long)pData + size - byteIndex - 1);
            *(char *)((long)pData + size - byteIndex - 1) = swapByte;
        }
#endif
    }
    
    bool DiPVRTC::Load(DiDataStreamPtr& stream)
    {
        // Assume its a pvr 2 header
        PVRTCTexHeaderV2 headerV2;
        stream->Read(&headerV2, sizeof(PVRTCTexHeaderV2));
        stream->Seek(0);
        
        if (PVR2_MAGIC == headerV2.pvrTag)
        {
            DecodeV2(stream);
            return true;
        }
        
        // Try it as pvr 3 header
        PVRTCTexHeaderV3 headerV3;
        stream->Read(&headerV3, sizeof(PVRTCTexHeaderV3));
        stream->Seek(0);
        
        if (PVR3_MAGIC == headerV3.version)
        {
            DecodeV3(stream);
            return true;
        }

        DI_WARNING("Invalid pvr format");
        return false;
    }
    
    void DiPVRTC::DecodeV2(DiDataStreamPtr& stream)
    {
        PVRTCTexHeaderV2 header;
        uint32 flags = 0, formatFlags = 0;
        mNumFaces = 1; // Assume one face until we know otherwise
        
        // Read the PVRTC header
        stream->Read(&header, sizeof(PVRTCTexHeaderV2));
        
        // Get format flags
        flags = header.flags;
        FlipEndian(reinterpret_cast<void*>(flags), sizeof(uint32));
        formatFlags = flags & PVR_TEXTURE_FLAG_TYPE_MASK;
        
        uint32 bitmaskAlpha = header.bitmaskAlpha;
        FlipEndian(reinterpret_cast<void*>(bitmaskAlpha), sizeof(uint32));
        
        if (formatFlags == kPVRTextureFlagTypePVRTC_4 || formatFlags == kPVRTextureFlagTypePVRTC_2)
        {
            if (formatFlags == kPVRTextureFlagTypePVRTC_4)
            {
                mFormat = bitmaskAlpha ? PF_PVRTC_RGBA4 : PF_PVRTC_RGB4;
            }
            else if (formatFlags == kPVRTextureFlagTypePVRTC_2)
            {
                mFormat = bitmaskAlpha ? PF_PVRTC_RGBA2 : PF_PVRTC_RGB2;
            }
            
            mWidth = header.width;
            mHeight = header.height;
            mMipmaps = static_cast<uint16>(header.numMipmaps);
        }
        
        // Calculate total size from number of mipmaps, faces and size
        uint32 size = DiPixelBox::ComputeImageByteSize(mMipmaps, mNumFaces, mWidth, mHeight, mFormat);

        // Bind output buffer
        auto data = make_shared<DiMemoryDataStream>(size);
        void *destPtr = data->GetPtr();
        stream->Read(destPtr, size);
        destPtr = static_cast<void*>(static_cast<uint8*>(destPtr));
        
        mData = data;
        mData->Seek(0);
    }

    void DiPVRTC::DecodeV3(DiDataStreamPtr& stream)
    {
        PVRTCTexHeaderV3 header;
        //PVRTCMetadata metadata;
        uint32 flags = 0;
        
        // Read the PVRTC header
        stream->Read(&header, sizeof(PVRTCTexHeaderV3));
        
        // Read the PVRTC metadata
        if(header.metaDataSize)
        {
//            stream->Read(&metadata, sizeof(PVRTCMetadata));
            stream->Skip(header.metaDataSize);
        }
        
        // Identify the pixel format
        switch (header.pixelFormat)
        {
            case kPVRTC1_PF_2BPP_RGB:
                mFormat = PF_PVRTC_RGB2;
                break;
            case kPVRTC1_PF_2BPP_RGBA:
                mFormat = PF_PVRTC_RGBA2;
                break;
            case kPVRTC1_PF_4BPP_RGB:
                mFormat = PF_PVRTC_RGB4;
                break;
            case kPVRTC1_PF_4BPP_RGBA:
                mFormat = PF_PVRTC_RGBA4;
                break;
            case kPVRTC2_PF_2BPP:
                mFormat = PF_PVRTC2_2BPP;
                break;
            case kPVRTC2_PF_4BPP:
                mFormat = PF_PVRTC2_4BPP;
                break;
        }
        
        // Get format flags
        flags = header.flags;
        FlipEndian(reinterpret_cast<void*>(flags), sizeof(uint32));
        
        mWidth = header.width;
        mHeight = header.height;
        // ignore 3d texture for now
        // uint32 depth = header.depth;
        mMipmaps = header.mipMapCount;
        mNumFaces = header.numFaces;
        
        // Calculate total size from number of mipmaps, faces and size
        uint32 size = DiPixelBox::ComputeImageByteSize(mMipmaps, mNumFaces,
                                             mWidth, mHeight, mFormat);
        
        // Bind output buffer
        auto output = make_shared<DiMemoryDataStream>(size);
        
        // Now deal with the data
        void *destPtr = output->GetPtr();
    
        uint32 w = mWidth;
        uint32 h = mHeight;
        // All mips for a surface, then each face
        for(size_t mip = 0; mip <= mMipmaps; ++mip)
        {
            size_t pvrSize = DiPixelBox::ComputeImageByteSize(w, h, mFormat);
            for(size_t surface = 0; surface < header.numSurfaces; ++surface)
            {
                for(size_t i = 0; i < mNumFaces; ++i)
                {
                    // Load directly
                    stream->Read(destPtr, pvrSize);
                    destPtr = static_cast<void*>(static_cast<uint8*>(destPtr) + pvrSize);
                }
            }
            
            // Next mip
            if(h!=1) h /= 2;
            if(w!=1) w /= 2;
        }
        
        mData = output;
        mData->Seek(0);
    }
}