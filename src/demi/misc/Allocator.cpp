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
#include "MiscPch.h"
#include "Allocator.h"

namespace Demi
{
    DiAllocatorBlock* AllocatorReserveBlock(DiAllocatorBlock* allocator, unsigned nodeSize, unsigned capacity)
    {
        if (!capacity)
        {
            capacity = 1;
        }

        unsigned char* blockPtr = DI_NEW unsigned char[sizeof(DiAllocatorBlock) + capacity * (sizeof(DiAllocatorNode) + nodeSize)];
        DiAllocatorBlock* newBlock = reinterpret_cast<DiAllocatorBlock*>(blockPtr);
        newBlock->mNodeSize = nodeSize;
        newBlock->mCapacity = capacity;
        newBlock->mFree = 0;
        newBlock->mNext = 0;

        if (!allocator)
        {
            allocator = newBlock;
        }
        else
        {
            newBlock->mNext = allocator->mNext;
            allocator->mNext = newBlock;
        }

        // Init the nodes. Free nodes are always chained to the first (parent) allocator
        unsigned char* nodePtr = blockPtr + sizeof(DiAllocatorBlock);
        DiAllocatorNode* firstNewNode = reinterpret_cast<DiAllocatorNode*>(nodePtr);

        for (unsigned i = 0; i < capacity; ++i)
        {
            DiAllocatorNode* newNode = reinterpret_cast<DiAllocatorNode*>(nodePtr);

            if (i < capacity - 1)
            {
                newNode->mNext = reinterpret_cast<DiAllocatorNode*>(nodePtr + sizeof(DiAllocatorNode) + nodeSize);
            }
            else
            {
                newNode->mNext = allocator->mFree;
            }

            nodePtr += sizeof(DiAllocatorNode) + nodeSize;
        }

        allocator->mFree = firstNewNode;

        return newBlock;
    }

    DiAllocatorBlock* AllocatorInitialize(unsigned nodeSize, unsigned initialCapacity)
    {
        DiAllocatorBlock* block = AllocatorReserveBlock(0, nodeSize, initialCapacity);
        return block;
    }

    void AllocatorUninitialize(DiAllocatorBlock* allocator)
    {
        while (allocator)
        {
            DiAllocatorBlock* next = allocator->mNext;
            delete[] reinterpret_cast<unsigned char*>(allocator);
            allocator = next;
        }
    }

    void* AllocatorReserve(DiAllocatorBlock* allocator)
    {
        if (!allocator)
        {
            return 0;
        }

        if (allocator->mFree)
        {
            DiAllocatorNode* freeNode = allocator->mFree;
            void* ptr = (reinterpret_cast<unsigned char*>(freeNode)) + sizeof(DiAllocatorNode);
            allocator->mFree = freeNode->mNext;
            freeNode->mNext = 0;
            return ptr;
        }

        // Free nodes have been exhausted. Allocate a new larger block
        unsigned newCapacity = (allocator->mCapacity + 1) >> 1;
        AllocatorReserveBlock(allocator, allocator->mNodeSize, newCapacity);
        allocator->mCapacity += newCapacity;

        // We should have new free node(s) chained
        DiAllocatorNode* freeNode = allocator->mFree;
        void* ptr = (reinterpret_cast<unsigned char*>(freeNode)) + sizeof(DiAllocatorNode);
        allocator->mFree = freeNode->mNext;
        freeNode->mNext = 0;

        return ptr;
    }

    void AllocatorFree(DiAllocatorBlock* allocator, void* ptr)
    {
        if (!allocator || !ptr)
            return;

        unsigned char* dataPtr = static_cast<unsigned char*>(ptr);
        DiAllocatorNode* node = reinterpret_cast<DiAllocatorNode*>(dataPtr - sizeof(DiAllocatorNode));

        // Chain the node back to free nodes
        node->mNext = allocator->mFree;
        allocator->mFree = node;
    }
}
