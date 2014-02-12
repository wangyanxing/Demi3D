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

#ifndef DiProfiler_h__
#define DiProfiler_h__


#include "Singleton.h"
#include "Timer.h"

namespace Demi
{
    class DiProfilerBlock
    {
    public:
        DiProfilerBlock(DiProfilerBlock* parent, const char* name) :
            mName(name),
            mTime(0),
            mMaxTime(0),
            mCount(0),
            mParent(parent),
            mFrameTime(0),
            mFrameMaxTime(0),
            mFrameCount(0),
            mIntervalTime(0),
            mIntervalMaxTime(0),
            mIntervalCount(0),
            mTotalTime(0),
            mTotalMaxTime(0),
            mTotalCount(0)
        {
        }

        ~DiProfilerBlock()
        {
            for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
            {
                delete *i;
                *i = 0;
            }
        }

        void begin()
        {
            mTimer.Start();
            ++mCount;
        }

        void end()
        {
            double time = mTimer.GetElapse()*1000;
            if (time > mMaxTime)
                mMaxTime = time;
            mTime += time;
        }

        void EndFrame()
        {
            mFrameTime = mTime;
            mFrameMaxTime = mMaxTime;
            mFrameCount = mCount;
            mIntervalTime += mTime;
            if (mMaxTime > mIntervalMaxTime)
                mIntervalMaxTime = mMaxTime;
            mIntervalCount += mCount;
            mTotalTime += mTime;
            if (mMaxTime > mTotalMaxTime)
                mTotalMaxTime = mMaxTime;
            mTotalCount += mCount;
            mTime = 0;
            mMaxTime = 0;
            mCount = 0;

            for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
                (*i)->EndFrame();
        }

        void BeginInterval()
        {
            mIntervalTime = 0;
            mIntervalMaxTime = 0;
            mIntervalCount = 0;

            for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
                (*i)->BeginInterval();
        }

        DiProfilerBlock* GetChild(const char* name)
        {
            // First check using string pointers only, then resort to actual strcmp
            for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
            {
                if ((*i)->mName == name)
                    return *i;
            }

            for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
            {
                if (!DiString::StrCmp((*i)->mName, name))
                    return *i;
            }

            DiProfilerBlock* newBlock = new DiProfilerBlock(this, name);
            mChildren.push_back(newBlock);

            return newBlock;
        }

        const char*                 mName;

        DiTimer                     mTimer;
        
        double                      mTime;
        
        double                      mMaxTime;
        
        unsigned                    mCount;
        
        DiProfilerBlock*            mParent;
        
        DiVector<DiProfilerBlock*>  mChildren;
        
        double                      mFrameTime;
        
        double                      mFrameMaxTime;
        
        unsigned                    mFrameCount;
        
        double                      mIntervalTime;
        
        double                      mIntervalMaxTime;
        
        unsigned                    mIntervalCount;
        
        double                      mTotalTime;
        
        double                      mTotalMaxTime;
        
        unsigned                    mTotalCount;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class DI_MISC_API DiProfiler : public DiSingleton<DiProfiler>
    {
    public:
        DiProfiler();

        virtual ~DiProfiler();

        _DECLARE_SINGLETON(DiProfiler)

        static void InitProfiler()
        {
            DI_NEW DiProfiler();
        }

        static void CloseProfiler()
        {
            DI_DELETE DiProfiler::GetInstancePtr();
        }

        /// begin timing a profiling block.
        void BeginBlock(const char* name)
        {
            mCurrent = mCurrent->GetChild(name);
            mCurrent->begin();
        }

        /// end timing the current profiling block.
        void EndBlock()
        {
            if (mCurrent != mRoot)
            {
                mCurrent->end();
                mCurrent = mCurrent->mParent;
            }
        }

        void BeginFrame();

        void EndFrame();

        void BeginInterval();

        DiString GetData(bool showUnused = false, bool showTotal = false, unsigned maxDepth = 1000000) const;

        const DiProfilerBlock* GetCurrentBlock() { return mCurrent; }

        const DiProfilerBlock* GetRootBlock() { return mRoot; }

    private:

        void GetData(DiProfilerBlock* block, DiString& output, unsigned depth, unsigned maxDepth, bool showUnused, bool showTotal) const;

        DiProfilerBlock*    mCurrent;

        DiProfilerBlock*    mRoot;
        
        unsigned            mIntervalFrames;
        
        unsigned            mTotalFrames;
    };

    class DiAutoProfileBlock
    {
    public:
        DiAutoProfileBlock(const char* name)
        {
            if (DiProfiler::GetInstancePtr())
                DiProfiler::GetInstancePtr()->BeginBlock(name);
        }

        ~DiAutoProfileBlock()
        {
            if (DiProfiler::GetInstancePtr())
                DiProfiler::GetInstancePtr()->EndBlock();
        }
    };

#ifdef ENABLE_PROFILE
#   define DI_PROFILE(name) DiAutoProfileBlock _profile_ ## name (#name)
#   define DI_INIT_PROFILER DiProfiler::InitProfiler()
#   define DI_CLOSE_PROFILER DiProfiler::CloseProfiler()
#   define DI_PROFILE_BEGIN_FRAME DiProfiler::GetInstancePtr()->BeginFrame();
#   define DI_PROFILE_END_FRAME DiProfiler::GetInstancePtr()->EndFrame();
#   define DI_PROFILE_BEGIN_INTERVAL DiProfiler::GetInstancePtr()->BeginInterval();
#else
#   define DI_PROFILE(name) 
#   define DI_INIT_PROFILER 
#   define DI_CLOSE_PROFILER
#   define DI_PROFILE_BEGIN_FRAME
#   define DI_PROFILE_END_FRAME
#   define DI_PROFILE_BEGIN_INTERVAL
#endif
}

#endif
