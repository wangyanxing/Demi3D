
/********************************************************************
    File:       OptimisedUtil.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    class DI_GFX_API DiOptimisedUtil
    {
    public:
    
        DiOptimisedUtil(){}

        virtual ~DiOptimisedUtil(){}

    public:

        static DiOptimisedUtil* GetInstance(void);

        virtual void             ConcatenateAffineMatrices(
                                        const DiMat4& baseMatrix,
                                        const DiMat4* srcMatrices,
                                        DiMat4* dstMatrices,
                                        size_t numMatrices) = 0;


    protected:

        static DiOptimisedUtil* sImplementation;

        static DiOptimisedUtil* DetectImplementation(void);
    };

    template <class T>
    static FORCEINLINE T* RawOffsetPointer(T* ptr, ptrdiff_t offset)
    {
        return (T*)((char*)(ptr) + offset);
    }

    template <class T>
    static FORCEINLINE void AdvanceRawPointer(T*& ptr, ptrdiff_t offset)
    {
        ptr = RawOffsetPointer(ptr, offset);
    }
}