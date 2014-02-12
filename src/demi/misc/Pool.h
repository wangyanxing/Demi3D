


#ifndef DiPool_h__
#define DiPool_h__


#include "SimpleSet.h"

namespace Demi
{
    template <class T, class TAlloc> 
    class DiPoolContainer
    {
    public:
        DiPoolContainer(unsigned int uiSize);

        ~DiPoolContainer();

        inline T*                    GetObject(unsigned int uiIndex);

        inline void                  SetNext(DiPoolContainer<T, TAlloc>* pkNext);

    protected:
        T*                           mObjectArray;

        unsigned int                 mSize;

        DiPoolContainer<T,TAlloc>*   mNext;

    private:

        DiPoolContainer(const DiPoolContainer&);

        DiPoolContainer&             operator = (const DiPoolContainer&);
    };

    template <class T, class TAlloc>
    class DiPool
    {
    public:
        DiPool(unsigned int uiInitialSize = 8);

        ~DiPool();

        inline T*                     GetFreeObject();

        inline void                   ReleaseObject(T* pkObject);

        inline void                   ReleaseAll();

    protected:

        inline void                   CreateNewObjects(uint32 uiSize);

        DiSimplePodSet<T*>            mFreeObjects;

        DiPoolContainer<T, TAlloc>*   mContainers;

        unsigned int                  mCurrentSize;

        unsigned int                  mInitialSize;

    private:

        DiPool(const DiPool&);

        DiPool&                       operator=(const DiPool&);
    };

    //////////////////////////////////////////////////////////////////////////

    template <class T> 
    class DiObjectPool : public DiPool<T, DiNewInterface<T> >
    {
    public:
        DiObjectPool(unsigned int uiInitialSize = 8);
    };

    //////////////////////////////////////////////////////////////////////////

    template <class T> 
    class DiPodPool : public DiPool<T, DiMallocInterface<T> >
    {
    public:
        DiPodPool(unsigned int uiInitialSize = 8);
    };

    //////////////////////////////////////////////////////////////////////////

    template <class T, class TAlloc>
    inline DiPoolContainer<T,TAlloc>::DiPoolContainer(unsigned int uiSize) :
        mSize(uiSize),
        mNext(NULL)
    {
        if (uiSize > 0)
        {
            mObjectArray = TAlloc::Allocate(uiSize);
            DI_ASSERT(mObjectArray != NULL);
        }
        else
        {
            mObjectArray = NULL;
        }
    }
    
    template <class T, class TAlloc>
    inline DiPoolContainer<T,TAlloc>::~DiPoolContainer()
    {
        TAlloc::Deallocate(mObjectArray);
        DI_DELETE mNext;
    }
    
    template <class T, class TAlloc>
    inline T* DiPoolContainer<T,TAlloc>::GetObject(unsigned int uiIndex)
    {
        if (uiIndex >= mSize)
        {
            return NULL;
        }
        return &mObjectArray[uiIndex];
    }
    
    template <class T, class TAlloc>
    inline void DiPoolContainer<T,TAlloc>::SetNext(DiPoolContainer<T,TAlloc>* pkNext)
    {
        mNext = pkNext;
    }
    
    template <class T, class TAlloc>
    inline DiPool<T,TAlloc>::DiPool(unsigned int uiInitialSize) :
        mContainers(NULL),
        mCurrentSize(uiInitialSize),
        mInitialSize(uiInitialSize)
    {
        DI_ASSERT(uiInitialSize > 0);
    }
    
    template <class T, class TAlloc>
    inline DiPool<T,TAlloc>::~DiPool()
    {
        mFreeObjects.RemoveAll();
        DI_DELETE mContainers;
    }
    
    template <class T, class TAlloc>
    inline T* DiPool<T,TAlloc>::GetFreeObject()
    {
        if (mFreeObjects.GetSize() == 0)
        {
            CreateNewObjects(mCurrentSize);
            mCurrentSize *= 2;
        }

        T* pkReturn = mFreeObjects.GetAt(0);
        mFreeObjects.RemoveAt(0);
        return pkReturn;
    }

    template <class T, class TAlloc>
    inline void DiPool<T,TAlloc>::ReleaseObject(T* pkObject)
    {
        DI_ASSERT(mFreeObjects.Find(pkObject) == -1);
        mFreeObjects.Add(pkObject);
    }
    
    template <class T, class TAlloc>
    inline void DiPool<T,TAlloc>::CreateNewObjects(unsigned int uiSize)
    {
        DiPoolContainer<T,TAlloc>* pkNewContainer = DI_NEW DiPoolContainer<T, TAlloc>(uiSize);
        for (unsigned int i = 0; i < uiSize; i++)
        {
            mFreeObjects.Add(pkNewContainer->GetObject(i));
        }

        pkNewContainer->SetNext(mContainers);
        mContainers = pkNewContainer;
    }
    
    template <class T, class TAlloc>
    inline void DiPool<T,TAlloc>::ReleaseAll()
    {
        mFreeObjects.RemoveAll();
        DI_DELETE mContainers;
        mContainers = NULL;
        mCurrentSize = mInitialSize;
    }
    
    template <class T>
    inline DiObjectPool<T>::DiObjectPool(unsigned int uiInitialSize) : 
    DiPool<T, DiNewInterface<T> >(uiInitialSize)
    {  
    }
    
    template <class T>
    inline DiPodPool<T>::DiPodPool(unsigned int uiInitialSize) :
    DiPool<T, DiMallocInterface<T> >(uiInitialSize)
    {
    }
}

#endif
