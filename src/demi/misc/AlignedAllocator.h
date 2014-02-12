


#ifndef DiAlignedAllocator_h__
#define DiAlignedAllocator_h__


namespace Demi
{
    class DI_MISC_API DiAlignedMemory
    {
    public:
       
        static void*    Allocate(size_t size, size_t alignment);

        static void*    Allocate(size_t size);

        static void     Deallocate(void* p);
    };
}

#endif
