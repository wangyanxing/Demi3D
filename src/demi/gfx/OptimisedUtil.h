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

#ifndef DiOptimisedUtil_h__
#define DiOptimisedUtil_h__


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

#endif
