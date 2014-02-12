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

#ifndef DiStateCache_h__
#define DiStateCache_h__


namespace Demi
{
    class DiBitSetUtil
    {
    public:

        inline static void Set(uint32* bs, int bit, bool val)
        {
            bs[bit / 32] = val ? (bs[bit / 32] | (1 << (bit & (32 - 1)))) : (bs[bit / 32] & ~(1 << (bit & (32 - 1))));
        }

        inline static void Set(uint8* bs, int bit, bool val)
        {
            bs[bit / 8] = val ? (bs[bit / 8] | (uint8)(1 << (bit & (8 - 1)))) : (bs[bit / 8] & (uint8)(~(1 << (bit & (8 - 1)))));
        }

        inline static uint32 Set(uint32& bs, int bit, bool val)
        {
            return bs = val ? (bs | (1 << bit)) : (bs & ~(1 << bit));
        }

        inline static bool Test(const uint32* bs, int bit)
        {
            return (bs[bit / 32] & (1 << (bit & (32 - 1)))) != 0;
        }

        inline static bool Test(const uint8* bs, int bit)
        {
            return (bs[bit / 8] & (1 << (bit & (8 - 1)))) != 0;
        }

        inline static bool Test(const uint32 & bs, int bit)
        {
            return (bs & (1 << (bit & (32 - 1)))) != 0;
        }
    };

#define U32_FLAG_SIZE(x)    ((((x) + (32 - 1)))/32)

    template< typename T, int count >
    struct DiStateCache
    {
        static const int    SIZE = count;

        bool                Test(uint32 index)
        {
            return DiBitSetUtil::Test(flags, index);
        }

        void                ClearCache()
        {
            memset(flags, 0, sizeof(flags));
        }

        bool                SetCache(uint32 index, T& value)
        {
            if (Test(index))
            {
                T cur = states[index];
                if (value == cur)
                    return false;
            }

            Set(index, value);
            return true;
        }
        
        uint32                flags[U32_FLAG_SIZE(count)];

        T                    states[count];

    private:

        void                Set(int index, T& value)
        {
            DiBitSetUtil::Set(flags, index, true);
            states[index] = value;
        }
    };
}

#endif
