
/********************************************************************
    File:       PoolMap.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    template <typename T>
    class DI_GFX_API DiPoolMap
    {
    public:
        typedef std::multimap<DiString, T*> PoolMapMap;
        typedef typename PoolMapMap::iterator PoolMapIterator;
        PoolMapIterator mPoolMapIterator;

        DiPoolMap (bool managed = false) : mManaged(managed)
        {
            if (mManaged)
            {
            }
        }
        
        virtual ~DiPoolMap (void)
        {
            if (mManaged)
            {
            }
        }
    
        inline bool IsEmpty(void)
        {
            return mReleased.empty();
        }
        
        inline size_t GetSize(void)
        {
            return mReleased.size();
        }
        
        inline void ResetIterator (void)
        {
            mPoolMapIterator = mReleased.begin();
        }
        
        inline T* GetFirst (void)
        {
            ResetIterator();
            if (End())
            {
                return 0;
            }

            return mPoolMapIterator->second;
        }
        
        inline T* GetNext (void)
        {
            if (End())
            {
                return 0;
            }

            mPoolMapIterator++;
            if (End())
            {
                return 0;
            }
            
            return mPoolMapIterator->second;
        }
        
        inline bool End (void)
        {
            return mPoolMapIterator == mReleased.end();
        }
        
        inline void Clear (void)
        {
            mLocked.clear();
            mReleased.clear();
        }
    
        inline void AddElement (const DiString& key, T* element)
        {
            if (mManaged)
            {
                DI_ERROR("Cannot add element DiPoolMap::AddElement");
            }

            mLocked.insert(std::make_pair(key, element));
        }

        inline T* ReleaseElement (const DiString& key)
        {
            if (mLocked.empty())
            {
                return 0;
            }

            T* t = 0;
            PoolMapIterator it;
            it = mLocked.find(key);
            if (it != mLocked.end())
            {
                t = it->second;
                mReleased.insert(std::make_pair(key, t));
                mLocked.erase(it);
            }

            return t;
        }
        
        inline void ReleaseAllElements (void)
        {
            PoolMapIterator it;
            for (it = mLocked.begin(); it != mLocked.end(); ++it)
            {
                mReleased.insert(std::make_pair(it->first, it->second));
            }
            mLocked.clear();
            ResetIterator();
        }
    
        inline void LockLatestElement (void)
        {
            mLocked.insert(std::make_pair(mPoolMapIterator->first, mPoolMapIterator->second));
            mReleased.erase(mPoolMapIterator++); 
            if (mPoolMapIterator != mReleased.begin() && mPoolMapIterator != mReleased.end())
            {
                mPoolMapIterator--;
            }
        }
        
        inline void LockAllElements (void)
        {
            PoolMapIterator it;
            for (it = mReleased.begin(); it != mReleased.end(); ++it)
            {
                mLocked.insert(std::make_pair(it->first, it->second));
            }
            mReleased.clear();
            ResetIterator();
        }

    protected:
        bool        mManaged;

        PoolMapMap    mReleased;

        PoolMapMap    mLocked;
    };
}