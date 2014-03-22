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

#ifndef DiK2WorldSerial_h__
#define DiK2WorldSerial_h__

#include "K2Prerequisites.h"

namespace Demi
{
    class DEMI_K2_API DiK2WorldSerial
    {
    public:

        DiK2WorldSerial(){}

        ~DiK2WorldSerial(){}

    public:

        /// load the world by a map path
        /// e.g. maps/test_map
        void    Load(const DiString& path, DiK2World* world);

        void    LoadTextureList(const DiString& path, DiTerrainDescPtr terrainDesc);
    };
}

#endif