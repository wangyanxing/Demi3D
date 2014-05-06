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

#ifndef DiK2PathFinder_h__
#define DiK2PathFinder_h__

#ifdef _DEBUG
#   define CAN_GO_EDGE
#endif

#define MAX_FILE_PATH               260
#define NAV_FILE_Version            1
#define DEF_CollisionGrid_Scale	    1.0f

#define MAX_PATH_NODE_NUMBER        64
#define MAX_ROUTES                  360000
#define OFFSETZ                     1200

// light ware
#define LIGHT_MAX_PATH_NODE_NUMBER	5
#define LIGHT_MAX_ROUTES            90000 
#define LIGHT_OFFSETZ               300

#define EIGHT_DIRECTIONS            4


#define EMPTY_WIDTH                 1000
#define EMPTY_HEIGHT                1000

#include "K2Coords.h"

namespace Demi
{
    typedef DiVector<DiK2Pos> WORLD_POS_LIST;

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    class PathFinder
    {
    private:
        struct _D2XZLOOKUP
        {
            unsigned short costmultiplier;
            short zx;
            unsigned char route;
        } DZX[EIGHT_DIRECTIONS];

        struct AIROUTE
        {
            unsigned short count;

            unsigned short walkpoint;
            unsigned int startzx;
            unsigned int endzx;

            unsigned char route[MAX_ROUTES_t];
        };

        struct _WORLD
        {
            unsigned char state : 4;
            unsigned char route : 4;
        };

        struct _NODES
        {
            unsigned short f;
            unsigned short g;

            unsigned int zx;
        };

        struct _NAVMAP_HEAD
        {
            unsigned short width;
            unsigned short height;
        };

        enum
        {
            EMPTY_NODE = 0,
            MIN_NODE = 1,
            NO_ROUTE = EIGHT_DIRECTIONS,
            MAX_HEAP_LEAFS = MAX_ROUTES_t,
        };

        enum
        {
            UNKNOWN = 0,
            IMPASSABLE = 1,
            OPEN = 8,
            CLOSED = 14,
        };

        enum
        {
            FINDING = 0,
            NO_PATH = 1,
            PATH_FOUND = 2,
        };
        enum
        {
            ROOT_HEAP = 1
        };
    public:

        enum ENUM_BLOCK_LEVEL
        {
            BLOCK_LEVEL_WALK = 1,
        };
    private:
#ifdef _DEBUG
        char mFileName[MAX_FILE_PATH];
        //	Map* mOwner;
#endif

        int mWidth;
        int mHeight;
        
        // real units (because of the DEF_CollisionGrid_Scale)
        int mRelWidth;
        int mRelHeight;
        
        float mGridSize;
        float mInvGridSize;
        
        float mLeftTopx;
        float mLeftTopz;

        _WORLD* mWorld;
        _WORLD* mWorkWorld;
        _NODES* mNodes;

        unsigned short mLastHeapLeaf;
        unsigned short mHeap[MAX_HEAP_LEAFS];

        unsigned short mBestFNode;
        unsigned short mFreeNode;
        int mMaxNode;

        unsigned int mStartzx, mEndzx;
        unsigned short mStartx, mStartz, mEndx, mEndz;
        float mfStartX, mfStartZ, mfEndX, mfEndZ;

        int mCallTimes;
        int	mGridNum;
        int mGrids[MAX_ROUTES_t];

        DiK2Pos * mFirstCanGoPos;
        int mDistance;
        int curStep;

    public:
        PathFinder();
        virtual ~PathFinder();

        bool Init(const char* filename, const unsigned int mx, const unsigned int mz);

        bool Init(const char* flieData, const unsigned int size, const unsigned int mx, const unsigned int mz);

        bool Init(const char* filename);

        int GetRelWidth(){ return mRelWidth; }
        int GetRelHeight(){ return mRelHeight; }

        void Reset(int startz, int endz);
        
        bool FindPath(const DiK2Pos* startPt, const DiK2Pos* endPos, DiK2Pos* posNode, int& numNode, int nLevel, bool bLine = false, int maxRoute = MAX_ROUTES_t);
        
        bool IsReachable(const DiK2Pos& pos, const int nLevel);
        
        void SaveNavMap(const char* filename);
        
        void ResizeNavMap(int rLeft, int rTop, int rRight, int rBottom);
        
        void SetLevel(int x, int z, unsigned char level);
        
        unsigned char GetLevel(int x, int z);
        
        bool IsStraightLine(float fAStarBeginPtx, float fAStarBeginPty, float fAStarEndPtx, float fAStarEndPty, int nLevel);

    private:
        inline unsigned short Distance(const unsigned int zx);

        inline unsigned short RealDistance(const unsigned int zxFirst, const unsigned int zxSecond);
        
        inline int LEFT(int k);
        
        inline int RIGHT(int k);
        
        inline int PARENT(int k);
        
        inline bool NOTEMPTY_UP(int k);
        
        inline bool NOTEMPTY_DOWN(int k);
        
        inline void SwapHeap(const int k1, const int k2);

        void InsertNodeToHeap(unsigned short node);
        
        void RemoveRootFromHeap();

        bool ReadNavMap(const char* filename);
        
        bool ReadNavMapFormMemory(const char* filedata, const unsigned int datasize);
        
        void NewEmpty(unsigned int mx, unsigned int mz);

        void InitEightDirections();
        
        void EditAStarPathOpt(float startPtx, float startPty, DiK2Pos* posNode, int& numNode, int nLevel);
        
        bool PackRoute(DiK2Pos* posNode, int& numNode, int nLevel);
        
        int GetDistance();

        bool IsReachable(const int state, const int nDriverLevel);
        
        bool _IsStraightLine(float fAStarBeginPtx, float fAStarBeginPty, float fAStarEndPtx, float fAStarEndPty, int nLevel);
    };

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::PathFinder()
    {
        mWorld = NULL;
        mWorkWorld = NULL;
        mNodes = NULL;
        mFirstCanGoPos = new DiK2Pos();
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    bool PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::Init(const char* filename, const unsigned int mx, const unsigned int mz)
    {
        bool bRet = true;
        if (!ReadNavMap(filename))
        {
            NewEmpty(mx, mz);
            bRet = false;
        }
        InitEightDirections();
        return bRet;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    bool PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::Init(const char* fileData, const unsigned int size, const unsigned int mx, const unsigned int mz)
    {
        bool bRet = true;
        if (fileData)
        {
            if (!ReadNavMapFormMemory(fileData, size))
            {
                NewEmpty(mx, mz);
                bRet = false;
            }
        }
        else
        {
            NewEmpty(mx, mz);
            bRet = false;
        }
        InitEightDirections();
        return bRet;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    bool PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::Init(const char* filename)
    {
        bool bRet = true;
        if (!ReadNavMap(filename))
        {
            NewEmpty(EMPTY_WIDTH, EMPTY_HEIGHT);
            bRet = false;
        }
        InitEightDirections();
        return bRet;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    void PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::NewEmpty(unsigned int mx, unsigned int mz)
    {
        mRelWidth = (int)(mx / DEF_CollisionGrid_Scale);
        mRelHeight = (int)(mz / DEF_CollisionGrid_Scale);
        mWidth = mx;
        mHeight = mz;

        mGridSize = 1.0f / DEF_CollisionGrid_Scale;
        mInvGridSize = 1.0f / mGridSize;
        mLeftTopx = 0;
        mLeftTopz = 0;
        mMaxNode = mWidth * mHeight;
        mWorld = new _WORLD[mMaxNode];
        mWorkWorld = new _WORLD[mMaxNode];
        mNodes = new _NODES[mMaxNode + 1];

        memset(mWorld, 0, mMaxNode*sizeof(_WORLD));
        memset(mWorkWorld, 0, mMaxNode*sizeof(_WORLD));
        memset(mNodes, 0, (mMaxNode + 1)*sizeof(_NODES));

    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    bool PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::ReadNavMap(const char* filename)
    {
        if (filename == NULL)
            return false;

        FILE* fp = fopen(filename, "rb");
        if (0 == fp)
        {
            return false;
        }

        int nVersion = NAV_FILE_Version;
        fread(&nVersion, sizeof(int), 1, fp);
        _NAVMAP_HEAD head;
        fread(&head, sizeof(_NAVMAP_HEAD), 1, fp);

        mRelWidth = (int)(head.width / DEF_CollisionGrid_Scale);
        mRelHeight = (int)(head.height / DEF_CollisionGrid_Scale);

        mWidth = head.width;
        mHeight = head.height;
        mGridSize = 1.0f / DEF_CollisionGrid_Scale;
        mInvGridSize = 1 / mGridSize;

        mLeftTopx = 0;
        mLeftTopz = 0;

        mMaxNode = mWidth * mHeight;

        mWorld = new _WORLD[mMaxNode];
        mWorkWorld = new _WORLD[mMaxNode];
        mNodes = new _NODES[mMaxNode + 1];

        int size = sizeof(struct _NAVMAP_HEAD);
        for (int j = 0; j<mHeight; j++)
        {
            for (int i = 0; i<mWidth; i++)
            {
                int info;
                fread(&info, size, 1, fp);

                _WORLD* pWorld = mWorld + j * mWidth + i;
                pWorld->state = info;
            }
        }

        mNodes[0].zx = 0;
        mNodes[0].f = 0;
        mNodes[0].g = 0;

        curStep = 0;

        fclose(fp);
        return true;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    bool PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::ReadNavMapFormMemory(const char* filedata, const unsigned int datasize)
    {
        if (!filedata || datasize < sizeof(int)+sizeof(_NAVMAP_HEAD))
            return false;
        int nVersion = NAV_FILE_Version;
        //fread(&nVersion, sizeof(int), 1, fp);
        const char* data = filedata;

        memcpy(&nVersion, data, sizeof(int));
        data += sizeof(int);
        _NAVMAP_HEAD head;
        //fread(&head, sizeof(_NAVMAP_HEAD), 1, fp);
        memcpy(&head, data, sizeof(_NAVMAP_HEAD));
        data += sizeof(_NAVMAP_HEAD);

        mRelWidth = (int)(head.width / DEF_CollisionGrid_Scale);
        mRelHeight = (int)(head.height / DEF_CollisionGrid_Scale);

        mWidth = head.width;
        mHeight = head.height;
        mGridSize = 1.0f / DEF_CollisionGrid_Scale;
        mInvGridSize = 1 / mGridSize;

        unsigned int rsize = sizeof(int)+sizeof(_NAVMAP_HEAD)+(sizeof(struct _NAVMAP_HEAD)*mWidth*mHeight);
        if (datasize < rsize)
            return false;

        // 	mx = (unsigned int)(mWidth * mGridSize);
        // 	mz = (unsigned int)(mHeight * mGridSize);

        mLeftTopx = 0;
        mLeftTopz = 0;

        mMaxNode = mWidth * mHeight;

        mWorld = new _WORLD[mMaxNode];
        mWorkWorld = new _WORLD[mMaxNode];
        mNodes = new _NODES[mMaxNode + 1];

        int size = sizeof(struct _NAVMAP_HEAD);
        for (int j = 0; j<mHeight; j++)
        {
            for (int i = 0; i<mWidth; i++)
            {
                int info;
                //fread(&info,size,1,fp);
                memcpy(&info, data, size);
                data += size;

                _WORLD* pWorld = mWorld + j * mWidth + i;
                pWorld->state = info;
            }
        }

        mNodes[0].zx = 0;
        mNodes[0].f = 0;
        mNodes[0].g = 0;

        curStep = 0;

        return true;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    void PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::SaveNavMap(const char* filename)
    {
        FILE* fp = fopen(filename, "wb");
        if (0 == fp)
        {
            return;
        }

        int nVersion = NAV_FILE_Version;
        fwrite(&nVersion, sizeof(int), 1, fp);
        _NAVMAP_HEAD head;
        head.width = mWidth;
        head.height = mHeight;
        fwrite(&head, sizeof(_NAVMAP_HEAD), 1, fp);
        for (int j = 0; j<mHeight; j++)
        {
            for (int i = 0; i<mWidth; i++)
            {
                _WORLD* pWorld = mWorld + j * mWidth + i;
                int info = pWorld->state;
                fwrite(&info, sizeof(int), 1, fp);
            }
        }
        fclose(fp);
    }
    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    void PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::ResizeNavMap(int rLeft, int rTop, int rRight, int rBottom)
    {
        int xsize = rRight - rLeft;
        int zsize = rBottom - rTop;
        int old_mwidth = mWidth;
        int old_mhegiht = mHeight;

        _WORLD* oldWorld = mWorld;

        delete[] mWorkWorld;
        delete[] mNodes;
        NewEmpty(xsize, zsize);

        for (int z = 0; z < mHeight; z++)
        {
            int oldz = z + rTop;
            if (oldz < 0 || oldz >= old_mhegiht)
                continue;
            for (int x = 0; x < mWidth; x++)
            {
                int oldx = x + rLeft;
                if (oldx < 0 || oldx >= old_mwidth)
                    continue;

                _WORLD* pWorld = mWorld + z * mWidth + x;
                _WORLD* pOld = oldWorld + oldz * old_mwidth + oldx;
                pWorld->state = pOld->state;
            }
        }
        delete[] oldWorld;

        mNodes[0].zx = 0;
        mNodes[0].f = 0;
        mNodes[0].g = 0;

        curStep = 0;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    void PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::SetLevel(int x, int z, unsigned char level)
    {
        _WORLD* pWorld = mWorld + z * mWidth + x;
        pWorld->state = level;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    unsigned char PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::GetLevel(int x, int z)
    {
        _WORLD* pWorld = mWorld + z * mWidth + x;
        return pWorld->state;
    }
    //-----------------------------------------------------------------------------
    /*
    *
    5	2   6

    1	*   3

    4   0   7


    4 5 6 7		0 1 2 3
    6 7 4 5     2 3 0 1
    */
    //-----------------------------------------------------------------------------

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    void PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::InitEightDirections()
    {


        int n;
        for (n = 0; n<4; n++)
        {
            DZX[n].costmultiplier = 10;
        }

        DZX[0].zx = -mWidth;
        DZX[0].route = 2;

        DZX[1].zx = 1;
        DZX[1].route = 3;

        DZX[2].zx = mWidth;
        DZX[2].route = 0;

        DZX[3].zx = -1;
        DZX[3].route = 1;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::~PathFinder()
    {
        if (mWorld)
            delete[]mWorld;
        if (mWorkWorld)
            delete[]mWorkWorld;
        if (mNodes)
            delete[]mNodes;
        mWorld = NULL;
        mWorkWorld = NULL;
        mNodes = NULL;
        delete mFirstCanGoPos;
    }


    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    void PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::Reset(int startz, int endz)
    {
        memcpy((void*)(mWorkWorld + (startz * mWidth)), (void*)(mWorld + (startz * mWidth)), (endz - startz) * mWidth * sizeof(_WORLD));

        mBestFNode = 1;
        mNodes[mBestFNode].zx = mStartzx;
        mNodes[mBestFNode].g = 0;
        mNodes[mBestFNode].f = mNodes[mBestFNode].g + Distance(mStartzx);

        mWorkWorld[mStartzx].route = NO_ROUTE;

        mFreeNode = 1;

        mHeap[0] = EMPTY_NODE;
        mLastHeapLeaf = 1;
        mHeap[mLastHeapLeaf] = mBestFNode;
    }

    // Binary heap A*
    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    void PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::RemoveRootFromHeap()
    {
        mHeap[ROOT_HEAP] = mHeap[mLastHeapLeaf--];

        int k = ROOT_HEAP;
        while (NOTEMPTY_DOWN(k))
        {
            int leftk = LEFT(k);
            int rightk = RIGHT(k);
            int bestk;
            if (NOTEMPTY_DOWN(leftk) && NOTEMPTY_DOWN(rightk))
            {
                if (mNodes[mHeap[leftk]].f < mNodes[mHeap[rightk]].f)
                    bestk = leftk;
                else
                    bestk = rightk;
            }
            else if (NOTEMPTY_DOWN(leftk))
                bestk = leftk;
            else
                break;

            if (mNodes[mHeap[bestk]].f < mNodes[mHeap[k]].f)
            {
                SwapHeap(k, bestk);
                k = bestk;
            }
            else
                break;
        }
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    void PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::InsertNodeToHeap(unsigned short node)
    {
        if (mLastHeapLeaf < MAX_HEAP_LEAFS)
            mLastHeapLeaf++;

        mHeap[mLastHeapLeaf] = node;

        int k = mLastHeapLeaf;
        while (NOTEMPTY_UP(k))
        {
            int parentk = PARENT(k);
            if (NOTEMPTY_UP(parentk))
            {
                if (mNodes[mHeap[k]].f < mNodes[mHeap[parentk]].f)
                {
                    SwapHeap(k, parentk);
                    k = parentk;
                }
                else
                    break;
            }
            else
                break;
        }
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    inline int PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::LEFT(int k)
    {
        return k << 1;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    inline int PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::RIGHT(int k)
    {
        return (k << 1) + 1;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    inline int PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::PARENT(int k)
    {
        return (k >> 1);
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    inline bool PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::NOTEMPTY_UP(int k)
    {
        return k != 0;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    inline bool PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::NOTEMPTY_DOWN(int k)
    {
        return k <= mLastHeapLeaf;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    inline void PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::SwapHeap(const int k1, const int k2)
    {
        unsigned short tmp = mHeap[k1];
        mHeap[k1] = mHeap[k2];
        mHeap[k2] = tmp;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    bool PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::FindPath(const DiK2Pos* startPt, const DiK2Pos* endPt, DiK2Pos* posNode, int& numNode, int nLevel, bool bLine, int maxRoute)
    {
        if (startPt->x < 1.0 || startPt->x >(mWidth - 1) / DEF_CollisionGrid_Scale || startPt->z < 1.0 || startPt->z >(mHeight - 1) / DEF_CollisionGrid_Scale ||
            endPt->x < 1.0 || endPt->x >(mWidth - 1) / DEF_CollisionGrid_Scale || endPt->z < 1.0 || endPt->z >(mHeight - 1) / DEF_CollisionGrid_Scale)
        {
            return false;
        }

        mfStartX = startPt->x;
        mfStartZ = startPt->z;
        mfEndX = endPt->x;
        mfEndZ = endPt->z;

        mStartx = (unsigned short)((mfStartX - mLeftTopx) * mInvGridSize);
        mStartz = (unsigned short)((mfStartZ - mLeftTopz) * mInvGridSize);

        mEndx = (unsigned short)((mfEndX - mLeftTopx) * mInvGridSize);
        mEndz = (unsigned short)((mfEndZ - mLeftTopz) * mInvGridSize);

        if (mStartx < 0 || mStartx >= mWidth || mStartz < 0 || mStartz >= mHeight ||
            mEndx < 0 || mEndx >= mWidth || mEndz < 0 || mEndz >= mHeight)
        {
            return false;
        }

        mStartzx = mStartz * mWidth + mStartx;
        mEndzx = mEndz * mWidth + mEndx;

        numNode = 0;

        mDistance = 0;

        if (bLine)
        {
            //if( mWorld[mStartzx].state != IMPASSABLE)
            if (IsReachable(mWorld[mStartzx].state, nLevel))
            {
                if (_IsStraightLine(mfStartX, mfStartZ, mfEndX, mfEndZ, nLevel))
                {
                    posNode[numNode++] = DiK2Pos(mfEndX, mfEndZ);
                    mDistance = RealDistance(mStartzx, mEndzx);
                    return true;
                }
                else
                {
                    posNode[numNode++] = *mFirstCanGoPos;
                    mDistance = RealDistance(mStartzx, (unsigned short)((mFirstCanGoPos->z - mLeftTopz) * mInvGridSize) * mWidth + (unsigned short)((mFirstCanGoPos->x - mLeftTopx) * mInvGridSize));
                    return true;
                }
            }
            else
            {
                return false;
            }
        }

        // A*
        if (!IsReachable(mWorld[mEndzx].state, nLevel))
        {
            return false;
        }
        else
        {
            int deltax = abs(mEndx - mStartx);
            int deltaz = abs(mEndz - mStartz);

            if ((deltax + deltaz) > maxRoute)
            {
                mDistance = RealDistance(mStartzx, (unsigned short)((mFirstCanGoPos->z - mLeftTopz) * mInvGridSize) * mWidth + (unsigned short)((mFirstCanGoPos->x - mLeftTopx) * mInvGridSize));
                return false;
            }

            if (_IsStraightLine(mfStartX, mfStartZ, mfEndX, mfEndZ, nLevel))
            {
                posNode[numNode++] = DiK2Pos(mfEndX, mfEndZ);
                mDistance = RealDistance(mStartzx, mEndzx);
                return true;
            }

            int minz = std::min(mStartz, mEndz);
            int maxz = std::max(mStartz, mEndz);
            minz = std::max(minz - OFFSETZ_t, 0);
            maxz = std::min(maxz + OFFSETZ_t, mHeight - 1);

            Reset(minz, maxz);
            int count = maxRoute;

            do
            {
                mBestFNode = mHeap[ROOT_HEAP];

                _NODES *pparent_node = mNodes + mBestFNode;
               
                if (pparent_node->zx == mEndzx)
                {
                    return PackRoute(posNode, numNode, nLevel);
                    //INFO_LOG("count = %d" ,count);

                    //return true;
                }

                mWorkWorld[pparent_node->zx].state = CLOSED;
                RemoveRootFromHeap();

                for (unsigned char d = 0; d < EIGHT_DIRECTIONS; d++)
                {
                    /*
                    *
                    0     1      2      3                                           127

                    127	 128   129    130

                    256   257    258
                    */

#ifdef CAN_GO_EDGE
                    int zx = (int)(pparent_node->zx);
                    if ((zx % mWidth == 0) && ((d == 3) || (d == 6) || (d == 7)))
                        continue;
                    if ((zx % mWidth == (mWidth - 1)) && ((d == 1) || (d == 4) || (d == 5)))
                        continue;

                    zx += DZX[d].zx;
                    if (zx < 0 || zx >= mWidth * mHeight)
                        continue;

#else
                    int zx = (int)pparent_node->zx + (int)DZX[d].zx;
#endif
                    _WORLD *pworld = mWorkWorld + zx;

                    if (pworld->state == UNKNOWN)
                    {
                        pworld->state = OPEN;
                        pworld->route = d;

                        mFreeNode++;

                        _NODES *pfree_node = mNodes + mFreeNode;
                        pfree_node->zx = zx;
                        pfree_node->g = pparent_node->g + DZX[d].costmultiplier;
                        pfree_node->f = pfree_node->g + Distance(zx);

                        InsertNodeToHeap(mFreeNode);
                    }
                }

                if (mLastHeapLeaf <= 0)
                {
                    posNode[numNode++] = *mFirstCanGoPos;
                    mDistance = RealDistance(mStartzx, (unsigned short)((mFirstCanGoPos->z - mLeftTopz) * mInvGridSize) * mWidth + (unsigned short)((mFirstCanGoPos->x - mLeftTopx) * mInvGridSize));
                    return true;
                }
            } while (--count>0);
        }
        return false;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    inline unsigned short PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::Distance(const unsigned int zx)
    {
        //int x = zx % mWidth;
        //int z = zx / mWidth;	
        return (unsigned short)((abs((int)((zx % mWidth) - (int)mEndx)) + abs((int)((zx / mWidth) - (int)mEndz))) * 10);
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    inline unsigned short PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::RealDistance(const unsigned int zxFirst, const unsigned int zxSecond)
    {
        return (unsigned short)((abs((int)(zxFirst & (mWidth - 1)) - (int)(zxSecond & (mWidth - 1))) + abs((int)(zxFirst / mWidth) - (int)(zxSecond / mWidth))));
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    bool PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::PackRoute(DiK2Pos* posNode, int& numNode, int nLevel)
    {
        unsigned int zx = mEndzx;
        //int start = MAX_ROUTES - 1;
        unsigned char route = NO_ROUTE;

        mGridNum = 0;
        DiK2Pos firstPos;
        DiK2Pos startPos;
        mGrids[mGridNum++] = zx;
        mDistance = 0;
        int x;
        int z;
        while (zx != mStartzx)
        {
            route = mWorkWorld[zx].route;
            zx += DZX[DZX[route].route].zx;

            x = zx % mWidth;
            z = zx / mWidth;

            DiK2Pos goPos;
            goPos.x = mLeftTopx + x * mGridSize + mGridSize / 2;
            goPos.z = mLeftTopz + z * mGridSize + mGridSize / 2;

            if (_IsStraightLine(mfStartX, mfStartZ, goPos.x, goPos.z, nLevel))
            {
                firstPos.x = mLeftTopx + x * mGridSize + mGridSize / 2;
                firstPos.z = mLeftTopz + z * mGridSize + mGridSize / 2;

                mGrids[mGridNum++] = zx;
                posNode[numNode++] = firstPos;
                mDistance += RealDistance(zx, mGrids[mGridNum - 1]);
                break;
            }
            else
            {
                mGrids[mGridNum++] = zx;
                mDistance += RealDistance(zx, mGrids[mGridNum - 1]);
            }
        }

        if (mGridNum)
        {
            mCallTimes = numNode;
            curStep = mGridNum - 1;

            while (curStep > 0 && mCallTimes++ < MAX_PATH_NODE_NUMBER_t)
            {
                x = mGrids[curStep] % mWidth;
                z = mGrids[curStep] / mWidth;

                startPos.x = mLeftTopx + x * mGridSize + mGridSize / 2;
                startPos.z = mLeftTopz + z * mGridSize + mGridSize / 2;

                EditAStarPathOpt(startPos.x, startPos.z, posNode, numNode, nLevel);
            }
            if (curStep > 0)
                return false;

            return true;
        }

        return true;
    }

    // must be called after path finding
    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    int PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::GetDistance()
    {
        return mDistance;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    bool PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::IsStraightLine(float fAStarBeginPtx, float fAStarBeginPty, float fAStarEndPtx, float fAStarEndPty, int nLevel)
    {
        if (fAStarBeginPtx < 1.0 || fAStarBeginPtx >(mWidth - 1) / DEF_CollisionGrid_Scale || fAStarBeginPty < 1.0 || fAStarBeginPty >(mHeight - 1) / DEF_CollisionGrid_Scale ||
            fAStarEndPtx < 1.0 || fAStarEndPtx >(mWidth - 1) / DEF_CollisionGrid_Scale || fAStarEndPty < 1.0 || fAStarEndPty >(mHeight - 1) / DEF_CollisionGrid_Scale)
        {
            return false;
        }
        return _IsStraightLine(fAStarBeginPtx, fAStarBeginPty, fAStarEndPtx, fAStarEndPty, nLevel);
    }

    // in 3d space
    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    bool PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::_IsStraightLine(float fAStarBeginPtx, float fAStarBeginPty, float fAStarEndPtx, float fAStarEndPty, int nLevel)
    {

        DiK2Pos startPt(fAStarBeginPtx - mLeftTopx, fAStarBeginPty - mLeftTopz);
        DiK2Pos endPt(fAStarEndPtx - mLeftTopx, fAStarEndPty - mLeftTopz);

        int xinc1, yinc1;

        int x1, y1, x2, y2;
        x1 = (int)(startPt.x  * mInvGridSize);
        y1 = (int)(startPt.z  * mInvGridSize);
        x2 = (int)(endPt.x  * mInvGridSize);
        y2 = (int)(endPt.z  * mInvGridSize);

        float deltax, deltay;
        deltax = (endPt.x - startPt.x) * mInvGridSize;
        deltay = (endPt.z - startPt.z) * mInvGridSize;

        *mFirstCanGoPos = startPt;
        
        // slope < 1
        if (fabs(deltax) >= fabs(deltay))
        {
            float slerp = deltay / deltax;
            float parameterB = (startPt.z - startPt.x  * slerp) * mInvGridSize;
            if (x2 >= x1)
            {
                xinc1 = 1;

                for (int x = x1 + xinc1; x <= x2; x += xinc1)
                {
                    float fy = slerp * x + parameterB;
                    int y = (int)fy;
                    int index = x + y * mWidth;

                    if (!IsReachable(mWorld[index].state, nLevel))
                        return false;

                    *mFirstCanGoPos = DiK2Pos(mLeftTopx + x * mGridSize + mGridSize / 2,
                        mLeftTopz + y * mGridSize + mGridSize / 2);

                    index = x - xinc1 + y * mWidth;

                    if (!IsReachable(mWorld[index].state, nLevel))
                        return false;

                    *mFirstCanGoPos = DiK2Pos(mLeftTopx + (x - xinc1) * mGridSize + mGridSize / 2,
                        mLeftTopz + y * mGridSize + mGridSize / 2);

                    if (fy - y < 0.01)
                    {
                        index = x + (y - 1) * mWidth;
                        if (!IsReachable(mWorld[index].state, nLevel))
                            return false;

                        *mFirstCanGoPos = DiK2Pos(mLeftTopx + x * mGridSize + mGridSize / 2,
                            mLeftTopz + (y - 1) * mGridSize + mGridSize / 2);
                    }
                    else if (fy - y > 0.99)
                    {
                        index = x + (y + 1) * mWidth;
                        if (!IsReachable(mWorld[index].state, nLevel))
                            return false;

                        *mFirstCanGoPos = DiK2Pos(mLeftTopx + x * mGridSize + mGridSize / 2,
                            mLeftTopz + (y + 1) * mGridSize + mGridSize / 2);

                        index = x - xinc1 + (y + 1) * mWidth;
                        if (!IsReachable(mWorld[index].state, nLevel))
                            return false;

                        *mFirstCanGoPos = DiK2Pos(mLeftTopx + (x - xinc1) * mGridSize + mGridSize / 2,
                            mLeftTopz + (y + 1) * mGridSize + mGridSize / 2);
                    }

                }
            }
            else
            {
                xinc1 = -1;
                for (int x = x1; x >= x2 - xinc1; x += xinc1)
                {
                    float fy = slerp * x + parameterB;
                    int y = (int)fy;

                    int index = x + y * mWidth;
                    if (!IsReachable(mWorld[x + y * mWidth].state, nLevel))
                        return false;

                    *mFirstCanGoPos = DiK2Pos(mLeftTopx + x * mGridSize + mGridSize / 2,
                        mLeftTopz + y * mGridSize + mGridSize / 2);

                    index = x + xinc1 + y * mWidth;

                    if (!IsReachable(mWorld[index].state, nLevel))
                        return false;

                    *mFirstCanGoPos = DiK2Pos(mLeftTopx + (x + xinc1) * mGridSize + mGridSize / 2,
                        mLeftTopz + y * mGridSize + mGridSize / 2);

                    if (fy - y < 0.01)
                    {
                        index = x + (y - 1) * mWidth;
                        if (!IsReachable(mWorld[index].state, nLevel))
                            return false;

                        *mFirstCanGoPos = DiK2Pos(mLeftTopx + x * mGridSize + mGridSize / 2,
                            mLeftTopz + (y - 1) * mGridSize + mGridSize / 2);
                    }
                    else if (fy - y > 0.99)
                    {
                        index = x + (y + 1) * mWidth;
                        if (!IsReachable(mWorld[index].state, nLevel))
                            return false;

                        *mFirstCanGoPos = DiK2Pos(mLeftTopx + x * mGridSize + mGridSize / 2,
                            mLeftTopz + (y + 1) * mGridSize + mGridSize / 2);

                        index = x + xinc1 + (y + 1) * mWidth;
                        if (!IsReachable(mWorld[index].state, nLevel))
                            return false;

                        *mFirstCanGoPos = DiK2Pos(mLeftTopx + (x + xinc1) * mGridSize + mGridSize / 2,
                            mLeftTopz + (y + 1) * mGridSize + mGridSize / 2);
                    }

                }
            }
        }
        // scope > 1
        else
        {
            float slerpInv = deltax / deltay;
            float parameterB = (startPt.x - startPt.z  * slerpInv) * mInvGridSize;
            if (y2 >= y1)
            {
                yinc1 = 1;
                for (int y = y1 + yinc1; y <= y2; y += yinc1)
                {
                    float fx = slerpInv * y + parameterB;
                    int x = (int)fx;
                    int index = x + y * mWidth;

                    if (!IsReachable(mWorld[index].state, nLevel))
                        return false;

                    *mFirstCanGoPos = DiK2Pos(mLeftTopx + x * mGridSize + mGridSize / 2,
                        mLeftTopz + y * mGridSize + mGridSize / 2);

                    index = x + (y - yinc1) * mWidth;

                    if (!IsReachable(mWorld[index].state, nLevel))
                        return false;

                    *mFirstCanGoPos = DiK2Pos(mLeftTopx + x * mGridSize + mGridSize / 2,
                        mLeftTopz + (y - yinc1) * mGridSize + mGridSize / 2);

                    if (fx - x < 0.01)
                    {
                        index = x - 1 + y * mWidth;
                        if (!IsReachable(mWorld[index].state, nLevel))
                            return false;

                        *mFirstCanGoPos = DiK2Pos(mLeftTopx + (x - 1) * mGridSize + mGridSize / 2,
                            mLeftTopz + y * mGridSize + mGridSize / 2);
                    }
                    else if (fx - x > 0.99)
                    {
                        index = x + 1 + y * mWidth;
                        if (!IsReachable(mWorld[index].state, nLevel))
                            return false;

                        *mFirstCanGoPos = DiK2Pos(mLeftTopx + (x + 1) * mGridSize + mGridSize / 2,
                            mLeftTopz + y * mGridSize + mGridSize / 2);

                        index = x + 1 + (y - yinc1) * mWidth;
                        if (!IsReachable(mWorld[index].state, nLevel))
                            return false;

                        *mFirstCanGoPos = DiK2Pos(mLeftTopx + (x + 1) * mGridSize + mGridSize / 2,
                            mLeftTopz + (y - yinc1) * mGridSize + mGridSize / 2);
                    }
                }
            }
            else
            {
                yinc1 = -1;
                for (int y = y1; y >= y2 - yinc1; y += yinc1)
                {
                    float fx = slerpInv * y + parameterB;
                    int x = (int)fx;

                    int index = x + y * mWidth;

                    if (!IsReachable(mWorld[index].state, nLevel))
                        return false;

                    *mFirstCanGoPos = DiK2Pos(mLeftTopx + x * mGridSize + mGridSize / 2,
                        mLeftTopz + y * mGridSize + mGridSize / 2);

                    index = x + (y + yinc1) * mWidth;

                    if (!IsReachable(mWorld[index].state, nLevel))
                        return false;

                    *mFirstCanGoPos = DiK2Pos(mLeftTopx + x * mGridSize + mGridSize / 2,
                        mLeftTopz + (y + yinc1) * mGridSize + mGridSize / 2);

                    if (fx - x < 0.01)
                    {
                        index = x - 1 + y * mWidth;
                        if (!IsReachable(mWorld[index].state, nLevel))
                            return false;

                        *mFirstCanGoPos = DiK2Pos(mLeftTopx + (x - 1) * mGridSize + mGridSize / 2,
                            mLeftTopz + y * mGridSize + mGridSize / 2);
                    }
                    else if (fx - x > 0.99)
                    {
                        index = x + 1 + y * mWidth;
                        if (!IsReachable(mWorld[index].state, nLevel))
                            return false;

                        *mFirstCanGoPos = DiK2Pos(mLeftTopx + (x + 1) * mGridSize + mGridSize / 2,
                            mLeftTopz + y * mGridSize + mGridSize / 2);

                        index = x + 1 + (y + yinc1)* mWidth;
                        if (!IsReachable(mWorld[index].state, nLevel))
                            return false;

                        *mFirstCanGoPos = DiK2Pos(mLeftTopx + (x + 1) * mGridSize + mGridSize / 2,
                            mLeftTopz + (y + yinc1) * mGridSize + mGridSize / 2);
                    }
                }
            }


        }
        return true;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    void PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::EditAStarPathOpt(float startPtx, float startPty, DiK2Pos* posNode, int& numNode, int nLevel)
    {
        for (int i = 0; i<curStep; i++)
        {
            int x = mGrids[i] % mWidth;
            int y = mGrids[i] / mWidth;

            DiK2Pos goPos;
            if (i != 0)
            {
                goPos.x = mLeftTopx + x * mGridSize + mGridSize / 2;
                goPos.z = mLeftTopz + y * mGridSize + mGridSize / 2;
            }
            else
            {
                goPos = DiK2Pos(mfEndX, mfEndZ);
            }

            if (_IsStraightLine(startPtx, startPty, goPos.x, goPos.z, nLevel))
            {
                posNode[numNode++] = goPos;
                curStep = i;

                break;
            }
        }
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    bool PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::IsReachable(const DiK2Pos& pos, const int nLevel)
    {


        int gridx = (unsigned short)((pos.x - mLeftTopx) * mInvGridSize);
        int gridz = (unsigned short)((pos.z - mLeftTopz) * mInvGridSize);

        if ((gridx >= 0 && gridx < mWidth) && (gridz >= 0 && gridz < mHeight))
            return IsReachable(mWorld[gridz * mWidth + gridx].state, nLevel);

        return false;

        return false;
    }

    template< int MAX_ROUTES_t, int MAX_PATH_NODE_NUMBER_t, int OFFSETZ_t >
    bool PathFinder<MAX_ROUTES_t, MAX_PATH_NODE_NUMBER_t, OFFSETZ_t>::IsReachable(const int state, const int nDriverLevel)
    {
        return nDriverLevel >= state;
    }

    typedef PathFinder<LIGHT_MAX_ROUTES, LIGHT_MAX_PATH_NODE_NUMBER, LIGHT_OFFSETZ> LightPathFinder;
    typedef PathFinder<MAX_ROUTES, MAX_PATH_NODE_NUMBER, OFFSETZ> HeavyPathFinder;
}

#endif
