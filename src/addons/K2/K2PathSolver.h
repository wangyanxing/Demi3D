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

#ifndef DiPathSolver_h__
#define DiPathSolver_h__

#include "K2Prerequisites.h"
#include "micropather.h"

namespace Demi
{
    class DiPathSolver : public micropather::Graph, public DiBase
    {
    public:

        DiPathSolver(DiTerrainDescPtr d);

        ~DiPathSolver();

        int Passable(int nx, int ny);

        void NodeToXY(void* node, int* x, int* y)
        {
            intptr_t index = (intptr_t)node;
            *y = index / mapX;
            *x = index - *y * mapX;
        }

        void* XYToNode(int x, int y)
        {
            return (void*)(y*mapX + x);
        }

        int MoveTo(int srcx, int srcy, int nx, int ny, MP_VECTOR<void*>* path);

        float LeastCostEstimate(void* nodeStart, void* nodeEnd);

        void AdjacentCost(void* node, micropather::MPVector< micropather::StateCost > *neighbors);

        void PrintStateInfo(void* node);

        int playerX, playerY;
        
        int mapX, mapY;

        micropather::MicroPather* pather;
        
        DiTerrainDescPtr desc;
    };
}

#endif
