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

#ifndef GLTexture_h__
#define GLTexture_h__

#include "Texture.h"

namespace Demi
{
    class DI_GLDRV_API DiGLTextureDrv : public DiTextureDrv
    {
    public:

        DiGLTextureDrv(DiTexture* parent);

        ~DiGLTextureDrv();

    public:

        void                    CreateTexture();

        void                    Release();
        
        void*                   LockLevel(uint32 level, uint32 surface = 0, DiLockFlag lockflag = LOCK_NORMAL);

        void                    UnlockLevel(uint32 level, uint32 surface = 0);

        void                    Bind(uint32 samplerIndex);

        void                    CopyToMemory(const DiBox &srcBox, const DiPixelBox &dst);

        void                    CopyFromMemory(const DiPixelBox &srcBox, const DiBox &dst, uint32 level, uint32 surface = 0);

        void*                   GetSurfaceHandle();

        void                    Upload(const DiPixelBox &src, const DiBox &dst, uint32 level, uint32 surface = 0);

        void                    Download(const DiPixelBox &data, uint32 level, uint32 surface);

        void                    BindToFrameBuffer(GLenum attachment, uint32 level, uint32 surface);

    private:

        void                    AllocateBuffer();

        void                    DeallocateBuffer();

    private:

        DiLockFlag              mCurrentLockFlag;

        DiPixelBox*             mBuffer;

        GLenum                  mGLFormat;

        GLenum                  mGLOriginFormat;

        GLenum                  mGLTextureType;

        GLuint                  mTextureID;

        uint32                  mImageSize;
    };
}
#endif // GLTexture_h__