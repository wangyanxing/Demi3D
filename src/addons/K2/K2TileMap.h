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

#ifndef DiK2TileMap_h__
#define DiK2TileMap_h__

#include "K2Prerequisites.h"

namespace Demi
{
    struct K2TileLayer
    {
        uint16 data0;
        uint16 data1;
        uint16 textureID;
    };
    
    /** height map class used for terrain rendering
     */
    class DEMI_K2_API DiK2TileMap
    {
    public:

        DiK2TileMap();

        ~DiK2TileMap();

    public:

        void            Load(DiDataStreamPtr data);
        
        void            Unload();
        
        uint32          GetWidth() const {return mWidth;}
        
        uint32          GetHeight() const {return mHeight;}
        
        K2TileLayer*    GetBuffer(int layer) {return mBuffer[layer];}
        
    private:
        
        uint32          mWidth;

        uint32          mHeight;
        
        K2TileLayer*    mBuffer[2];
    };
}

#endif