
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

/****************************************************
  This module was originally from Particle Universe
  Repo: https://github.com/scrawl/particleuniverse
  License: MIT 
****************************************************/

#ifndef ParticlePoolBase_h__
#define ParticlePoolBase_h__

#include "K2Prerequisites.h"

namespace Demi
{
    template <typename T>
    class DEMI_K2_API DiParticlePoolBase
    {
    public:
        typedef std::list<T*> PoolList;
        typedef typename PoolList::iterator PoolIterator;
        PoolIterator mPoolIterator;

        
        DiParticlePoolBase (bool managed = false) : mManaged(managed)
        {
            if (mManaged)
            {
            }
        }
        
        virtual ~DiParticlePoolBase (void)
        {
            if (mManaged)
            {
            }
        }
        
        inline bool            IsEmpty(void)
        {
            return mReleased.empty();
        }
        
        inline size_t        GetSize(void)
        {
            return mReleased.size();
        }
        
        inline void            ResetIterator (void)
        {
            mPoolIterator = mReleased.begin();
        }
        
        inline T*            GetFirst (void)
        {
            ResetIterator();
            if (End())
            {
                return 0;
            }

            return *mPoolIterator;
        }
        
        inline T*            GetNext (void)
        {
            if (End())
            {
                return 0;
            }

            mPoolIterator++;
            if (End())
            {
                return 0;
            }

            return *mPoolIterator;
        }
        
        inline bool            End (void)
        {
            return mPoolIterator == mReleased.end();
        }
        
        inline void            Clear (void)
        {
            mLocked.clear();
            mReleased.clear();
        }
    
        inline void            AddElement (T* element)
        {
            if (mManaged)
            {
                DI_WARNING("Managing, cannot add element");
                return;
            }

            mLocked.push_back(element);
        }
        
        inline T*            ReleaseElement (void)
        {
            if (mLocked.empty())
            {
                return 0;
            }

            T* t = mLocked.front();
            mReleased.splice(mReleased.end(), mLocked, mLocked.begin());
            return t;
        }
        
        inline void            ReleaseAllElements (void)
        {
            mReleased.splice(mReleased.end(), mLocked);
            ResetIterator();
        }
        
        inline void            LockLatestElement (void)
        {
            mLocked.push_back(*mPoolIterator);
            mPoolIterator = mReleased.erase(mPoolIterator);
            if (mPoolIterator != mReleased.begin() && mPoolIterator != mReleased.end())
            {
                mPoolIterator--;
            }
        }
        
        inline void            LockAllElements (void)
        {
            mLocked.splice(mLocked.end(), mReleased);
            ResetIterator();
        }

        inline PoolList&    GetActiveElementsList(void)
        {
            return mReleased;
        }

    protected:
        bool        mManaged;

        PoolList    mReleased;

        PoolList    mLocked;
    };
}

#endif // ParticlePoolBase_h__
