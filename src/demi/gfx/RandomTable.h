


#ifndef DiRandomTable_h__
#define DiRandomTable_h__


#include <time.h>
#include "MersenneTwister.h"

namespace Demi
{
    class DiRandomTable
    {
    public:
        DiRandomTable(unsigned long size=0x8000);

        ~DiRandomTable();

        void     ResetRandomIndex();

        float    GetUnitRandom();

        float    GetRangeRandom(float start, float end);

    protected:
        unsigned long    mTableSize;

        float*           mTable;

        unsigned long    mCustomRandomIndex;

        void             GenerateRandomNumbers();    
    };


    inline DiRandomTable::DiRandomTable(unsigned long size) : mTableSize(size), mTable(0), mCustomRandomIndex(0)
    {
        mTable = (float*)malloc(sizeof(float) * mTableSize);
        GenerateRandomNumbers();
    }

    inline DiRandomTable::~DiRandomTable()
    {
        if(mTable)
        {
            free(mTable);
            mTable=0;
        }
    }

    inline void DiRandomTable::ResetRandomIndex()
    {
        mCustomRandomIndex = 0;
    }

    inline float DiRandomTable::GetUnitRandom()
    {
        if(mCustomRandomIndex > mTableSize - 1)
        {
            mCustomRandomIndex = 0;
        }
        return mTable[mCustomRandomIndex++];
    }

    inline float DiRandomTable::GetRangeRandom(float start, float end)
    {
        return (start + ((end - start) * GetUnitRandom()));
    }

    inline void DiRandomTable::GenerateRandomNumbers()
    {
        DiRand mtrand(time(NULL));

        for(unsigned long i = 0; i < mTableSize; i++)
        {
            mTable[i] = (float)mtrand.Rand();
        }
    }

}


#endif
