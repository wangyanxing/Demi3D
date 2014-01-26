
/********************************************************************
    File:       GLBufferManager.h
    Creator:    demiwangya
*********************************************************************/

namespace Demi
{
#define _GL_SCRATCH_THRESHOLD (1024 * 32)   //32k threshold

    struct ScratchBuffer
    {
        ScratchBuffer() :
        size(0),
        reserved(false)
        {
        }

        uint8*   data;
        unsigned size;
        bool     reserved;
    };

    class DI_GLDRV_API DiGLBufferManager
    {
    public:

        DiGLBufferManager();

        ~DiGLBufferManager();

    public:

        void*   AllocateScratch(uint32 size);

        void    DeallocateScratch(void* ptr);

        void*   AllocScratchBuffer(uint32 size);

        void    DeallocScratchBuffer(void* buffer);

        void    ReleaseScratchBuffers();

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

        DiVector<ScratchBuffer> mScratchBuffers;

        uint32  mMaxScratchBufRequest;
    };
}