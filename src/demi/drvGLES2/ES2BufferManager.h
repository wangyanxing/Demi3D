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

namespace Demi
{
#define _GL_SCRATCH_THRESHOLD (1024 * 32)   //32k threshold

    class DI_GLES2_API DiGLBufferManager
    {
    public:

        DiGLBufferManager();

        ~DiGLBufferManager();

    public:

        void*   AllocateScratch(uint32 size);

        void    DeallocateScratch(void* ptr);

        uint32  GetMapBufferThreshold() const
        {
            return mMapBufferThreshold;
        }

        void    SetMapBufferThreshold(const uint32 val)
        {
            mMapBufferThreshold = val;
        }

    protected:

        char*   mScratchBufferPool;

        uint32  mMapBufferThreshold;
    };
}