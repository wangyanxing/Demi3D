
/********************************************************************
    File:       RadixSort.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    template <class TContainer, class TContainerValueType, typename TCompValueType>
    class DiRadixSort
    {
    public:
        typedef typename TContainer::iterator ContainerIter;

    protected:
        /// Alpha-pass counters of values (histogram)
        /// 4 of them so we can radix sort a maximum of a 32bit value
        int mCounters[4][256];
        /// Beta-pass offsets 
        int mOffsets[256];
        /// Sort area size
        int mSortSize;
        /// Number of passes for this type
        int mNumPasses;

        struct SortEntry
        {
            TCompValueType key;
            ContainerIter iter;
            SortEntry() {}
            SortEntry(TCompValueType k, ContainerIter it)
                : key(k), iter(it) {}

        };

        typedef DiVector<SortEntry> SortVector; 
        SortVector  mSortArea1;
        SortVector  mSortArea2;
        SortVector* mSrc;
        SortVector* mDest;
        TContainer  mTmpContainer;

        void SortPass(int byteIndex)
        {
            // Calculate offsets
            // Basically this just leaves gaps for duplicate entries to fill
            mOffsets[0] = 0;
            for (int i = 1; i < 256; ++i)
            {
                mOffsets[i] = mOffsets[i-1] + mCounters[byteIndex][i-1];
            }

            // Sort pass
            for (int i = 0; i < mSortSize; ++i)
            {
                unsigned char byteVal = getByte(byteIndex, (*mSrc)[i].key);
                (*mDest)[mOffsets[byteVal]++] = (*mSrc)[i];
            }

        }
        template <typename T>
        void FinalPass(int byteIndex, T val)
        {
            // default is to do normal pass
            SortPass(byteIndex);
        }
        
        // special case signed int
        void FinalPass(int byteIndex, int val)
        {
            int numNeg = 0;
            // all negative values are in entries 128+ in most significant byte
            for (int i = 128; i < 256; ++i)
            {
                numNeg += mCounters[byteIndex][i];
            }
            // Calculate offsets - positive ones start at the number of negatives
            // do positive numbers
            mOffsets[0] = numNeg;
            for (int i = 1; i < 128; ++i)
            {
                mOffsets[i] = mOffsets[i-1] + mCounters[byteIndex][i-1];
            }
            // Do negative numbers (must start at zero)
            // No need to invert ordering, already correct (-1 is highest number)
            mOffsets[128] = 0;
            for (int i = 129; i < 256; ++i)
            {
                mOffsets[i] = mOffsets[i-1] + mCounters[byteIndex][i-1];
            }

            // Sort pass
            for (int i = 0; i < mSortSize; ++i)
            {
                unsigned char byteVal = getByte(byteIndex, (*mSrc)[i].key);
                (*mDest)[mOffsets[byteVal]++] = (*mSrc)[i];
            }
        }
        

        // special case float
        void FinalPass(int byteIndex, float val)
        {
            // floats need to be special cased since negative numbers will come
            // after positives (high bit = sign) and will be in reverse order
            // (no ones-complement of the +ve value)
            int numNeg = 0;
            // all negative values are in entries 128+ in most significant byte
            for (int i = 128; i < 256; ++i)
            {
                numNeg += mCounters[byteIndex][i];
            }
            // Calculate offsets - positive ones start at the number of negatives
            // do positive numbers normally
            mOffsets[0] = numNeg;
            for (int i = 1; i < 128; ++i)
            {
                mOffsets[i] = mOffsets[i-1] + mCounters[byteIndex][i-1];
            }
            // Do negative numbers (must start at zero)
            // Also need to invert ordering
            // In order to preserve the stability of the sort (essential since
            // we rely on previous bytes already being sorted) we have to count
            // backwards in our offsets from 
            mOffsets[255] = mCounters[byteIndex][255];
            for (int i = 254; i > 127; --i)
            {
                mOffsets[i] = mOffsets[i+1] + mCounters[byteIndex][i];
            }

            // Sort pass
            for (int i = 0; i < mSortSize; ++i)
            {
                unsigned char byteVal = getByte(byteIndex, (*mSrc)[i].key);
                if (byteVal > 127)
                {
                    // -ve; pre-decrement since offsets set to count
                    (*mDest)[--mOffsets[byteVal]] = (*mSrc)[i];
                }
                else
                {
                    // +ve
                    (*mDest)[mOffsets[byteVal]++] = (*mSrc)[i];
                }
            }
        }

        inline unsigned char getByte(int byteIndex, TCompValueType val)
        {
            return ((unsigned char*)(&val))[byteIndex];
        }

    public:

        DiRadixSort() {}
        ~DiRadixSort() {}

        template <class TFunction>
        void Sort(TContainer& container, TFunction func)
        {
            if (container.empty())
            {
                return;
            }

            // Set up the sort areas
            mSortSize = static_cast<int>(container.size());
            mSortArea1.resize(container.size());
            mSortArea2.resize(container.size());

            // Copy data now (we need constant iterators for sorting)
            mTmpContainer = container;

            mNumPasses = sizeof(TCompValueType);

            // Counter pass
            // Initialise the counts
            int p;
            for (p = 0; p < mNumPasses; ++p)
            {
                memset(mCounters[p], 0, sizeof(int) * 256);
            }

            // Perform alpha pass to count
            ContainerIter i = mTmpContainer.begin();
            TCompValueType prevValue = func.operator()(*i); 
            bool needsSorting = false;
            for (int u = 0; i != mTmpContainer.end(); ++i, ++u)
            {
                // get sort value
                TCompValueType val = func.operator()(*i);
                // cheap check to see if needs sorting (temporal coherence)
                if (!needsSorting && val < prevValue)
                {
                    needsSorting = true;
                }

                // Create a sort entry
                mSortArea1[u].key = val;
                mSortArea1[u].iter = i;

                // increase counters
                for (p = 0; p < mNumPasses; ++p)
                {
                    unsigned char byteVal = getByte(p, val);
                    mCounters[p][byteVal]++;
                }

                prevValue = val;

            }

            // early exit if already sorted
            if (!needsSorting)
            {
                return;
            }

            // Sort passes
            mSrc = &mSortArea1;
            mDest = &mSortArea2;

            for (p = 0; p < mNumPasses - 1; ++p)
            {
                SortPass(p);
                // flip src/dst
                SortVector* tmp = mSrc;
                mSrc = mDest;
                mDest = tmp;
            }
            // Final pass may differ, make polymorphic
            FinalPass(p, prevValue);

            // Copy everything back
            int c = 0;
            for (i = container.begin(); 
                i != container.end(); ++i, ++c)
            {
                *i = *((*mDest)[c].iter);
            }
        }

    };
}