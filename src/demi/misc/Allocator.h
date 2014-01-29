
/********************************************************************
    File:       Allocator.h
    Creator:    demiwangya

    Adapted from urho3d engine (free source code)
*********************************************************************/

#ifndef DiAllocator_h__
#define DiAllocator_h__

#include <new>

namespace Demi
{
    struct DiAllocatorBlock;
    struct DiAllocatorNode;

    struct DiAllocatorBlock
    {
        unsigned            mNodeSize;
        unsigned            mCapacity;
        DiAllocatorNode*    mFree;
        DiAllocatorBlock*   mNext;
    };

    struct DiAllocatorNode
    {
        DiAllocatorNode* mNext;
    };

    /// Init a fixed-size allocator with the node size and initial capacity.
    DI_MISC_API DiAllocatorBlock* AllocatorInitialize(unsigned nodeSize, unsigned initialCapacity = 1);
    /// Uninitialize a fixed-size allocator. Frees all blocks in the chain.
    DI_MISC_API void AllocatorUninitialize(DiAllocatorBlock* allocator);
    /// Reserve a node. Creates a new block if necessary.
    DI_MISC_API void* AllocatorReserve(DiAllocatorBlock* allocator);
    /// Free a node. Does not free any blocks.
    DI_MISC_API void AllocatorFree(DiAllocatorBlock* allocator, void* ptr);

    /// %Allocator template class. Allocates objects of a specific class.
    template <class T> 
    class DI_MISC_API DiAllocator
    {
    public:
        DiAllocator(unsigned initialCapacity = 0) :
          mAllocator(0)
          {
              if (initialCapacity)
              {
                  mAllocator = AllocatorInitialize(sizeof(T), initialCapacity);
              }
          }

          ~DiAllocator()
          {
              AllocatorUninitialize(mAllocator);
          }

          /// Reserve and default-construct an object.
          T* Reserve()
          {
              if (!mAllocator)
              {
                  mAllocator = AllocatorInitialize(sizeof(T));
              }
              T* newObject = static_cast<T*>(AllocatorReserve(mAllocator));
              new(newObject) T();

              return newObject;
          }

          /// Reserve and copy-construct an object.
          T* Reserve(const T& object)
          {
              if (!mAllocator)
              {
                  mAllocator = AllocatorInitialize(sizeof(T));
              }
              T* newObject = static_cast<T*>(AllocatorReserve(mAllocator));
              new(newObject) T(object);

              return newObject;
          }

          /// Destruct and free an object.
          void Free(T* object)
          {
              (object)->~T();
              AllocatorFree(mAllocator, object);
          }

    private:
        /// Prevent copy construction.
        DiAllocator(const DiAllocator<T>& rhs);

        /// Prevent assignment.
        DiAllocator<T>& operator = (const DiAllocator<T>& rhs);

        /// Allocator block.
        DiAllocatorBlock* mAllocator;
    };
}
#endif // Allocator_h__
