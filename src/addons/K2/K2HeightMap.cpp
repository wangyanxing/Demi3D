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
#include "K2HeightMap.h"

namespace Demi
{
    DiK2HeightMap::DiK2HeightMap()
        :mWidth(0)
        ,mHeight(0)
        ,mBuffer(nullptr)
    {
    }
    
    DiK2HeightMap::~DiK2HeightMap()
    {
        Unload();
    }
    
    void DiK2HeightMap::Load(DiDataStreamPtr data)
    {
        int w = 0, h = 0;
        data->Read(&w, sizeof(int));
        data->Read(&h, sizeof(int));
        
        bool asFloat = true;
        if(w < 0)
        {
            asFloat = false;
            w = -w;
        }
        
        mWidth = w;
        mHeight = h;
        
        Unload();
        
        mBuffer = DI_NEW float[w * h];
        
        if (asFloat)
        {
            data->Read(mBuffer, sizeof(float) * w * h);
        }
        else
        {
            for (int i = 0; i < w * h; ++i)
            {
                uint8 val = 0;
                data->Read(&val, sizeof(uint8));
                mBuffer[i] = val / 256.0f;
            }
            
            for (int i = 0; i < w * h; ++i)
            {
                uint8 val = 0;
                data->Read(&val, sizeof(uint8));
                mBuffer[i] += val;
            }
            
            for (int i = 0; i < w * h; ++i)
            {
                uint8 val = 0;
                data->Read(&val, sizeof(uint8));
                mBuffer[i] += val * 256.0f;
                mBuffer[i] -= 32768.0f;
            }
        }
    }
    
    void DiK2HeightMap::Unload()
    {
        if(mBuffer)
        {
            DI_DELETE[] mBuffer;
            mBuffer = nullptr;
        }
    }
}