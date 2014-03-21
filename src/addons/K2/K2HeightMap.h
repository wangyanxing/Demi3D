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

#ifndef DiK2HeightMap_h__
#define DiK2HeightMap_h__

#include "K2Prerequisites.h"

namespace Demi
{
    /** height map class used for terrain rendering
     */
    class DEMI_K2_API DiK2HeightMap
    {
    public:

        DiK2HeightMap();

        ~DiK2HeightMap();

    public:

        void    Load(DiDataStreamPtr data);
        
        void    Unload();
        
        uint32  GetWidth() const {return mWidth;}
        
        uint32  GetHeight() const {return mHeight;}
        
        float*  GetBuffer() {return mBuffer;}
        
    private:
        
        uint32  mWidth;

        uint32  mHeight;
        
        float*  mBuffer;
    };
}

#endif