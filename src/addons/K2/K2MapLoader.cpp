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
#include "K2MapLoader.h"

namespace Demi
{
    DiK2HeightMap::DiK2HeightMap()
        :mBuffer(nullptr)
    {
    }
    
    DiK2HeightMap::~DiK2HeightMap()
    {
        Unload();
    }
    
    void DiK2HeightMap::Load(DiDataStreamPtr data)
    {
        DI_LOG("Loading k2 height map");

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
                //mBuffer[i] *= 100;
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

    DiK2TileMap::DiK2TileMap()
    {
        mBuffer[0] = nullptr;
        mBuffer[1] = nullptr;
    }

    DiK2TileMap::~DiK2TileMap()
    {
        Unload();
    }

    void DiK2TileMap::Load(DiDataStreamPtr data)
    {
        DI_LOG("Loading k2 tile map");

        int w = 0, h = 0;
        int layers = 0;

        data->Read(&layers, sizeof(int));
        data->Read(&w, sizeof(int));
        data->Read(&h, sizeof(int));

        bool asFloat = true;
        if (w < 0)
        {
            asFloat = false;
            w = -w;
        }

        mWidth = w;
        mHeight = h;

        Unload();

        for (int l = 0; l < 2; l++)
        {
            mBuffer[l] = DI_NEW K2TileLayer[w * h];

            for (int i = 0; i < w*h; ++i)
            {
                if (asFloat)
                {
                    int t;
                    data->Read(&t, sizeof(int));
                    mBuffer[l][i].value = (uint16)t;
                    data->Read(&t, sizeof(int));
                    mBuffer[l][i].diffuseID = (uint16)t;
                    data->Read(&t, sizeof(int));
                    mBuffer[l][i].normalID = (uint16)t;
                    data->Skip(sizeof(float)* 7); // I don't know what they are doing
                }
                else
                {
                    data->Read(&mBuffer[l][i], sizeof(K2TileLayer));
                    data->Skip(sizeof(float)* 3 + sizeof(uint16)); // I don't know either
                }
            }
        }
    }

    void DiK2TileMap::Load(uint32 width, uint32 height)
    {
        Unload();
        mWidth = width;
        mHeight = height;
        for (int l = 0; l < 2; l++)
            mBuffer[l] = DI_NEW K2TileLayer[width*height];
    }

    void DiK2TileMap::Unload()
    {
        if (mBuffer)
        {
            DI_DELETE[] mBuffer[0];
            DI_DELETE[] mBuffer[1];
            mBuffer[0] = nullptr;
            mBuffer[1] = nullptr;
        }
    }

    DiK2VertexColorMap::DiK2VertexColorMap()
        :mBuffer(nullptr)
    {
    }

    DiK2VertexColorMap::~DiK2VertexColorMap()
    {
        Unload();
    }

    void DiK2VertexColorMap::Load(DiDataStreamPtr data)
    {
        DI_LOG("Loading k2 vertex color map");

        int w = 0, h = 0;
        data->Read(&w, sizeof(int));
        data->Read(&h, sizeof(int));

        mWidth = w;
        mHeight = h;

        Unload();

        mBuffer = DI_NEW uint32[w * h];

        data->Read(mBuffer, sizeof(uint32)* w * h);
    }

    void DiK2VertexColorMap::Load(uint32 width, uint32 height)
    {
        Unload();
        mWidth = width;
        mHeight = height;
        mBuffer = DI_NEW uint32[width*height];
        uint32 orgColr = 0xFFFFFF00;
        for (uint32 i = 0; i < width*height; ++i)
            mBuffer[i] = orgColr;
    }

    void DiK2VertexColorMap::Unload()
    {
        if (mBuffer)
        {
            DI_DELETE[] mBuffer;
            mBuffer = nullptr;
        }
    }

    DiK2VertexCliffMap::DiK2VertexCliffMap()
        : mBuffer(nullptr)
    {
    }

    DiK2VertexCliffMap::~DiK2VertexCliffMap()
    {
    }

    void DiK2VertexCliffMap::Load(DiDataStreamPtr data)
    {
        DI_LOG("Loading k2 vertex cliff map");

        int w = 0, h = 0;
        data->Read(&w, sizeof(int));
        data->Read(&h, sizeof(int));

        mWidth = w;
        mHeight = h;

        data->Read(mBuffer, w*h*sizeof(uint32));
    }

    void DiK2VertexCliffMap::Load(uint32 width, uint32 height)
    {
        Unload();
        mWidth = width;
        mHeight = height;
        mBuffer = DI_NEW uint32[width*height];
        for (uint32 i = 0; i < width*height; ++i)
            mBuffer[i] = 0;
    }

    void DiK2VertexCliffMap::Unload()
    {
        if (mBuffer)
        {
            DI_DELETE[] mBuffer;
            mBuffer = nullptr;
        }
    }

    DiK2TileCliffMap::DiK2TileCliffMap()
        : mBuffer(nullptr)
    {
    }

    DiK2TileCliffMap::~DiK2TileCliffMap()
    {
    }

    void DiK2TileCliffMap::Load(DiDataStreamPtr data)
    {
        DI_LOG("Loading k2 tile cliff map");

        int w = 0, h = 0;
        data->Read(&w, sizeof(int));
        data->Read(&h, sizeof(int));

        mWidth = w;
        mHeight = h;

        data->Read(mBuffer, w*h*sizeof(uint8));
    }

    void DiK2TileCliffMap::Load(uint32 width, uint32 height)
    {
        Unload();
        mWidth = width;
        mHeight = height;
        mBuffer = DI_NEW uint8[width*height];
        for (uint32 i = 0; i < width*height; ++i)
            mBuffer[i] = 0;
    }

    void DiK2TileCliffMap::Unload()
    {
        if (mBuffer)
        {
            DI_DELETE[] mBuffer;
            mBuffer = nullptr;
        }
    }

}