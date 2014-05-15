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

#ifndef DiPVRTC_h__
#define DiPVRTC_h__

#include "GfxPrerequisites.h"

namespace Demi 
{
    class DI_GFX_API DiPVRTC : public DiBase
    {

    public:

        DiPVRTC();

        ~DiPVRTC(void);

    public:
        
        /// now we can directly save the image data to texture,
        /// or pass a null texture pointer and save the image to memory
        bool Load(DiDataStreamPtr& stream);
        
        bool IsCubeMap() const {return mNumFaces == 6;}
        
        DiPixelBox GetPixelBox(uint32 face, uint32 mipmap);
        
        static bool CheckHead(const DiString& head);
        
    private:

        /// Decode PVRTCV3 image format
        void DecodeV2(DiDataStreamPtr& stream);
        
        /// Decode PVRTCV3 image format
        void DecodeV3(DiDataStreamPtr& stream);
        
        void FlipEndian(void * pData, size_t size, size_t count) const;
        
        void FlipEndian(void * pData, size_t size) const;

    public:
        
        /// loaded data
        DiMemoryDataStreamPtr mData;
        
        uint32          mWidth;
        
        uint32          mHeight;
        
        DiPixelFormat   mFormat;
        
        uint32          mMipmaps;
        
        uint32          mNumFaces;
    };
}

#endif
