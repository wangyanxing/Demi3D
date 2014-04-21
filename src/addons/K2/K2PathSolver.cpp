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

#include "K2Pch.h"
#include "K2TerrainDesc.h"
#include "K2PathSolver.h"
#include "K2MapLoader.h"

namespace Demi
{
    DiPathSolver::DiPathSolver(DiTerrainDescPtr d) : playerX(0), playerY(0), pather(0), desc(d)
    {
        pather = DI_NEW micropather::MicroPather(this);

        mapX = CHUNK_GRID_SIZE*desc->mSizeX + 1;
        mapY = CHUNK_GRID_SIZE*desc->mSizeY + 1;
    }

    DiPathSolver::~DiPathSolver()
    {
        DI_DELETE pather;
    }

    int DiPathSolver::Passable(int nx, int ny)
    {
        if (nx >= 0 && nx < mapX
            && ny >= 0 && ny < mapY)
        {
            int index = ny*mapX + nx;
            uint8 c = desc->mVertBlockerMap->GetBuffer()[index];
            if (c == 0)
                return 1;
        }
        return 0;
    }

    int DiPathSolver::MoveTo(int srcx, int srcy, int nx, int ny, MP_VECTOR<void*>* path)
    {
        int result = 1;
        if (Passable(nx, ny) == 1)
        {
            float totalCost;
            result = pather->Solve(XYToNode(srcx, srcy), XYToNode(nx, ny), path, &totalCost);

            if (result == micropather::MicroPather::SOLVED)
            {
                playerX = nx;
                playerY = ny;
            }
        }
        return result;
    }

    float DiPathSolver::LeastCostEstimate(void* nodeStart, void* nodeEnd)
    {
        int xStart, yStart, xEnd, yEnd;
        NodeToXY(nodeStart, &xStart, &yStart);
        NodeToXY(nodeEnd, &xEnd, &yEnd);

        /* Compute the minimum path cost using distance measurement. It is possible
        to compute the exact minimum path using the fact that you can move only
        on a straight line or on a diagonal, and this will yield a better result.
        */
        int dx = xStart - xEnd;
        int dy = yStart - yEnd;
        return (float)sqrt((double)(dx*dx) + (double)(dy*dy));
    }

    void DiPathSolver::AdjacentCost(void* node, micropather::MPVector< micropather::StateCost > *neighbors)
    {
        int x, y;
        const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
        const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
        const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };

        NodeToXY(node, &x, &y);

        for (int i = 0; i<8; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            int pass = Passable(nx, ny);
            if (pass > 0)
            {
                if (pass == 1)
                {
                    // Normal floor
                    micropather::StateCost nodeCost = { XYToNode(nx, ny), cost[i] };
                    neighbors->push_back(nodeCost);
                }
                else
                {
                    // Normal floor
                    micropather::StateCost nodeCost = { XYToNode(nx, ny), FLT_MAX };
                    neighbors->push_back(nodeCost);
                }
            }
        }
    }

    void DiPathSolver::PrintStateInfo(void* node)
    {
        int x, y;
        NodeToXY(node, &x, &y);
        DI_DEBUG("(%d,%d)", x, y);
    }

}