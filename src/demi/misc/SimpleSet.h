
/********************************************************************
    File:       SimpleSet.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "MemoryAllocator.h"

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   pragma warning(disable:4100)
#endif

namespace Demi
{
    template <class T, class TAlloc> 
    class DiTSimpleSet
    {
    public:
        DiTSimpleSet(unsigned int uiInitialSize = 0)
        {
            if (uiInitialSize > 0)
            {
                mBase = TAlloc::Allocate(uiInitialSize);
                DI_ASSERT(mBase != NULL);
            }
            else
            {
                mBase = NULL;
            }
            mAlloced = uiInitialSize;
            mUsed = 0;
        }

        ~DiTSimpleSet()
        {
            TAlloc::Deallocate(mBase);
        }

        inline unsigned int GetSize() const
        {
            return mUsed;
        }
        
        inline T*            GetBase() const
        {
            return mBase;
        }
        
        inline const T&        GetAt(unsigned int uiIndex) const
        {
            DI_ASSERT(uiIndex < mUsed);
            return mBase[uiIndex];
        }
        
        inline T&            GetAt(unsigned int uiIndex)
        {
            DI_ASSERT(uiIndex < mUsed);
            return mBase[uiIndex];
        }
        
        inline unsigned int Add(const T& element)
        {
            DI_ASSERT(mUsed <= mAlloced);

            if (mUsed == mAlloced)
            {
                Realloc(mAlloced > 0 ? (2 * mAlloced) : 1);
            }

            DI_ASSERT(mUsed < mAlloced);
            mBase[mUsed++] = element;

            return mUsed - 1;
        }
        
        inline unsigned int AddUnique(const T& element)
        {
            DI_ASSERT(mUsed <= mAlloced);

            int iFind = Find(element);
            if (iFind == -1)
            {
                return Add(element);
            }
            else
            {
                return (unsigned int) iFind;
            }
        }
        
        inline void         RemoveAt(unsigned int uiIndex)
        {
            DI_ASSERT(uiIndex < mUsed);

            mBase[uiIndex] = mBase[--mUsed];
        }
        
        inline void         OrderedRemoveAt(unsigned int uiIndex)
        {
            DI_ASSERT(uiIndex < mUsed);

            for (unsigned int ui = uiIndex; ui < mUsed - 1; ui++)
            {
                mBase[ui] = mBase[ui + 1];
            }
            mUsed--;
        }
        
        inline void         ReplaceAt(unsigned int uiIndex, const T& element)
        {
            if (uiIndex >= mUsed)
            {
                return;
            }

            mBase[uiIndex] = element;
        }
        
        inline void         RemoveAll()
        {
            mUsed = 0;
        }

        inline int            Find(const T& element) const
        {
            DI_ASSERT(mUsed <= mAlloced);

            unsigned int i;

            for (i = 0; i < mUsed; i++)
            {
                if (mBase[i] == element)
                {
                    return i;
                }
            }
            return -1;
        }

        inline bool            Contains(const T& element) const
        {
            return Find(element) != -1;
        }

        inline void            Realloc()
        {
            Realloc(mUsed);
        }

        void                Realloc(unsigned int uiNewSize)
        {
            if (uiNewSize < mUsed)
            {
                uiNewSize = mUsed;
            }

            if (uiNewSize != mAlloced)
            {
                T *pNewBase;
                unsigned int i;

                if (uiNewSize > 0)
                {
                    pNewBase = TAlloc::Allocate(uiNewSize);
                    DI_ASSERT(pNewBase != NULL);

                    for (i = 0; i < mUsed; i++)
                    {
                        pNewBase[i] = mBase[i];
                    }
                }
                else
                {
                    pNewBase = NULL;
                }

                TAlloc::Deallocate(mBase);
                mBase = pNewBase;
                mAlloced = uiNewSize;
            }
        }

        inline unsigned int GetAllocationSize() const
        {
            return mAlloced;
        }

    protected: 
        T*                    mBase;

        unsigned int        mAlloced;

        unsigned int        mUsed;

    private:
        DiTSimpleSet(const DiTSimpleSet&);

        DiTSimpleSet& operator=(const DiTSimpleSet&);
    };

    template <class T> 
    class DiSimpleSet:public DiTSimpleSet<T, DiNewInterface<T> >
    {
    public:
        DiSimpleSet(unsigned int uiInitialSize = 0)
        {
        }
    };

    template <class T> 
    class DiSimplePodSet:public DiTSimpleSet<T, DiMallocInterface<T> >
    {
    public:
        DiSimplePodSet(unsigned int uiInitialSize = 0)
        {
        }
    };

    typedef DiSimplePodSet<unsigned int>    DiSpUintSet;
    typedef DiSimplePodSet<unsigned short>    DiSpUshortSet;
    typedef DiSimplePodSet<float>            DiSpFloatSet;

}