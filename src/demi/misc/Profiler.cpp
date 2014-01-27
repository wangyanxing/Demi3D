
#include "MiscPch.h"
#include "Profiler.h"

namespace Demi
{
    _IMPLEMENT_SINGLETON(DiProfiler)

    static const int LINE_MAX_LENGTH = 256;
    static const int NAME_MAX_LENGTH = 30;

    DiProfiler::DiProfiler() :
        mCurrent(0),
        mRoot(0),
        mIntervalFrames(0),
        mTotalFrames(0)
    {
        mRoot = new DiProfilerBlock(0, "Root");
        mCurrent = mRoot;
    }

    DiProfiler::~DiProfiler()
    {
        delete mRoot;
        mRoot = 0;
    }

    void DiProfiler::BeginFrame()
    {
        // end the previous frame if any
        EndFrame();

        BeginBlock("RunFrame");
    }

    void DiProfiler::EndFrame()
    {
        if (mCurrent != mRoot)
        {
            EndBlock();
            ++mIntervalFrames;
            ++mTotalFrames;
            if (!mTotalFrames)
                ++mTotalFrames;
            mRoot->EndFrame();
            mCurrent = mRoot;
        }
    }

    void DiProfiler::BeginInterval()
    {
        mRoot->BeginInterval();
        mIntervalFrames = 0;
    }

    DiString DiProfiler::GetData(bool showUnused, bool showTotal, unsigned maxDepth) const
    {
        DiString output;

        if (!showTotal)
            output += DiString("Block                            Cnt     Avg      Max     Frame     Total\n\n");
        else
        {
            output += DiString("Block                                       Last frame                       Whole execution time\n\n");
            output += DiString("                                 Cnt     Avg      Max      Total      Cnt      Avg       Max        Total\n\n");
        }

        if (!maxDepth)
            maxDepth = 1;

        GetData(mRoot, output, 0, maxDepth, showUnused, showTotal);

        return output;
    }

    void DiProfiler::GetData(DiProfilerBlock* block, DiString& output, unsigned depth, unsigned maxDepth, bool showUnused, bool showTotal) const
    {
        char line[LINE_MAX_LENGTH];
        char indentedName[LINE_MAX_LENGTH];

        unsigned intervalFrames = DiMath::Max(mIntervalFrames, 1);

        if (depth >= maxDepth)
            return;

        // Do not print the root block as it does not collect any actual data
        if (block != mRoot)
        {
            if (showUnused || block->mIntervalCount || (showTotal && block->mTotalCount))
            {
                memset(indentedName, ' ', NAME_MAX_LENGTH);
                indentedName[depth] = 0;
                strcat(indentedName, block->mName);
                indentedName[strlen(indentedName)] = ' ';
                indentedName[NAME_MAX_LENGTH] = 0;

                if (!showTotal)
                {
                    double avg = (block->mIntervalCount ? block->mIntervalTime / block->mIntervalCount : 0.0f);
                    double max = block->mIntervalMaxTime;
                    double frame = block->mIntervalTime / intervalFrames;
                    double all = block->mIntervalTime;

                    sprintf(line, "%s %5u %8.3f %8.3f %8.3f %9.3f\n", indentedName, block->mIntervalCount, avg, max, frame, all);
                }
                else
                {
                    double avg = (block->mFrameCount ? block->mFrameTime / block->mFrameCount : 0.0f);
                    double max = block->mFrameMaxTime;
                    double all = block->mFrameTime;

                    double totalAvg = (block->mTotalCount ? block->mTotalTime / block->mTotalCount : 0.0f);
                    double totalMax = block->mTotalMaxTime;
                    double totalAll = block->mTotalTime;

                    sprintf(line, "%s %5u %8.3f %8.3f %9.3f  %7u %9.3f %9.3f %11.3f\n", indentedName, block->mFrameCount, avg, max,
                        all, block->mTotalCount, totalAvg, totalMax, totalAll);
                }

                output += DiString(line);
            }

            ++depth;
        }

        for (auto i = block->mChildren.begin(); i != block->mChildren.end(); ++i)
            GetData(*i, output, depth, maxDepth, showUnused, showTotal);
    }
}