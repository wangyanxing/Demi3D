


namespace Demi
{
#define _GL_SCRATCH_THRESHOLD (1024 * 32)   //32k threshold

    class DI_GLDRV_API DiGLBufferManager
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